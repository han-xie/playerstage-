#if !defined (WIN32)
#include <unistd.h>
#include <netinet/in.h>
#endif
#include <string.h>
#include <libplayercore/playercore.h>
#include "multidriver.h"


//int initialTimes=0;
// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver* multidriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	return ((Driver*) (new multidriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void multidriver_Register(DriverTable* table) {
	table->AddDriver((char *)"multidriver", multidriver_Init);
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
	multidriver_Register(table);
	return (0);
}
}
/*multidriver::~multidriver(void){
	return;
}*/
////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
multidriver::multidriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {
	int initialTimes;
	initialTimes++;
	if(initialTimes<2){
		MFCapOpen();
		MFInit();
		int portdirect = 0;
		MFSetPortDirect(portdirect);
		for (int i = 0; i < 8; i++) {
			MFADEnable(i);
		}
		MFDigiInit(100);
		DelayMS(100);
		MFADInit(100);
		DelayMS(100);
		MFUSonicEnable();
		DelayMS(100);
	}
	int device_count = cf->GetTupleCount(section, "provides");
	for (int d = 0; d < device_count; d++) {
		player_devaddr_t player_addr;
		if (cf->ReadDeviceAddr(&player_addr, section, "provides", 0, d, NULL)
				!= 0) {
			this->SetError(-1);
			return;
		}
		Interface *ifsrc = NULL;
		switch (player_addr.interf) {
		case PLAYER_BLOBFINDER_CODE:
			ifsrc = new InterfaceBlobfinder(player_addr, this, cf, section);
			break;
		case PLAYER_LASER_CODE:
			ifsrc = new InterfaceLaser(player_addr, this, cf, section);
			break;
		case PLAYER_POSITION2D_CODE:
			ifsrc = new InterfacePosition2d(player_addr, this, cf, section);
			break;
		case PLAYER_OPAQUE_CODE:
			ifsrc = new InterfaceOpaque(player_addr, this, cf, section);
			break;
		default:
			PRINT_ERR1(
					"error: multi driver doesn't support interface type %d\n",
					player_addr.interf);
			this->SetError(-1);
			return;
		}
		if (ifsrc) {
			if (this->AddInterface(ifsrc->addr)) {
				PRINT_ERR("AddInterface() failed");
				this->SetError(-2);
				return;
			}
			devices.push_back(ifsrc);
		} else {
			PRINT_ERR3("No Stage source found for interface %d:%d:%d",
					player_addr.robot, player_addr.interf, player_addr.index);
			this->SetError(-3);
			return;
		}
	}
}

Interface* multidriver::LookupDevice(player_devaddr_t addr) {
	FOR_EACH(it, this->devices)
	{
		Interface* candidate = *it;
		if (candidate->addr.robot == addr.robot
				&& candidate->addr.interf == addr.interf
				&& candidate->addr.index == addr.index)
			return candidate; // found
	}
	return NULL; // not found
}

// subscribe to a device
int multidriver::Subscribe(QueuePointer &queue, player_devaddr_t addr) {
	Interface* device = this->LookupDevice(addr);
	if (device) {
		device->Subscribe();
		device->Subscribe(queue);
		return Driver::Subscribe(addr);
	}
	return 1; // error
}
// unsubscribe to a device
int multidriver::Unsubscribe(QueuePointer &queue, player_devaddr_t addr) {
	Interface* device = this->LookupDevice(addr);
	if (device) {
		device->Unsubscribe();
		device->Unsubscribe(queue);
		return Driver::Unsubscribe(addr);
	} else
		return 1; // error
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int multidriver::MainSetup() {
	// Subscribe to the laser device
	/*if(!(this->laser_dev = deviceTable->GetDevice(this->laser_addr)))
	 {
	 PLAYER_ERROR("unable to locate suitable laser device");
	 return(-1);
	 }
	 if(this->laser_dev->Subscribe(this->InQueue) != 0)
	 {
	 PLAYER_ERROR("unable to subscribe to laser device");
	 return(-1);
	 }*/

	// Here you do whatever else is necessary to setup the device, like open and
	// configure a serial port.
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void multidriver::MainQuit() {
	// Unsubscribe from the laser

	FOR_EACH(it, this->devices)
		(*it)->Unsubscribe();

	return;
	// Here you would shut the device down by, for example, closing a
	// serial port.
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void multidriver::Main() {
	// The main loop; interact with the device here
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  Calls ProcessMessage() on each pending
		// message.
		this->ProcessMessages();

		for(std::vector<Interface*>::iterator in = this->devices.begin(); in != this->devices.end(); ++in){
			Interface* interface = *in;
			assert( interface );
			if(interface->PublishSign)
				interface->Publish();
		}
		/*FOR_EACH( it, this->devices ){
				Interface* interface = *it;
				assert( interface );
		}*/

		// Sleep (or you might, for example, block on a read() instead)
		usleep(100000);
	}
	return;
}

int multidriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Handle new data from the laser
	/*if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
	 this->laser_addr))
	 {
	 // Do someting with it
	 return(0);
	 }*/

	// find the right interface to handle this config
	Interface* in = this->LookupDevice(hdr->addr);
	if (in) {
		return (in->ProcessMessage(resp_queue, hdr, data));
	} else {
		PRINT_WARN3("can't find interface for device %d.%d.%d",
				this->device_addr.robot, this->device_addr.interf,
				this->device_addr.index);
		return (-1);
	}
}

Interface::Interface(player_devaddr_t addr, multidriver* driver, ConfigFile* cf,
		int section){
	this->Interface::addr=addr;
	this->Interface::driver=driver;
}

/*
 * A simple Ringer of how to write a driver that will be built as a
 * shared object.
 */

// ONLY if you need something that was #define'd as a result of configure 
// (e.g., HAVE_CFMAKERAW), then #include <config.h>, like so:
/*
 #include <config.h>
 */

#if !defined (WIN32)
#include <unistd.h>
#endif
#include <string.h>

#include <libplayercore/playercore.h>
#include <libplayerinterface/player_interfaces.h>
#include "RingerDriver.h"

#include "background.h"
#include <stRinger.h>
#include <fcntl.h>
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class RingerDriver: public ThreadedDriver {
public:

	// Constructor; need that
	RingerDriver(ConfigFile* cf, int section);

	// This method will be invoked on each incoming message
	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr * hdr,
			void * data);

private:

	// Main function for device thread.
	virtual void Main();
	virtual int MainSetup();
	virtual void MainQuit();

	int position_id;
	player_devaddr_t m_position_addr;
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
RingerDriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	return ((Driver*) (new RingerDriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void RingerDriver_Register(DriverTable* table) {
	table->AddDriver("Ringerdriver", RingerDriver_Init);
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
/*
 snd::snd(ConfigFile* cf, int section) :
 ThreadedDriver(cf, section) {
 memset(&this->position_id, 0, sizeof(position_id));

 printf("discovering drivers\n");
 // Read an option from the configuration file
 if (cf->ReadDeviceAddr((player_devaddr_t *) &(this->position_id), section,
 "provides", PLAYER_Ringer_CODE, -1, NULL) == 0) {
 if (this->AddInterface(*(player_devaddr_t *) &(this->position_id))
 != 0) {
 this->SetError(-1);
 return;
 }
 }
 }
 */
RingerDriver::RingerDriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {
	//memset(&this->position_id, 0, sizeof(position_id));
	PLAYER_MSG0(3, "INITIALIZING INTERFACE ...");

	printf("discovering drivers\n");
	/*
	 // Read an option from the configuration file
	 if (cf->ReadDeviceAddr((player_devaddr_t *) &(this->position_id), section,
	 "provides", PLAYER_Ringer_CODE, -1, NULL) == 0) {
	 //if(this->AddInterface(*(player_devaddr_t *)&(this->position_id)) != 0)
	 //	{
	 //	    this->SetError(-1);
	 //	    return ;
	 //	}
	 }
	 */

	// Create my position interface
	if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
			PLAYER_Ringer_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	}
	if (this->AddInterface(this->m_position_addr)) {
		this->SetError(-1);
		return;
	}
	printf("driver construct done !\n");

	return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int RingerDriver::MainSetup() {
	puts("Ringer driver MainSetup");

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.

	MFInit();
	DelayMS(10);
	MFDigiInit(1000);
	DelayMS(10);
	MFSetPortDirect(0x00000FFF);
	DelayMS(10);

	outputlog = open("kelog", O_RDWR | O_CREAT | O_TRUNC);

	puts("NorthStar driver ready");

	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void RingerDriver::MainQuit() {
	puts("Shutting Ringer driver down");

	// Here you would shut the device down by, for Ringer, closing a
	// serial port.
	close(outputlog);

	puts("Ringer driver has been shutdown");
}

int RingerDriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.


	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void RingerDriver::Main() {
	// The main loop; interact with the device here
	printf("begin to processMessages !\n");
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  RingerDriver::ProcessMessage() is
		// called on each message.
		ProcessMessages();
		//printf("ProcessMessages() excuted\n");

		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()

		// Sleep (you might, for Ringer, block on a read() instead)
		usleep(100000);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
	puts("Ringer driver initializing");
	RingerDriver_Register(table);
	puts("Ringer driver done");
	return (0);
}
}


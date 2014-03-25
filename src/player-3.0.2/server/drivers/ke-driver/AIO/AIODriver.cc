/*
 * A simple AIO of how to write a driver that will be built as a
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
#include "AIODriver.h"

#include "background.h"
#include <stdio.h>
#include <fcntl.h>
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class AIODriver: public ThreadedDriver {
public:

	// Constructor; need that
	AIODriver(ConfigFile* cf, int section);

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
AIODriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	return ((Driver*) (new AIODriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void AIODriver_Register(DriverTable* table) {
	table->AddDriver("AIOdriver", AIODriver_Init);
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
 "provides", PLAYER_AIO_CODE, -1, NULL) == 0) {
 if (this->AddInterface(*(player_devaddr_t *) &(this->position_id))
 != 0) {
 this->SetError(-1);
 return;
 }
 }
 }
 */
AIODriver::AIODriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {
	//memset(&this->position_id, 0, sizeof(position_id));
	PLAYER_MSG0(3, "INITIALIZING INTERFACE ...");

	printf("discovering drivers\n");
	/*
	 // Read an option from the configuration file
	 if (cf->ReadDeviceAddr((player_devaddr_t *) &(this->position_id), section,
	 "provides", PLAYER_AIO_CODE, -1, NULL) == 0) {
	 //if(this->AddInterface(*(player_devaddr_t *)&(this->position_id)) != 0)
	 //	{
	 //	    this->SetError(-1);
	 //	    return ;
	 //	}
	 }
	 */

	// Create my position interface
	if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
			PLAYER_AIO_CODE, -1, NULL) != 0) {
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
int AIODriver::MainSetup() {
	puts("AIO driver MainSetup");

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
void AIODriver::MainQuit() {
	puts("Shutting AIO driver down");

	// Here you would shut the device down by, for AIO, closing a
	// serial port.
	close(outputlog);

	puts("AIO driver has been shutdown");
}

int AIODriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.
	/*
	 #define 	PLAYER_AIO_CMD_STATE   1
	 Data: state (PLAYER_AIO_DATA_STATE)
	 #define 	PLAYER_AIO_DATA_STATE   1
	 Data: state (PLAYER_AIO_DATA_STATE) */
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_AIO_CMD_STATE,
			*(player_devaddr_t *) &(this->position_id))) {
		if (WRITELOG) {

			char buf[] = "porcessMessage PLAYER_AIO_CMD_STATE \n";
			write(outputlog, buf, sizeof(buf));
			//close(outputlog);
		}
		int vt = MFGetDigiInput(this->m_position_addr.index);

		//Publish(*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_AIO_CMD_STATE, &vt, sizeof(vt),NULL);

		Publish(resp_queue, hdr, &vt, NULL);

		return 0;
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA, PLAYER_AIO_DATA_STATE,
			*(player_devaddr_t *) &(this->position_id))) {
		if (WRITELOG) {
			//int outputlog=open("kelog",O_RDWR|O_CREAT|O_TRUNC);
			char buf[] = "porcessMessage PLAYER_AIO_DATA_STATE \n";
			write(outputlog, buf, sizeof(buf));
			//close(outputlog);
		}
		return 0;
	} else {
		if (WRITELOG) {
			//int outputlog=open("kelog",O_RDWR|O_CREAT|O_TRUNC);
			char buf[] = "porcessMessage other message !!! \n";
			write(outputlog, buf, sizeof(buf));
			//close(outputlog);
		}
		return 0;
	}
	/*
	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_AIO_REQ_GET_GEOM,*(player_devaddr_t *)&(this->position_id)))
	 {
	 player_AIO_geom_t geom;

	 //TODO : get values from somewhere.
	 geom.pose.px = -0.1;//htons((short) (-100));
	 geom.pose.py = 0;//htons((short) (0));
	 geom.pose.pyaw = 0;//htons((short) (0));
	 geom.size.sw = 0.25;//htons((short) (250));
	 geom.size.sl = 0.425;//htons((short) (425));
	 Publish(*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_AIO_REQ_GET_GEOM, &geom, sizeof(geom),NULL);

	 return 0;
	 }

	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_AIO_REQ_MOTOR_POWER, *(player_devaddr_t *)&(this->position_id)))
	 {
	 player_AIO_power_config_t * powercfg = reinterpret_cast<player_AIO_power_config_t *> (data);

	 printf("got motor power req: %d\n", powercfg->state);
	 if(ChangeMotorState(powercfg->state) < 0)
	 Publish(*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_NACK, PLAYER_AIO_REQ_MOTOR_POWER);
	 else
	 Publish(*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_AIO_REQ_MOTOR_POWER);
	 return 0;
	 }

	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_AIO_REQ_SET_ODOM, *(player_devaddr_t *)&(this->position_id)))
	 {
	 player_AIO_set_odom_req_t * odom = (player_AIO_set_odom_req_t *)data;
	 this->_px = odom->pose.px;
	 this->_py = odom->pose.py;
	 this->_pa = odom->pose.pa;
	 while (this->_pa > M_PI) this->_pa -= M_PI;
	 while (this->_pa < -M_PI) this->_pa += M_PI;
	 this->_resting = false;
	 Publish (*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_AIO_REQ_SET_ODOM);
	 return 0;
	 }

	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_AIO_REQ_RESET_ODOM, *(player_devaddr_t *)&(this->position_id)))
	 {
	 this->_px = 0.0;
	 this->_py = 0.0;
	 this->_pa = 0.0;
	 this->_resting = false;
	 Publish (*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_AIO_REQ_RESET_ODOM);
	 return 0;
	 }

	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_AIO_CMD_VEL, *(player_devaddr_t *)&(this->position_id)) ||
	 Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_AIO_CMD_CAR, *(player_devaddr_t *)&(this->position_id)))
	 {
	 double velocity, angle;
	 printf("cmd set velocity received\n");
	 if (hdr->subtype == PLAYER_AIO_CMD_VEL) {
	 player_AIO_cmd_vel_t *position_cmd = (player_AIO_cmd_vel_t *)data;
	 velocity = position_cmd->vel.px;
	 angle = position_cmd->vel.pa;
	 } else {
	 player_AIO_cmd_car_t *carlike_cmd = (player_AIO_cmd_car_t *)data;
	 velocity = carlike_cmd->velocity;
	 angle = carlike_cmd->angle;
	 }

	 if ((velocity == 0 && angle == 0))
	 {
	 Stop();
	 return 0;
	 }
	 printf("velocity = %f, angle = %f\n",velocity,angle);

	 double rotational_term;
	 double command_rvel, command_lvel;
	 double final_lvel = 0, final_rvel = 0;

	 // convert (tv,rv) to (lv,rv) and send to robot
	 rotational_term = angle * 0.1 / 2.0;
	 command_rvel = velocity + rotational_term;
	 command_lvel = velocity - rotational_term;

	 final_lvel = command_lvel;
	 final_rvel = command_rvel;

	 printf( "final_lvel: %f, final_rvel: %f\n", final_lvel, final_rvel );

	 SetVelocity(final_lvel, final_rvel);

	 }
	 */

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void AIODriver::Main() {
	// The main loop; interact with the device here
	printf("begin to processMessages !\n");
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  AIODriver::ProcessMessage() is
		// called on each message.
		ProcessMessages();
		//printf("ProcessMessages() excuted\n");

		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()

		// Sleep (you might, for AIO, block on a read() instead)
		usleep(100000);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
	puts("AIO driver initializing");
	AIODriver_Register(table);
	puts("AIO driver done");
	return (0);
}
}


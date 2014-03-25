/*
 * A simple example of how to write a driver that will be built as a
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
#include "exampledriver.h"

#include "background.h"
#include <stdio.h>
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class exampledriver: public ThreadedDriver {
public:

	// Constructor; need that
	exampledriver(ConfigFile* cf, int section);

	// This method will be invoked on each incoming message
	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr * hdr,
			void * data);

private:

	// Main function for device thread.
	virtual void Main();
	virtual int MainSetup();
	virtual void MainQuit();

	int SetVelocity(double lvel, double rvel);
	void Stop(void);
	int ChangeMotorState(int state);

	double _px, _py, _pa;  // integrated odometric pose (m,m,rad)
	int _powered, _resting; // If _powered is false, no constant updates. _resting means the last update was a 0,0 one.
	int position_id;
	FILE *fr;
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
exampledriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	printf("exampledriver_Init begin !!!\n");
	return ((Driver*) (new exampledriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void exampledriver_Register(DriverTable* table) {
	printf("exampledriver_Register begin !!!\n");
	table->AddDriver("exampledriver", exampledriver_Init);
	printf("exampledriver_Register end !!!\n");
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
exampledriver::exampledriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section, false, PLAYER_MSGQUEUE_DEFAULT_MAXLEN,
				PLAYER_POSITION2D_CODE) {
	memset(&this->position_id, 0, sizeof(position_id));

	printf("exampledriver init begin !!!\n");
	// Read an option from the configuration file
	if (cf->ReadDeviceAddr((player_devaddr_t *) &(this->position_id), section,
			"provides", PLAYER_POSITION2D_CODE, -1, NULL) == 0) {
		//if(this->AddInterface(*(player_devaddr_t *)&(this->position_id)) != 0)
//	{
//	    this->SetError(-1);
//	    return ;
//	}
	}
	printf("exampledriver init end !!!\n");

	return;
}

int exampledriver::SetVelocity(double lvel, double rvel) {
	MFSetServoRotaSpd(8, 1000);
	DelayMS(1);
	MFSetServoRotaSpd(9, lvel);
	DelayMS(1);
	MFServoAction();
	printf("servo action:lv = %f, rv = %f\n", lvel, rvel);
	return 0;
}
void exampledriver::Stop(void) {
	//stop the motor
	MFSetServoRotaSpd(8, 0);
	DelayMS(1);
	MFSetServoRotaSpd(9, 0);
	DelayMS(1);
	MFServoAction();

	puts("Stoped");
}

int exampledriver::ChangeMotorState(int state) {
	if (!state) {
		SetVelocity(0, 0);
		this->_powered = false;
	} else {
		this->_powered = true;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int exampledriver::MainSetup() {
	if (WRITELOG) {
		if (NULL == (fr = fopen("kelog-c", "w+"))) {
			printf("open file error !");
		}
	}
	if(WRITELOG){
		fprintf(fr,"MainSetup begin !!!\n");
	}
	puts("Example driver MainSetup");

	puts("MFInit executing");
	//init
	MFInit();
	puts("after MFInit()");
	//set servo mode
	MFSetServoMode(8, 1);
	MFSetServoMode(9, 1);
	DelayMS(1);

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.

	puts("NorthStar driver ready");

	if (WRITELOG) {
		fprintf(fr, "MainSetup done !!!\n");
	}

	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void exampledriver::MainQuit() {
	if(WRITELOG){
		fprintf(fr,"MainQuit begin !!!\n");
	}
	puts("Shutting example driver down");

	//stop the motor
	MFSetServoRotaSpd(8, 0);
	DelayMS(1);
	MFSetServoRotaSpd(9, 0);
	DelayMS(1);
	MFServoAction();
	// Here you would shut the device down by, for example, closing a
	// serial port.

	puts("Example driver has been shutdown");
	if (WRITELOG) {
		fprintf(fr,"MainQuit done !!!\n");
	}
	if (WRITELOG) {
		fclose(fr);
	}

}

int exampledriver::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_POSITION2D_REQ_GET_GEOM,
			*(player_devaddr_t *) &(this->position_id))) {
		player_position2d_geom_t geom;

		//TODO : get values from somewhere.
		geom.pose.px = -0.1;  //htons((short) (-100));
		geom.pose.py = 0;  //htons((short) (0));
		geom.pose.pyaw = 0;  //htons((short) (0));
		geom.size.sw = 0.25;  //htons((short) (250));
		geom.size.sl = 0.425;  //htons((short) (425));
		Publish(*(player_devaddr_t *) &(this->position_id), resp_queue,
				PLAYER_MSGTYPE_RESP_ACK, PLAYER_POSITION2D_REQ_GET_GEOM, &geom,
				sizeof(geom), NULL);

		return 0;
	}

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_POSITION2D_REQ_MOTOR_POWER,
			*(player_devaddr_t *) &(this->position_id))) {
		player_position2d_power_config_t * powercfg =
				reinterpret_cast<player_position2d_power_config_t *>(data);

		printf("got motor power req: %d\n", powercfg->state);
		if (ChangeMotorState(powercfg->state) < 0)
			Publish(*(player_devaddr_t *) &(this->position_id), resp_queue,
					PLAYER_MSGTYPE_RESP_NACK,
					PLAYER_POSITION2D_REQ_MOTOR_POWER);
		else
			Publish(*(player_devaddr_t *) &(this->position_id), resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_POSITION2D_REQ_MOTOR_POWER);
		return 0;
	}

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_POSITION2D_REQ_SET_ODOM,
			*(player_devaddr_t *) &(this->position_id))) {
		player_position2d_set_odom_req_t * odom =
				(player_position2d_set_odom_req_t *) data;
		this->_px = odom->pose.px;
		this->_py = odom->pose.py;
		this->_pa = odom->pose.pa;
		while (this->_pa > M_PI)
			this->_pa -= M_PI;
		while (this->_pa < -M_PI)
			this->_pa += M_PI;
		this->_resting = false;
		Publish(*(player_devaddr_t *) &(this->position_id), resp_queue,
				PLAYER_MSGTYPE_RESP_ACK, PLAYER_POSITION2D_REQ_SET_ODOM);
		return 0;
	}

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_POSITION2D_REQ_RESET_ODOM,
			*(player_devaddr_t *) &(this->position_id))) {
		this->_px = 0.0;
		this->_py = 0.0;
		this->_pa = 0.0;
		this->_resting = false;
		Publish(*(player_devaddr_t *) &(this->position_id), resp_queue,
				PLAYER_MSGTYPE_RESP_ACK, PLAYER_POSITION2D_REQ_RESET_ODOM);
		return 0;
	}

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_VEL,
			*(player_devaddr_t *) &(this->position_id))
			|| Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
					PLAYER_POSITION2D_CMD_CAR,
					*(player_devaddr_t *) &(this->position_id))) {
		double velocity, angle;
		printf("cmd set velocity received\n");
		if (hdr->subtype == PLAYER_POSITION2D_CMD_VEL) {
			player_position2d_cmd_vel_t *position_cmd =
					(player_position2d_cmd_vel_t *) data;
			velocity = position_cmd->vel.px;
			angle = position_cmd->vel.pa;
		} else {
			player_position2d_cmd_car_t *carlike_cmd =
					(player_position2d_cmd_car_t *) data;
			velocity = carlike_cmd->velocity;
			angle = carlike_cmd->angle;
		}

		if ((velocity == 0 && angle == 0)) {
			Stop();
			return 0;
		}
		printf("velocity = %f, angle = %f\n", velocity, angle);

		double rotational_term;
		double command_rvel, command_lvel;
		double final_lvel = 0, final_rvel = 0;

		// convert (tv,rv) to (lv,rv) and send to robot
		rotational_term = angle * 0.1 / 2.0;
		command_rvel = velocity + rotational_term;
		command_lvel = velocity - rotational_term;

		final_lvel = command_lvel;
		final_rvel = command_rvel;

		printf("final_lvel: %f, final_rvel: %f\n", final_lvel, final_rvel);

		SetVelocity(final_lvel, final_rvel);

	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void exampledriver::Main() {
	// The main loop; interact with the device here
	if(WRITELOG){
		fprintf(fr,"Main begin !!!\n");
	}
	printf("exampledriver::Main begin !!!\n");
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  exampledriver::ProcessMessage() is
		// called on each message.
		ProcessMessages();
		printf("ProcessMessages() excuted\n");

		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()

		// Sleep (you might, for example, block on a read() instead)
		usleep(100000);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
	puts("Example driver initializing !!!");
	exampledriver_Register(table);
	puts("Example driver done !!!");
	return (0);
}
}


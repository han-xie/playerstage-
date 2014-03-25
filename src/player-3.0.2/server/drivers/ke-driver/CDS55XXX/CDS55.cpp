/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2003  
 *     Brian Gerkey
 *                      
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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
//#include <libplayerinterface/player_interfaces.h>

#include "CDS55.h"
#include "../upt-provide-function/background.h"

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
CDS55_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	return ((Driver*) (new CDS55(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void CDS55_Register(DriverTable* table) {
	table->AddDriver("CDS55", CDS55_Init);
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
/*
 CDS55::CDS55(ConfigFile* cf, int section)
 : ThreadedDriver(cf, section, false, PLAYER_MSGQUEUE_DEFAULT_MAXLEN,
 PLAYER_POSITION2D_CODE)
 {
 // Read an option from the configuration file
 this->foop = cf->ReadInt(section, "foo", 0);

 return;
 }
 */

CDS55::CDS55(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section)

{
	// Create my position interface
	if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
			PLAYER_POSITION2D_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	}
	if (this->AddInterface(this->m_position_addr)) {
		this->SetError(-1);
		return;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int CDS55::MainSetup() {
	PLAYER_MSG0(3, "CDS55 driver initialising  ...");

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.

	PLAYER_MSG0(3, "CDS55 driver ready ...");
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void CDS55::MainQuit() {
	PLAYER_MSG0(3, "shutting CDS55 driver down ...");

	// Here you would shut the device down by, for example, closing a
	// serial port.

	PLAYER_MSG0(3, "CDS55 driver has been shutdown ...");
}

void CDS55::SetSpeedCmd(player_position2d_cmd_vel_t cmd) {
	/*because now not mature so
	 * void MFSetServoRotaSpd(int inID,int inSpeed);
	 * cmd.vel.px = inID
	 * cmd.vel.pa = inSpeed
	 * cmd.vel.px && cmd.vel.pa =0 -> rot
	 */
	int id = (int) cmd.vel.px;
	int speed = (int) cmd.vel.pa;
	PLAYER_MSG3(3, "px py pa", cmd.vel.px, cmd.vel.py, cmd.vel.pa);

	if (0 == id && 0 == speed) {
		//initial to rot
		MFInit();
		DelayMS(200);
		MFSetServoMode(8, 1);
		DelayMS(200);
		MFSetServoMode(9, 1);
		DelayMS(200);
	}

	else {
		//void MFSetServoRotaSpd(int inID,int inSpeed);
		MFSetServoRotaSpd(id, speed);
		DelayMS(10);
		MFServoAction();
	}
	return;
}

int CDS55::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_POS, this->device_addr)) {
		PLAYER_MSG0(3,
				"CDS55:PLAYER_POSITION2D_CMD_POS don't supports right now  ...");
		return 0;

	}

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_VEL, this->device_addr)) {
		PLAYER_MSG0(3, "CDS55:PLAYER_POSITION2D_CMD_VEL ...");
		player_position2d_cmd_vel_t* cmd = (player_position2d_cmd_vel_t*) data;
		this->SetSpeedCmd(*cmd);
		return 0;

	}
	//other message just return 0
	if (1) {
		PLAYER_MSG0(3, "CDS55:others message don't supports right now  ...");
		return 0;
	}

	return (-1);
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void CDS55::Main() {
	// The main loop; interact with the device here
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  CDS55::ProcessMessage() is
		// called on each message.
		ProcessMessages();

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
	puts("Example driver initializing");
	CDS55_Register(table);
	puts("Example driver done");
	return (0);
}
}


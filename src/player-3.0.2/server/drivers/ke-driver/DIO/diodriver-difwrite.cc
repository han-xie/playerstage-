/*
 * A simple DIO of how to write a driver that will be built as a
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
#include "diodriver.h"

#include "background.h"
#include <stdio.h>
#include <fcntl.h>
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class diodriver: public ThreadedDriver {
public:

	// Constructor; need that
	diodriver(ConfigFile* cf, int section);

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
	uint32_t getDIOValue(uint32_t port);
	void writeMessagehrToFile(player_msghdr * hdr, void * data);
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
diodriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	printf("diodriver_Init begin !!!\n");
	return ((Driver*) (new diodriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void diodriver_Register(DriverTable* table) {
	printf("diodriver_Register begin !!! \n");
	table->AddDriver("diodriver", diodriver_Init);
	printf("diodriver_Register end !!!\n");
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
 "provides", PLAYER_DIO_CODE, -1, NULL) == 0) {
 if (this->AddInterface(*(player_devaddr_t *) &(this->position_id))
 != 0) {
 this->SetError(-1);
 return;
 }
 }
 }
 */

diodriver::diodriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section, false, PLAYER_MSGQUEUE_DEFAULT_MAXLEN,
				PLAYER_DIO_CODE) {
	memset(&this->position_id, 0, sizeof(position_id));

	printf("DIO driver initial begin\n");
	// Read an option from the configuration file
	if (cf->ReadDeviceAddr((player_devaddr_t *) &(this->position_id), section,
			"provides", PLAYER_DIO_CODE, -1, NULL) == 0) {
		printf("ReadDeviceAddr  success !!! \n");
	}
	printf("DIO driver initial end\n");
	return;
}

/*
 *
 diodriver::diodriver(ConfigFile* cf, int section) :
 ThreadedDriver(cf, section) {
 //memset(&this->position_id, 0, sizeof(position_id));
 //PLAYER_MSG0(3, "INITIALIZING INTERFACE ...");

 printf("discovering drivers\n");

 // Create my position interface
 if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
 PLAYER_DIO_CODE, -1, NULL) != 0) {
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
 */

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int diodriver::MainSetup() {
	puts("DIO driver MainSetup");

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.

	MFInit();
	DelayMS(10);
	MFDigiInit(1000);
	DelayMS(10);
	MFSetPortDirect(0x00000FFF);
	DelayMS(10);

	puts("NorthStar driver ready");

	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void diodriver::MainQuit() {
	puts("Shutting DIO driver down");

	// Here you would shut the device down by, for DIO, closing a
	// serial port.

	puts("DIO driver has been shutdown");
}
uint32_t diodriver::getDIOValue(uint32_t port) {
	//int vt = MFGetDigiInput(this->m_position_addr.index);
	return port;
}
void diodriver::writeMessagehrToFile(player_msghdr * hdr, void * data) {
	if (WRITELOG) {
		char *buf;

	}
}
int diodriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.
	/*
	 #define 	PLAYER_DIO_CMD_STATE   1
	 Data: state (PLAYER_DIO_DATA_STATE)
	 #define 	PLAYER_DIO_DATA_STATE   1
	 Data: state (PLAYER_DIO_DATA_STATE) */
	if (WRITELOG) {
		outputlog = open("kelog", O_RDWR | O_CREAT | O_TRUNC);
		if (0 == outputlog) {
			printf("can not open !\n");
			close(outputlog);
			return 0;
		}
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_DIO_CMD_VALUES,
			*(player_devaddr_t *) &(this->position_id))) {
		if (WRITELOG) {
			char buf[] = "porcessMessage PLAYER_DIO_CMD_STATE !!!\n";
			printf("write bytes : %d \n", write(outputlog, buf, sizeof(buf)));
		}

		player_dio_cmd_t *pdc = (player_dio_cmd_t *) data;
		player_dio_data_t pdd;
		pdd.count = getDIOValue(pdc->count);

		if (WRITELOG) {
			char buf[] =
					"porcessMessage PLAYER_DIO_CMD_STATE - publish begin \n";
			printf("write bytes : %d \n", write(outputlog, buf, sizeof(buf)));
		}

		//Publish(*(player_devaddr_t *)&(this->position_id), resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_DIO_CMD_STATE, &vt, sizeof(vt),NULL);

		Publish(resp_queue, hdr, &pdd, NULL);
		if (WRITELOG) {
			char buf[] = "porcessMessage PLAYER_DIO_CMD_STATE - publish end \n";
			printf("write bytes : %d \n", write(outputlog, buf, sizeof(buf)));
		}

		if (WRITELOG) {
			close(outputlog);
		}
		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
			PLAYER_DIO_DATA_VALUES,
			*(player_devaddr_t *) &(this->position_id))) {
		if (WRITELOG) {
			char buf[] = "porcessMessage PLAYER_DIO_DATA_VALUES \n";
			printf("write bytes : %d \n", write(outputlog, buf, sizeof(buf)));
		}
		if (WRITELOG) {
			close(outputlog);
		}
		return 0;
	} else {
		if (WRITELOG) {
			char buf[] = "porcessMessage not know message \n";
			printf("write bytes : %d \n", write(outputlog, buf, sizeof(buf)));
		}
		if (WRITELOG) {
			close(outputlog);
		}
		return 0;
	}
	if (WRITELOG) {
		close(outputlog);
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void diodriver::Main() {
	// The main loop; interact with the device here
	printf("begin to processMessages !\n");
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  diodriver::ProcessMessage() is
		// called on each message.
		ProcessMessages();
		//printf("ProcessMessages() excuted\n");

		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()

		// Sleep (you might, for DIO, block on a read() instead)
		usleep(100000);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
	puts("DIO driver initializing");
	diodriver_Register(table);
	puts("DIO driver done");
	return (0);
}
}


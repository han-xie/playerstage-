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
#include "multidriver.h"

//#include "background.h"
#include <stdio.h>
#include <fcntl.h>
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class multidriver: public ThreadedDriver {
public:

	// Constructor; need that
	multidriver(ConfigFile* cf, int section);

	// This method will be invoked on each incoming message
	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr * hdr,
			void * data);

private:

	// Main function for device thread.
	virtual void Main();
	virtual int MainSetup();
	virtual void MainQuit();

	// My position interface
	player_devaddr_t m_dio_addr;
	player_devaddr_t m_aio_addr;
	player_devaddr_t m_position_addr;
	player_devaddr_t m_opaque_addr;

	//uint32_t getDIOValue(uint32_t port);
	//uint32_t getAIOValue(uint32_t port);
	uint32_t getDIOValue(player_dio_cmd_t *pdioc, player_dio_data_t *pr);
	uint32_t getAIOValue(player_aio_cmd_t *paioc, player_aio_data_t *pr);

	int dealDIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealAIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealPOSmessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealOpaqueCommand(my_opaque_st *popa);
	int dealOpaqueReq(my_opaque_st *popa, my_opaque_st *pr);
	void writeMessagehrToFile(player_msghdr * hdr, void * data);

	int frontl;
	int frontr;
	int behindl;
	int behindr;
#ifdef WRITELOG
	//all messages count
	int mc;
	//don't know messages count
	int nmc;
	int aiomc;
	int naiomc;
	int diomc;
	int ndiomc;
	int posmc;
	int nposmc;
	int opaquemc;
	int nopaquemc;
#endif
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
multidriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	printf("multidriver_Init begin !!!\n");
	return ((Driver*) (new multidriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void multidriver_Register(DriverTable* table) {
	printf("multidriver_Register begin !!! \n");
	table->AddDriver("multidriver", multidriver_Init);
	printf("multidriver_Register end !!!\n");
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.

/*
 * if (cf->ReadDeviceAddr(&(this->m_position_addr), section,
 "provides", PLAYER_POSITION2D_CODE, -1, NULL) != 0)
 {
 this->SetError(-1);
 return;
 }
 if (this->AddInterface(this->m_position_addr))
 {
 this->SetError(-1);
 return;
 }

 // Create my laser interface
 if (cf->ReadDeviceAddr(&(this->m_laser_addr), section,
 "provides", PLAYER_LASER_CODE, -1, NULL) != 0)
 {
 this->SetError(-1);
 return;
 }
 if (this->AddInterface(this->m_laser_addr))
 {
 this->SetError(-1);
 return;
 }
 * */

multidriver::multidriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {

	printf("DIO driver initial begin\n");
	// Read an option from the configuration file
	if (cf->ReadDeviceAddr(&(this->m_dio_addr), section, "provides",
			PLAYER_DIO_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	}
	if (this->AddInterface(this->m_dio_addr)) {
		this->SetError(-1);
		return;
	}

	if (cf->ReadDeviceAddr(&(this->m_aio_addr), section, "provides",
			PLAYER_AIO_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	}
	if (this->AddInterface(this->m_aio_addr)) {
		this->SetError(-1);
		return;
	}
	if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
			PLAYER_POSITION2D_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	}
	if (this->AddInterface(this->m_position_addr)) {
		this->SetError(-1);
		return;
	}
	if (cf->ReadDeviceAddr(&(this->m_opaque_addr), section, "provides",
			PLAYER_OPAQUE_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	}
	if (this->AddInterface(this->m_opaque_addr)) {
		this->SetError(-1);
		return;
	}
	printf("DIO driver initial end\n");

#ifdef WRITELOG
	this->mc = 0;
	this->nmc = 0;
	this->diomc = 0;
	this->ndiomc = 0;
	this->aiomc = 0;
	this->naiomc = 0;
	this->posmc = 0;
	this->nposmc = 0;
	this->opaquemc = 0;
	this->nopaquemc = 0;
#endif

	this->frontl = 8;
	this->frontr = 9;
	this->behindl = 10;
	this->behindr = 11;

	return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int multidriver::MainSetup() {
	puts("DIO driver MainSetup");

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.


	puts("NorthStar driver ready");

	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void multidriver::MainQuit() {
	puts("Shutting DIO driver down");

	// Here you would shut the device down by, for DIO, closing a
	// serial port.
#ifdef WRITELOG
	printf(
			"all messages count %d , aio message %d , dio message %d , position message %d , opaque message %d \n",
			this->mc, this->aiomc, this->diomc, this->posmc, this->opaquemc);
	printf(
			"not messages %d , not aio %d , not dio %d , not pos %d , not opaque %d \n",
			this->nmc, this->naiomc, this->ndiomc, this->nposmc,
			this->nopaquemc);
#endif

	puts("DIO driver has been shutdown");
}

uint32_t multidriver::getDIOValue(player_dio_cmd_t *pdioc,
		player_dio_data_t *pr) {
#ifdef WRITELOG
	printf("comming dio message : %d , %d\n", pdioc->count, pdioc->digout);
#endif
	pr->count = 4;
#ifdef WRITELOG
	printf("outing dio message : %d , %d\n", pr->count, pr->bits);
#endif
	return 0;
}

void multidriver::writeMessagehrToFile(player_msghdr * hdr, void * data) {
#ifdef WRITELOG
	char *buf;

#endif
}

uint32_t multidriver::getAIOValue(player_aio_cmd_t *paioc,
		player_aio_data_t *pr) {
#ifdef WRITELOG
	printf("comming aio message : %d , %f\n", paioc->id, paioc->voltage);
#endif
	pr->voltages_count = 10;
	float t = 0.0;
	pr->voltages = &t;

#ifdef WRITELOG
	printf("outing aio message : %d , %f\n", pr->voltages_count, pr->voltages);
#endif
	return 0;
}

int multidriver::dealAIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_AIO_CMD_STATE,
			*(player_devaddr_t *) &(this->m_aio_addr))) {
		player_aio_data_t aiod;
		player_aio_cmd_t *aioc = (player_aio_cmd_t*) data;
		this->getAIOValue(aioc, &aiod);
		Publish(this->m_aio_addr, resp_queue, PLAYER_MSGTYPE_DATA,
				PLAYER_AIO_DATA_STATE, (void*) &aiod, sizeof(aiod), NULL);
		return 0;
	} else {
#ifdef WRITELOG
		this->naiomc++;
#endif
		return 0;
	}
	return -1;
}

int multidriver::dealOpaqueCommand(my_opaque_st *popa) {
	/*
	 * struct my_opaque_st{
	 int type;
	 int subtype;
	 int p1;
	 int p2;
	 };
	 void MFCapSetH(int inHmax,int inHmin);  -subtype 1
	 void MFCapSetS(int inSmax,int inSmin);  -2
	 void MFCapSetI(int inImax,int inImin);  -3
	 void MFCapSetMode(int inMode);          -4
	 int MFCapGetCenterX();                  -5
	 int MFCapGetCenterY();                  -6
	 int MFCapGetSum();                      -7
	 * */
#ifdef WRITELOG
	printf("coming opaque command : %d %d %d %d\n", popa->type, popa->subtype,
			popa->p1, popa->p2);
#endif
	switch (popa->subtype) {
	case 1: {
		//MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case 2: {
		//MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case 3: {
		//MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case 4: {
		//MFCapSetMode(popa->p1);
		break;
	}
	default:
		break;
	}
	return 0;
}
int multidriver::dealOpaqueReq(my_opaque_st *popa, my_opaque_st *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case 5: {
		pr->p1 = pr->p1++;
		break;
	}
	case 6: {
		pr->p1 = pr->p1++;
		break;
	}
	case 7: {
		pr->p1 = pr->p1++;
		break;
	}
	default: {
		break;
	}
	}
#ifdef WRITELOG
	printf("comming opaque req : %d %d %d %d \n", popa->type, popa->subtype,
			popa->p1, popa->p2);
	printf("outing opaque message : %d %d %d\n", popa->type, popa->subtype,
			popa->p1);
#endif
	return 0;
}

int multidriver::dealOpaqueMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 * struct my_opaque_st{
	 int type;
	 int subtype;
	 int p1;
	 int p2;
	 };
	 * */
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	if (opaquedata->data_count != sizeof(my_opaque_st)) {
#ifdef WRITELOG
		printf("coming opaque messages size wrong ! \n");
		this->nopaquemc++;
		return -1;
#endif
	}
	my_opaque_st *popa = (my_opaque_st *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
#ifdef WRITELOG
		int t = sizeof(my_opaque_st);
		printf("size %d, ", t);
		char *pt = (char *) data;
		for (int i = 0; i < t; i++) {
			if (NULL != data)
				printf("%x ", pt[i]);
		}
		printf("\n");
#endif

		dealOpaqueCommand(popa);

		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_OPAQUE_REQ_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		my_opaque_st res;
		player_opaque_data_t opaqueRes;

#ifdef WRITELOG
		int t = sizeof(my_opaque_st);
		printf("size %d, ", t);
		char *pt = (char *) data;
		for (int i = 0; i < t; i++) {
			if (NULL != data)
				printf("%x ", pt[i]);
		}
		printf("\n");
#endif

		dealOpaqueReq(popa, &res);

		/*Publish(this->m_opaque_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, (void*) &res, sizeof(res), NULL);*/
		/*Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
				 reinterpret_cast<void*>(&opaqueRes), size, NULL);*/
		opaqueRes.data_count = sizeof(my_opaque_st);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("response data_count %d , size %d\n",opaqueRes.data_count,size);
#endif
		/*Publish(this->m_opaque_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, reinterpret_cast<void*>(&opaqueRes),
				size, NULL);*/
		  Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA, PLAYER_OPAQUE_DATA_STATE,
		  				 reinterpret_cast<void*>(&opaqueRes), size, NULL);

		return 0;
	} else {
#ifdef WRITELOG
		this->nopaquemc++;
#endif
		return 0;
	}
	return -1;
}

int multidriver::dealDIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_DIO_CMD_VALUES,
			*(player_devaddr_t *) &(this->m_dio_addr))) {

		player_dio_cmd_t *pdc = (player_dio_cmd_t *) data;
		player_dio_data_t pdd;
		this->getDIOValue(pdc, &pdd);

		Publish(this->m_dio_addr, resp_queue, PLAYER_MSGTYPE_DATA,
				PLAYER_DIO_DATA_VALUES, (void*) &pdd, sizeof(pdd), NULL);

		return 0;
	} else {
#ifdef WRITELOG
		this->ndiomc++;
#endif
		return 0;
	}
	return -1;
}

int multidriver::dealPOSmessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {

	if (hdr->type == (uint8_t) PLAYER_MSGTYPE_CMD) {
		player_position2d_cmd_vel *pposc = (player_position2d_cmd_vel *) data;
#ifdef WRITELOG
		printf("--postion-- %f , %f , %f \n", pposc->vel.px, pposc->vel.py,
				pposc->vel.pa);
#endif
		/*px
		 * 1.0 - forward
		 * 2.0 - backward
		 * 3.0 - stop
		 * 4.0 - left round
		 * 5.0 - right round
		 *
		 * pa - velocity
		 */

		if (1.0 == pposc->vel.px) {

		} else if (2.0 == pposc->vel.px) {

		} else if (3.0 == pposc->vel.px) {

		} else if (4.0 == pposc->vel.px) {

		} else if (5.0 == pposc->vel.px) {

		} else {
#ifdef WRITELOG
			this->nposmc++;
#endif
			return -1;
		}

		return 0;
	}
#ifdef WRITELOG
	this->nposmc++;
#endif
	return -1;
}

int multidriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Process messages here.  Send a response if necessary, using Publish().
	// If you handle the message successfully, return 0.  Otherwise,
	// return -1, and a NACK will be sent for you, if a response is required.
#ifdef WRITELOG
	printf("--comming message--");
	this->mc++;
#endif

	if (hdr->addr.interf == this->m_dio_addr.interf) {

#ifdef WRITELOG
		printf("--dio message--\n");
		this->diomc++;
#endif
		if (-1 == dealDIOmessages(resp_queue, hdr, data)) {
#ifdef WRITELOG
			printf("--dio message can not deal--\n");
#endif
			return -1;
		}
		return 0;
	} else if (hdr->addr.interf == this->m_aio_addr.interf) {
#ifdef WRITELOG
		printf("--aio message--\n");
		this->aiomc++;
#endif
		if (-1 == dealAIOmessages(resp_queue, hdr, data)) {
#ifdef WRITELOG
			printf("--aio message can not deal--\n");
#endif
			return -1;
		}
		return 0;
	} else if (hdr->addr.interf == this->m_position_addr.interf) {
#ifdef WRITELOG
		printf("--position message --\n");
		this->posmc++;
#endif
		if (-1 == dealPOSmessages(resp_queue, hdr, data))
#ifdef WRITELOG
			printf("--position message can not deal--\n");
#endif
		return 0;
	} else if (hdr->addr.interf == this->m_opaque_addr.interf) {
#ifdef WRITELOG
		printf("--opaque message --\n");
		this->opaquemc++;
#endif
		if (-1 == dealOpaqueMessages(resp_queue, hdr, data))
#ifdef WRITELOG
			printf("--opaque message can not deal--\n");
#endif
		return 0;
	} else {
#ifdef WRITELOG
		printf("--don not know message--\n");
		this->nmc++;
#endif
		return 0;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void multidriver::Main() {
	// The main loop; interact with the device here
	printf("begin to processMessages !\n");
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  multidriver::ProcessMessage() is
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
	multidriver_Register(table);
	puts("DIO driver done");
	return (0);
}
}


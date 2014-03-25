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
#include "../driver-client/CYZXInter.h"

#include "background.h"
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

	int dealOpaqueMFCAPCommand(myOpaqueSt *popa);
	int dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res);

	int dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *res);

	int dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res);

	int dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaquePosition2d(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

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
	int opaquemfcapmc;
	int nopaquemfcapmc;
	int opaquediomc;
	int nopaquediomc;
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
	} else {
		if (this->AddInterface(this->m_opaque_addr)) {
			this->SetError(-1);
			return;
		}
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
	this->opaquemfcapmc = 0;
	this->nopaquemfcapmc = 0;
	this->opaquediomc = 0;
	this->nopaquediomc = 0;
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

	/*
	 MFInit();
	 for (int i = 0; i < 8; i++) {
	 MFADEnable(i);
	 }
	 MFADInit(100);
	 MFDigiInit(100);
	 MFSetPortDirect(0x00000000);//set the GPIO,0~7 for output,and 8~11 for input
	 DelayMS(100);
	 */
	MFCapOpen();
	MFInit();
	MFSetPortDirect(0x00000000);
	for (int i = 0; i < 8; i++) {
		MFADEnable(i);
	}
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	MFSetServoMode(this->frontl, 1);
	MFSetServoMode(this->frontr, 1);
	MFSetServoMode(this->behindl, 1);
	MFSetServoMode(this->behindr, 1);
	DelayMS(100);

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
	pr->count = MFGetDigiInput(pdioc->count);
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
	pr->voltages_count = MFGetAD(paioc->id) / 10;
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

int multidriver::dealOpaqueMFCAPCommand(myOpaqueSt *popa) {
#ifdef WRITELOG
	printf("coming opaque command : %d %d %d %d\n", popa->type, popa->subtype,
			popa->p1, popa->p2);
#endif
	switch (popa->subtype) {
	case 1: {
		MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case 2: {
		MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case 3: {
		MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case 4: {
		MFCapSetMode(popa->p1);
		break;
	}
	default:
		break;
	}
	return 0;
}

int multidriver::dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case CAPSETHF: {
		MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case CAPSETSF: {
		MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case CAPSETIF: {
		MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case CAPSETMODEF: {
		MFCapSetMode(popa->p1);
		break;
	}
	case CAPGETCENTERXF: {
		pr->p1 = MFCapGetCenterX();
		break;
	}
	case CAPGETCENTERYF: {
		pr->p1 = MFCapGetCenterY();
		break;
	}
	case CAPGETSUMF: {
		pr->p1 = MFCapGetSum();
		break;
	}
	default: {
		break;
	}
	}
	return 0;
}
int multidriver::dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case 5: {
		pr->p1 = MFCapGetCenterX();
		break;
	}
	case 6: {
		pr->p1 = MFCapGetCenterY();
		break;
	}
	case 7: {
		pr->p1 = MFCapGetSum();
		break;
	}
	default: {
		break;
	}
	}
#ifdef WRITELOG
	printf("comming opaque req : %d %d %d %d \n", popa->type, popa->subtype,
			popa->p1, popa->p2);
	printf("outing opaque message : %d %d %d %d\n", pr->type, pr->subtype,
			pr->p1, pr->p2);
#endif
	return 0;
}

int multidriver::dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueMFCap size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueMFCAPComReq(popa, &res);
		if ((popa->subtype == CAPGETCENTERXF)
				|| (popa->subtype == CAPGETCENTERYF)
				|| (popa->subtype == CAPGETSUMF)) {
			opaqueRes.data_count = sizeof(myOpaqueSt);
			uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
					+ opaqueRes.data_count;
			opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
			printf("opaqueMFCap response data_count %d , size %d\n",
					opaqueRes.data_count, size);
			printf("outing MFCap message %d %d %d %d\n", res.type, res.subtype,
					res.p1, res.p2);
#endif
			Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
					PLAYER_OPAQUE_DATA_STATE,
					reinterpret_cast<void*>(&opaqueRes), size, NULL);
		}
		return 0;
	} else {
#ifdef WRITELOG
		this->nopaquemc++;
#endif
		return 0;
	}
	return -1;
}
int multidriver::dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueAIO size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueAIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaqueAIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing AIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
				PLAYER_OPAQUE_DATA_STATE, reinterpret_cast<void*>(&opaqueRes),
				size, NULL);
	}
	return -1;
}
int multidriver::dealOpaquePosition2d(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaquePosition2d size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaquePosition2dComReq(popa, &res);
	}
	return -1;
}

int multidriver::dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueDIO size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueDIOComReq(popa, &res);

		if (popa->subtype == DIOGETDIGIINPUTF) {
			opaqueRes.data_count = sizeof(myOpaqueSt);
			uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
					+ opaqueRes.data_count;
			opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
			printf("opaqueDIO response data_count %d , size %d\n",
					opaqueRes.data_count, size);
			printf("outing DIO message %d %d %d %d\n", res.type, res.subtype,
					res.p1, res.p2);
#endif
			Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
					PLAYER_OPAQUE_DATA_STATE,
					reinterpret_cast<void*>(&opaqueRes), size, NULL);
		}

	}
	return -1;
}

inline int deaperDealAIO(int t, int type) {
	if (type == whatKindAIOAIOINFRAREDR) {
		float volt = 0.0;
		t *= 5;
		float ft = (float) t;
		volt = ft / 1023;
#ifdef WRITELOG
		printf("t %d , ft %f , volt %f\n", t, ft, volt);
#endif

		int tmp = 0;

		if (volt > 2.45)
			return 10;
		else if (volt > 1.4) {
			ft = 20 - 10 * ((volt - 1.4) / 1.05);
			return (int) ft;
		} else if (volt > 1.0) {
			ft = 30 - 10 * ((volt - 1.0) / 0.4);
			return (int) ft;
		} else if (volt > 0.75) {
			ft = 40 - 10 * ((volt - 0.75) / 0.25);
			return (int) ft;
		} else if (volt > 0.6) {
			ft = 50 - 10 * ((volt - 0.6) / 0.15);
			return (int) ft;
		} else if (volt > 0.53) {
			ft = 60 - 10 * ((volt - 0.53) / 0.07);
			return (int) ft;
		} else if (volt > 0.45) {
			ft = 70 - 10 * ((volt - 0.45) / 0.08);
			return (int) ft;
		} else if (volt > 0, 4) {
			ft = 80 - 10 * ((volt - 0.4) / 0.05);
			return (int) ft;
		} else
			return 80;
	} else {
		return t;
	}
}
int multidriver::dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	res->p1 = MFGetAD(popa->p1);
	if (popa->p2 != whatKindAIOAIODEF)
		res->p1 = deaperDealAIO(res->p1, popa->p2);
	return 0;
}
int multidriver::dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	if (POS2DSERVOMODESERVOF == popa->subtype) {
		MFSetServoMode(this->frontl, 0);
		MFSetServoMode(this->frontr, 0);
		MFSetServoMode(this->behindl, 0);
		MFSetServoMode(this->behindr, 0);
	} else if (POS2DSERVOMODEMOTOF == popa->subtype) {
		MFSetServoMode(this->frontl, 1);
		MFSetServoMode(this->frontr, 1);
		MFSetServoMode(this->behindl, 1);
		MFSetServoMode(this->behindr, 1);
	} else if (POS2DFORWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, popa->p1);
		MFSetServoRotaSpd(this->frontr, popa->p1);
		MFSetServoRotaSpd(this->behindl, popa->p1);
		MFSetServoRotaSpd(this->behindr, popa->p1);
		MFServoAction();
	} else if (POS2DBACKWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, -(popa->p1));
		MFSetServoRotaSpd(this->frontr, -(popa->p1));
		MFSetServoRotaSpd(this->behindl, -(popa->p1));
		MFSetServoRotaSpd(this->behindr, -(popa->p1));
		MFServoAction();
	} else if (POS2DTURNLEFTF == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, popa->p1);
		MFSetServoRotaSpd(this->frontr, -(popa->p1));
		MFSetServoRotaSpd(this->behindl, popa->p1);
		MFSetServoRotaSpd(this->behindr, -(popa->p1));
		MFServoAction();
	} else if (POS2DTURNRIGHT == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, -(popa->p1));
		MFSetServoRotaSpd(this->frontr, popa->p1);
		MFSetServoRotaSpd(this->behindl, -(popa->p1));
		MFSetServoRotaSpd(this->behindr, popa->p1);
		MFServoAction();
	} else if (POS2DSTOP == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, 0);
		MFSetServoRotaSpd(this->frontr, 0);
		MFSetServoRotaSpd(this->behindl, 0);
		MFSetServoRotaSpd(this->behindr, 0);
		MFServoAction();
	} else {
		return -1;
	}
	return 0;
}
int multidriver::dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	if (DIOSETPORTDIRECTF == popa->subtype) {
		MFSetPortDirect(popa->p1);
	} else if (DIOGETDIGIINPUTF == popa->subtype) {
		pr->p1 = MFGetDigiInput(popa->p1);
	} else if (DIODIGIOUTPUTF == popa->subtype) {
		MFDigiOutput(popa->p1, popa->p2);
	} else {
		return -1;
	}
	return 0;
}

int multidriver::dealOpaqueMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 * struct myOpaqueSt{
	 int type;
	 int subtype;
	 int p1;
	 int p2;
	 };
	 * */
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	if (opaquedata->data_count != sizeof(myOpaqueSt)) {
#ifdef WRITELOG
		printf("coming opaque messages size wrong ! \n");
		this->nopaquemc++;
		return -1;
#endif
	}
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;
	if (popa->type == MYOPAQUESTTYPEMFCAP) {
		dealOpaqueMFCAP(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEAIO) {
		dealOpaqueAIO(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEDIO) {
		dealOpaqueDIO(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEPOS) {
		dealOpaquePosition2d(resp_queue, hdr, data);
		return 0;
	} else {
#ifdef WRITELOG
		printf("coming opaque messages type wrong ! \n");
#endif
		return -1;
	}

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
			MFSetServoRotaSpd(this->frontl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, (int) pposc->vel.pa);
			MFServoAction();
		} else if (2.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, -(int) pposc->vel.pa);
			MFServoAction();
		} else if (3.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, 0);
			MFSetServoRotaSpd(this->frontr, 0);
			MFSetServoRotaSpd(this->behindl, 0);
			MFSetServoRotaSpd(this->behindr, 0);
			MFServoAction();
		} else if (4.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, -(int) pposc->vel.pa);
			MFServoAction();
		} else if (5.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, (int) pposc->vel.pa);
			MFServoAction();
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
		if (-1 == dealPOSmessages(resp_queue, hdr, data)) {
#ifdef WRITELOG
			printf("--position message can not deal--\n");
#endif
			return -1;
		}
		return 0;
	} else if (hdr->addr.interf == this->m_opaque_addr.interf) {
#ifdef WRITELOG
		printf("--opaque message --\n");
		this->opaquemc++;
#endif
		if (-1 == dealOpaqueMessages(resp_queue, hdr, data)) {
#ifdef WRITELOG
			printf("--opaque message can not deal--\n");
#endif
			return -1;
		}
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


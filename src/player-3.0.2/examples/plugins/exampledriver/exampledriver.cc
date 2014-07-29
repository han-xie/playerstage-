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
#define LABLESTRING "CYZXSPECIALSTRING:"
#define LOGFILENAME "/mnt/yaffs/test/CYZXexampledriver.log"

#define WRITELOG 1
#define DEBUGTEMP 1
#define WAITTIMES 4

#define AIOPORTSMIN 0
#define AIOPORTSMAX 7
#define DIOPORTSMIN 0
#define DIOPORTSMAX 11

#define SLEEPTIME1 100
#define SLEEPTIME2 10000
#define WHILELOOP  10000
//mp3
#define MP3MAXLENGTH 20
//LCD
#define LCDMAXLENGTH 16

//define struct myOpaqueSt type
#define MYOPAQUESTTYPEMFCAP 1  //MFCap
#define MYOPAQUESTTYPEAIO 2    //aio
#define MYOPAQUESTTYPEDIO 3    //dio
#define MYOPAQUESTTYPEPOS 4    //position2d
#define MYOPAQUEBk        5    //backgroud's special function
#define MYOPAQUESR        6    //speech recognize
#define MAXCOMMANDNUMBER 256

#define SRSTARTF 1       //void SRStart();
#define SRSTOPF  2   //void SRStop();
#define SRPAUSEF  3       //bool SRPause();
#define SRCONTINUEF    4  //bool SRContinue();
#define SRINSERTTEXTF    5 //bool SRInsertText(int index,const char*item,int insize);
#define SRGETRESINDEXF  6 //int  SRGetResIndex();
#define SRCLEARITEMSF    7//void SRClearItems();
#define SRISSTARTEDF    8//bool SRisStarted();
#define BKDELAYMSF 9      //void bkDelayMS(int inMS);
#define BKLCDDISF 10       //void bkLCDdis(const char*item,int insize);
#define BKUSONICENABLEF 11  //void bkUSonicEnable();
#define BKGETULTRASONICF 12//int  bkGetUltrasonic();
#define BKCOMSENDF 13       //void bkComSend(const void *pBuffer, const int iLength);
#define BKMP3PLAYF 14       //void bkMp3Play(const char*item,int insize);
#define BKMP3STOPF  15      //void bkMp3Stop();
#define BKMFSETSERVOMODEF  16//void bkMFSetServoMode(int inID,int inMode);
#define BKMFSETSERVOPOSF 17//void bkMFSetServoPos(int inID,int inPos,int inSpeed);
#define BKMFSETSERVOROTASPDF 18//void bkMFSetServoRotaSpd(int inID,int inSpeed);
#define BKMFGETSERVOPOSF 19//int bkMFGetServoPos(int inID);
#define BKMFSERVOACTIONF 20//void bkMFServoAction();
#define BKMFSETSERVOROTASPDTIMEF 21 //void bkMFSetServoRotaSpdTime(int inID,int inSpeed,unsigned int seconds);
#define AIOCOMINFRAREDF 22            //int aioComInfrared(int port);
#define AIOCOMF 23                    //int aioCom(int port);
#define AIOCOMTF 24                   //int aioComT(int port, int whatKindAIO);
#define whatKindAIOAIODEF 25         	  //int whatKindAIO   默认值
#define whatKindAIOAIOINFRAREDR  26  	  //int whatKindAIO   红外测距传感器
#define DIOSETPORTDIRECTF 27     //void dioSetPortDirect(unsigned int inData);
#define DIOGETDIGIINPUTF 28      //int dioGetDigiInput(int port);
#define DIODIGIOUTPUTF 29       //void dioDigiOutput(int port, int inVal);
#define POS2DSERVOMODESERVOF 30     //void pos2dServoModeServo();
#define POS2DSERVOMODEMOTOF 31      //void pos2dServoModeMoto();
#define POS2DFORWARDF 32//void pos2dForward(int speed);
#define POS2DBACKWARDF 33          //void pos2dBackward(int speed);
#define POS2DTURNLEFTF 34           //void pos2dTurnLeft(int speed);
#define POS2DTURNRIGHT 35           //void pos2dTurnRight(int speed);
#define POS2DSTOP 36               //void pos2dStop();
#define POS2DSETFRONTL 37           //void pos2dSetFrontL(unsigned int fl);
#define POS2DSETFRONTR 38           //void pos2dSetFrontR(unsigned int fr);
#define POS2DSETREARL 39          //void pos2dSetRearL(unsigned int rl);
#define POS2DSETREARR 40           //void pos2dSetRearR(unsigned int rr);
#define POS2DGETFRONTL 41          //int pos2dGetFrontL();
#define POS2DGETFRONTR 42          //int pos2dGetFrontR();
#define POS2DGETREARL 43           //int Pos2dGetRearL();
#define POS2DGETREARR 44           //int pos2dGetRearR();
#define POS2DSETTIRES 45         //void pos2dSetTires(unsigned int fl,unsigned int fr,unsigned int rl,unsigned int rr);
#define POS2DGETTIRES 46           //int pos2dGetTires(unsigned int *pt);
#define POS2DSETSERVOTURN 47       //void pos2dSetServoTurn(int angle,int inSpeed);
#define POS2DSETSERVOTURNA 48     //void pos2dSetServoTurnA(int angle,int inSpeed);
#define POS2DSET4TIRESF 49        //void pos2dSet4Tires(int id1,int id2,int id3,int id4,int speed1,int speed2,int speed3,int speed4);
#define POS2DSET4TIRESPOSF 50      //void pos2dSet4TiresPos(int id1,int id2,int id3,int id4,int pos,int speed);
#define CAPSETHF 51                    //void capSetH(int inHmax, int inHmin);
#define CAPSETSF 52                    //void capSetS(int inSmax, int inSmin);
#define CAPSETIF 53                    //void capSetI(int inImax, int inImin);
#define CAPSETMODEF 54                 //void capSetMode(int inMode);
#define CAPGETCENTERXF 55              //int capGetCenterX();
#define CAPGETCENTERYF 56              //int capGetCenterY();
#define CAPGETSUMF 57                  //int capGetSum();
#define CAPSETYELLOW 58               //void capSetYellow();
#define CAPSETRED 59                   //void capSetRed();
#define CAPSETGREEN 60              //void capSetGreen();
/*sensor List index*/
#define infrProxSenLI 0
#define colliSenLI 1
#define  soundSenLI 2
#define gestSenLI 3
#define  hallSenLI 4
#define SENS0 "infrProxSen"
#define SENS1 "colliSen"
#define SENS2 "soundSen"
#define SENS3 "gestSen"
#define SENS4 "hallSen"
#define SENSCFGNAME "CYZX.cfg"
#define SENSNUM 5
#define infrDistSenLI 0
#define  tempSenLI 1
#define graySenLI 2
#define lightSenLI 3
#define SENSAIO0 "infrDistSen"
#define SENSAIO1 "tempSen"
#define SENSAIO2 "graySen"
#define SENSAIO3 "lightSen"
#define SENSAIONUM 4

struct myOpaqueHead {
	int type;
	int subtype;
	char name[8];
};
struct myOpaqueSt {
	int type;
	int subtype;
	char name[8];
	int p1;
	int p2;
	//int f1;
};
struct myOpaqueSt0 {
	int type;
	int subtype;
	char name[8];
};
struct myOpaqueSt1 {
	int type;
	int subtype;
	char name[8];
	int p1;
};
typedef myOpaqueSt myOpaqueSt2;
struct myOpaqueSt3 {
	int type;
	int subtype;
	char name[8];
	int p1;
	int p2;
	int p3;
};
struct myOpaqueSt4 {
	int type;
	int subtype;
	char name[8];
	int p1;
	int p2;
	int p3;
	int p4;
};
struct myOpaqueStLCD {
	int type;
	int subtype;
	char name[8];
	char disp[LCDMAXLENGTH];
};
struct myOpaqueStMp3 {
	int type;
	int subtype;
	char name[8];
	char mp3Title[MP3MAXLENGTH];
};
struct myOpaqueSt4Tires {
	int type;
	int subtype;
	char name[8];
	char tires[4];
	int speed1;
	int speed2;
	int speed3;
	int speed4;
};
struct myOpaqueSt4TiresPos {
	int type;
	int subtype;
	char name[8];
	char tires[4];
	int pos;
	int speed;
};
struct myOpaqueStTime {
	int type;
	int subtype;
	char name[8];
	int p1;
	int p2;
	int time;
};
class mlist {
public:
	mlist();
	mlist(int v);
	~mlist() {
	}
	void push_back(int v);
	int at(int index);

private:
	int val;
	mlist *next;
};
typedef myOpaqueSt respAIO;
typedef myOpaqueSt respDIO;
typedef myOpaqueSt respMFCap;
typedef myOpaqueSt respPos2d;
typedef myOpaqueSt comReqAIO;
typedef myOpaqueSt comReqDIO;
typedef myOpaqueSt comReqMFCap;
typedef myOpaqueSt comReqPos2d;
typedef myOpaqueSt1 comReqBk1;
typedef myOpaqueSt1 respBk1;

#define AIOPORTS 8
#define DIOPORTS 12
#define RS422PORTS 2

////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class exampledriver: public ThreadedDriver {
public:
	exampledriver(ConfigFile* cf, int section);
	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr * hdr,
			void * data);
private:
	// Main function for device thread.
	virtual void Main();
	virtual int MainSetup();
	virtual void MainQuit();

	// My position interface
	//player_devaddr_t m_dio_addr;
	//player_devaddr_t m_aio_addr;
	player_devaddr_t m_position_addr;
	player_devaddr_t m_opaque_addr;
	player_devaddr_t m_laser_addr;
	player_devaddr_t m_blobfinder_addr;
	char position2dPublish;
	char opaquePublish;
	char blobfinderPublish;
	char laserPublish;

	void LaserPublish();
	void Position2dPublish();
	void BlobfinderPublish();
	void OpaquePublish();

	int dealPosMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealLaserMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealBlobFinderMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealOpaqueMFCAPCommand(myOpaqueSt *popa);
	int dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueBKComReq(comReqBk1 *popa, respBk1 *res);
	int dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaquePosition2d(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueBK(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueSR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void writeMessagehrToFile(player_msghdr * hdr, void * data);
	int dealOpaqueBKGETULTRASONICF(comReqBk1 *popa);

private:
	int foop;

	//laser
	int scan_id = 0;
	int aioPortMin;
	int aioPortMax;
	int dioPortMin;
	int dioPortMax;
	char dio[12];
	char rs422p0;
	char rs422p1;
	float fov;
	int sample_count;
	struct Bounds {
		double min;
		double max;
	};
	Bounds range_bounds;

	//blobfinder
	unsigned int scan_width;
	unsigned int scan_height;
	uint32_t color;

	//position2d
	int frontL;
	int frontR;
	int rearL;
	int rearR;
	int servoFR;
	int servoFL;
	int servoRR;
	int servoRL;
	int rationSpeedToServo;
	int maxSpeed;
	double aToAngle;

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
	FILE *fl;
	time_t logtime;
#endif

private:
	void MFInit() {
	}

	//LCD
	void MF2X4LCD(const char*item, int insize) {
	}

	//Ultrasonic
	void MFUSonicEnable() {
	}
	int MFGetUltrasonic() {
		return 0;
	}

	//Delay
	void DelayMS(int inMS) {
	}

	//SerialCom
	void MFComSend(const void *pBuffer, const int iLength) {
	}

	//Digi. IO
	void MFDigiInit(int interval) {
	}
	void MFSetPortDirect(unsigned int inData) {
	}
	int MFGetDigiInput(int inID) {
		return 0;
	}
	void MFDigiOutput(int inID, int inVal) {
	}

	//AD
	void MFADInit(int interval) {
	}
	void MFADEnable(int inID) {
	}
	int MFGetAD(int inID) {
		return 0;
	}

	//Servo
	void MFServoInit(int interval) {
	}
	void MFSetServoMode(int inID, int inMode) {
	}
	void MFSetServoPos(int inID, int inPos, int inSpeed) {
	}
	void MFSetServoRotaSpd(int inID, int inSpeed) {
	}
	int MFGetServoPos(int inID) {
		return 0;
	}
	void MFServoAction() {
	}

	//Video Capture
	bool MFCapOpen() {
	}
	void MFCapSetH(int inHmax, int inHmin) {
	}
	void MFCapSetS(int inSmax, int inSmin) {
	}
	void MFCapSetI(int inImax, int inImin) {
	}
	void MFCapSetMode(int inMode) {
	}
	int MFCapGetCenterX() {
		return 0;
	}
	int MFCapGetCenterY() {
		return 0;
	}
	int MFCapGetSum() {
		return 0;
	}

	//SpeechRecongn
	void MFSRStart() {
	}
	void MFSRStop() {
	}
	bool MFSRPause() {
		return 0;
	}
	bool MFSRContinue() {
		return 0;
	}
	bool MFSRInsertText(int index, const char*item, int insize) {
		return 0;
	}
	int MFSRGetResIndex() {
		return 0;
	}
	void MFSRClearItems() {
	}
	bool MFSRisStarted() {
		return 0;
	}

	//mp3
	void MFMp3Play(const char*item, int insize) {
	}
	void MFMp3Stop() {
	}

	//UDP
	int UDPConnect(char *inTarIP, int inTarPort) {
		return 0;
	}
	void UDPSend(char *inbuf, int inLen) {
	}
	int UDPListen(int inPort) {
		return 0;
	}
	//void UDPRecFunc(PBUFREC inFunc){}

	//debug
	int GetCmdVaildprc() {
		return 0;
	}
	int GetCmdWaitMax() {
		return 0;
	}
	int GetCmdCurCnt() {
		return 0;
	}
	bool ComCreat(int inNo, unsigned int inBaudrate) {
		return 0;
	}
	int deaperDealAIO(int t, int type);
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
exampledriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	return ((Driver*) (new exampledriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void exampledriver_Register(DriverTable* table) {
	table->AddDriver("exampledriver", exampledriver_Init);
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
exampledriver::exampledriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {
	// Read an option from the configuration file
	this->position2dPublish = 0;
	this->opaquePublish = 0;
	this->blobfinderPublish = 0;
	this->laserPublish = 0;
	if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
			PLAYER_POSITION2D_CODE, -1, NULL) != 0) {
		//this->SetError(-1);
		//return;
	} else {
		if (this->AddInterface(this->m_position_addr)) {
			this->SetError(-1);
			return;
		}
		this->position2dPublish = 1;
	}
	if (cf->ReadDeviceAddr(&(this->m_laser_addr), section, "provides",
			PLAYER_LASER_CODE, -1, NULL) != 0) {
		//this->SetError(-1);
		//return;
	} else {
		if (this->AddInterface(this->m_laser_addr)) {
			this->SetError(-1);
			return;
		}
		this->laserPublish = 1;
	}
	if (cf->ReadDeviceAddr(&(this->m_blobfinder_addr), section, "provides",
			PLAYER_BLOBFINDER_CODE, -1, NULL) != 0) {
		//this->SetError(-1);
		//return;
	} else {
		if (this->AddInterface(this->m_blobfinder_addr)) {
			this->SetError(-1);
			return;
		}
		this->blobfinderPublish = 1;
	}
	if (cf->ReadDeviceAddr(&(this->m_opaque_addr), section, "provides",
			PLAYER_OPAQUE_CODE, -1, NULL) != 0) {
		//this->SetError(-1);
		//return;
	} else {
		if (this->AddInterface(this->m_opaque_addr)) {
			this->SetError(-1);
			return;
		}
		this->opaquePublish = 1;
	}

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

#ifdef WRITELOG
	if ((this->fl = (fopen(LOGFILENAME, "w"))) == NULL) {
		printf("can't open log file:");
		printf(LOGFILENAME);
		printf("\n");
		return;
	}
#endif

	this->aioPortMin = cf->ReadInt(section, "aioportmin", 0);
	this->aioPortMax = cf->ReadInt(section, "aioportmax", 7);
	this->dioPortMin = cf->ReadInt(section, "dioportmin", 0);
	this->dioPortMax = cf->ReadInt(section, "dioportmax", 11);
	this->rs422p0 = cf->ReadInt(section, "rs422p0_return", 1);
	this->rs422p1 = cf->ReadInt(section, "rs422p1_return", 0);
	this->dio[0] = cf->ReadInt(section, "dio0", 0);
	this->dio[1] = cf->ReadInt(section, "dio1", 0);
	this->dio[2] = cf->ReadInt(section, "dio2", 0);
	this->dio[3] = cf->ReadInt(section, "dio3", 0);
	this->dio[4] = cf->ReadInt(section, "dio4", 0);
	this->dio[5] = cf->ReadInt(section, "dio5", 0);
	this->dio[6] = cf->ReadInt(section, "dio6", 0);
	this->dio[7] = cf->ReadInt(section, "dio7", 0);
	this->dio[8] = cf->ReadInt(section, "dio8", 0);
	this->dio[9] = cf->ReadInt(section, "dio9", 0);
	this->dio[10] = cf->ReadInt(section, "dio10", 0);
	this->dio[11] = cf->ReadInt(section, "dio11", 0);

	this->frontL = cf->ReadInt(section, "frontL", 1);
	this->frontR = cf->ReadInt(section, "frontR", 2);
	this->rearL = cf->ReadInt(section, "rearL", 3);
	this->rearR = cf->ReadInt(section, "rearR", 4);
	this->servoFR = cf->ReadInt(section, "servofr", 5);
	this->servoFL = cf->ReadInt(section, "servofl", 6);
	this->servoRR = cf->ReadInt(section, "servorr", 7);
	this->servoRL = cf->ReadInt(section, "servorl", 8);
	this->rationSpeedToServo=2000;
	this->maxSpeed=1023;
	this->aToAngle=57.4;

	MFCapOpen();
	MFInit();
	int portdirect = 0;
	for (int i = 0; i < 12; i++) {
		if (this->dio[i] == 1)
			portdirect |= (1 << i);
	}
	MFSetPortDirect(portdirect);
	for (int i = 0; i < 8; i++) {
		MFADEnable(i);
	}
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	MFSetServoMode(this->frontL, 1);
	MFSetServoMode(this->frontR, 1);
	MFSetServoMode(this->rearL, 1);
	MFSetServoMode(this->rearR, 1);
	MFSetServoMode(this->servoFR, 0);
	MFSetServoMode(this->servoFL, 0);
	MFSetServoMode(this->servoRR, 0);
	MFSetServoMode(this->servoRL, 0);
	MFSetServoPos(this->servoFR, 0, 400);
	MFSetServoPos(this->servoFL, 0, 400);
	MFSetServoPos(this->servoRR, 0, 400);
	MFSetServoPos(this->servoRL, 0, 400);
	DelayMS(100);
	MFUSonicEnable();
	DelayMS(100);

	return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int exampledriver::MainSetup() {
	puts("Example driver initialising");

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.

	printf("Was foo option given in config file? %d\n", this->foop);

	puts("Example driver ready");

	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void exampledriver::MainQuit() {
	puts("Shutting example driver down");

	// Here you would shut the device down by, for example, closing a
	// serial port.

	puts("Example driver has been shutdown");
}
int exampledriver::dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
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
int exampledriver::dealOpaqueMFCAP(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;
#ifdef WRITELOG
	int t = sizeof(myOpaqueSt);
	printf("opaqueMFCap size %d,coming message  ", t);
	printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

	switch (popa->subtype) {
	case CAPGETCENTERXF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPGETCENTERXF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPGETCENTERXF done:%s", ctime(&(this->logtime)));
#endif
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
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case CAPGETCENTERYF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPGETCENTERYF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPGETCENTERYF done:%s", ctime(&(this->logtime)));
#endif
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
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case CAPGETSUMF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPGETSUMF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPGETSUMF done:%s", ctime(&(this->logtime)));
#endif
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
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case CAPSETHF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETHF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETHF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case CAPSETSF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETSF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETSF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case CAPSETIF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETIF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETIF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case CAPSETMODEF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETMODEF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETMODEF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case CAPSETYELLOW: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETYELLOW:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETYELLOW done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case CAPSETRED: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETRED:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETRED done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case CAPSETGREEN: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETGREEN:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueMFCAPComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "CAPSETGREEN done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	default: {
#ifdef WRITELOG
		this->nopaquemc++;
#endif
		return -1;
		break;
	}
	}

	return 0;
}

int exampledriver::dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;
#ifdef WRITELOG
	int t = sizeof(myOpaqueSt);
	printf("opaqueDIO size %d,coming message  ", t);
	printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

	switch (popa->subtype) {
	case DIOSETPORTDIRECTF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "DIOSETPORTDIRECTF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueDIOComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "DIOSETPORTDIRECTF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case DIOGETDIGIINPUTF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "DIOGETDIGIINPUTF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueDIOComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "DIOGETDIGIINPUTF done:%s", ctime(&(this->logtime)));
#endif
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
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case DIODIGIOUTPUTF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "DIODIGIOUTPUTF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueDIOComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "DIODIGIOUTPUTF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;
}
int exampledriver::dealOpaquePosition2dComReq(myOpaqueSt *popa,
		myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	if (POS2DSERVOMODESERVOF == popa->subtype) {
		MFSetServoMode(this->frontL, 0);
		MFSetServoMode(this->frontR, 0);
		MFSetServoMode(this->rearL, 0);
		MFSetServoMode(this->rearR, 0);
	} else if (POS2DSERVOMODEMOTOF == popa->subtype) {
		MFSetServoMode(this->frontL, 1);
		MFSetServoMode(this->frontR, 1);
		MFSetServoMode(this->rearL, 1);
		MFSetServoMode(this->rearR, 1);
	} else if (POS2DFORWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->frontL, (popa->p1) & 0x0000FFFF);
		MFSetServoRotaSpd(this->frontR, (popa->p1) >> 16);
		MFSetServoRotaSpd(this->rearL, (popa->p2) & 0x0000FFFF);
		MFSetServoRotaSpd(this->rearR, (popa->p2) >> 16);
		MFServoAction();
	} else if (POS2DBACKWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->frontL, -((popa->p1) & 0x0000FFFF));
		MFSetServoRotaSpd(this->frontR, -((popa->p1) >> 16));
		MFSetServoRotaSpd(this->rearL, -((popa->p2) & 0x0000FFFF));
		MFSetServoRotaSpd(this->rearR, -((popa->p2) >> 16));
		MFServoAction();
	} else if (POS2DTURNLEFTF == popa->subtype) {
		MFSetServoRotaSpd(this->frontL, -((popa->p1) & 0x0000FFFF));
		MFSetServoRotaSpd(this->frontR, ((popa->p1) >> 16));
		MFSetServoRotaSpd(this->rearL, -((popa->p2) & 0x0000FFFF));
		MFSetServoRotaSpd(this->rearR, ((popa->p2) >> 16));
		MFServoAction();
	} else if (POS2DTURNRIGHT == popa->subtype) {
		MFSetServoRotaSpd(this->frontL, (popa->p1) & 0x0000FFFF);
		MFSetServoRotaSpd(this->frontR, -((popa->p1) >> 16));
		MFSetServoRotaSpd(this->rearL, (popa->p2) & 0x0000FFFF);
		MFSetServoRotaSpd(this->rearR, -((popa->p2) >> 16));
		MFServoAction();
	}
	/*else if (POS2DFORWARDF == popa->subtype) {
	 MFSetServoRotaSpd(this->frontL, popa->p1);
	 MFSetServoRotaSpd(this->frontR, -(popa->p1));
	 MFSetServoRotaSpd(this->rearL, popa->p1);
	 MFSetServoRotaSpd(this->rearR, -(popa->p1));
	 MFServoAction();
	 } else if (POS2DBACKWARDF == popa->subtype) {
	 MFSetServoRotaSpd(this->frontL, -(popa->p1));
	 MFSetServoRotaSpd(this->frontR, popa->p1);
	 MFSetServoRotaSpd(this->rearL, -(popa->p1));
	 MFSetServoRotaSpd(this->rearR, popa->p1);
	 MFServoAction();
	 } else if (POS2DTURNLEFTF == popa->subtype) {
	 MFSetServoRotaSpd(this->frontL, -(popa->p1));
	 MFSetServoRotaSpd(this->frontR, -(popa->p1));
	 MFSetServoRotaSpd(this->rearL, -(popa->p1));
	 MFSetServoRotaSpd(this->rearR, -(popa->p1));
	 MFServoAction();
	 } else if (POS2DTURNRIGHT == popa->subtype) {
	 MFSetServoRotaSpd(this->frontL, popa->p1);
	 MFSetServoRotaSpd(this->frontR, popa->p1);
	 MFSetServoRotaSpd(this->rearL, popa->p1);
	 MFSetServoRotaSpd(this->rearR, popa->p1);
	 MFServoAction();
	 } */else if (POS2DSTOP == popa->subtype) {
		MFSetServoRotaSpd(this->frontL, 0);
		MFSetServoRotaSpd(this->frontR, 0);
		MFSetServoRotaSpd(this->rearL, 0);
		MFSetServoRotaSpd(this->rearR, 0);
		MFServoAction();
	} else if (POS2DSETSERVOTURN == popa->subtype) {
		MFSetServoPos(this->frontL, popa->p1, popa->p2);
		MFSetServoPos(this->frontR, popa->p1, popa->p2);
		MFSetServoPos(this->rearL, popa->p1, popa->p2);
		MFSetServoPos(this->rearR, popa->p1, popa->p2);
		MFServoAction();
	} else if (POS2DSETFRONTL == popa->subtype) {
		this->frontL = popa->p1;
	} else if (POS2DSETFRONTR == popa->subtype) {
		this->frontR = popa->p1;
	} else if (POS2DSETREARL == popa->subtype) {
		this->rearL = popa->p1;
	} else if (POS2DSETREARR == popa->subtype) {
		this->rearR = popa->p1;
	} else if (POS2DGETFRONTL == popa->subtype) {
		res->p1 = this->frontL;
	} else if (POS2DGETFRONTR == popa->subtype) {
		res->p1 = this->frontR;
	} else if (POS2DGETREARL == popa->subtype) {
		res->p1 = this->rearL;
	} else if (POS2DGETREARR == popa->subtype) {
		res->p1 = this->rearR;
	} else if (POS2DSETTIRES == popa->subtype) {
		this->frontL = (popa->p1) & 0x0000FFFF;
		this->frontR = (popa->p1) >> 16;
		this->rearL = (popa->p2) & 0x0000FFFF;
		this->rearR = (popa->p2) >> 16;
	} else if (POS2DGETTIRES == popa->subtype) {
		unsigned int ttire = 0x00000000;
		ttire |= ((this->frontL & 0x0000FFFF)
				| ((this->frontR << 16) & 0xFFFF0000));
#ifdef WRITELOG
		printf("int size %d\n", sizeof(int));
		printf("test tire id %x , %x , %x\n", (this->frontL & 0x0000FFFF),
				(this->frontR << 16) & 0xFFFF0000, ttire);
#endif
		res->p1 = ttire;
		ttire = 0x00000000;
		ttire |= ((this->rearL & 0x0000FFFF)
				| ((this->rearR << 16) & 0xFFFF0000));
		res->p2 = ttire;
#ifdef WRITELOG
		printf("test tire id %x , %x , %x\n", (this->rearL & 0x0000FFFF),
				(this->rearR << 16) & 0xFFFF0000, ttire);
#endif
	} else {
		return -1;
	}
	return 0;
}
int exampledriver::dealOpaquePosition2d(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	comReqPos2d *popa = (comReqPos2d *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	respPos2d res;
#ifdef WRITELOG
	int t = sizeof(myOpaqueSt);
	printf("opaquePosition2d size %d,coming message  ", t);
	printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

	switch (popa->subtype) {
	case POS2DSET4TIRESF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSET4TIRESF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt4Tires *popa4tires = (myOpaqueSt4Tires *) opaquedata->data;
		int id1 = popa4tires->tires[0];
		int id2 = popa4tires->tires[1];
		int id3 = popa4tires->tires[2];
		int id4 = popa4tires->tires[3];
		int speed1 = popa4tires->speed1;
		int speed2 = popa4tires->speed2;
		int speed3 = popa4tires->speed3;
		int speed4 = popa4tires->speed4;
#ifdef WRITELOG
		printf("coming  pos2dset4Tires : %d %d %d %d, %d %d %d %d\n", id1, id2,
				id3, id4, speed1, speed2, speed3, speed4);
#endif
		if (id1 != 0)
			MFSetServoRotaSpd(id1, speed1);
		if (id2 != 0)
			MFSetServoRotaSpd(id2, speed2);
		if (id3 != 0)
			MFSetServoRotaSpd(id3, speed3);
		if (id4 != 0)
			MFSetServoRotaSpd(id4, speed4);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSET4TIRESF done:%s", ctime(&(this->logtime)));
#endif
		return 0;
		break;
	}
	case POS2DSET4TIRESPOSF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSET4TIRESPOSF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt4TiresPos *popa4TiresPos =
				(myOpaqueSt4TiresPos*) opaquedata->data;
		int id1 = popa4TiresPos->tires[0];
		int id2 = popa4TiresPos->tires[1];
		int id3 = popa4TiresPos->tires[2];
		int id4 = popa4TiresPos->tires[3];
		int speed = popa4TiresPos->speed;
		int pos = popa4TiresPos->pos;
#ifdef WRITELOG
		printf("coming pos2dset4TiresPos :pos %d,speed %d,%d %d %d %d\n", pos,
				speed, id1, id2, id3, id4);
#endif
		if (id1 != 0)
			MFSetServoPos(id1, pos, speed);
		if (id2 != 0)
			MFSetServoPos(id2, pos, speed);
		if (id3 != 0)
			MFSetServoPos(id3, pos, speed);
		if (id4 != 0)
			MFSetServoPos(id4, pos, speed);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSET4TIRESPOSF done:%s",
				ctime(&(this->logtime)));
#endif
		return 0;
		break;
	}
	case POS2DSTOP: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSTOP:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSTOP done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DFORWARDF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DFORWARDF:%s", ctime(&(this->logtime)));
#endif

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->frontL, popa->p1);
		MFSetServoRotaSpd(this->frontR, popa->p2);
		MFSetServoRotaSpd(this->rearL, popa->p3);
		MFSetServoRotaSpd(this->rearR, popa->p4);
		MFServoAction();

#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DFORWARDF done %d,%d,%d,%d:%s", popa->p1,
				popa->p2, popa->p3, popa->p4, ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DBACKWARDF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DBACKWARDF:%s", ctime(&(this->logtime)));
#endif

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->frontL, -(popa->p1));
		MFSetServoRotaSpd(this->frontR, -(popa->p2));
		MFSetServoRotaSpd(this->rearL, -(popa->p3));
		MFSetServoRotaSpd(this->rearR, -(popa->p4));
		MFServoAction();

#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DBACKWARDF done %d,%d,%d,%d:%s", popa->p1,
				popa->p2, popa->p3, popa->p4, ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DTURNLEFTF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DTURNLEFTF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->frontL, -(popa->p1));
		MFSetServoRotaSpd(this->frontR, (popa->p2));
		MFSetServoRotaSpd(this->rearL, -(popa->p3));
		MFSetServoRotaSpd(this->rearR, (popa->p4));
		MFServoAction();
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DTURNLEFTF done %d,%d,%d,%d:%s", popa->p1,
				popa->p2, popa->p3, popa->p4, ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DTURNRIGHT: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DTURNRIGHT:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->frontL, (popa->p1));
		MFSetServoRotaSpd(this->frontR, -(popa->p2));
		MFSetServoRotaSpd(this->rearL, (popa->p3));
		MFSetServoRotaSpd(this->rearR, -(popa->p4));
		MFServoAction();
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DTURNRIGHT done %d,%d,%d,%d:%s", popa->p1,
				popa->p2, popa->p3, popa->p4, ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSETFRONTL: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETfrontL:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETfrontL done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSETFRONTR: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETfrontR:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETfrontR done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSETREARL: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETREARL:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETREARL done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSETREARR: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETREARR:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETREARR done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DGETFRONTL: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETfrontL:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETfrontL done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaquePosition2d response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing position2d message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DGETFRONTR: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETfrontR:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETfrontR done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaquePosition2d response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing position2d message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DGETREARL: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETREARL:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETREARL done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaquePosition2d response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing position2d message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DGETREARR: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETREARR:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETREARR done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaquePosition2d response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing position2d message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DSETTIRES: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETTIRES:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETTIRES done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DGETTIRES: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETTIRES:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DGETTIRES done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaquePosition2d response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing position2d message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DSERVOMODESERVOF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSERVOMODESERVOF:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSERVOMODESERVOF done:%s",
				ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSERVOMODEMOTOF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSERVOMODEMOTOF:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSERVOMODEMOTOF done:%s",
				ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSETSERVOTURN: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETSERVOTURN:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETSERVOTURN done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case POS2DSETSERVOTURNA: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETSERVOTURNA:%s", ctime(&(this->logtime)));
#endif
		dealOpaquePosition2dComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "POS2DSETSERVOTURNA done:%s",
				ctime(&(this->logtime)));
#endif
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;

}
int exampledriver::dealOpaqueBK(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	pid_t pid;
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
	switch (popa->subtype) {
	case BKMFSETSERVOROTASPDF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSETSERVOROTASPDF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		MFSetServoRotaSpd(popa->p1, popa->p2);
		//MFServoAction();
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSETSERVOROTASPDF done:%s",
				ctime(&(this->logtime)));
#endif
		break;
	}
	case BKMFSETSERVOMODEF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSETSERVOMODEF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		MFSetServoMode(popa->p1, popa->p2);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSETSERVOMODEF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case BKMFSETSERVOPOSF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSETSERVOPOSF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt3 *popa = (myOpaqueSt3 *) opaquedata->data;
		MFSetServoPos(popa->p1, popa->p2, popa->p3);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSETSERVOPOSF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}

	case BKMFGETSERVOPOSF: {
		break;
	}

	case BKGETULTRASONICF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKGETULTRASONICF:%s", ctime(&(this->logtime)));
#endif
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		if (-1 == dealOpaqueBKGETULTRASONICF(popa))
			return -1;
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKGETULTRASONICF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case BKMFSERVOACTIONF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSERVOACTIONF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
		MFServoAction();
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMFSERVOACTIONF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case BKMFSETSERVOROTASPDTIMEF: {
		break;
	}

	case BKMP3PLAYF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMP3PLAYF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueStMp3 *popa = (myOpaqueStMp3 *) opaquedata->data;

		if ((pid = fork()) < 0) {
			printf("fork error\n");
		} else if (pid == 0) {
			char mp3Title[MP3MAXLENGTH + 1];
			for (int i = 0; i < MP3MAXLENGTH + 1; i++)
				mp3Title[i] = '\0';
			for (int i = 0; i < MP3MAXLENGTH; i++)
				mp3Title[i] = popa->mp3Title[i];
			if (execl("/mnt/yaffs/test/MP3Play", mp3Title, (char *) 0) < 0)
				printf("exe error\n");
		}
		/*
		 if (waitpid(pid, NULL, 0) < 0) {
		 printf("wait error\n");
		 }*/
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMP3PLAYF done:%s", ctime(&(this->logtime)));
#endif
		return 0;
		break;
	}

	case BKCOMSENDF: {
		break;
	}
	case BKMP3STOPF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMP3STOPF:%s", ctime(&(this->logtime)));
#endif
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;

		//MFMp3Stop();

		if ((pid = fork()) < 0) {
			printf("fork error\n");
		} else if (pid == 0) {
			if (execl("/mnt/yaffs/test/MP3Stop", "MP3STOP", (char *) 0) < 0)
				printf("exe error\n");
		}
		/*
		 if (waitpid(pid, NULL, 0) < 0) {
		 printf("wait error\n");
		 }*/
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKMP3STOPF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}

	case BKDELAYMSF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKDELAYMSF:%s", ctime(&(this->logtime)));
#endif
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		DelayMS(popa->p1);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKDELAYMSF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	case BKLCDDISF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKLCDDISF:%s", ctime(&(this->logtime)));
#endif
		myOpaqueStLCD *popa = (myOpaqueStLCD*) opaquedata->data;

		if (pid = fork() < 0) {
			printf("fork error\n");
		} else if (pid == 0) {
			char lcdDisp[LCDMAXLENGTH];
			for (int i = 0; i < LCDMAXLENGTH; i++)
				lcdDisp[i] = '\x20';
			for (int i = 0; i < LCDMAXLENGTH; i++) {
				lcdDisp[i] = popa->disp[i];
			}
			if (execl("/mnt/yaffs/test/LCDDisp", lcdDisp, (char *) 0) < 0)
				printf("exe error\n");
		}
		/*
		 if (waitpid(pid, NULL, 0) < 0) {
		 printf("wait error\n");
		 }
		 */

#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKLCDDISF done:%s", ctime(&(this->logtime)));
#endif
		return 0;
		break;
	}
	case BKUSONICENABLEF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKUSONICENABLEF:%s", ctime(&(this->logtime)));
#endif
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		MFUSonicEnable();
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "BKUSONICENABLEF done:%s", ctime(&(this->logtime)));
#endif
		break;
	}
	}

	return 0;
}
int exampledriver::dealOpaqueSR(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	return -1;
}
int exampledriver::deaperDealAIO(int t, int type) {
	if (type == whatKindAIOAIOINFRAREDR) {
		int dis = 1;
#ifdef WRITELOG
#endif
		if (t > 500) {
			return 1;
		} else if (t <= 500 && t > 470) {
			return 2;
		} else if (t <= 470 && t > 400) {
			return 5;
		} else if (t <= 400 && t > 350) {
			return 6;
		} else if (t <= 350 && t > 320) {
			return 7;
		} else if (t <= 320 && t > 230) {
			return 10;
		} else if (t <= 235 && t > 115) {
			return (20 - (t - 115) / 12);
		} else if (t <= 115 && t > 72) {
			return (30 - (t - 72) / 4);
		} else if (t <= 72 && t > 52) {
			return (40 - (t - 52) / 2);
		} else if (t <= 52 && t > 42) {
			return (92 - t);
		} else if (t <= 42 && t > 33) {
			return (60 - (10 * t - 330) / 9);
		} else if (t <= 33 && t > 29) {
			return (70 - (10 * t - 290) / 4);
		} else if (t <= 29 && t > 23) {
			return (80 - (10 * t - 230) / 6);
		} else
			return 80;
		return dis;
	} else {
		return t;
	}
}

int exampledriver::dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	res->p1 = MFGetAD(popa->p1);
	if (popa->p2 != whatKindAIOAIODEF)
		res->p1 = deaperDealAIO(res->p1, popa->p2);
	return 0;
}
int exampledriver::dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;
#ifdef WRITELOG
	int t = sizeof(myOpaqueSt);
	printf("opaqueAIO size %d,coming message  ", t);
	printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

	switch (popa->subtype) {
	case AIOCOMTF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "AIOCOMTF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueAIOComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "AIOCOMTF done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

#ifdef WRITELOG
		printf("req opaqueAIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing AIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case AIOCOMINFRAREDF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "AIOCOMINFRAREDF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueAIOComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "AIOCOMINFRAREDF done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

#ifdef WRITELOG
		printf("req opaqueAIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing AIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case AIOCOMF: {
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "AIOCOMF:%s", ctime(&(this->logtime)));
#endif
		dealOpaqueAIOComReq(popa, &res);
#ifdef WRITELOG
		time(&(this->logtime));
		fprintf(this->fl, "AIOCOMF done:%s", ctime(&(this->logtime)));
#endif
		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

#ifdef WRITELOG
		printf("req opaqueAIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing AIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;
}
int exampledriver::dealOpaqueMessages(QueuePointer & resp_queue,
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
	} else if (popa->type == MYOPAQUEBk) {
		dealOpaqueBK(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESR) {
		dealOpaqueSR(resp_queue, hdr, data);
		return 0;
	} else {
#ifdef WRITELOG
		printf("coming opaque messages type wrong ! \n");
#endif
		return -1;
	}

}
int exampledriver::dealPosMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {

	/*
	 #define PLAYER_POSITION2D_REQ_GET_GEOM 1
	 #define PLAYER_POSITION2D_REQ_MOTOR_POWER 2
	 #define PLAYER_POSITION2D_REQ_VELOCITY_MODE 3
	 #define PLAYER_POSITION2D_REQ_POSITION_MODE 4
	 #define PLAYER_POSITION2D_REQ_SET_ODOM 5
	 #define PLAYER_POSITION2D_REQ_RESET_ODOM 6
	 #define PLAYER_POSITION2D_REQ_SPEED_PID 7
	 #define PLAYER_POSITION2D_REQ_POSITION_PID 8
	 #define PLAYER_POSITION2D_REQ_SPEED_PROF 9
	 #define PLAYER_POSITION2D_DATA_STATE 1
	 #define PLAYER_POSITION2D_DATA_GEOM 2
	 #define PLAYER_POSITION2D_CMD_VEL 1
	 #define PLAYER_POSITION2D_CMD_POS 2
	 #define PLAYER_POSITION2D_CMD_CAR 3
	 #define PLAYER_POSITION2D_CMD_VEL_HEAD 4
	 */
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_VEL, this->m_position_addr)) {
		player_position2d_cmd_vel_t* pcmd = (player_position2d_cmd_vel_t*) data;

		//mod->SetSpeed(pcmd->vel.px, pcmd->vel.py, pcmd->vel.pa);
		double speed=pcmd->vel.px;
		double turn=pcmd->vel.pa;
		double frontLSpeed=speed-turn;
		double rearLSpeed=speed-turn;
		double frontRSpeed=speed+turn;
		double rearRSpeed=speed+turn;
		if((int)frontLSpeed>this->maxSpeed)
			frontLSpeed=rearLSpeed=this->maxSpeed;
		if((int)frontLSpeed < -this->maxSpeed)
			frontLSpeed=rearLSpeed=-this->maxSpeed;
		if((int)frontRSpeed>this->maxSpeed)
			frontRSpeed=rearRSpeed=this->maxSpeed;
		if((int)frontRSpeed<-this->maxSpeed)
			frontRSpeed=rearRSpeed=-this->maxSpeed;

		MFSetServoRotaSpd(this->frontL,((int)frontLSpeed)*this->rationSpeedToServo);
		MFSetServoRotaSpd(this->rearL,((int)rearLSpeed)*this->rationSpeedToServo);
		MFSetServoRotaSpd(this->frontR,((int)frontRSpeed)*this->rationSpeedToServo);
		MFSetServoRotaSpd(this->rearR,((int)rearRSpeed)*this->rationSpeedToServo);
		MFServoAction();

		return 0;
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_POS, this->m_position_addr)) {
		player_position2d_cmd_pos_t* pcmd = (player_position2d_cmd_pos_t*) data;
		//mod->GoTo(pcmd->pos.px, pcmd->pos.py, pcmd->pos.pa);
		double turn=pcmd->pos.pa;
		turn *=this->aToAngle;
		if(turn > 300 || turn <0) return -1;
		int pos=(turn*1023)/300;
		MFSetServoPos(this->servoFR,pos,400);
		MFSetServoPos(this->servoFL,pos,400);
		MFSetServoPos(this->servoRR,pos,400);
		MFSetServoPos(this->servoRL,pos,400);
		MFServoAction();
		return -1;
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_CAR, this->m_position_addr)) {
		player_position2d_cmd_car_t* pcmd = (player_position2d_cmd_car_t*) data;
		//mod->SetSpeed(pcmd->velocity, 0, pcmd->angle);
		double speed=pcmd->velocity;
		double turn=pcmd->angle;
		double frontLSpeed=speed-turn;
		double rearLSpeed=speed-turn;
		double frontRSpeed=speed+turn;
		double rearRSpeed=speed+turn;
		if((int)frontLSpeed>this->maxSpeed)
			frontLSpeed=rearLSpeed=this->maxSpeed;
		if((int)frontLSpeed < -this->maxSpeed)
			frontLSpeed=rearLSpeed=-this->maxSpeed;
		if((int)frontRSpeed>this->maxSpeed)
			frontRSpeed=rearRSpeed=this->maxSpeed;
		if((int)frontRSpeed<-this->maxSpeed)
			frontRSpeed=rearRSpeed=-this->maxSpeed;

		MFSetServoRotaSpd(this->frontL,((int)frontLSpeed)*this->rationSpeedToServo);
		MFSetServoRotaSpd(this->rearL,((int)rearLSpeed)*this->rationSpeedToServo);
		MFSetServoRotaSpd(this->frontR,((int)frontRSpeed)*this->rationSpeedToServo);
		MFSetServoRotaSpd(this->rearR,((int)rearRSpeed)*this->rationSpeedToServo);
		MFServoAction();
		return 0;
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_POSITION2D_CMD_VEL_HEAD, this->m_position_addr)) {
		return -1;
	}
	return -1;
}
int exampledriver::dealLaserMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 #define PLAYER_LASER_DATA_SCAN 1
	 #define PLAYER_LASER_DATA_SCANPOSE 2
	 #define PLAYER_LASER_DATA_SCANANGLE 3
	 Y	 #define PLAYER_LASER_REQ_GET_GEOM 1
	 #define PLAYER_LASER_REQ_SET_CONFIG 2
	 Y#define PLAYER_LASER_REQ_GET_CONFIG 3
	 #define PLAYER_LASER_REQ_POWER 4
	 Y#define PLAYER_LASER_REQ_GET_ID 5
	 #define PLAYER_LASER_REQ_SET_FILTER 6*/
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_CONFIG, this->m_laser_addr)) {
		player_laser_config_t* plc = (player_laser_config_t*) data;
		if (hdr->size == sizeof(player_laser_config_t)) {
			Publish(this->m_laser_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
					PLAYER_LASER_REQ_GET_CONFIG);
			return (0);
		} else {
			return (-1);
		}
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_GEOM, this->m_laser_addr)) {
		player_laser_geom_t pgeom;
		pgeom.pose.px = 0;
		pgeom.pose.py = 0;
		pgeom.pose.pyaw = 0;
		pgeom.size.sl = 0;
		pgeom.size.sw = 0;

		Publish(this->m_laser_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_LASER_REQ_GET_GEOM, (void*) &pgeom, sizeof(pgeom), NULL);
		return (0);
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_GET_ID,
			this->m_laser_addr)) {
		return -1;
	}
	// ---[ Get IDentification information
	/*if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_GET_ID,
	 this->m_laser_addr)) {
	 return (0);
	 } else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_GET_GEOM, this->m_laser_addr)) {
	 player_laser_geom_t geom;
	 memset(&geom, 0, sizeof(geom));
	 geom.pose.px = 0;
	 geom.pose.py = 0;
	 geom.pose.pyaw = 0;
	 geom.size.sl = 0;
	 geom.size.sw = 0;

	 Publish(this->m_laser_addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
	 PLAYER_LASER_REQ_GET_GEOM, (void*) &geom, sizeof(geom), NULL);
	 return (0);
	 }
	 // ---[ Set power
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_POWER, this->m_laser_addr)) {

	 return (0);
	 }
	 // ---[ Set configuration
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_SET_CONFIG, device_addr)) {
	 return (-1);
	 }
	 // ---[ Get configuration
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_GET_CONFIG, device_addr)) {
	 return (0);
	 }
	 // ---[ Set filter settings
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_SET_FILTER, device_addr)) {
	 return (-1);
	 } else*/
	return (-1);
}
int exampledriver::dealBlobFinderMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 #define PLAYER_BLOBFINDER_DATA_BLOBS 1
	 #define PLAYER_BLOBFINDER_REQ_SET_COLOR 1
	 #define PLAYER_BLOBFINDER_REQ_SET_IMAGER_PARAMS 2
	 #define PLAYER_BLOBFINDER_REQ_GET_COLOR 3
	 */
	printf("test blobfinder\n");
	if (Message::MatchMessage(hdr, PLAYER_BLOBFINDER_REQ_GET_COLOR, -1,
			this->m_blobfinder_addr)) {
		return 0;
	}
	/*if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
	 PLAYER_CAMERA_DATA_STATE, this->camera_id)) {
	 this->Publish(this->blobfinder_provided_addr, PLAYER_MSGTYPE_DATA,
	 PLAYER_BLOBFINDER_DATA_BLOBS, reinterpret_cast<void *>(output),
	 0, &(hdr->timestamp), false); // copy = false
	 }*/
	return -1;
}
int exampledriver::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
// Process messages here.  Send a response if necessary, using Publish().
// If you handle the message successfully, return 0.  Otherwise,
// return -1, and a NACK will be sent for you, if a response is required.

	if (hdr->addr.interf == this->m_opaque_addr.interf) {
		if (-1 == dealOpaqueMessages(resp_queue, hdr, data)) {
			return -1;
		}
		return 0;
	} else if (hdr->addr.interf == this->m_position_addr.interf) {
		if (-1 == dealPosMessages(resp_queue, hdr, data)) {
			return -1;
		}
		return 0;
	} else if (hdr->addr.interf == this->m_laser_addr.interf) {
		if (-1 == dealLaserMessages(resp_queue, hdr, data)) {
			return -1;
		}
		return 0;
	} else if (hdr->addr.interf == this->m_blobfinder_addr.interf) {
		if (-1 == dealBlobFinderMessages(resp_queue, hdr, data)) {
			return -1;
		}
		return 0;
	} else {
		return -1;
	}
	/*else if (hdr->addr.interf == this->m_dio_addr.interf) {

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
	 } */
}
void exampledriver::Position2dPublish() {

	/*player_position2d_data_t ppd;
	 bzero(&ppd, sizeof(ppd));

	 ppd.pos.px = 0;
	 ppd.pos.py = 0;
	 ppd.pos.pa = 0;

	 ppd.vel.px = 0;
	 ppd.vel.py = 0;
	 ppd.vel.pa = 0;

	 ppd.stall = 0;

	 Publish(this->m_position_addr, PLAYER_MSGTYPE_DATA,
	 PLAYER_POSITION2D_DATA_STATE, (void*) &ppd, sizeof(ppd), NULL);*/
	return;
}
void exampledriver::BlobfinderPublish() {
	player_blobfinder_data_t bfd;
	bzero(&bfd, sizeof(bfd));

	uint32_t bcount = 1;
	player_blobfinder_blob_t blob;
	if (bcount > 0) {
		// and set the image width * height
		bfd.width = this->scan_width;
		bfd.height = this->scan_height;
		bfd.blobs_count = bcount;
		bfd.blobs = &blob;

		unsigned int b;
		for (b = 0; b < bcount; b++) {
			bfd.blobs[b].x = MFCapGetCenterX();
			bfd.blobs[b].y = MFCapGetCenterY();
			bfd.blobs[b].left = 0;
			bfd.blobs[b].right = 0;
			bfd.blobs[b].top = 0;
			bfd.blobs[b].bottom = 0;
			bfd.blobs[b].color = 0;
			bfd.blobs[b].area = MFCapGetSum();
			bfd.blobs[b].range = 0;
		}
	}
	Publish(m_blobfinder_addr, PLAYER_MSGTYPE_DATA,
			PLAYER_BLOBFINDER_DATA_BLOBS, &bfd, sizeof(bfd), NULL);
	return;
}
void exampledriver::LaserPublish() {
	float dioV[DIOPORTS];
	float aioV[AIOPORTS];
	float RS422V[RS422PORTS];
	for (int i = 0; i < DIOPORTS; i++)
		dioV[i] = 0;
	for (int i = 0; i < AIOPORTS; i++)
		aioV[i] = 0;
	for (int i = 0; i < RS422PORTS; i++)
		RS422V[i] = 0;
	for (int i = this->dioPortMin; i <= this->dioPortMax; i++) {
		if (this->dio[i] == 0) {
			dioV[i] = (float) MFGetDigiInput(i);
		}
	}
	for (int i = this->aioPortMin; i <= this->aioPortMax; i++) {
		aioV[i] = (float) MFGetAD(i);
	}
	for (int i = 0; i < RS422PORTS; i++) {
		if (this->rs422p0 == 1)
			RS422V[0] = (float) MFGetUltrasonic();
		if (this->rs422p1 == 1)
			RS422V[1] = RS422V[0];
	}

	player_laser_data_t pdata;
	memset(&pdata, 0, sizeof(pdata));

	pdata.min_angle = -this->fov / 2.0;
	pdata.max_angle = +this->fov / 2.0;
	pdata.resolution = this->fov / this->sample_count;
	pdata.max_range = this->range_bounds.max;
	pdata.ranges_count = pdata.intensity_count = this->sample_count = AIOPORTS
			+ DIOPORTS + RS422PORTS;
	pdata.id = this->scan_id++;

	pdata.ranges = new float[pdata.ranges_count];
	pdata.intensity = new uint8_t[pdata.ranges_count];

	int j = 0;
	for (int i = 0; i < DIOPORTS; i++) {
		pdata.ranges[j] = dioV[i];
		pdata.intensity[j] = 0;
		j++;
	}
	for (int i = 0; i < AIOPORTS; i++) {
		pdata.ranges[j] = aioV[i];
		pdata.intensity[j] = 0;
		j++;
	}
	for (int i = 0; i < RS422PORTS; i++) {
		pdata.ranges[j] = RS422V[i];
		pdata.intensity[j] = 0;
		j++;
	}

	// Write laser data
	Publish(this->m_laser_addr, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
			(void*) &pdata, sizeof(pdata), NULL);
	return;
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void exampledriver::Main() {
	// The main loop; interact with the device here
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  exampledriver::ProcessMessage() is
		// called on each message.
		ProcessMessages();

		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()
		this->position2dPublish = 0;
		this->opaquePublish = 0;
		if (this->laserPublish)
			this->LaserPublish();
		if (this->position2dPublish)
			this->Position2dPublish();
		if (this->blobfinderPublish)
			this->BlobfinderPublish();
		if (this->opaquePublish)
			this->OpaquePublish();

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
	exampledriver_Register(table);
	puts("Example driver done");
	return (0);
}
}


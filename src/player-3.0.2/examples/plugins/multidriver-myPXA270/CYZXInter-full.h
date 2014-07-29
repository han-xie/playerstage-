/*
 * CYZXInter.h
 *
 *  Created on: Jul 19, 2013
 *      Author: keevi7c3l
 */

#ifndef CYZXINTER_H_
#define CYZXINTER_H_

#include <string>
#include <map>

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

#define LABLESTRING "CYZXSPECIALSTRING:"
#define LOGFILENAME "/mnt/yaffs/test/CYZXmultidriver.log"

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
#define WIFIPERIPHERAL    7
#define MAXCOMMANDNUMBER 256

#define WIFISETWEIBOF 1 //void WifiSetWeibo(char *ip=NULL);
#define WIFISETLCDF 2 //void WifiSetLCD(char *displayer,char *ip=NULL);
#define WIFISETSOUNDF 3//void WifiSetSound(char *type,char *ip=NULL);
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

typedef unsigned int   uint32_t;
typedef unsigned char  uint8_t;

struct myOpaqueHead {
	uint32_t type;
	uint32_t subtype;
};
struct myOpaqueSt {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint32_t p1;
	uint32_t p2;
	//int f1;
};
struct myOpaqueSt0 {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
};
struct myOpaqueSt1 {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint32_t p1;
};
typedef myOpaqueSt myOpaqueSt2;
struct myOpaqueSt3 {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint32_t p1;
	uint32_t p2;
	uint32_t p3;
};
struct myOpaqueSt4{
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint32_t p1;
	uint32_t p2;
	uint32_t p3;
	uint32_t p4;
};
struct myOpaqueStLCD {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint8_t disp[LCDMAXLENGTH];
};
struct myOpaqueStMp3 {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint8_t mp3Title[MP3MAXLENGTH];
};
struct myOpaqueSt4Tires {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	char tires[4];
	uint32_t speed1;
	uint32_t speed2;
	uint32_t speed3;
	uint32_t speed4;
};
struct myOpaqueSt4TiresPos {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	char tires[4];
	uint32_t pos;
	uint32_t speed;
};
struct myOpaqueStTime {
	uint32_t type;
	uint32_t subtype;
	uint8_t name[8];
	uint32_t p1;
	uint32_t p2;
	uint32_t time;
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

class mConfigFile {
public:
	mConfigFile();
	int load(const char *filename);
	int GetTupleCount(const char *s);
	int ReadTupleInt(const char *s, int index);
	char* ReadString(const char *s, char *rs);
private:
	int dealAline(const char *s);
	int count(std::string s);
	int atoiIndex(const char *s, int index);
	std::map<std::string, std::string> tsadports;
};

class CYZXInter {
private:
	PlayerClient *robot;
	DioProxy *diop;
	AioProxy *aiop;
	OpaqueProxy *opaquep;
	Position2dProxy *posp;
	BlobfinderProxy *bfp;
	LaserProxy *lp;

	unsigned int DIOPortDirect;
	mlist *senl[SENSNUM + 1];
	mlist *DIOoutport;
	mlist *senlAIO[SENSAIONUM + 1];
	unsigned int servoDirect;

	//position2d
	int frontl;
	int frontr;
	int rearl;
	int rearr;

public:
	CYZXInter();
public:
	CYZXInter(PlayerClient *probot);

public:
	CYZXInter(DioProxy *pdiop);
public:
	CYZXInter(AioProxy *paiop);
public:
	CYZXInter(OpaqueProxy *popaquep);
public:
	CYZXInter(Position2dProxy *pposp);
public:
	CYZXInter(PlayerClient *probot,OpaqueProxy *opaquep,Position2dProxy *posp,BlobfinderProxy *bfp,LaserProxy *lp);
public:
	CYZXInter(PlayerClient *probot, OpaqueProxy *popaquep);
public:
	CYZXInter(const std::string aHostname,
            uint32_t aPort=PLAYER_PORTNUM,unsigned int index=0);
public:
	~CYZXInter();

	//backgroud special
public:
	void bkDelayMS(int inMS);
public:
	void bkLCDdisp(const char*item); 	//LCD
public:
	void bkUSonicEnable();    	//Ultrasonic
public:
	int bkGetUltrasonic();        //Ultrasonic
public:
	void bkComSend(const void *pBuffer, const int iLength);  //SerialCom
public:
	void bkMp3Play(const char*item);          	//mp3
public:
	void bkMp3Stop();                                     //mp3
//private:
public:
	void bkMFSetServoMode(int inID, int inMode);
public:
	void bkMFSetServoModeServo(int inID);
public:
	void bkMFSetServoModeMoto(int inID);
public:
	void bkMFSetServoPos(int inID, int inPos, int inSpeed);
public:
	void bkMFSetServoRotaSpd(int inID, int inSpeed);
protected:
	void bkMFSetServoRotaSpdTime(int inID, int inSpeed, int seconds);
	//int bkMFGetServoPos(int inID);
public:
	void bkMFServoAction();

	//SpeechRecongn
public:
	void SRStart();
public:
	void SRStop();
public:
	bool SRPause();
public:
	bool SRContinue();
public:
	bool SRInsertText(int index, const char*item, int insize);
public:
	int SRGetResIndex();
public:
	void SRClearItems();
public:
	bool SRisStarted();

	//dio
public:
	void dioSetPortDirect(unsigned int inData);
public:
	void dioSetPortDirectOutput(unsigned int from, unsigned int to); //others is input
public:
	int dioGetDigiInput(int port);                  //can't get result return -1
public:
	void dioDigiOutput(int port, int inVal);
	//aio
public:
	int aioComInfrared(int port);
public:
	int aioCom(int port);
private:
	int aioComT(int port, int whatKindAIO);
	//dio+aio
public:
	int infrProxSen(unsigned int index = 0);
public:
	int colliSen(unsigned int index = 0);
public:
	int lightSen(unsigned int index = 0);
public:
	int soundSen(unsigned int index = 0);
public:
	int gestSen(unsigned int index = 0);
public:
	int graySen(unsigned int index = 0);
public:
	int tempSen(unsigned int index = 0);
public:
	int hallSen(unsigned int index = 0);
public:
	void dioout(int value, unsigned int index = 0);
public:
	int infrDistSen(unsigned int index = 0);

	//position2d
public:
	virtual void pos2dForward(int speed = 500);
public:
	virtual void pos2dBackward(int speed = 500);
public:
	virtual void pos2dTurnLeft(int speed = 500);
public:
	virtual void pos2dTurnRight(int speed = 500);
public:
	virtual void pos2dSetServoTurn(int angle, int inSpeed); //void MFSetServoPos(int inID,int inPos,int inSpeed);
public:
	void pos2dSetServoTurnA(int angle, int inSpeed);
public:
	virtual void pos2dStop();
public:
	void pos2dServoModeServo();
public:
	void pos2dServoModeMoto();
public:
	void pos2dSetFrontL(unsigned int fl);
public:
	void pos2dSetFrontR(unsigned int fr);
public:
	void pos2dSetRearL(unsigned int rl);
public:
	void pos2dSetRearR(unsigned int rr);
public:
	int pos2dGetFrontL();
public:
	int pos2dGetFrontR();
public:
	int Pos2dGetRearL();
public:
	int pos2dGetRearR();
public:
	void pos2dSetTires(unsigned int fl, unsigned int fr, unsigned int rl,
			unsigned int rr);
public:
	int pos2dGetTires(unsigned int *ptire); //fl fr rl rr
public:
	void pos2dSet4Tires(int id1, int id2, int id3, int id4, int speed1,
			int speed2, int speed3, int speed4);
public:
	void pos2dSet4TiresPos(int id1, int id2, int id3, int id4, int pos,
			int speed);

	//MFCap
public:
	virtual void capSetYellow();
public:
	virtual void capSetRed();
public:
	virtual void capSetGreen();
public:
	void capSetH(int inHmax, int inHmin);
public:
	void capSetS(int inSmax, int inSmin);
public:
	void capSetI(int inImax, int inImin);
public:
	void capSetMode(int inMode);
public:
	int capGetCenterX();
public:
	int capGetCenterY();
public:
	int capGetSum();

	//wifi device
public:
	void WifiSetWeibo(char *ip=NULL);
	void WifiSetLCD(char *displayer,char *ip=NULL);
	void WifiSetSound(char *type,char *ip=NULL);
	//int  WifiGet(char *cmd,char *ip=NULL);
};

#endif /* CYZXINTER_H_ */

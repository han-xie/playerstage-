/*
 * CYZXInter.h
 *
 *  Created on: Jul 19, 2013
 *      Author: keevi7c3l
 */

#ifndef CYZXINTER_H_
#define CYZXINTER_H_

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

#define LABLESTRING "CYZXSPECIALSTRING:"
#define LOGFILENAME "CYZXLOG"
#define WRITELOG 1
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

#define COMTYPE1 '\x01'      //舵机
#define COMTYPE2 '\x02'      //初始化command
#define COMTYPE3 '\x03'      //一般的command
#define REQTYPE1 '\X10'      //request

#define MAXCOMMANDNUMBER 256

#define SRSTARTF '\x01'       //void SRStart();
#define SRSTOPF  '\x02'   //void SRStop();
#define SRPAUSEF  '\x03'       //bool SRPause();
#define SRCONTINUEF    '\x04'  //bool SRContinue();
#define SRINSERTTEXTF    '\x05'//bool SRInsertText(int index,const char*item,int insize);
#define SRGETRESINDEXF  '\x06' //int  SRGetResIndex();
#define SRCLEARITEMSF    '\x07'//void SRClearItems();
#define SRISSTARTEDF    '\08'//bool SRisStarted();
#define BKDELAYMSF '\x09'      //void bkDelayMS(int inMS);
#define BKLCDDISF '\x0a'       //void bkLCDdis(const char*item,int insize);
#define BKUSONICENABLEF '\x0b'  //void bkUSonicEnable();
#define BKGETULTRASONICF '\x0c' //int  bkGetUltrasonic();
#define BKCOMSENDF '\x0d'       //void bkComSend(const void *pBuffer, const int iLength);
#define BKMP3PLAYF '\x0e'       //void bkMp3Play(const char*item,int insize);
#define BKMP3STOPF  '\x0f'      //void bkMp3Stop();
#define BKMFSETSERVOMODEF '\x10'//void bkMFSetServoMode(int inID,int inMode);
#define BKMFSETSERVOPOSF '\x11'//void bkMFSetServoPos(int inID,int inPos,int inSpeed);
#define BKMFSETSERVOROTASPDF '\x12'//void bkMFSetServoRotaSpd(int inID,int inSpeed);
#define BKMFGETSERVOPOSF '\x13'//int bkMFGetServoPos(int inID);
#define BKMFSERVOACTIONF '\x14'//void bkMFServoAction();
#define BKMFSETSERVOROTASPDTIMEF '\x15' //void bkMFSetServoRotaSpdTime(int inID,int inSpeed,unsigned int seconds);
#define AIOCOMINFRAREDF '\x16'            //int aioComInfrared(int port);
#define AIOCOMF '\x17'                    //int aioCom(int port);
#define AIOCOMTF '\x18'                   //int aioComT(int port, int whatKindAIO);
#define whatKindAIOAIODEF '\x19'         	  //int whatKindAIO   默认值
#define whatKindAIOAIOINFRAREDR  '\x1a'   	  //int whatKindAIO   红外测距传感器
#define DIOSETPORTDIRECTF '\x1b'     //void dioSetPortDirect(unsigned int inData);
#define DIOGETDIGIINPUTF '\x1c'      //int dioGetDigiInput(int port);
#define DIODIGIOUTPUTF '\x1d'        //void dioDigiOutput(int port, int inVal);
#define POS2DSERVOMODESERVOF '\x1e'     //void pos2dServoModeServo();
#define POS2DSERVOMODEMOTOF '\x1f'      //void pos2dServoModeMoto();
#define POS2DFORWARDF '\x20'//void pos2dForward(int speed);
#define POS2DBACKWARDF '\x21'           //void pos2dBackward(int speed);
#define POS2DTURNLEFTF '\x22'           //void pos2dTurnLeft(int speed);
#define POS2DTURNRIGHT '\x23'           //void pos2dTurnRight(int speed);
#define POS2DSTOP '\x24'                //void pos2dStop();
#define POS2DSETFRONTL '\x25'           //void pos2dSetFrontL(unsigned int fl);
#define POS2DSETFRONTR '\x26'           //void pos2dSetFrontR(unsigned int fr);
#define POS2DSETREARL '\x27'           //void pos2dSetRearL(unsigned int rl);
#define POS2DSETREARR '\x28'           //void pos2dSetRearR(unsigned int rr);
#define POS2DGETFRONTL '\x29'          //int pos2dGetFrontL();
#define POS2DGETFRONTR '\x2a'          //int pos2dGetFrontR();
#define POS2DGETREARL '\x2b'           //int Pos2dGetRearL();
#define POS2DGETREARR '\x2c'           //int pos2dGetRearR();
#define POS2DSETTIRES '\x2d'          //void pos2dSetTires(unsigned int fl,unsigned int fr,unsigned int rl,unsigned int rr);
#define POS2DGETTIRES '\x2e'           //int pos2dGetTires(unsigned int *pt);
#define POS2DSETSERVOTURN '\x2f'       //void pos2dSetServoTurn(int angle,int inSpeed);
#define POS2DSETSERVOTURNA '\x30'      //void pos2dSetServoTurnA(int angle,int inSpeed);
#define POS2DSET4TIRESF '\x31'         //void pos2dSet4Tires(int id1,int id2,int id3,int id4,int speed1,int speed2,int speed3,int speed4);
#define POS2DSET4TIRESPOSF '\x32'      //void pos2dSet4TiresPos(int id1,int id2,int id3,int id4,int pos,int speed);
#define CAPSETHF '\x33'                    //void capSetH(int inHmax, int inHmin);
#define CAPSETSF '\x34'                    //void capSetS(int inSmax, int inSmin);
#define CAPSETIF '\x35'                    //void capSetI(int inImax, int inImin);
#define CAPSETMODEF '\x36'                 //void capSetMode(int inMode);
#define CAPGETCENTERXF '\x37'              //int capGetCenterX();
#define CAPGETCENTERYF '\x38'              //int capGetCenterY();
#define CAPGETSUMF '\x39'                  //int capGetSum();
#define CAPSETYELLOW '\x3a'                //void capSetYellow();
#define CAPSETRED '\x3b'                   //void capSetRed();
#define CAPSETGREEN '\x3c'               //void capSetGreen();

//below structure storage the moto' speed records to determine whether execute the next command
struct motoRecord{
	char statue;
	int value;
};
#define SEVOMODE '\x01'  //电机模式
#define MOTOMODE '\0'   //舵机模式
#define MAXTIRENUMBER 50

//below redefine the message structure
struct myOpaqueHeadI{
	char type;
	char subtype;
};
struct myOpaqueHI{
	struct myOpaqueHeadI head;
};
struct myOpaqueSt0{
	struct myOpaqueHeadI head;
};
struct myOpaqueSt1{
	struct myOpaqueHeadI head;
	int p1;
};
struct myOpaqueSt2{
	struct myOpaqueHeadI head;
	int p1;
	int p2;
};
struct myOpaqueSt3{
	struct myOpaqueHeadI head;
	int p1;
	int p2;
	int p3;
};
struct myOpaqueSt1c{
	struct myOpaqueHeadI head;
	char c1;
};
struct myOpaqueSt2c{
	struct myOpaqueHeadI head;
	char c1;
	char c2;
};
struct myOpaqueSt4c{
	struct myOpaqueHeadI head;
	char c1;
	char c2;
	char c3;
	char c4;
};

struct myOpaqueStLCD{
	struct myOpaqueHeadI head;
	char disp[LCDMAXLENGTH];
};
struct myOpaqueStMp3{
	struct myOpaqueHeadI head;
	char mp3Title[MP3MAXLENGTH];
};
struct myOpaqueSt4Tires{
	struct myOpaqueHeadI head;
	char tires[4];
	int speed1;
	int speed2;
	int speed3;
	int speed4;
};
struct myOpaqueSt4TiresPos{
	struct myOpaqueHeadI head;
	char tires[4];
	int pos;
	int speed;
};
struct myOpaqueStTime{
	struct myOpaqueHeadI head;
	int p1;
	int p2;
	int time;
};

class CYZXInter {
private:
	PlayerClient *robot;
	DioProxy *diop;
	AioProxy *aiop;
	OpaqueProxy *opaquep;
	Position2dProxy *posp;
	unsigned int DIOPortDirect;
public:
	CYZXInter();
	CYZXInter(PlayerClient *probot);
	CYZXInter(DioProxy *pdiop);
	CYZXInter(AioProxy *paiop);
	CYZXInter(OpaqueProxy *popaquep);
	CYZXInter(Position2dProxy *pposp);
	CYZXInter(PlayerClient *probot, DioProxy *pdiop, AioProxy *paiop,
			OpaqueProxy *popaquep, Position2dProxy *pposp);
	CYZXInter(PlayerClient *probot, OpaqueProxy *popaquep);
	~CYZXInter();

	//backgroud special
	void bkDelayMS(int inMS);
	void bkLCDdisp(const char*item); 	//LCD
	void bkUSonicEnable();    	//Ultrasonic
	int  bkGetUltrasonic();        //Ultrasonic
	void bkComSend(const void *pBuffer, const int iLength);  //SerialCom
	void bkMp3Play(const char*item);          	//mp3
	void bkMp3Stop();                                     //mp3
	void bkMFSetServoMode(int inID,int inMode);
	void bkMFSetServoPos(int inID,int inPos,int inSpeed);
	void bkMFSetServoRotaSpd(int inID,int inSpeed);
	void bkMFSetServoRotaSpdTime(int inID,int inSpeed,int seconds);
	//int bkMFGetServoPos(int inID);
	void bkMFServoAction();


	//SpeechRecongn
	void SRStart();
	void SRStop();
	bool SRPause();
	bool SRContinue();
	bool SRInsertText(int index,const char*item,int insize);
	int  SRGetResIndex();
	void SRClearItems();
	bool SRisStarted();

	//dio
	void dioSetPortDirect(unsigned int inData);
	void dioSetPortDirectOutput(unsigned int from,unsigned int to);    //others is input
	int dioGetDigiInput(int port);                  //can't get result return -1
	void dioDigiOutput(int port, int inVal);

	//aio
	int aioComInfrared(int port);
	int aioCom(int port);
	int aioComT(int port, int whatKindAIO);

	//position2d
	void pos2dServoModeServo();
	void pos2dServoModeMoto();
	void pos2dForward(int speed = 500);
	void pos2dBackward(int speed = 500);
	void pos2dTurnLeft(int speed = 500);
	void pos2dTurnRight(int speed = 500);
	void pos2dStop();
	void pos2dSetFrontL(unsigned int fl);
	void pos2dSetFrontR(unsigned int fr);
	void pos2dSetRearL(unsigned int rl);
	void pos2dSetRearR(unsigned int rr);
	int pos2dGetFrontL();
	int pos2dGetFrontR();
	int Pos2dGetRearL();
	int pos2dGetRearR();
	void pos2dSetTires(unsigned int fl,unsigned int fr,unsigned int rl,unsigned int rr);
	int pos2dGetTires(unsigned int *ptire); //fl fr rl rr
	void pos2dSetServoTurn(int angle,int inSpeed); //void MFSetServoPos(int inID,int inPos,int inSpeed);
	void pos2dSetServoTurnA(int angle,int inSpeed);
	void pos2dSet4Tires(int id1,int id2,int id3,int id4,int speed1,int speed2,int speed3,int speed4);
	void pos2dSet4TiresPos(int id1,int id2,int id3,int id4,int pos,int speed);

	//MFCap
	void capSetH(int inHmax, int inHmin);
	void capSetS(int inSmax, int inSmin);
	void capSetI(int inImax, int inImin);
	void capSetMode(int inMode);
	int capGetCenterX();
	int capGetCenterY();
	int capGetSum();
	void capSetYellow();
	void capSetRed();
	void capSetGreen();
	//...

};

#endif /* CYZXINTER_H_ */

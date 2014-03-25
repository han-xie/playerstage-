#include "background.h"

void MFInit(){}

//LCD
void MF2X4LCD(const char*item,int insize){}

//Ultrasonic
void MFUSonicEnable(){}
int MFGetUltrasonic(){return 0;}

//Delay
void DelayMS(int inMS){}

//SerialCom
void MFComSend(const void *pBuffer, const int iLength){}

//Digi. IO
void MFDigiInit(int interval){}
void MFSetPortDirect(unsigned int inData){}
int MFGetDigiInput(int inID){return 0;}
void MFDigiOutput(int inID,int inVal){}

//AD
void MFADInit(int interval){}
void MFADEnable(int inID){}
int MFGetAD(int inID){return 0;}

//Servo
void MFServoInit(int interval){}
void MFSetServoMode(int inID,int inMode){}
void MFSetServoPos(int inID,int inPos,int inSpeed){}
void MFSetServoRotaSpd(int inID,int inSpeed){}
int MFGetServoPos(int inID){return 0;}
void MFServoAction(){}

//Video Capture
bool MFCapOpen(){}
void MFCapSetH(int inHmax,int inHmin){}
void MFCapSetS(int inSmax,int inSmin){}
void MFCapSetI(int inImax,int inImin){}
void MFCapSetMode(int inMode){}
int MFCapGetCenterX(){return 0;}
int MFCapGetCenterY(){return 0;}
int MFCapGetSum(){return 0;}

//SpeechRecongn
void MFSRStart(){}
void MFSRStop(){}
bool MFSRPause(){return 0;}
bool MFSRContinue(){return 0;}
bool MFSRInsertText(int index,const char*item,int insize){return 0;}
int MFSRGetResIndex(){return 0;}
void MFSRClearItems(){}
bool MFSRisStarted(){return 0;}

//mp3
void MFMp3Play(const char*item,int insize){}
void MFMp3Stop(){}

//UDP
int UDPConnect(char *inTarIP, int inTarPort){return 0;}
void UDPSend(char *inbuf, int inLen){}
int UDPListen(int inPort){return 0;}
//void UDPRecFunc(PBUFREC inFunc){}

//debug
int GetCmdVaildprc(){return 0;}
int GetCmdWaitMax(){return 0;}
int GetCmdCurCnt(){return 0;}
bool ComCreat(int inNo,unsigned int inBaudrate){return 0;}	//inNo:  1-RS422   2-mega128



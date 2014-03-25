//////////////////////////////////////////////////////////////////////////
// for MultiflexNG
// http://robot.up-tech.com
// zwj_uptech@126.com
//////////////////////////////////////////////////////////////////////////

#ifndef MFBACK_H
#define MFBACK_H

#include "debuglevel.h"
#ifdef WIN32
#include "StdAfx.h"
#endif

#include <stdio.h>
//#include "protocol.h"

#ifdef __LINUX__
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#endif
#include <stdlib.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#define SOCKET int
#endif

#include "type.h"

void MFInit();

//LCD
void MF2X4LCD(const char*item,int insize);

//Ultrasonic
void MFUSonicEnable();
int MFGetUltrasonic();

//Delay
void DelayMS(int inMS);

//SerialCom
void MFComSend(const void *pBuffer, const int iLength);

//Digi. IO
void MFDigiInit(int interval);
void MFSetPortDirect(unsigned int inData);
int MFGetDigiInput(int inID);
void MFDigiOutput(int inID,int inVal);

//AD
void MFADInit(int interval);
void MFADEnable(int inID);
int MFGetAD(int inID);

//Servo
void MFServoInit(int interval);
void MFSetServoMode(int inID,int inMode);
void MFSetServoPos(int inID,int inPos,int inSpeed);
void MFSetServoRotaSpd(int inID,int inSpeed);
int MFGetServoPos(int inID);
void MFServoAction();

//Video Capture
bool MFCapOpen();
void MFCapSetH(int inHmax,int inHmin);
void MFCapSetS(int inSmax,int inSmin);
void MFCapSetI(int inImax,int inImin);
void MFCapSetMode(int inMode);
int MFCapGetCenterX();
int MFCapGetCenterY();
int MFCapGetSum();

//SpeechRecongn
void MFSRStart();
void MFSRStop();
bool MFSRPause();
bool MFSRContinue();
bool MFSRInsertText(int index,const char*item,int insize);
int MFSRGetResIndex();
void MFSRClearItems();
bool MFSRisStarted();

//mp3
void MFMp3Play(const char*item,int insize);
void MFMp3Stop();

//UDP
int UDPConnect(char *inTarIP, int inTarPort);
void UDPSend(char *inbuf, int inLen);
int UDPListen(int inPort);
void UDPRecFunc(PBUFREC inFunc);

//debug
int GetCmdVaildprc();
int GetCmdWaitMax();
int GetCmdCurCnt();
bool ComCreat(int inNo,unsigned int inBaudrate);	//inNo:  1-RS422   2-mega128
#endif


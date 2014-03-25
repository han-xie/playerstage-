//////////////////////////////////////////////////////////////////////////
// for MultiflexNG
// http://robot.up-tech.com 
// zwj_uptech@126.com
//////////////////////////////////////////////////////////////////////////
#ifndef MCOMMAND_H
#define MCOMMAND_H

#include "../SerialCom/serial.h"

#define MFBUFLEN 64
#define MFMOTORNO 4
#define MFADNO	  8
#define MFIOMAX   12
#define MFSERVOMAX   0xff

typedef struct tag_MF_Servo_Info  
{
	int id;
	unsigned char PosH;	
	unsigned char PosL;
	tag_MF_Servo_Info* next;
}MFServoInfo;

class CMFCmd : public CSerialCom 
{
public:
	void m_Parse0x30Dev();

	//Digi. Input
	void SetDigiDirection(unsigned char* inDir);
	void UpdateInput();								//单次查询
	void ScheUpdateInput(int interval);				//周期轮询启动函数
	static void *updateInputThread(void* pParam);	//轮询线程函数

	//Digi. Output
	void PutDigiOutput(int inID,int inVal);

	//AD Input
	void ADEnable(int inID);
	void UpdateAD(int inID);
	void ScheUpdateAD(int interval);
	static void* updateADThread(void* pParam);

	//GET Interface
	int GetADInputVal(int inNo);
	bool GetDigiInputVal(int inNo);

	//output
	void DCMotors(int* inRotSpd,int* inDur);
	void Servo(int* inAngle,int* inSpeed);

	//servo	
	int GetServoPos(int inID);
	void SetServoRotaSpd(int inID,int inSpeed);
	void SetServoPos(int inID,int inPos,int inSpeed);
	void SetServoMode(int inID,int inMode);
	void ServoAction(int inID);
	void ServoAction();

	void ServoEnable(int inID);

	static void* updateServoPosThread(void *pParam);
	void ScheUpdateServoPos(int interval);
	void UpdateServoPos(int inID);

	//all
	void ScheUpdateAll(int interval);
	static void* updateAllThread(void* pParam);

	//data
	unsigned char sendbuf[MFBUFLEN];
	unsigned char parsbuf[MFBUFLEN];
	void DataRecv(unsigned char *inBuf, int inLen);
	
	CMFCmd();
	~CMFCmd();
	
protected:
	void m_Split2Bytes(unsigned int inSrc,unsigned char* inTarg);
	void m_CalBufSum(unsigned char* inBuf,int inLen);
	void m_ParseByte(unsigned char inchar);
	void m_ParseFrame(unsigned char* inBuf,int inLen);
	void m_ResetRcvBuf();

	//buf
	int m_nRcvIndex;
	int m_nFrameLength;

	//Digi Input
	unsigned char m_dataDigiInput[2];	
	int m_inputInterval;
	pthread_t pidInputThread;
	bool m_bDigiInputEnable;

	//Digi Output
	unsigned char m_dataDigiOutput[2];

	//AD
	unsigned char m_dataADInputH[MFADNO];
	unsigned char m_dataADInputL[MFADNO];
	bool m_arADEnable[MFADNO];

	int m_ADInterval;
	pthread_t pidADThread;

	//servo
	unsigned char m_dataServoPosH[MFSERVOMAX];
	unsigned char m_dataServoPosL[MFSERVOMAX];
	bool m_arServoEnable[MFSERVOMAX];

	int m_ServoInterval;
	pthread_t pidServoThread;

	//all
	int m_nUpdateAllInterval;
	pthread_t pidUpdateAllThread;

	char m_last;
	bool m_bFrameStart;

	WORD m_wTemp;
};

#endif

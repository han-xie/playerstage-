//////////////////////////////////////////////////////////////////////////
// for MultiflexNG
// http://robot.up-tech.com 
// Designed by lsy
// Modified by zwj @ 20090223
// zwj_uptech@126.com
//////////////////////////////////////////////////////////////////////////
#ifndef SERIAL_H_H
#define SERIAL_H_H
//#include <queue>
#ifdef __LINUX__
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <termios.h>
#endif//__LINUX__

#include <stdio.h>
#include "../type.h"
#include "CQueue.h"

struct CMDBUF 
{
	UCHAR * pCmdBuf;	//指令数组指针
	UINT nLen;			//指令长度
};

class CSerialCom
{
public:
	void ResetCnt();
	virtual void DataRecv(unsigned char* inBuf,int inLen){};			//接收数据
#ifdef WIN32
	void SetComProp(DCB* inProp);
#endif
	BOOL Create(int inCom , unsigned int inBaudrate);									//打开一个串口端口
	void Close();											//关闭当前打开的端口
//	void SetCmd(CFstarCmd *pCmd);								//设置指令类对象（标准接口）
	void Send(const void *pBuffer, const int iLength);		//通过缓冲列表进行指令发送（标准接口）
#ifdef WIN32
	void ComSend(const void *pBuffer, const int iLength);	//直接发送指令
#endif
	CSerialCom();
	virtual ~CSerialCom();	
#ifdef WIN32
	static DWORD WINAPI ComMonitor(LPVOID pParam);
	static DWORD WINAPI SendingThread(LPVOID pParam);
#endif
#ifdef __LINUX__
	static void *ComMonitor(void *pParam);
	static void *SendingThread(void *pParam);
#endif
	
	bool m_bCmdRsp;
	
	//debug
	unsigned int nMaxCnt;
	unsigned int nWaitMax;
	unsigned int nSendCnt;
	unsigned int nRspCnt;
	unsigned int nCurCnt;

protected:
	bool bToStop;
	//串口属性
#ifdef WIN32
	int m_com;												
	int m_baudrate;
	BYTE m_bytesize;
	BYTE m_parity;
	BYTE m_stopbits;
#endif
private:
	CQueue<CMDBUF *> m_cmdlist;	
#ifdef WIN32
	BOOL driveDebug_flag;
//	std::queue<CMDBUF *> m_cmdqueue;										//发送指令的缓冲列表
	HANDLE mutex;
	HANDLE m_hCom;											//已打开的串口句柄
#endif 
#ifdef __LINUX__
	int m_iCom;
//	pthread_mutex_t mutex;
	struct termios m_oldtio;
	pthread_t pidSendThread, pidMonitorThread;
#endif
};


#endif

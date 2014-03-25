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
	UCHAR * pCmdBuf;	//ָ������ָ��
	UINT nLen;			//ָ���
};

class CSerialCom
{
public:
	void ResetCnt();
	virtual void DataRecv(unsigned char* inBuf,int inLen){};			//��������
#ifdef WIN32
	void SetComProp(DCB* inProp);
#endif
	BOOL Create(int inCom , unsigned int inBaudrate);									//��һ�����ڶ˿�
	void Close();											//�رյ�ǰ�򿪵Ķ˿�
//	void SetCmd(CFstarCmd *pCmd);								//����ָ������󣨱�׼�ӿڣ�
	void Send(const void *pBuffer, const int iLength);		//ͨ�������б����ָ��ͣ���׼�ӿڣ�
#ifdef WIN32
	void ComSend(const void *pBuffer, const int iLength);	//ֱ�ӷ���ָ��
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
	//��������
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
//	std::queue<CMDBUF *> m_cmdqueue;										//����ָ��Ļ����б�
	HANDLE mutex;
	HANDLE m_hCom;											//�Ѵ򿪵Ĵ��ھ��
#endif 
#ifdef __LINUX__
	int m_iCom;
//	pthread_mutex_t mutex;
	struct termios m_oldtio;
	pthread_t pidSendThread, pidMonitorThread;
#endif
};


#endif

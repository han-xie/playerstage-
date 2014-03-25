/*
 * CYZXInter.cc
 *
 *  Created on: Jul 19, 2013
 *      Author: keevi7c3l
 */

#include "CYZXInter.h"

#include <iostream>
#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>
using namespace PlayerCc;

void OpaqueProxy::newSizeFormDevice(int size) {
	if (NULL == this->mDevice->data)
		this->mDevice->data = new uint8_t[size];
}

void OpaqueProxy::newFormDevice(int size) {
}

void OpaqueProxy::deleteSize() {
	if (NULL != this->mDevice->data)
		delete[] this->mDevice->data;
}

CYZXInter::CYZXInter() {
	this->robot = NULL;
	this->diop = NULL;
	this->aiop = NULL;
	this->opaquep = NULL;
	this->posp = NULL;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(PlayerClient *probot) {
	this->robot = probot;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(DioProxy *pdiop) {
	this->diop = pdiop;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(AioProxy *paiop) {
	this->aiop = paiop;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(OpaqueProxy *popaquep) {
	this->opaquep = popaquep;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(Position2dProxy *pposp) {
	this->posp = pposp;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(PlayerClient *probot, OpaqueProxy *popaquep) {
	this->robot = probot;
	this->opaquep = popaquep;
	this->DIOPortDirect = 0x0;
}
CYZXInter::CYZXInter(PlayerClient *probot, DioProxy *pdiop, AioProxy *paiop,
		OpaqueProxy *popaquep, Position2dProxy *pposp) {
	this->robot = probot;
	this->diop = pdiop;
	this->aiop = paiop;
	this->opaquep = popaquep;
	this->posp = pposp;
	this->DIOPortDirect = 0x0;
}
CYZXInter::~CYZXInter() {
}
/*
 void CYZXInter::dioCom(int port) {
 if (this->diop != NULL) {
 this->diop->SetOutput(port, port);
 }
 return;
 }

 uint32_t CYZXInter::dioRes() {
 if (this->robot != NULL) {
 this->robot->Read();
 return this->diop->GetCount();
 }
 return 0;
 }
 */

int CYZXInter::aioComInfrared(int port) {
	return aioComT(port, whatKindAIOAIOINFRAREDR);
}
int CYZXInter::aioCom(int port) {
	return aioComT(port, whatKindAIOAIODEF);
}
int CYZXInter::aioComT(int port, int whatKindAIO) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEAIO;
	sendComd.subtype = AIOCOMTF;
	sendComd.p1 = port;
	sendComd.p2 = whatKindAIO;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respAIO) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}

//dio
void CYZXInter::dioSetPortDirect(unsigned int inData) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIOSETPORTDIRECTF;
	sendComd.p1 = inData;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::dioSetPortDirectOutput(unsigned int from, unsigned int to) { //others is input
	unsigned int inData = 0x00000000;
	if (from < DIOPORTSMIN)
		from = DIOPORTSMIN;
	if (to > DIOPORTSMAX)
		to = DIOPORTSMAX;
	for (int i = from; i <= to; i++) {
		inData |= (1 << i);
	}
#ifdef WRITELOGC
	printf("%x\n",inData);
#endif
	dioSetPortDirect(inData);
}
int CYZXInter::dioGetDigiInput(int port) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIOGETDIGIINPUTF;
	sendComd.p1 = port;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respDIO) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
void CYZXInter::dioDigiOutput(int port, int inVal) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIODIGIOUTPUTF;
	sendComd.p1 = port;
	sendComd.p2 = inVal;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}

//position2d
void CYZXInter::pos2dServoModeServo() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSERVOMODESERVOF;
	sendComd.p1 = 0;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dServoModeMoto() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSERVOMODEMOTOF;
	sendComd.p1 = 1;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dForward(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DFORWARDF;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dBackward(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DBACKWARDF;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dTurnLeft(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DTURNLEFTF;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dTurnRight(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DTURNRIGHT;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dStop() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSTOP;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetFrontL(unsigned int fl) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETFRONTL;
	sendComd.p1 = fl;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetFrontR(unsigned int fr) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETFRONTR;
	sendComd.p1 = fr;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetRearL(unsigned int rl) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETREARL;
	sendComd.p1 = rl;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetRearR(unsigned int rr) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETREARR;
	sendComd.p1 = rr;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::pos2dGetFrontL() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETFRONTL;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
int CYZXInter::pos2dGetFrontR() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETFRONTR;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
int CYZXInter::Pos2dGetRearL() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETREARL;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
int CYZXInter::pos2dGetRearR() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETREARR;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
void CYZXInter::pos2dSetTires(unsigned int fl, unsigned int fr, unsigned int rl,
		unsigned int rr) {

	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETTIRES;

	unsigned int ttire = 0x00000000;
	ttire |= (fl & 0x0000FFFF) | ((fr << 16) & 0xFFFF0000);
	sendComd.p1 = ttire;
	ttire = 0x00000000;
	ttire |= (rl & 0x0000FFFF) | ((rr << 16) & 0xFFFF0000);
	sendComd.p2 = ttire;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

}
int CYZXInter::pos2dGetTires(unsigned int *ptire) { //fl fr rl rr
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETTIRES;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get 4 tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	*ptire = (recRes.p1) & 0x0000FFFF;
	*(ptire + 1) = (recRes.p1) >> 16;
	*(ptire + 2) = (recRes.p2) & 0x0000FFFF;
	*(ptire + 3) = (recRes.p2) >> 16;
	this->opaquep->deleteSize();
	return 0;
}
//MFCap
void CYZXInter::capSetH(int inHmax, int inHmin) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETHF;
	sendComd.p1 = inHmax;
	sendComd.p2 = inHmin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetS(int inSmax, int inSmin) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETSF;
	sendComd.p1 = inSmax;
	sendComd.p2 = inSmin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetI(int inImax, int inImin) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETIF;
	sendComd.p1 = inImax;
	sendComd.p2 = inImin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetMode(int inMode) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETMODEF;
	sendComd.p1 = inMode;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::capGetCenterX() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETCENTERXF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respMFCap) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
int CYZXInter::capGetCenterY() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETCENTERYF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respMFCap) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
int CYZXInter::capGetSum() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETSUMF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respMFCap) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
void CYZXInter::capSetYellow() {
	this->capSetH(110, 20);
	this->capSetS(200, 60);
}
void CYZXInter::capSetRed() {
	this->capSetH(50, 0);
	this->capSetS(255, 100);
}
void CYZXInter::capSetGreen() {

}

//backgroud special
void CYZXInter::bkDelayMS(int inMS) {
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKDELAYMSF;
	sendComd.p1 = inMS;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::bkLCDdis(const char*item, int insize) {	//LCD

}
void CYZXInter::bkUSonicEnable() {   	//Ultrasonic
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKUSONICENABLEF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::bkGetUltrasonic() {       //Ultrasonic
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKGETULTRASONICF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respBk1) + 20);
	if (0 != this->opaquep->SendReq(&popa)) {
		this->opaquep->deleteSize();
		return -1;
	}
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respBk1 recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	this->opaquep->deleteSize();
	return recRes.p1;
}
void CYZXInter::bkComSend(const void *pBuffer, const int iLength) {  //SerialCom

}
void CYZXInter::bkMp3Play(const char*item, int insize) {          	//mp3

}
void CYZXInter::bkMp3Stop() {                                    //mp3
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMP3STOPF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}

//SpeechRecongn
void CYZXInter::SRStart() {

}
void CYZXInter::SRStop() {

}
bool CYZXInter::SRPause() {
	return false;
}
bool CYZXInter::SRContinue() {
	return false;
}
bool CYZXInter::SRInsertText(int index, const char*item, int insize) {
	return false;
}
int CYZXInter::SRGetResIndex() {
	return -1;
}
void CYZXInter::SRClearItems() {

}
bool CYZXInter::SRisStarted() {
	return false;
}

//dio - improve
/*
 void dioSetPortDirect(unsigned int inData);
 void dioSetPortDirectOutput(unsigned int from,unsigned int to);    //others is input
 int dioGetDigiInput(int port);                  //can't get result return -1
 void dioDigiOutput(int port, int inVal);
 */
int CYZXInter::hallsensorInit(unsigned int port) {
	this->DIOPortDirect = (this->DIOPortDirect) && (~(1 << port));
	this->dioSetPortDirect(this->DIOPortDirect);
}

int CYZXInter::hallsensor(unsigned int port) {
	return this->dioGetDigiInput(port);
}

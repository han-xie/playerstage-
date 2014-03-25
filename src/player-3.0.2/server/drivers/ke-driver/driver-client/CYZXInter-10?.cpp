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

char ConvertIntToChar[51] = { '\0', '\x01', '\x02', '\x03', '\x04', '\x05',
		'\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e',
		'\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
		'\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', '\x20',
		'\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28', '\x29',
		'\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f', '\x30', '\x31', '\x32' };

void OpaqueProxy::newSizeFormDevice(int size) {
	if (NULL == this->mDevice->data)
		this->mDevice->data = new uint8_t[size];
}

void OpaqueProxy::newFormDevice(int size) {
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
	myOpaqueSt2c sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = AIOCOMTF;
	sendComd.c1 = ConvertIntToChar[port];
	sendComd.c2 = ConvertIntToChar[whatKindAIO];

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}

//dio
void CYZXInter::dioSetPortDirect(unsigned int inData) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = DIOSETPORTDIRECTF;
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
	myOpaqueSt1c sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = DIOGETDIGIINPUTF;
	sendComd.c1 = ConvertIntToChar[port];

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
void CYZXInter::dioDigiOutput(int port, int inVal) {
	player_opaque_data_t popa;
	myOpaqueSt2c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = DIODIGIOUTPUTF;
	sendComd.c1 = ConvertIntToChar[port];
	sendComd.c2 = ConvertIntToChar[inVal];

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}

//position2d
void CYZXInter::pos2dSet4TiresPos(int id1, int id2, int id3, int id4, int pos,
		int speed) {
	player_opaque_data_t popa;
	myOpaqueSt4TiresPos sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DSET4TIRESPOSF;
	sendComd.pos = pos;
	sendComd.speed = speed;
	sendComd.tires[0] = ConvertIntToChar[id1];
	sendComd.tires[1] = ConvertIntToChar[id2];
	sendComd.tires[2] = ConvertIntToChar[id3];
	sendComd.tires[3] = ConvertIntToChar[id4];

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSet4Tires(int id1, int id2, int id3, int id4, int sp1,
		int sp2, int sp3, int sp4) {
	player_opaque_data_t popa;
	myOpaqueSt4Tires sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DSET4TIRESF;
	sendComd.tires[0] = ConvertIntToChar[id1];
	sendComd.tires[1] = ConvertIntToChar[id2];
	sendComd.tires[2] = ConvertIntToChar[id3];
	sendComd.tires[3] = ConvertIntToChar[id4];
	sendComd.speed1 = sp1;
	sendComd.speed2 = sp2;
	sendComd.speed3 = sp3;
	sendComd.speed4 = sp4;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dServoModeServo() {
	player_opaque_data_t popa;
	myOpaqueSt1c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSERVOMODESERVOF;
	sendComd.c1 = '\0';

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dServoModeMoto() {
	player_opaque_data_t popa;
	myOpaqueSt1c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSERVOMODEMOTOF;
	sendComd.c1 = '\x01';

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dForward(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DFORWARDF;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dBackward(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DBACKWARDF;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dTurnLeft(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DTURNLEFTF;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dTurnRight(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DTURNRIGHT;
	sendComd.p1 = speed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dStop() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = POS2DSTOP;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetFrontL(unsigned int fl) {
	player_opaque_data_t popa;
	myOpaqueSt1c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSETFRONTL;
	sendComd.c1 = fl;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetFrontR(unsigned int fr) {
	player_opaque_data_t popa;
	myOpaqueSt1c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSETFRONTR;
	sendComd.c1 = fr;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetRearL(unsigned int rl) {
	player_opaque_data_t popa;
	myOpaqueSt1c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSETREARL;
	sendComd.c1 = rl;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetRearR(unsigned int rr) {
	player_opaque_data_t popa;
	myOpaqueSt1c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSETREARR;
	sendComd.c1 = rr;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::pos2dGetFrontL() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = POS2DGETFRONTL;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
int CYZXInter::pos2dGetFrontR() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = POS2DGETFRONTR;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
int CYZXInter::Pos2dGetRearL() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = POS2DGETREARL;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
int CYZXInter::pos2dGetRearR() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = POS2DGETREARR;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
void CYZXInter::pos2dSetTires(unsigned int fl, unsigned int fr, unsigned int rl,
		unsigned int rr) {

	player_opaque_data_t popa;
	myOpaqueSt4c sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSETTIRES;
	sendComd.c1 = fl;
	sendComd.c2 = fr;
	sendComd.c3 = rl;
	sendComd.c4 = rr;
	/*
	 unsigned int ttire = 0x00000000;
	 ttire |= (fl & 0x0000FFFF) | ((fr << 16) & 0xFFFF0000);
	 sendComd.p1 = ttire;
	 ttire = 0x00000000;
	 ttire |= (rl & 0x0000FFFF) | ((rr << 16) & 0xFFFF0000);
	 sendComd.p2 = ttire;
	 */

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

}
void CYZXInter::pos2dSetServoTurn(int angle, int inSpeed) { //void MFSetServoPos(int inID,int inPos,int inSpeed);
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = POS2DSETSERVOTURN;

	sendComd.p1 = angle;
	sendComd.p2 = inSpeed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetServoTurnA(int angle, int inSpeed) {

}

int CYZXInter::pos2dGetTires(unsigned int *ptire) { //fl fr rl rr
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = POS2DGETTIRES;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get 4 tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt4c recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	/*
	 *ptire = (recRes.p1) & 0x0000FFFF;
	 *(ptire + 1) = (recRes.p1) >> 16;
	 *(ptire + 2) = (recRes.p2) & 0x0000FFFF;
	 *(ptire + 3) = (recRes.p2) >> 16;
	 */
	*ptire = recRes.c1;
	*(ptire + 1) = recRes.c2;
	*(ptire + 2) = recRes.c3;
	*(ptire + 3) = recRes.c4;
	return 0;
}
//MFCap
void CYZXInter::capSetH(int inHmax, int inHmin) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = CAPSETHF;
	sendComd.p1 = inHmax;
	sendComd.p2 = inHmin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetS(int inSmax, int inSmin) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = CAPSETSF;
	sendComd.p1 = inSmax;
	sendComd.p2 = inSmin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetI(int inImax, int inImin) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = CAPSETIF;
	sendComd.p1 = inImax;
	sendComd.p2 = inImin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetMode(int inMode) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = CAPSETMODEF;
	sendComd.p1 = inMode;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::capGetCenterX() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = CAPGETCENTERXF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
int CYZXInter::capGetCenterY() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = CAPGETCENTERYF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
int CYZXInter::capGetSum() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = CAPGETSUMF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
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

//background special
void CYZXInter::bkMFSetServoRotaSpdTime(int inID, int inSpeed, int seconds) {
	player_opaque_data_t popa;
	myOpaqueSt3 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = BKMFSETSERVOROTASPDTIMEF;
	sendComd.p1 = inID;
	sendComd.p2 = inSpeed;
	sendComd.p3 = seconds;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::bkMp3Play(const char*item) {          	//mp3
	player_opaque_data_t popa;
	myOpaqueStMp3 sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKMP3PLAYF;
	for (int i = 0; i < MP3MAXLENGTH; i++)
		sendComd.mp3Title[i] = '\0';
	for (int i = 0; i < strlen(item) && i < MP3MAXLENGTH; i++)
		sendComd.mp3Title[i] = *(item + i);

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::bkMp3Stop() {                                    //mp3
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKMP3STOPF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::bkLCDdisp(const char*item) {
	player_opaque_data_t popa;
	myOpaqueStLCD sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKLCDDISF;
	for (int i = 0; i < LCDMAXLENGTH; i++)
		sendComd.disp[i] = '\x20';
	for (int i = 0; i < strlen(item) && i < LCDMAXLENGTH; i++) {
		sendComd.disp[i] = *(item + i);
	}

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::bkMFSetServoPos(int inID, int inPos, int inSpeed) {
	player_opaque_data_t popa;
	myOpaqueSt3 sendComd;

	sendComd.head.type = COMTYPE1;
	sendComd.head.subtype = BKMFSETSERVOPOSF;
	sendComd.p1 = inID;
	sendComd.p2 = inPos;
	sendComd.p3 = inSpeed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
void CYZXInter::bkMFSetServoMode(int inID, int inMode) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKMFSETSERVOMODEF;
	sendComd.p1 = inID;
	sendComd.p2 = inMode;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
//void bkMFSetServoPos(int inID,int inPos,int inSpeed);
void CYZXInter::bkMFSetServoRotaSpd(int inID, int inSpeed) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKMFSETSERVOROTASPDF;
	sendComd.p1 = inID;
	sendComd.p2 = inSpeed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
//int bkMFGetServoPos(int inID);
void CYZXInter::bkMFServoAction() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKMFSERVOACTIONF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
void CYZXInter::bkDelayMS(int inMS) {
	player_opaque_data_t popa;
	myOpaqueSt1 sendComd;

	sendComd.head.type = COMTYPE3;
	sendComd.head.subtype = BKDELAYMSF;
	sendComd.p1 = inMS;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}

void CYZXInter::bkUSonicEnable() {   	//Ultrasonic
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = COMTYPE2;
	sendComd.head.subtype = BKUSONICENABLEF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::bkGetUltrasonic() {       //Ultrasonic
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.head.type = REQTYPE1;
	sendComd.head.subtype = BKGETULTRASONICF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(100);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif

	int recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes;
}
void CYZXInter::bkComSend(const void *pBuffer, const int iLength) {  //SerialCom

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


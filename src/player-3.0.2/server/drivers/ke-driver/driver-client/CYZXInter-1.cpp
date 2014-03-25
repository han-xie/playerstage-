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
#include <libplayerinterface/player_interfaces.h>
using namespace PlayerCc;

CYZXInter::CYZXInter() {
	this->robot = NULL;
	this->diop = NULL;
	this->aiop = NULL;
	this->opaquep = NULL;
	this->posp = NULL;
}
CYZXInter::CYZXInter(PlayerClient *probot) {
	this->robot = probot;
}
CYZXInter::CYZXInter(DioProxy *pdiop) {
	this->diop = pdiop;
}
CYZXInter::CYZXInter(AioProxy *paiop) {
	this->aiop = paiop;
}
CYZXInter::CYZXInter(OpaqueProxy *popaquep) {
	this->opaquep = popaquep;
}
CYZXInter::CYZXInter(Position2dProxy *pposp) {
	this->posp = pposp;
}
CYZXInter::CYZXInter(PlayerClient *probot, OpaqueProxy *popaquep) {
	this->robot = probot;
	this->opaquep = popaquep;
}
CYZXInter::CYZXInter(PlayerClient *probot, DioProxy *pdiop, AioProxy *paiop,
		OpaqueProxy *popaquep, Position2dProxy *pposp) {
	this->robot = probot;
	this->diop = pdiop;
	this->aiop = paiop;
	this->opaquep = popaquep;
	this->posp = pposp;
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
	this->opaquep->SendCmd(&popa);

	myOpaqueSt recRes;
	this->robot->Read();
	this->opaquep->GetData((uint8_t *) &recRes);
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
int CYZXInter::dioGetDigiInput(int port) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIOGETDIGIINPUTF;
	sendComd.p1 = port;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	myOpaqueSt recRes;
	this->robot->Read();
	this->opaquep->GetData((uint8_t *) &recRes);
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
	sleep(1);
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
	sleep(1);
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
	sleep(1);
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
	this->opaquep->SendCmd(&popa);

	myOpaqueSt recRes;
	this->robot->Read();
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::capGetCenterY() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETCENTERYF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	myOpaqueSt recRes;
	this->robot->Read();
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::capGetSum() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETSUMF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	myOpaqueSt recRes;
	this->robot->Read();
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}

void CYZXInter::capSetYellow() {
	MFCapSetH(110, 20);
	MFCapSetS(200, 60);
}
void CYZXInter::capSetRed() {

}
void CYZXInter::capSetGreen() {

}

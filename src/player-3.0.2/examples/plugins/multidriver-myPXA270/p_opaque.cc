#include "multidriver.h"

InterfaceOpaque::InterfaceOpaque(player_devaddr_t addr, multidriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section,"publish",1);
	this->conf.frontL = cf->ReadInt(section, "frontL", 1);
	this->conf.frontR = cf->ReadInt(section, "frontR", 2);
	this->conf.rearL = cf->ReadInt(section, "rearL", 3);
	this->conf.rearR = cf->ReadInt(section, "rearR", 4);
	this->conf.servoFR = cf->ReadInt(section, "servofr", 5);
	this->conf.servoFL = cf->ReadInt(section, "servofl", 6);
	this->conf.servoRR = cf->ReadInt(section, "servorr", 7);
	this->conf.servoRL = cf->ReadInt(section, "servorl", 8);
	MFSetServoMode(this->conf.frontL, 1);
	MFSetServoMode(this->conf.frontR, 1);
	MFSetServoMode(this->conf.rearL, 1);
	MFSetServoMode(this->conf.rearR, 1);
	MFSetServoMode(this->conf.servoFR, 0);
	MFSetServoMode(this->conf.servoFL, 0);
	MFSetServoMode(this->conf.servoRR, 0);
	MFSetServoMode(this->conf.servoRL, 0);
}

int InterfaceOpaque::ProcessMessage(QueuePointer &resp_queue,
		player_msghdr_t* hdr, void* data) {

	if (hdr->addr.interf == this->Interface::addr.interf) {
		if (-1 == this->dealOpaqueMessages(resp_queue, hdr, data)) {
			return -1;
		}
		return 0;
	}else {
		return -1;
	}
}
void InterfaceOpaque::Publish(void) {
	return;
}

int InterfaceOpaque::dealOpaqueMFCAPCommand(myOpaqueSt *popa) {
#ifdef WRITELOG
	printf("coming opaque command : %d %d %d %d\n", popa->type, popa->subtype,
			popa->p1, popa->p2);
#endif
	switch (popa->subtype) {
	case 1: {
		MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case 2: {
		MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case 3: {
		MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case 4: {
		MFCapSetMode(popa->p1);
		break;
	}
	default:
		break;
	}
	return 0;
}
int InterfaceOpaque::dealOpaqueBKComReq(comReqBk1 *popa, respBk1 *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;

	if (BKDELAYMSF == popa->subtype) {
		DelayMS(popa->p1);
	} else if (BKLCDDISF == popa->subtype) {

	} else if (BKUSONICENABLEF == popa->subtype) {
		MFUSonicEnable();
	} else if (BKGETULTRASONICF == popa->subtype) {

	} else if (BKCOMSENDF == popa->subtype) {

	} else if (BKMP3PLAYF == popa->subtype) {

	} else if (BKMP3STOPF == popa->subtype) {
		MFMp3Stop();
	} else
		return -1;
	return 0;
}
int InterfaceOpaque::dealOpaqueBKGETULTRASONICF(comReqBk1 *popa) {
	player_opaque_data_t opaqueRes;
	respBk1 res;

#ifdef WRITELOG
	printf("req opaque get ultrasonic ");
	printf("%d %d %d\n", popa->type, popa->subtype, popa->p1);
#endif

	res.type = popa->type;
	res.subtype = popa->subtype;
	res.p1 = MFGetUltrasonic();

	opaqueRes.data_count = sizeof(res);
	uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
			+ opaqueRes.data_count;
	opaqueRes.data = new uint8_t[opaqueRes.data_count];
	memcpy(opaqueRes.data, &res, opaqueRes.data_count);

#ifdef WRITELOG
	printf("req opaque ultrasonic response data_count %d , size %d\n",
			opaqueRes.data_count, size);
	printf("outing ultrasonic message %d %d %d\n", res.type, res.subtype,
			res.p1);
#endif

	this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
			PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
	return 0;
}
int InterfaceOpaque::dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case CAPSETHF: {
		MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case CAPSETSF: {
		MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case CAPSETIF: {
		MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case CAPSETMODEF: {
		MFCapSetMode(popa->p1);
		break;
	}
	case CAPGETCENTERXF: {
		pr->p1 = MFCapGetCenterX();
		break;
	}
	case CAPGETCENTERYF: {
		pr->p1 = MFCapGetCenterY();
		break;
	}
	case CAPGETSUMF: {
		pr->p1 = MFCapGetSum();
		break;
	}
	default: {
		break;
	}
	}
	return 0;
}
int InterfaceOpaque::dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case 5: {
		pr->p1 = MFCapGetCenterX();
		break;
	}
	case 6: {
		pr->p1 = MFCapGetCenterY();
		break;
	}
	case 7: {
		pr->p1 = MFCapGetSum();
		break;
	}
	default: {
		break;
	}
	}
#ifdef WRITELOG
	printf("comming opaque req : %d %d %d %d \n", popa->type, popa->subtype,
			popa->p1, popa->p2);
	printf("outing opaque message : %d %d %d %d\n", pr->type, pr->subtype,
			pr->p1, pr->p2);
#endif
	return 0;
}

int InterfaceOpaque::dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;

	switch (popa->subtype) {
	case CAPGETCENTERXF: {
		dealOpaqueMFCAPComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case CAPGETCENTERYF: {

		dealOpaqueMFCAPComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case CAPGETSUMF: {

		dealOpaqueMFCAPComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case CAPSETHF: {

		dealOpaqueMFCAPComReq(popa, &res);
		break;
	}
	case CAPSETSF: {

		dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETIF: {

		dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETMODEF: {

		dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETYELLOW: {

		dealOpaqueMFCAPComReq(popa, &res);
		break;
	}
	case CAPSETRED: {
		dealOpaqueMFCAPComReq(popa, &res);
		break;
	}
	case CAPSETGREEN: {
		dealOpaqueMFCAPComReq(popa, &res);
		break;
	}
	default: {
		return -1;
		break;
	}
	}

	return 0;
}
int InterfaceOpaque::dealOpaqueBK(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	pid_t pid;
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
	switch (popa->subtype) {
	case BKMFSETSERVOROTASPDF: {
		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		MFSetServoRotaSpd(popa->p1, popa->p2);
		//MFServoAction();
		break;
	}
	case BKMFSETSERVOMODEF: {
		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		MFSetServoMode(popa->p1, popa->p2);
		break;
	}
	case BKMFSETSERVOPOSF: {
		myOpaqueSt3 *popa = (myOpaqueSt3 *) opaquedata->data;
		MFSetServoPos(popa->p1, popa->p2, popa->p3);
		break;
	}

	case BKMFGETSERVOPOSF: {
		break;
	}

	case BKGETULTRASONICF: {
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		if (-1 == dealOpaqueBKGETULTRASONICF(popa))
			return -1;
		break;
	}
	case BKMFSERVOACTIONF: {
		myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
		MFServoAction();
		break;
	}
	case BKMFSETSERVOROTASPDTIMEF: {
		break;
	}

	case BKMP3PLAYF: {
		myOpaqueStMp3 *popa = (myOpaqueStMp3 *) opaquedata->data;

		if ((pid = fork()) < 0) {
			printf("fork error\n");
		} else if (pid == 0) {
			char mp3Title[MP3MAXLENGTH + 1];
			for (int i = 0; i < MP3MAXLENGTH + 1; i++)
				mp3Title[i] = '\0';
			for (int i = 0; i < MP3MAXLENGTH; i++)
				mp3Title[i] = popa->mp3Title[i];
			if (execl("/mnt/yaffs/test/MP3Play", mp3Title, (char *) 0) < 0)
				printf("exe error\n");
		}
		/*
		 if (waitpid(pid, NULL, 0) < 0) {
		 printf("wait error\n");
		 }*/
		return 0;
		break;
	}

	case BKCOMSENDF: {
		break;
	}
	case BKMP3STOPF: {
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;

		//MFMp3Stop();

		if ((pid = fork()) < 0) {
			printf("fork error\n");
		} else if (pid == 0) {
			if (execl("/mnt/yaffs/test/MP3Stop", "MP3STOP", (char *) 0) < 0)
				printf("exe error\n");
		}
		/*
		 if (waitpid(pid, NULL, 0) < 0) {
		 printf("wait error\n");
		 }*/
		break;
	}

	case BKDELAYMSF: {
		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		DelayMS(popa->p1);

		break;
	}
	case BKLCDDISF: {

		myOpaqueStLCD *popa = (myOpaqueStLCD*) opaquedata->data;

		if (pid = fork() < 0) {
			printf("fork error\n");
		} else if (pid == 0) {
			char lcdDisp[LCDMAXLENGTH];
			for (int i = 0; i < LCDMAXLENGTH; i++)
				lcdDisp[i] = '\x20';
			for (int i = 0; i < LCDMAXLENGTH; i++) {
				lcdDisp[i] = popa->disp[i];
			}
			if (execl("/mnt/yaffs/test/LCDDisp", lcdDisp, (char *) 0) < 0)
				printf("exe error\n");
		}
		/*
		 if (waitpid(pid, NULL, 0) < 0) {
		 printf("wait error\n");
		 }
		 */

		return 0;
		break;
	}
	case BKUSONICENABLEF: {

		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		MFUSonicEnable();

		break;
	}
	}

	return 0;
}
int InterfaceOpaque::dealOpaqueSR(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	return -1;
}
int InterfaceOpaque::dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;


	switch (popa->subtype) {
	case AIOCOMTF: {

		dealOpaqueAIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case AIOCOMINFRAREDF: {

		dealOpaqueAIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case AIOCOMF: {

		dealOpaqueAIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;
}
int InterfaceOpaque::dealOpaquePosition2d(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	comReqPos2d *popa = (comReqPos2d *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	respPos2d res;

	switch (popa->subtype) {
	case POS2DSET4TIRESF: {

		myOpaqueSt4Tires *popa4tires = (myOpaqueSt4Tires *) opaquedata->data;
		int id1 = popa4tires->tires[0];
		int id2 = popa4tires->tires[1];
		int id3 = popa4tires->tires[2];
		int id4 = popa4tires->tires[3];
		int speed1 = popa4tires->speed1;
		int speed2 = popa4tires->speed2;
		int speed3 = popa4tires->speed3;
		int speed4 = popa4tires->speed4;

		if (id1 != 0)
			MFSetServoRotaSpd(id1, speed1);
		if (id2 != 0)
			MFSetServoRotaSpd(id2, speed2);
		if (id3 != 0)
			MFSetServoRotaSpd(id3, speed3);
		if (id4 != 0)
			MFSetServoRotaSpd(id4, speed4);

		return 0;
		break;
	}
	case POS2DSET4TIRESPOSF: {

		myOpaqueSt4TiresPos *popa4TiresPos =
				(myOpaqueSt4TiresPos*) opaquedata->data;
		int id1 = popa4TiresPos->tires[0];
		int id2 = popa4TiresPos->tires[1];
		int id3 = popa4TiresPos->tires[2];
		int id4 = popa4TiresPos->tires[3];
		int speed = popa4TiresPos->speed;
		int pos = popa4TiresPos->pos;

		if (id1 != 0)
			MFSetServoPos(id1, pos, speed);
		if (id2 != 0)
			MFSetServoPos(id2, pos, speed);
		if (id3 != 0)
			MFSetServoPos(id3, pos, speed);
		if (id4 != 0)
			MFSetServoPos(id4, pos, speed);

		return 0;
		break;
	}
	case POS2DSTOP: {

		dealOpaquePosition2dComReq(popa, &res);

		break;
	}
	case POS2DFORWARDF: {


		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->conf.frontL, popa->p1);
		MFSetServoRotaSpd(this->conf.frontR, -popa->p2);
		MFSetServoRotaSpd(this->conf.rearL, popa->p3);
		MFSetServoRotaSpd(this->conf.rearR, -popa->p4);
		MFServoAction();

		break;
	}
	case POS2DBACKWARDF: {

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->conf.frontL, -(popa->p1));
		MFSetServoRotaSpd(this->conf.frontR, (popa->p2));
		MFSetServoRotaSpd(this->conf.rearL, -(popa->p3));
		MFSetServoRotaSpd(this->conf.rearR, (popa->p4));
		MFServoAction();
		break;
	}
	case POS2DTURNLEFTF: {
		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->conf.frontL, -(popa->p1));
		MFSetServoRotaSpd(this->conf.frontR, -(popa->p2));
		MFSetServoRotaSpd(this->conf.rearL, -(popa->p3));
		MFSetServoRotaSpd(this->conf.rearR, -(popa->p4));
		MFServoAction();
		break;
	}
	case POS2DTURNRIGHT: {

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		MFSetServoRotaSpd(this->conf.frontL, (popa->p1));
		MFSetServoRotaSpd(this->conf.frontR, (popa->p2));
		MFSetServoRotaSpd(this->conf.rearL, (popa->p3));
		MFSetServoRotaSpd(this->conf.rearR, (popa->p4));
		MFServoAction();

		break;
	}
	case POS2DSETFRONTL: {

		dealOpaquePosition2dComReq(popa, &res);

		break;
	}
	case POS2DSETFRONTR: {

		dealOpaquePosition2dComReq(popa, &res);

		break;
	}
	case POS2DSETREARL: {

		dealOpaquePosition2dComReq(popa, &res);

		break;
	}
	case POS2DSETREARR: {

		dealOpaquePosition2dComReq(popa, &res);

		break;
	}
	case POS2DGETFRONTL: {

		dealOpaquePosition2dComReq(popa, &res);

		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DGETFRONTR: {

		dealOpaquePosition2dComReq(popa, &res);

		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DGETREARL: {

		dealOpaquePosition2dComReq(popa, &res);

		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DGETREARR: {

		dealOpaquePosition2dComReq(popa, &res);

		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DSETTIRES: {
		dealOpaquePosition2dComReq(popa, &res);
		break;
	}
	case POS2DGETTIRES: {
		dealOpaquePosition2dComReq(popa, &res);
		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case POS2DSERVOMODESERVOF: {
		dealOpaquePosition2dComReq(popa, &res);
		break;
	}
	case POS2DSERVOMODEMOTOF: {
		dealOpaquePosition2dComReq(popa, &res);
		break;
	}
	case POS2DSETSERVOTURN: {
		dealOpaquePosition2dComReq(popa, &res);
		break;
	}
	case POS2DSETSERVOTURNA: {
		dealOpaquePosition2dComReq(popa, &res);
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;

}

int InterfaceOpaque::dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;
	switch (popa->subtype) {
	case DIOSETPORTDIRECTF: {
		dealOpaqueDIOComReq(popa, &res);
		break;
	}
	case DIOGETDIGIINPUTF: {
		dealOpaqueDIOComReq(popa, &res);
		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
		this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		break;
	}
	case DIODIGIOUTPUTF: {
		dealOpaqueDIOComReq(popa, &res);
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;
}
int InterfaceOpaque::deaperDealAIO(int t, int type) {
	if (type == whatKindAIOAIOINFRAREDR) {
		int dis = 1;
#ifdef WRITELOG
#endif
		if (t > 500) {
			return 1;
		} else if (t <= 500 && t > 470) {
			return 2;
		} else if (t <= 470 && t > 400) {
			return 5;
		} else if (t <= 400 && t > 350) {
			return 6;
		} else if (t <= 350 && t > 320) {
			return 7;
		} else if (t <= 320 && t > 230) {
			return 10;
		} else if (t <= 235 && t > 115) {
			return (20 - (t - 115) / 12);
		} else if (t <= 115 && t > 72) {
			return (30 - (t - 72) / 4);
		} else if (t <= 72 && t > 52) {
			return (40 - (t - 52) / 2);
		} else if (t <= 52 && t > 42) {
			return (92 - t);
		} else if (t <= 42 && t > 33) {
			return (60 - (10 * t - 330) / 9);
		} else if (t <= 33 && t > 29) {
			return (70 - (10 * t - 290) / 4);
		} else if (t <= 29 && t > 23) {
			return (80 - (10 * t - 230) / 6);
		} else
			return 80;
		return dis;
	} else {
		return t;
	}
}


int InterfaceOpaque::dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	res->p1 = MFGetAD(popa->p1);
	if (popa->p2 != whatKindAIOAIODEF)
		res->p1 = deaperDealAIO(res->p1, popa->p2);
	return 0;
}
int InterfaceOpaque::dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	if (POS2DSERVOMODESERVOF == popa->subtype) {
		MFSetServoMode(this->conf.frontL, 0);
		MFSetServoMode(this->conf.frontR, 0);
		MFSetServoMode(this->conf.rearL, 0);
		MFSetServoMode(this->conf.rearR, 0);
	} else if (POS2DSERVOMODEMOTOF == popa->subtype) {
		MFSetServoMode(this->conf.frontL, 1);
		MFSetServoMode(this->conf.frontR, 1);
		MFSetServoMode(this->conf.rearL, 1);
		MFSetServoMode(this->conf.rearR, 1);
	} else if (POS2DFORWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->conf.frontL, (popa->p1) & 0x0000FFFF);
		MFSetServoRotaSpd(this->conf.frontR, (popa->p1) >> 16);
		MFSetServoRotaSpd(this->conf.rearL, (popa->p2) & 0x0000FFFF);
		MFSetServoRotaSpd(this->conf.rearR, (popa->p2) >> 16);
		MFServoAction();
	} else if (POS2DBACKWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->conf.frontL, -((popa->p1) & 0x0000FFFF));
		MFSetServoRotaSpd(this->conf.frontR, -((popa->p1) >> 16));
		MFSetServoRotaSpd(this->conf.rearL, -((popa->p2) & 0x0000FFFF));
		MFSetServoRotaSpd(this->conf.rearR, -((popa->p2) >> 16));
		MFServoAction();
	} else if (POS2DTURNLEFTF == popa->subtype) {
		MFSetServoRotaSpd(this->conf.frontL, -((popa->p1) & 0x0000FFFF));
		MFSetServoRotaSpd(this->conf.frontR, ((popa->p1) >> 16));
		MFSetServoRotaSpd(this->conf.rearL, -((popa->p2) & 0x0000FFFF));
		MFSetServoRotaSpd(this->conf.rearR, ((popa->p2) >> 16));
		MFServoAction();
	} else if (POS2DTURNRIGHT == popa->subtype) {
		MFSetServoRotaSpd(this->conf.frontL, (popa->p1) & 0x0000FFFF);
		MFSetServoRotaSpd(this->conf.frontR, -((popa->p1) >> 16));
		MFSetServoRotaSpd(this->conf.rearL, (popa->p2) & 0x0000FFFF);
		MFSetServoRotaSpd(this->conf.rearR, -((popa->p2) >> 16));
		MFServoAction();
	}
	/*else if (POS2DFORWARDF == popa->subtype) {
	 MFSetServoRotaSpd(this->conf.frontL, popa->p1);
	 MFSetServoRotaSpd(this->conf.frontR, -(popa->p1));
	 MFSetServoRotaSpd(this->conf.rearL, popa->p1);
	 MFSetServoRotaSpd(this->conf.rearR, -(popa->p1));
	 MFServoAction();
	 } else if (POS2DBACKWARDF == popa->subtype) {
	 MFSetServoRotaSpd(this->conf.frontL, -(popa->p1));
	 MFSetServoRotaSpd(this->conf.frontR, popa->p1);
	 MFSetServoRotaSpd(this->conf.rearL, -(popa->p1));
	 MFSetServoRotaSpd(this->conf.rearR, popa->p1);
	 MFServoAction();
	 } else if (POS2DTURNLEFTF == popa->subtype) {
	 MFSetServoRotaSpd(this->conf.frontL, -(popa->p1));
	 MFSetServoRotaSpd(this->conf.frontR, -(popa->p1));
	 MFSetServoRotaSpd(this->conf.rearL, -(popa->p1));
	 MFSetServoRotaSpd(this->conf.rearR, -(popa->p1));
	 MFServoAction();
	 } else if (POS2DTURNRIGHT == popa->subtype) {
	 MFSetServoRotaSpd(this->conf.frontL, popa->p1);
	 MFSetServoRotaSpd(this->conf.frontR, popa->p1);
	 MFSetServoRotaSpd(this->conf.rearL, popa->p1);
	 MFSetServoRotaSpd(this->conf.rearR, popa->p1);
	 MFServoAction();
	 } */else if (POS2DSTOP == popa->subtype) {
		MFSetServoRotaSpd(this->conf.frontL, 0);
		MFSetServoRotaSpd(this->conf.frontR, 0);
		MFSetServoRotaSpd(this->conf.rearL, 0);
		MFSetServoRotaSpd(this->conf.rearR, 0);
		MFServoAction();
	} else if (POS2DSETSERVOTURN == popa->subtype) {
		MFSetServoPos(this->conf.frontL, popa->p1, popa->p2);
		MFSetServoPos(this->conf.frontR, popa->p1, popa->p2);
		MFSetServoPos(this->conf.rearL, popa->p1, popa->p2);
		MFSetServoPos(this->conf.rearR, popa->p1, popa->p2);
		MFServoAction();
	} else if (POS2DSETFRONTL == popa->subtype) {
		this->conf.frontL = popa->p1;
	} else if (POS2DSETFRONTR == popa->subtype) {
		this->conf.frontR = popa->p1;
	} else if (POS2DSETREARL == popa->subtype) {
		this->conf.rearL = popa->p1;
	} else if (POS2DSETREARR == popa->subtype) {
		this->conf.rearR = popa->p1;
	} else if (POS2DGETFRONTL == popa->subtype) {
		res->p1 = this->conf.frontL;
	} else if (POS2DGETFRONTR == popa->subtype) {
		res->p1 = this->conf.frontR;
	} else if (POS2DGETREARL == popa->subtype) {
		res->p1 = this->conf.rearL;
	} else if (POS2DGETREARR == popa->subtype) {
		res->p1 = this->conf.rearR;
	} else if (POS2DSETTIRES == popa->subtype) {
		this->conf.frontL = (popa->p1) & 0x0000FFFF;
		this->conf.frontR = (popa->p1) >> 16;
		this->conf.rearL = (popa->p2) & 0x0000FFFF;
		this->conf.rearR = (popa->p2) >> 16;
	} else if (POS2DGETTIRES == popa->subtype) {
		unsigned int ttire = 0x00000000;
		ttire |= ((this->conf.frontL & 0x0000FFFF)
				| ((this->conf.frontR << 16) & 0xFFFF0000));
#ifdef WRITELOG
		printf("int size %d\n", sizeof(int));
		printf("test tire id %x , %x , %x\n", (this->conf.frontL & 0x0000FFFF),
				(this->conf.frontR << 16) & 0xFFFF0000, ttire);
#endif
		res->p1 = ttire;
		ttire = 0x00000000;
		ttire |= ((this->conf.rearL & 0x0000FFFF)
				| ((this->conf.rearR << 16) & 0xFFFF0000));
		res->p2 = ttire;
#ifdef WRITELOG
		printf("test tire id %x , %x , %x\n", (this->conf.rearL & 0x0000FFFF),
				(this->conf.rearR << 16) & 0xFFFF0000, ttire);
#endif
	} else {
		return -1;
	}
	return 0;
}
int InterfaceOpaque::dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	if (DIOSETPORTDIRECTF == popa->subtype) {
		MFSetPortDirect(popa->p1);
	} else if (DIOGETDIGIINPUTF == popa->subtype) {
		pr->p1 = MFGetDigiInput(popa->p1);
	} else if (DIODIGIOUTPUTF == popa->subtype) {
		MFDigiOutput(popa->p1, popa->p2);
	} else {
		return -1;
	}
	return 0;
}

int InterfaceOpaque::dealOpaqueMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 * struct myOpaqueSt{
	 int type;
	 int subtype;
	 int p1;
	 int p2;
	 };
	 * */
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;

	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;
	if (popa->type == MYOPAQUESTTYPEMFCAP) {
		dealOpaqueMFCAP(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEAIO) {
		dealOpaqueAIO(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEDIO) {
		dealOpaqueDIO(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEPOS) {
		dealOpaquePosition2d(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUEBk) {
		dealOpaqueBK(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESR) {
		dealOpaqueSR(resp_queue, hdr, data);
		return 0;
	} else {
#ifdef WRITELOG
		printf("coming opaque messages type wrong ! \n");
#endif
		return -1;
	}

}

/*int InterfaceOpaque::dealDIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
 void * data) {
 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_DIO_CMD_VALUES,
 *(player_devaddr_t *) &(this->m_dio_addr))) {

 player_dio_cmd_t *pdc = (player_dio_cmd_t *) data;
 player_dio_data_t pdd;
 this->getDIOValue(pdc, &pdd);

 Publish(this->m_dio_addr, resp_queue, PLAYER_MSGTYPE_DATA,
 PLAYER_DIO_DATA_VALUES, (void*) &pdd, sizeof(pdd), NULL);

 return 0;
 } else {
 #ifdef WRITELOG
 this->ndiomc++;
 #endif
 return 0;
 }
 return -1;
 }*/
int InterfaceOpaque::dealLaserMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 #define PLAYER_LASER_DATA_SCAN 1
	 #define PLAYER_LASER_DATA_SCANPOSE 2
	 #define PLAYER_LASER_DATA_SCANANGLE 3
	 Y	 #define PLAYER_LASER_REQ_GET_GEOM 1
	 #define PLAYER_LASER_REQ_SET_CONFIG 2
	 #define PLAYER_LASER_REQ_GET_CONFIG 3
	 #define PLAYER_LASER_REQ_POWER 4
	 Y#define PLAYER_LASER_REQ_GET_ID 5
	 #define PLAYER_LASER_REQ_SET_FILTER 6*/
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, -1,
			this->Interface::addr)) {
		return (0);
	}
	// ---[ Get IDentification information
	/*if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_GET_ID,
	 this->Interface::addr)) {
	 return (0);
	 } else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_GET_GEOM, this->Interface::addr)) {
	 player_laser_geom_t geom;
	 memset(&geom, 0, sizeof(geom));
	 geom.pose.px = 0;
	 geom.pose.py = 0;
	 geom.pose.pyaw = 0;
	 geom.size.sl = 0;
	 geom.size.sw = 0;

	 this->driver->Publish(this->Interface::addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
	 PLAYER_LASER_REQ_GET_GEOM, (void*) &geom, sizeof(geom), NULL);
	 return (0);
	 }
	 // ---[ Set power
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_POWER, this->Interface::addr)) {

	 return (0);
	 }
	 // ---[ Set configuration
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_SET_CONFIG, device_addr)) {
	 return (-1);
	 }
	 // ---[ Get configuration
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_GET_CONFIG, device_addr)) {
	 return (0);
	 }
	 // ---[ Set filter settings
	 else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
	 PLAYER_LASER_REQ_SET_FILTER, device_addr)) {
	 return (-1);
	 } else*/
	return (-1);
}
int InterfaceOpaque::dealBlobFinderMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 #define PLAYER_BLOBFINDER_DATA_BLOBS 1
	 #define PLAYER_BLOBFINDER_REQ_SET_COLOR 1
	 #define PLAYER_BLOBFINDER_REQ_SET_IMAGER_PARAMS 2
	 #define PLAYER_BLOBFINDER_REQ_GET_COLOR 3
	 */
	if (Message::MatchMessage(hdr, PLAYER_BLOBFINDER_REQ_GET_COLOR, -1,
			this->Interface::addr)) {
		return 0;
	}
	/*if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
	 PLAYER_CAMERA_DATA_STATE, this->camera_id)) {
	 this->Publish(this->blobfinder_provided_addr, PLAYER_MSGTYPE_DATA,
	 PLAYER_BLOBFINDER_DATA_BLOBS, reinterpret_cast<void *>(output),
	 0, &(hdr->timestamp), false); // copy = false
	 }*/
	return -1;
}
int InterfaceOpaque::dealPosMessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {

	/*
	 #define PLAYER_POSITION2D_REQ_GET_GEOM 1
	 #define PLAYER_POSITION2D_REQ_MOTOR_POWER 2
	 #define PLAYER_POSITION2D_REQ_VELOCITY_MODE 3
	 #define PLAYER_POSITION2D_REQ_POSITION_MODE 4
	 #define PLAYER_POSITION2D_REQ_SET_ODOM 5
	 #define PLAYER_POSITION2D_REQ_RESET_ODOM 6
	 #define PLAYER_POSITION2D_REQ_SPEED_PID 7
	 #define PLAYER_POSITION2D_REQ_POSITION_PID 8
	 #define PLAYER_POSITION2D_REQ_SPEED_PROF 9
	 #define PLAYER_POSITION2D_DATA_STATE 1
	 #define PLAYER_POSITION2D_DATA_GEOM 2
	 #define PLAYER_POSITION2D_CMD_VEL 1
	 #define PLAYER_POSITION2D_CMD_POS 2
	 #define PLAYER_POSITION2D_CMD_CAR 3
	 #define PLAYER_POSITION2D_CMD_VEL_HEAD 4
	 */
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, -1,
			this->Interface::addr)) {
		player_position2d_cmd_pos_t* cmd = (player_position2d_cmd_pos_t*) data;
		return 0;
	}
// Handle new data from the laser
	/*
	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, -1, this->device_addr)) {
	 Message* msg;
	 this->Publish(resp_queue, rephdr, repdata);
	 delete msg;
	 return (0);
	 }
	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
	 PLAYER_POSITION2D_CMD_POS, this->device_addr)) {
	 return 0;
	 }

	 if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
	 PLAYER_POSITION2D_CMD_VEL, this->device_addr)) {
	 return 0;
	 }
	 */
	return -1;
}

/*void InterfaceOpaque::Publish(player_msghdr_t* hdr, void* src, bool copy = true) {
 //publish laserData

 ModelLaser* mod = (ModelLaser*)this->mod;
 ModelLaser::Sample* samples = mod->GetSamples(NULL);

 // don't publish anything until we have some real data
 if( samples == NULL )
 return;

 player_laser_data_t pdata;
 memset( &pdata, 0, sizeof(pdata) );

 pdata.min_angle = -cfg.fov/2.0;
 pdata.max_angle = +cfg.fov/2.0;
 pdata.resolution = cfg.fov / cfg.sample_count;
 pdata.max_range = cfg.range_bounds.max;
 pdata.ranges_count = pdata.intensity_count = 21;
 pdata.id = this->conf.scan_id++;

 pdata.ranges = new float[pdata.ranges_count];
 pdata.intensity = new uint8_t[pdata.ranges_count];

 for( unsigned int i=0; i<cfg.sample_count; i++ )
 {
 //printf( "range %d %d\n", i, samples[i].range);
 pdata.ranges[i] = samples[i].range;
 pdata.intensity[i] = (uint8_t)samples[i].reflectance;
 }

 // Write laser data
 this->driver->Publish(this->addr,
 PLAYER_MSGTYPE_DATA,
 PLAYER_LASER_DATA_SCAN,
 (void*)&pdata, sizeof(pdata), NULL);

 delete [] pdata.ranges;
 delete [] pdata.intensity;

 }*/

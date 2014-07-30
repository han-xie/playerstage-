#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include <libgen.h> // for dirname(3)
#include <libplayercore/globals.h> // for player_argc & player_argv
#include "p_driver.h"
using namespace Stg;
#include "CYZXInter.h"

extern bool player_quiet_startup;
extern PlayerTime* GlobalTime;
extern bool usegui;
extern int http;
extern bool wlocalhost;

InterfaceOpaque::InterfaceOpaque(player_devaddr_t addr, StgDriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	printf("a opaque world");
	fflush (stdout);
	//puts( "InterfaceSimulation constructor" );

	Stg::Init(&player_argc, &player_argv);

	const char* worldfile_name = cf->ReadString(section, "worldfile", NULL);

	if (worldfile_name == NULL) {
		PRINT_ERR1( "device \"%s\" uses the Stage driver but has "
		"no \"model\" value defined. You must specify a "
		"model name that matches one of the models in "
		"the worldfile.", worldfile_name);
		return; // error
	}

	char fullname[MAXPATHLEN];

	if (worldfile_name[0] == '/')
		strcpy(fullname, worldfile_name);
	else {
		char *tmp = strdup(cf->filename);
		snprintf(fullname, MAXPATHLEN, "%s/%s", dirname(tmp), worldfile_name);
		free(tmp);
	}

	std::string fedfilename = "";
	std::string host = "localhost";
	unsigned short phttp = http;

	int ch = 0, optindex = 0;
	bool usefedfile = false;

	StgDriver::world = (Stg::World*) new WorldGui(400, 300, "Player/Stage");
	assert(StgDriver::world);

	puts("");
	StgDriver::world->Load(fullname);

	delete GlobalTime;
	GlobalTime = new StTime(driver);
	assert(GlobalTime);
	// start the simulation
	// printf( "  Starting world clock... " ); fflush(stdout);
	//stg_world_resume( world );

	if (wlocalhost == false) {
		int i = 0;
		int sockfd;
		struct ifconf ifconf;
		char buf[512];
		struct ifreq *ifreq;
		char* ip;
		//初始化ifconf
		ifconf.ifc_len = 512;
		ifconf.ifc_buf = buf;

		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			//return -1;
		}
		ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
		close(sockfd);

		//接下来一个一个的获取IP地址
		ifreq = (struct ifreq*) buf;
		for (i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; i--) {
			ip = inet_ntoa(
					((struct sockaddr_in*) &(ifreq->ifr_addr))->sin_addr);

			if (strcmp(ip, "127.0.0.1") == 0)  //排除127.0.0.1，继续下一个
					{
				ifreq++;
				continue;
			}
			host = ip;
		}

		//return -1;
	}


	WebStage* ws = new WebStage(StgDriver::world, host, phttp);
	assert(ws);
	if (usefedfile)
		ws->LoadFederationFile(fedfilename);
	ws->Startup(true); // start http server

	StgDriver::world->Start();

	// this causes Driver::Update() to be called even when the device is
	// not subscribed
	driver->alwayson = TRUE;

	puts(""); // end the Stage startup line
}

int InterfaceOpaque::ProcessMessage(QueuePointer &resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (hdr->addr.interf == this->addr.interf) {
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
			return -1;
		}
	} else
		return -1;
}
int InterfaceOpaque::dealOpaqueSR(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	return -1;
}
int InterfaceOpaque::dealOpaqueBK(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
	switch (popa->subtype) {
	case BKMFSETSERVOROTASPDF: {

		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		//MFSetServoRotaSpd(popa->p1, popa->p2);
		//MFServoAction();

		break;
	}
	case BKMFSETSERVOMODEF: {

		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		//MFSetServoMode(popa->p1, popa->p2);

		break;
	}
	case BKMFSETSERVOPOSF: {

		myOpaqueSt3 *popa = (myOpaqueSt3 *) opaquedata->data;
		//MFSetServoPos(popa->p1, popa->p2, popa->p3);

		break;
	}

	case BKMFGETSERVOPOSF: {
		break;
	}

	case BKGETULTRASONICF: {

		comReqBk1 *popa = (comReqBk1 *) opaquedata->data;
		//if (-1 == dealOpaqueBKGETULTRASONICF(popa))
		return -1;

		break;
	}
	case BKMFSERVOACTIONF: {

		myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
		//MFServoAction();

		break;
	}
	case BKMFSETSERVOROTASPDTIMEF: {
		break;
	}

	case BKMP3PLAYF: {

		myOpaqueStMp3 *popa = (myOpaqueStMp3 *) opaquedata->data;

		/*if ((pid = fork()) < 0) {
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

		/*if ((pid = fork()) < 0) {
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
		//DelayMS(popa->p1);

		break;
	}
	case BKLCDDISF: {

		myOpaqueStLCD *popa = (myOpaqueStLCD*) opaquedata->data;

		/*if (pid = fork() < 0) {
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
		//MFUSonicEnable();

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

	int SPratio = 500;
	int Aratio = 57;

	Stg::ModelPosition *mod = (Stg::ModelPosition *) StgDriver::world->GetModel(
			popa->name);

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

		if (!mod)
			return -1;

		speed1 = speed1 / SPratio;
		if (id1 != 0 && id2 != 0 && id3 != 0 && id4 != 0) {
			if (speed1 > 0 && speed2 > 0 && speed3 > 0 && speed4 > 0) {
				mod->SetSpeed(speed1, 0, 0);
			} else if (speed1 < 0 && speed2 < 0 && speed3 < 0 && speed4 < 0) {
				mod->SetSpeed(-speed1, 0, 0);
			} else {

			}
		}

		return -1;
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

		/*if (id1 != 0)
		 MFSetServoPos(id1, pos, speed);
		 if (id2 != 0)
		 MFSetServoPos(id2, pos, speed);
		 if (id3 != 0)
		 MFSetServoPos(id3, pos, speed);
		 if (id4 != 0)
		 MFSetServoPos(id4, pos, speed);*/

		return 0;
		break;
	}
	case POS2DSTOP: {

		mod->Stop();

		break;
	}
	case POS2DFORWARDF: {

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		mod->SetSpeed(popa->p1 / SPratio, 0, 0);
		/*Pose pose = mod->GetPose();
		printf("%d,%d,%d\n",pose.x, pose.y,pose.a);
		pose.x += 1;
		pose.y += 1;
		mod->SetPose( pose );*/
		break;
	}
	case POS2DBACKWARDF: {

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		mod->SetSpeed(-(popa->p1 / SPratio), 0, 0);

		break;
	}
	case POS2DTURNLEFTF: {

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		mod->SetTurnSpeed(-(popa->p1));

		break;
	}
	case POS2DTURNRIGHT: {

		myOpaqueSt4 *popa = (myOpaqueSt4 *) opaquedata->data;

		mod->SetTurnSpeed(popa->p1);

		break;
	}
	case POS2DSETFRONTL: {

		break;
	}
	case POS2DSETFRONTR: {

		break;
	}
	case POS2DSETREARL: {

		break;
	}
	case POS2DSETREARR: {

		break;
	}
	case POS2DGETFRONTL: {

		/*dealOpaquePosition2dComReq(popa, &res);

		 opaqueRes.data_count = sizeof(res);
		 uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
		 + opaqueRes.data_count;
		 opaqueRes.data = (uint8_t *) &res;

		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case POS2DGETFRONTR: {

		/*dealOpaquePosition2dComReq(popa, &res);

		 opaqueRes.data_count = sizeof(res);
		 uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
		 + opaqueRes.data_count;
		 opaqueRes.data = (uint8_t *) &res;

		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case POS2DGETREARL: {

		/*dealOpaquePosition2dComReq(popa, &res);

		 opaqueRes.data_count = sizeof(res);
		 uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
		 + opaqueRes.data_count;
		 opaqueRes.data = (uint8_t *) &res;

		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case POS2DGETREARR: {

		/*dealOpaquePosition2dComReq(popa, &res);

		 opaqueRes.data_count = sizeof(res);
		 uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
		 + opaqueRes.data_count;
		 opaqueRes.data = (uint8_t *) &res;

		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case POS2DSETTIRES: {

		break;
	}
	case POS2DGETTIRES: {

		/*dealOpaquePosition2dComReq(popa, &res);

		 opaqueRes.data_count = sizeof(res);
		 uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
		 + opaqueRes.data_count;
		 opaqueRes.data = (uint8_t *) &res;

		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case POS2DSERVOMODESERVOF: {

		break;
	}
	case POS2DSERVOMODEMOTOF: {

		break;
	}
	case POS2DSETSERVOTURN: {

		break;
	}
	case POS2DSETSERVOTURNA: {

		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;

}

int InterfaceOpaque::dealOpaqueDIO(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;

	switch (popa->subtype) {
	case DIOSETPORTDIRECTF: {

		//dealOpaqueDIOComReq(popa, &res);

		break;
	}
	case DIOGETDIGIINPUTF: {

		/*dealOpaqueDIOComReq(popa, &res);

		 opaqueRes.data_count = sizeof(myOpaqueSt);
		 uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
		 + opaqueRes.data_count;
		 opaqueRes.data = (uint8_t *) &res;

		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case DIODIGIOUTPUTF: {

		//dealOpaqueDIOComReq(popa, &res);

		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;
}
int InterfaceOpaque::dealOpaqueAIO(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	player_opaque_data_t opaqueRes;
	myOpaqueSt res;

	//Stg::ModelLaser* mod = (Stg::ModelLaser *) StgDriver::world->GetModel(popa->name);
	Stg::ModelLaser* mod;// = opaquem.laserModel;
	ModelLaser::Sample* samples = mod->GetSamples(NULL);

	switch (popa->subtype) {
	case AIOCOMTF: {

		if (samples == NULL)
			return -1;


		player_laser_data_t pdata;
		memset(&pdata, 0, sizeof(pdata));

		res.type = popa->type;
		res.subtype = popa->subtype;
		res.p1 = 1;
		/*if (popa->p2 != whatKindAIOAIODEF)
		 res->p1 = deaperDealAIO(res->p1, popa->p2);*/

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

		this->driver->Publish(this->addr,resp_queue, PLAYER_MSGTYPE_RESP_ACK, PLAYER_OPAQUE_REQ_DATA,
				&opaqueRes, size, NULL);


		break;
	}
	case AIOCOMINFRAREDF: {
		if (samples == NULL)
			return -1;

		player_laser_data_t pdata;
		memset(&pdata, 0, sizeof(pdata));

		res.type = popa->type;
		res.subtype = popa->subtype;
		res.p1 = (int) samples[popa->p1].range;

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

		this->driver->Publish(this->addr,resp_queue, PLAYER_MSGTYPE_RESP_ACK, hdr->subtype,
				&opaqueRes, size, NULL);

		break;
	}
	case AIOCOMF: {

		if (samples == NULL)
			return -1;

		player_laser_data_t pdata;
		memset(&pdata, 0, sizeof(pdata));

		res.type = popa->type;
		res.subtype = popa->subtype;
		res.p1 = (int) samples[popa->p1].range;

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

		this->driver->Publish(this->addr,resp_queue, PLAYER_MSGTYPE_RESP_ACK, hdr->subtype,
				&opaqueRes, size, NULL);

		break;
	}
	default: {
		return -1;
		break;
	}
	}
	return 0;
}

int InterfaceOpaque::dealOpaqueMFCAP(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;
	player_opaque_data_t opaqueRes;
	myOpaqueSt res;
	switch (popa->subtype) {
	case CAPGETCENTERXF: {
		//dealOpaqueMFCAPComReq(popa, &res);
		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		/*Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case CAPGETCENTERYF: {

		//dealOpaqueMFCAPComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		/*Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case CAPGETSUMF: {

		//dealOpaqueMFCAPComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;

		/*Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
		 PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);*/
		break;
	}
	case CAPSETHF: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETSF: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETIF: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETMODEF: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETYELLOW: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETRED: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	case CAPSETGREEN: {

		//dealOpaqueMFCAPComReq(popa, &res);

		break;
	}
	default: {
		return -1;
		break;
	}
	}

	return 0;
}

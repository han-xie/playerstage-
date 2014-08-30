#include "multidriver.h"
#include <string.h>

#include <arpa/inet.h>
#include <unistd.h> // fork, close
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

InterfacecyzxLaser::InterfacecyzxLaser(player_devaddr_t addr, multidriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section, "publish", 1);
	/*std::string cyzxlaserType = "";
	 cyzxlaserType = cf->ReadString(section, "cyzxlasertype", cyzxlaserType.data());
	 this->conf.type = StringToType(cyzxlaserType.data());
	 this->conf.portsNum = cf->GetTupleCount(section, "ports");
	 for (int i = 0; i < this->conf.portsNum; i++)
	 this->conf.ports[i] = cf->ReadTupleInt(section, "ports", i, 0);*/
	this->conf.portsNum = DIOMAX + 1 + AIOMAX + 1 + RS422MAX + 1;
	int i = 0;
	std::string cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a0", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a1", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a2", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a3", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a4", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a5", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a6", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "a7", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d0", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d1", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d2", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d3", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d4", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d5", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d6", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d7", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d8", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d9", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d10", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "d11", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "rs4220", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	cyzxlaserType = "";
	cyzxlaserType = cf->ReadString(section, "rs4221", cyzxlaserType.data());
	this->conf.portsType[i++] = StringToType(cyzxlaserType.data());
	this->conf.lcdIP = cf->ReadString(section, "lcdip", "192.168.1.2");
	this->conf.soundIP = cf->ReadString(section, "soundip", "192.168.1.2");
	this->conf.lcdport = cf->ReadInt(section, "lcdport", 9000);
	this->conf.soundport = cf->ReadInt(section, "soundport", 9001);
}

int InterfacecyzxLaser::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_GET_CONFIG, this->Interface::addr)) {
		player_cyzxlaser_config_t* plc = (player_cyzxlaser_config_t*) data;
		if (hdr->size == sizeof(player_cyzxlaser_config_t)) {
			this->Interface::driver->Publish(this->Interface::addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_CYZXLASER_REQ_GET_CONFIG);
			return (0);
		} else {
			return (-1);
		}
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_GET_GEOM, this->Interface::addr)) {
		player_cyzxlaser_geom_t pgeom;
		pgeom.pose.px = 0;
		pgeom.pose.py = 0;
		pgeom.pose.pyaw = 0;
		pgeom.size.sl = 0;
		pgeom.size.sw = 0;

		this->Interface::driver->Publish(this->Interface::addr, resp_queue,
				PLAYER_MSGTYPE_RESP_ACK, PLAYER_CYZXLASER_REQ_GET_GEOM,
				(void*) &pgeom, sizeof(pgeom), NULL);
		return (0);
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_CYZXLASER_REQ_GET_ID,
			this->Interface::addr)) {
		return -1;
	}else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_CYZXLASER_CMD_SETWEIBO, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;
		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		dealWifiSetweiboCMD(ip);
		/*mod->lcdswitch = 1;
		mod->lcdcount++;
		std::string display = "connect to ";
		std::string stemp = (char *) ip;
		display += stemp + " content : hello";
		mod->lcdcontent = display;*/
		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_CYZXLASER_CMD_SETLCD, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;

		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		int length = opaquedata->data_count - (8 + iplength) + 1;
		uint8_t *display = new uint8_t[length];
		to = display;
		for (int i = 0; i < length; i++) {
			*to = *from;
			from++;
			to++;
		}
		*to = 0x0;
		dealWifiSetLCDCMD(ip, display);
		/*mod->lcdswitch = 1;
		mod->lcdcount++;
		std::string stemp;
		stemp = (char*) display;
		mod->lcdcontent = stemp;*/

		delete[] display;

		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_CYZXLASER_CMD_SETSOUND, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;
		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		int length = opaquedata->data_count - (8 + iplength) + 1;
		uint8_t *type = new uint8_t[length];
		to = type;
		for (int i = 0; i < length; i++) {
			*to = *from;
			from++;
			to++;
		}
		*to = 0x0;
		dealWifiSetSoundCMD(ip, type);
		/*mod->soundswitch = 1;
		mod->soundcount++;
		std::string sound = "connect to ";
		std::string stemp = (char *) ip;
		sound += stemp + " content : ";
		stemp = (char *) type;
		sound += stemp;
		mod->soundcontent = sound;*/

		delete[] type;

		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_GETWEIBO, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;

		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		int length = opaquedata->data_count - (8 + iplength) + 1;
		uint8_t *cmd = new uint8_t[length];
		to = cmd;
		for (int i = 0; i < length; i++) {
			*to = *from;
			from++;
			to++;
		}
		*to = 0x0;
	    dealWifiGetWeibo(ip, cmd);

		/*int test=1;

		this->driver->Publish(this->addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_CYZXLASER_REQ_GETWEIBO, (void*) &test,
				sizeof(test), NULL);
       */

		player_cyzxlaser_opaque_data_t opaqueRes;
		std::string result = "weibo";
		opaqueRes.data_count = result.size();
		opaqueRes.data=(uint8_t *)result.c_str();
		this->driver->Publish(this->Interface::addr, resp_queue,PLAYER_MSGTYPE_RESP_ACK,
					PLAYER_CYZXLASER_REQ_GETWEIBO, (void *)&opaqueRes, sizeof(opaqueRes), NULL);

		delete[] cmd;

		return 0;
	}
	return -1;
}

void InterfacecyzxLaser::dealWifiSetweiboCMD(uint8_t *ip) {

	int sockfdc;
	char sendlinec[4096];
	struct sockaddr_in servaddrc;
	int iplength = 30;
	//char buff[4096];
	//int n;

	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	int i;
	uint8_t *travel = ip;
	for (i = 0; i < iplength; i++, travel++) {
		if ((*travel >= '0' && *travel <= '9') || (*travel == '.')
				|| (*travel == ':')) {
			if (*travel == ':') {
				this->conf.lcdport = atoi((char*) travel + 1);
				*travel = '\0';
			}
		} else
			break;
	}

	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(this->conf.lcdport);
	if (inet_pton(AF_INET, (i < 4) ? this->conf.lcdIP.data() : (char *) ip,
			&servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", ip);
		return;
	}

	if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
			< 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	sendlinec[0] = 'w';
	sendlinec[1] = 'e';
	sendlinec[2] = 'i';
	sendlinec[3] = 'b';
	sendlinec[4] = 'o';
	if (send(sockfdc, sendlinec, strlen(sendlinec), 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	close(sockfdc);
	return;
}
void InterfacecyzxLaser::dealWifiSetLCDCMD(uint8_t *ip, uint8_t *display) {

	int sockfdc;
	struct sockaddr_in servaddrc;
	int iplength = 30;
	//char buff[4096];
	//int n;

	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	int i;
	uint8_t *travel = ip;
	for (i = 0; i < iplength; i++, travel++) {
		if ((*travel >= '0' && *travel <= '9') || (*travel == '.')
				|| (*travel == ':')) {
			if (*travel == ':') {
				*travel = '\0';
				this->conf.lcdport = atoi((char*) travel + 1);
			}
		} else
			break;
	}
	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(this->conf.lcdport);
	if (inet_pton(AF_INET, (i < 4) ? this->conf.lcdIP.c_str() : (char *) ip,
			&servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", ip);
		return;
	}

	if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
			< 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	if (send(sockfdc, (char*) display, strlen((char*) display), 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	close(sockfdc);
	return;
}

void InterfacecyzxLaser::dealWifiSetSoundCMD(uint8_t *ip, uint8_t *type) {

	int sockfdc;
	//char sendlinec[4096];
	struct sockaddr_in servaddrc;
	int iplength = 30;
	//char buff[4096];
	//int n;

	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	int i;
	uint8_t *travel = ip;
	for (i = 0; i < iplength; i++, travel++) {
		if ((*travel >= '0' && *travel <= '9') || (*travel == '.')
				|| (*travel == ':')) {
			if (*travel == ':') {
				this->conf.soundport = atoi((char*) travel + 1);
				*travel = '\0';
			}
		} else
			break;
	}
	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(this->conf.soundport);
	if (inet_pton(AF_INET, (i < 4) ? this->conf.soundIP.c_str() : (char *) ip,
			&servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", ip);
		return;
	}

	if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
			< 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	if (send(sockfdc, type, strlen((char *) type), 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	close(sockfdc);
	return;
}

void InterfacecyzxLaser::dealWifiGetWeibo(uint8_t *ip, uint8_t *cmd) {
	int sockfdc;
	//char sendlinec[4096];
	struct sockaddr_in servaddrc;
	int iplength = 30;
	char buff[4096];
	//int n;

	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	int i;
	uint8_t *travel = ip;
	for (i = 0; i < iplength; i++, travel++) {
		if ((*travel >= '0' && *travel <= '9') || (*travel == '.')
				|| (*travel == ':')) {
			if (*travel == ':') {
				this->conf.weiboport = atoi((char*) travel + 1);
				*travel = '\0';
			}
		} else
			break;
	}
	memset(&servaddrc, 0, sizeof(servaddrc));
	servaddrc.sin_family = AF_INET;
	servaddrc.sin_port = htons(this->conf.weiboport);
	if (inet_pton(AF_INET, (i < 4) ? this->conf.weiboIP.c_str() : (char *) ip,
			&servaddrc.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", ip);
		return;
	}

	if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
			< 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	if (send(sockfdc, cmd, strlen((char *) cmd), 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		return;
	}

	int n = recv(sockfdc, buff, 4096, 0);
	buff[n] = '\0';


	close(sockfdc);

	player_opaque_data_t opaqueRes;

	/*opaqueRes.data_count = sizeof(buff);
	opaqueRes.data = new uint8_t[opaqueRes.data_count];
	memcpy(opaqueRes.data, (uint8_t*) &buff[0], opaqueRes.data_count);
	uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
			+ opaqueRes.data_count;*/
	opaqueRes.data_count = n;
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t*)&(buff[0]);
	this->driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_RESP_ACK,
			PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);

	return;
}

static int deaperDealAIO(int t) {
		int dis = 1;
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
}

void InterfacecyzxLaser::Publish() {
	/*float portsV[DIOMAX + 1];
	 if (AioDioRS422(this->conf.type) == AIO) {
	 for(int i=0;i<this->conf.portsNum;i++){
	 portsV[i]=(float) MFGetAD(this->conf.ports[i]);
	 }
	 } else if (AioDioRS422(this->conf.type) == DIOIN
	 || AioDioRS422(this->conf.type) == DIO) {
	 for(int i=0;i<this->conf.portsNum;i++){
	 portsV[i]=(float) MFGetDigiInput(this->conf.ports[i]);
	 }
	 } else if (AioDioRS422(this->conf.type) == DIOOUT) {

	 } else if (AioDioRS422(this->conf.type) == RS422) {
	 if((this->conf.portsNum) > 0)
	 portsV[0]=(float) MFGetUltrasonic();
	 portsV[1]=portsV[0];
	 }*/
	float portsV[this->conf.portsNum];
	for (int i = 0; i < this->conf.portsNum - 1; i++) {
		if (i < (AIOMAX + 1)) {
			if ((AioDioRS422(this->conf.portsType[i]) != DONTKNOW)
					&& (AioDioRS422(this->conf.portsType[i]) != DIOOUT)) {
				portsV[i] = (float) MFGetAD(i);
				if(this->conf.portsType[i]==infrDistSen){
					portsV[i]=(float) deaperDealAIO((int)portsV[i]);
				}
			} else {
				portsV[i] = 0;
			}
		} else if (i < (AIOMAX + 1 + DIOMAX + 1)) {
			if ((AioDioRS422(this->conf.portsType[i]) != DONTKNOW)
					&& (AioDioRS422(this->conf.portsType[i]) != DIOOUT)) {
				portsV[i] = (float) MFGetDigiInput(i - AIOMAX - 1);
			} else {
				portsV[i] = 0;
			}
		} else {
			if ((AioDioRS422(this->conf.portsType[i]) != DONTKNOW)
					&& (AioDioRS422(this->conf.portsType[i]) != DIOOUT)) {
				portsV[i] = (float) MFGetUltrasonic();
			} else {
				portsV[i] = 0;
			}
		}

	}
	portsV[this->conf.portsNum - 1] = portsV[this->conf.portsNum - 2];

	player_cyzxlaser_data_t pdata;
	memset(&pdata, 0, sizeof(pdata));

	pdata.min_angle = -this->conf.fov / 2.0;
	pdata.max_angle = +this->conf.fov / 2.0;
	pdata.resolution = this->conf.fov / this->conf.sample_count;
	pdata.max_range = this->conf.range_bounds.max;
	pdata.ranges_count = pdata.intensity_count = this->conf.sample_count =
			this->conf.portsNum;
	pdata.id = this->conf.scan_id++;
	pdata.ranges = new float[pdata.ranges_count];
	pdata.intensity = new uint8_t[pdata.ranges_count];

	for (int i = 0; i < this->conf.portsNum; i++) {
		pdata.ranges[i] = portsV[i];
		pdata.intensity[i] = 0;
	}

	// Write cyzxlaser data
	this->Interface::driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA,
			PLAYER_CYZXLASER_DATA_SCAN, (void*) &pdata, sizeof(pdata), NULL);
	return;
}

int InterfacecyzxLaser::AioDioRS422(int type) {
	if (type == infrProxSen)
		return DIOIN;
	else if (type == colliSen)
		return DIOIN;
	else if (type == soundSen)
		return DIOIN;
	else if (type == gestSen)
		return DIOIN;
	else if (type == hallSen)
		return DIOIN;
	else if (type == dout)
		return DIOOUT;
	else if (type == infrDistSen)
		return AIO;
	else if (type == tempSen)
		return AIO;
	else if (type == graySen)
		return AIO;
	else if (type == lightSen)
		return AIO;
	else if (type == RS422Sen)
		return RS422;
	else
		return DONTKNOW;
}

int InterfacecyzxLaser::StringToType(const char *p) {
	if (strcmp(p, "infrProxSen") == 0)
		return infrProxSen;
	else if (strcmp(p, "colliSen") == 0)
		return colliSen;
	else if (strcmp(p, "soundSen") == 0)
		return soundSen;
	else if (strcmp(p, "gestSen") == 0)
		return gestSen;
	else if (strcmp(p, "hallSen") == 0)
		return hallSen;
	else if (strcmp(p, "dout") == 0)
		return dout;
	else if (strcmp(p, "infrDistSen") == 0)
		return infrDistSen;
	else if (strcmp(p, "tempSen") == 0)
		return tempSen;
	else if (strcmp(p, "graySen") == 0)
		return graySen;
	else if (strcmp(p, "lightSen") == 0)
		return lightSen;
	else if (strcmp(p, "RS422Sen") == 0)
		return RS422Sen;
	else
		return DONTKNOW;
}

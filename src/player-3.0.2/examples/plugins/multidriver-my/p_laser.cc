#include "multidriver.h"
#include <string.h>

InterfaceLaser::InterfaceLaser(player_devaddr_t addr, multidriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section, "publish", 1);
	std::string laserType = "";
	laserType = cf->ReadString(section, "lasertype", laserType.data());
	this->conf.type = StringToType(laserType.data());
	this->conf.portsNum = cf->GetTupleCount(section, "ports");
	for (int i = 0; i < this->conf.portsNum; i++)
		this->conf.ports[i] = cf->ReadTupleInt(section, "ports", i, 0);
}

int InterfaceLaser::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_CONFIG, this->Interface::addr)) {
		player_laser_config_t* plc = (player_laser_config_t*) data;
		if (hdr->size == sizeof(player_laser_config_t)) {
			this->Interface::driver->Publish(this->Interface::addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
					PLAYER_LASER_REQ_GET_CONFIG);
			return (0);
		} else {
			return (-1);
		}
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_GEOM, this->Interface::addr)) {
		player_laser_geom_t pgeom;
		pgeom.pose.px = 0;
		pgeom.pose.py = 0;
		pgeom.pose.pyaw = 0;
		pgeom.size.sl = 0;
		pgeom.size.sw = 0;

		this->Interface::driver->Publish(this->Interface::addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_LASER_REQ_GET_GEOM, (void*) &pgeom, sizeof(pgeom), NULL);
		return (0);
	}
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, PLAYER_LASER_REQ_GET_ID,
			this->Interface::addr)) {
		return -1;
	}
	return -1;
}

void InterfaceLaser::Publish() {
	float portsV[DIOMAX + 1];
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
	}

	player_laser_data_t pdata;
	memset(&pdata, 0, sizeof(pdata));

	pdata.min_angle = -this->conf.fov / 2.0;
	pdata.max_angle = +this->conf.fov / 2.0;
	pdata.resolution = this->conf.fov / this->conf.sample_count;
	pdata.max_range = this->conf.range_bounds.max;
	pdata.ranges_count = pdata.intensity_count = this->conf.sample_count = this->conf.portsNum;
	pdata.id = this->conf.scan_id++;
	pdata.ranges = new float[pdata.ranges_count];
	pdata.intensity = new uint8_t[pdata.ranges_count];


	for (int i = 0; i < this->conf.portsNum; i++) {
		pdata.ranges[i] = this->conf.ports[i];
		pdata.intensity[i] = 0;
	}

	// Write laser data
	this->Interface::driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
			(void*) &pdata, sizeof(pdata), NULL);
	return;
}

int InterfaceLaser::AioDioRS422(int type) {
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

int InterfaceLaser::StringToType(const char *p) {
	if (strcmp(p, "infroProxSen") == 0)
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

#include "multidriver.h"
#include <string.h>

InterfaceLightsensor::InterfaceLightsensor(player_devaddr_t addr,
		multidriver* driver, ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section, "publish", 1);
}

int InterfaceLightsensor::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LIGHTSENSOR_REQ_GET_COLOR, this->Interface::addr)) {
		int pgeom = 0;
		this->Interface::driver->Publish(this->Interface::addr, resp_queue,
				PLAYER_MSGTYPE_RESP_ACK, PLAYER_LIGHTSENSOR_REQ_GET_COLOR,
				(void*) &pgeom, sizeof(pgeom), NULL);
		printf("----hello this is test req-command\n");
		return (0);
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_LIGHTSENSOR_CMD_INT, this->Interface::addr)) {
		int* pcmd =(int*) data;
		printf("----hello this is test CMD-command\n");
		return (0);
	} else {
		return -1;
	}
}

void InterfaceLightsensor::Publish() {
	player_lightsensor_data_t hsd;
	bzero(&hsd, sizeof(hsd));

	uint32_t bcount = 1;
	player_lightsensor_light_t light;
	if (bcount > 0) {
		// and set the image width * height
		hsd.width = 0;
		hsd.height = 0;
		hsd.lights_count = bcount;
		hsd.lights = &light;

		unsigned int b;
		for (b = 0; b < bcount; b++) {
			hsd.lights[b].x = MFCapGetCenterX();
			hsd.lights[b].y = MFCapGetCenterY();
			hsd.lights[b].left = 0;
			hsd.lights[b].right = 0;
			hsd.lights[b].top = 0;
			hsd.lights[b].bottom = 0;
			hsd.lights[b].color = 0;
			hsd.lights[b].area = MFCapGetSum();
			hsd.lights[b].range = 0;
		}
	}
	this->Interface::driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA,
			PLAYER_LIGHTSENSOR_DATA_LIGHTS, &hsd, sizeof(hsd), NULL);
	return;
}

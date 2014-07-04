#include "multidriver.h"
#include <string.h>

InterfaceHallsensor::InterfaceHallsensor(player_devaddr_t addr,
		multidriver* driver, ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section, "publish", 1);
}

int InterfaceHallsensor::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_HALLSENSOR_REQ_GET_COLOR, this->Interface::addr)) {
		int pgeom = 0;
		this->Interface::driver->Publish(this->Interface::addr, resp_queue,
				PLAYER_MSGTYPE_RESP_ACK, PLAYER_HALLSENSOR_REQ_GET_COLOR,
				(void*) &pgeom, sizeof(pgeom), NULL);
		printf("----hello this is test req-command\n");
		return (0);
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_HALLSENSOR_CMD_INT, this->Interface::addr)) {
		int* pcmd =(int*) data;
		printf("----hello this is test CMD-command\n");
		return (0);
	} else {
		return -1;
	}
}

void InterfaceHallsensor::Publish() {
	player_hallsensor_data_t hsd;
	bzero(&hsd, sizeof(hsd));

	uint32_t bcount = 1;
	player_hallsensor_hall_t hall;
	if (bcount > 0) {
		// and set the image width * height
		hsd.width = 0;
		hsd.height = 0;
		hsd.halls_count = bcount;
		hsd.halls = &hall;

		unsigned int b;
		for (b = 0; b < bcount; b++) {
			hsd.halls[b].x = MFCapGetCenterX();
			hsd.halls[b].y = MFCapGetCenterY();
			hsd.halls[b].left = 0;
			hsd.halls[b].right = 0;
			hsd.halls[b].top = 0;
			hsd.halls[b].bottom = 0;
			hsd.halls[b].color = 0;
			hsd.halls[b].area = MFCapGetSum();
			hsd.halls[b].range = 0;
		}
	}
	this->Interface::driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA,
			PLAYER_HALLSENSOR_DATA_HALLS, &hsd, sizeof(hsd), NULL);
	return;
}

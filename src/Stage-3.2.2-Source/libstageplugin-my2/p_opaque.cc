#include "multidriver.h"

InterfaceOpaque::InterfaceOpaque(player_devaddr_t addr, multidriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section,"publish",1);
}

int InterfaceOpaque::ProcessMessage(QueuePointer &resp_queue,
		player_msghdr_t* hdr, void* data) {

}
void InterfaceOpaque::Publish(void) {
}

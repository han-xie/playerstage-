#include "multidriver.h"

InterfaceBlobfinder::InterfaceBlobfinder( player_devaddr_t addr,
				multidriver* driver,
				ConfigFile* cf,
				int section )
  : Interface( addr, driver, cf, section)
{
	this->Interface::PublishSign = cf->ReadInt(section,"publish",1);
}

int InterfaceBlobfinder::ProcessMessage( QueuePointer& resp_queue,
													  player_msghdr_t* hdr,
													  void* data ){
	printf("test blobfinder\n");
	if (Message::MatchMessage(hdr, PLAYER_BLOBFINDER_REQ_GET_COLOR, -1,
			this->Interface::addr)) {
		return 0;
	}
	return -1;
}

void InterfaceBlobfinder::Publish(void){
	player_blobfinder_data_t bfd;
	bzero(&bfd, sizeof(bfd));

	uint32_t bcount = 1;
	player_blobfinder_blob_t blob;
	if (bcount > 0) {
		// and set the image width * height
		bfd.width = this->conf.scan_width;
		bfd.height = this->conf.scan_height;
		bfd.blobs_count = bcount;
		bfd.blobs = &blob;

		unsigned int b;
		for (b = 0; b < bcount; b++) {
			bfd.blobs[b].x = MFCapGetCenterX();
			bfd.blobs[b].y = MFCapGetCenterY();
			bfd.blobs[b].left = 0;
			bfd.blobs[b].right = 0;
			bfd.blobs[b].top = 0;
			bfd.blobs[b].bottom = 0;
			bfd.blobs[b].color = 0;
			bfd.blobs[b].area = MFCapGetSum();
			bfd.blobs[b].range = 0;
		}
	}
	this->Interface::driver->Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA,
			PLAYER_BLOBFINDER_DATA_BLOBS, &bfd, sizeof(bfd), NULL);
	return;
}

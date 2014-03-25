#if !defined (WIN32)
#include <unistd.h>
#include <netinet/in.h>
#endif
#include <string.h>
#include <libplayercore/playercore.h>
#include "multidriver.h"
#include "background.h"

void MFInit(){}

//LCD
void MF2X4LCD(const char*item,int insize){}

//Ultrasonic
void MFUSonicEnable(){}
int MFGetUltrasonic(){return 0;}

//Delay
void DelayMS(int inMS){}

//SerialCom
void MFComSend(const void *pBuffer, const int iLength){}

//Digi. IO
void MFDigiInit(int interval){}
void MFSetPortDirect(unsigned int inData){}
int MFGetDigiInput(int inID){return 0;}
void MFDigiOutput(int inID,int inVal){}

//AD
void MFADInit(int interval){}
void MFADEnable(int inID){}
int MFGetAD(int inID){return 0;}

//Servo
void MFServoInit(int interval){}
void MFSetServoMode(int inID,int inMode){}
void MFSetServoPos(int inID,int inPos,int inSpeed){}
void MFSetServoRotaSpd(int inID,int inSpeed){}
int MFGetServoPos(int inID){return 0;}
void MFServoAction(){}

//Video Capture
bool MFCapOpen(){}
void MFCapSetH(int inHmax,int inHmin){}
void MFCapSetS(int inSmax,int inSmin){}
void MFCapSetI(int inImax,int inImin){}
void MFCapSetMode(int inMode){}
int MFCapGetCenterX(){return 0;}
int MFCapGetCenterY(){return 0;}
int MFCapGetSum(){return 0;}

//SpeechRecongn
void MFSRStart(){}
void MFSRStop(){}
bool MFSRPause(){return 0;}
bool MFSRContinue(){return 0;}
bool MFSRInsertText(int index,const char*item,int insize){return 0;}
int MFSRGetResIndex(){return 0;}
void MFSRClearItems(){}
bool MFSRisStarted(){return 0;}

//mp3
void MFMp3Play(const char*item,int insize){}
void MFMp3Stop(){}

//UDP
int UDPConnect(char *inTarIP, int inTarPort){return 0;}
void UDPSend(char *inbuf, int inLen){}
int UDPListen(int inPort){return 0;}
//void UDPRecFunc(PBUFREC inFunc){}

//debug
int GetCmdVaildprc(){return 0;}
int GetCmdWaitMax(){return 0;}
int GetCmdCurCnt(){return 0;}
bool ComCreat(int inNo,unsigned int inBaudrate){return 0;}	//inNo:  1-RS422   2-mega128




//int initialTimes=0;
// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver* multidriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
	return ((Driver*) (new multidriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void multidriver_Register(DriverTable* table) {
	table->AddDriver("multidriver", multidriver_Init);
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
	multidriver_Register(table);
	return (0);
}
}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
multidriver::multidriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {
	int initialTimes;
	initialTimes++;
	if(initialTimes<2){
		MFCapOpen();
		MFInit();
		int portdirect = 0;
		MFSetPortDirect(portdirect);
		for (int i = 0; i < 8; i++) {
			MFADEnable(i);
		}
		MFDigiInit(100);
		DelayMS(100);
		MFADInit(100);
		DelayMS(100);
		MFUSonicEnable();
		DelayMS(100);
	}
	int device_count = cf->GetTupleCount(section, "provides");
	for (int d = 0; d < device_count; d++) {
		player_devaddr_t player_addr;
		if (cf->ReadDeviceAddr(&player_addr, section, "provides", 0, d, NULL)
				!= 0) {
			this->SetError(-1);
			return;
		}
		Interface *ifsrc = NULL;
		switch (player_addr.interf) {
		case PLAYER_BLOBFINDER_CODE:
			ifsrc = new InterfaceBlobfinder(player_addr, this, cf, section);
			break;
		case PLAYER_LASER_CODE:
			ifsrc = new InterfaceLaser(player_addr, this, cf, section);
			break;
		case PLAYER_POSITION2D_CODE:
			ifsrc = new InterfacePosition2d(player_addr, this, cf, section);
			break;
		case PLAYER_OPAQUE_CODE:
			ifsrc = new InterfaceOpaque(player_addr, this, cf, section);
			break;
		default:
			PRINT_ERR1(
					"error: multi driver doesn't support interface type %d\n",
					player_addr.interf);
			this->SetError(-1);
			return;
		}
		if (ifsrc) {
			if (this->AddInterface(ifsrc->addr)) {
				PRINT_ERR("AddInterface() failed");
				this->SetError(-2);
				return;
			}
			devices.push_back(ifsrc);
		} else {
			PRINT_ERR3("No Stage source found for interface %d:%d:%d",
					player_addr.robot, player_addr.interf, player_addr.index);
			this->SetError(-3);
			return;
		}
	}
}

Interface* multidriver::LookupDevice(player_devaddr_t addr) {
	FOR_EACH(it, this->devices)
	{
		Interface* candidate = *it;
		if (candidate->addr.robot == addr.robot
				&& candidate->addr.interf == addr.interf
				&& candidate->addr.index == addr.index)
			return candidate; // found
	}
	return NULL; // not found
}

// subscribe to a device
int multidriver::Subscribe(QueuePointer &queue, player_devaddr_t addr) {
	Interface* device = this->LookupDevice(addr);
	if (device) {
		device->Subscribe();
		device->Subscribe(queue);
		return Driver::Subscribe(addr);
	}
	return 1; // error
}
// unsubscribe to a device
int multidriver::Unsubscribe(QueuePointer &queue, player_devaddr_t addr) {
	Interface* device = this->LookupDevice(addr);
	if (device) {
		device->Unsubscribe();
		device->Unsubscribe(queue);
		return Driver::Unsubscribe(addr);
	} else
		return 1; // error
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int multidriver::MainSetup() {
	// Subscribe to the laser device
	/*if(!(this->laser_dev = deviceTable->GetDevice(this->laser_addr)))
	 {
	 PLAYER_ERROR("unable to locate suitable laser device");
	 return(-1);
	 }
	 if(this->laser_dev->Subscribe(this->InQueue) != 0)
	 {
	 PLAYER_ERROR("unable to subscribe to laser device");
	 return(-1);
	 }*/

	// Here you do whatever else is necessary to setup the device, like open and
	// configure a serial port.
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void multidriver::MainQuit() {
	// Unsubscribe from the laser

	FOR_EACH(it, this->devices)
		(*it)->Unsubscribe();

	return;
	// Here you would shut the device down by, for example, closing a
	// serial port.
}

////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void multidriver::Main() {
	// The main loop; interact with the device here
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  Calls ProcessMessage() on each pending
		// message.
		this->ProcessMessages();

		for(std::vector<Interface*>::iterator in = this->devices.begin(); in != this->devices.end(); ++in){
			Interface* interface = *in;
			assert( interface );
			if(interface->PublishSign)
				interface->Publish();
		}
		/*FOR_EACH( it, this->devices ){
				Interface* interface = *it;
				assert( interface );
		}*/

		// Sleep (or you might, for example, block on a read() instead)
		usleep(100000);
	}
	return;
}

int multidriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	// Handle new data from the laser
	/*if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
	 this->laser_addr))
	 {
	 // Do someting with it
	 return(0);
	 }*/

	// find the right interface to handle this config
	Interface* in = this->LookupDevice(hdr->addr);
	if (in) {
		return (in->ProcessMessage(resp_queue, hdr, data));
	} else {
		PRINT_WARN3("can't find interface for device %d.%d.%d",
				this->device_addr.robot, this->device_addr.interf,
				this->device_addr.index);
		return (-1);
	}
}


InterfaceBlobfinder::InterfaceBlobfinder( player_devaddr_t addr,
				MultiDriver* driver,
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
	Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA,
			PLAYER_BLOBFINDER_DATA_BLOBS, &bfd, sizeof(bfd), NULL);
	return;
}


#include "multidriver.h"
#include <string.h>

InterfaceLaser::InterfaceLaser(player_devaddr_t addr, MultiDriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section, "publish", 1);
	std::string laserType = "";
	laserType = cf->ReadString(section, "type", laserType.data());
	this->conf.type = StringToType(laserType.data());
	this->conf.portsNum = cf->GetTupleCount(section, "ports");
	for (int i = 0; i < this->conf.portsNum; i++)
		this->conf.ports[i] = cf->ReadTupleInt(section, "ports", i, "0");
}

int InterfaceLaser::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_CONFIG, this->Interface::addr)) {
		player_laser_config_t* plc = (player_laser_config_t*) data;
		if (hdr->size == sizeof(player_laser_config_t)) {
			Publish(this->Interface::addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
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

		Publish(this->Interface::addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
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
	pdata.id = this->scan_id++;
	pdata.ranges = new float[pdata.ranges_count];
	pdata.intensity = new uint8_t[pdata.ranges_count];


	for (int i = 0; i < this->conf.portsNum; i++) {
		pdata.ranges[i] = this->conf.ports[i];
		pdata.intensity[i] = 0;
	}

	// Write laser data
	Publish(this->Interface::addr, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN,
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

int InterfaceLaser::StringToType(char *p) {
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



InterfaceOpaque::InterfaceOpaque(player_devaddr_t addr, MultiDriver* driver,
		ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->Interface::PublishSign = cf->ReadInt(section,"publish","1");
}

int InterfaceOpaque::ProcessMessage(QueuePointer &resp_queue,
		player_msghdr_t* hdr, void* data) {

}
void InterfaceOpaque::Publish(void) {
}


InterfacePosition2d::InterfacePosition2d(player_devaddr_t addr,
		MultiDriver* driver, ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->conf.frontL = cf->ReadInt(section, "frontL", 1);
	this->conf.frontR = cf->ReadInt(section, "frontR", 2);
	this->conf.rearL = cf->ReadInt(section, "rearL", 3);
	this->conf.rearR = cf->ReadInt(section, "rearR", 4);
	this->conf.servoFR = cf->ReadInt(section, "servofr", 5);
	this->conf.servoFL = cf->ReadInt(section, "servofl", 6);
	this->conf.servoRR = cf->ReadInt(section, "servorr", 7);
	this->conf.servoRL = cf->ReadInt(section, "servorl", 8);
	this->conf.rationSpeedToServo = 2000;
	this->conf.maxSpeed = 1023;
	this->conf.aToAngle = 57.4;
	std::string pos2dType = "POS2D4TIRES";
	pos2dType = cf->ReadString(section, "type", pos2dType);
	if (pos2dType == "PPOS2D4TIRES")
		this->conf.type = POS2D4TIRES;
	else if (pos2dType == "POS2D8TOP")
		this->conf.type = POS2D8TOP;
	else if (pos2dType == "POS2D8BUTTON")
		this->conf.type = POS2D8BUTTON;
	else if (pos2dType == "POS2DOMNIDIR")
		this->conf.type = POS2DOMNIDIR;
	this->Interface::PublishSign = cf->ReadInt(section,"publish","1");
}

int InterfacePosition2d::ProcessMessage(QueuePointer &resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (this->conf.type == POS2D8TOP) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL, this->Interface::addr)) {
			player_position2d_cmd_vel_t* pcmd =
					(player_position2d_cmd_vel_t*) data;

			//mod->SetSpeed(pcmd->vel.px, pcmd->vel.py, pcmd->vel.pa);
			double speed = pcmd->vel.px;
			double turn = pcmd->vel.pa;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();

			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_CAR, this->Interface::addr)) {
			player_position2d_cmd_car_t* pcmd =
					(player_position2d_cmd_car_t*) data;
			//mod->SetSpeed(pcmd->velocity, 0, pcmd->angle);
			double speed = pcmd->velocity;
			double turn = pcmd->angle;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
			return -1;
		}
	} else if (this->conf.type == POS2D8BUTTON) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_POS, this->Interface::addr)) {
			player_position2d_cmd_pos_t* pcmd =
					(player_position2d_cmd_pos_t*) data;
			//mod->GoTo(pcmd->pos.px, pcmd->pos.py, pcmd->pos.pa);
			double turn = pcmd->pos.pa;
			turn *= this->conf.aToAngle;
			if (turn > 300 || turn < 0)
				return -1;
			int pos = (turn * 1023) / 300;
			MFSetServoPos(this->conf.servoFR, pos, 400);
			MFSetServoPos(this->conf.servoFL, pos, 400);
			MFSetServoPos(this->conf.servoRR, pos, 400);
			MFSetServoPos(this->conf.servoRL, pos, 400);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
				PLAYER_POSITION2D_DATA_STATE, this->Interface::addr)) {
			//fellow the Pos2d8Top
			return 0;
		}else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
			return -1;
		}
	} else if (this->conf.type == POS2DOMNIDIR) {
		return -1;
	} else if (this->conf.type == POS2D4TIRES) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
					PLAYER_POSITION2D_CMD_VEL, this->Interface::addr)) {
				player_position2d_cmd_vel_t* pcmd = (player_position2d_cmd_vel_t*) data;

				//mod->SetSpeed(pcmd->vel.px, pcmd->vel.py, pcmd->vel.pa);
				double speed = pcmd->vel.px;
				double turn = pcmd->vel.pa;
				double frontLSpeed = speed - turn;
				double rearLSpeed = speed - turn;
				double frontRSpeed = speed + turn;
				double rearRSpeed = speed + turn;
				if ((int) frontLSpeed > this->conf.maxSpeed)
					frontLSpeed = rearLSpeed = this->conf.maxSpeed;
				if ((int) frontLSpeed < -this->conf.maxSpeed)
					frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
				if ((int) frontRSpeed > this->conf.maxSpeed)
					frontRSpeed = rearRSpeed = this->conf.maxSpeed;
				if ((int) frontRSpeed < -this->conf.maxSpeed)
					frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

				MFSetServoRotaSpd(this->conf.frontL,
						((int) frontLSpeed) * this->conf.rationSpeedToServo);
				MFSetServoRotaSpd(this->conf.rearL,
						((int) rearLSpeed) * this->conf.rationSpeedToServo);
				MFSetServoRotaSpd(this->conf.frontR,
						((int) frontRSpeed) * this->conf.rationSpeedToServo);
				MFSetServoRotaSpd(this->conf.rearR,
						((int) rearRSpeed) * this->conf.rationSpeedToServo);
				MFServoAction();

				return 0;
			}else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
					PLAYER_POSITION2D_CMD_CAR, this->Interface::addr)) {
				player_position2d_cmd_car_t* pcmd = (player_position2d_cmd_car_t*) data;
				//mod->SetSpeed(pcmd->velocity, 0, pcmd->angle);
				double speed = pcmd->velocity;
				double turn = pcmd->angle;
				double frontLSpeed = speed - turn;
				double rearLSpeed = speed - turn;
				double frontRSpeed = speed + turn;
				double rearRSpeed = speed + turn;
				if ((int) frontLSpeed > this->conf.maxSpeed)
					frontLSpeed = rearLSpeed = this->conf.maxSpeed;
				if ((int) frontLSpeed < -this->conf.maxSpeed)
					frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
				if ((int) frontRSpeed > this->conf.maxSpeed)
					frontRSpeed = rearRSpeed = this->conf.maxSpeed;
				if ((int) frontRSpeed < -this->conf.maxSpeed)
					frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

				MFSetServoRotaSpd(this->conf.frontL,
						((int) frontLSpeed) * this->conf.rationSpeedToServo);
				MFSetServoRotaSpd(this->conf.rearL,
						((int) rearLSpeed) * this->conf.rationSpeedToServo);
				MFSetServoRotaSpd(this->conf.frontR,
						((int) frontRSpeed) * this->conf.rationSpeedToServo);
				MFSetServoRotaSpd(this->conf.rearR,
						((int) rearRSpeed) * this->conf.rationSpeedToServo);
				MFServoAction();
				return 0;
			} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
					PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
				return -1;
			}
	}
	return -1;
}

void InterfacePosition2d::Publish(void){
	return;
}


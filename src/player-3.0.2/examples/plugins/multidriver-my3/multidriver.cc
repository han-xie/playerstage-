#if !defined (WIN32)
  #include <unistd.h>
  #include <netinet/in.h>
#endif
#include <string.h>
#include <vector>
#include <libplayercore/playercore.h>

#define VAR(V,init) __typeof(init) V=(init)
#define FOR_EACH(I,C) for(VAR(I,(C).begin());I!=(C).end();I++)

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



class Interface;
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class multidriver : public ThreadedDriver
{
  public:
    
    // Constructor; need that
    multidriver(ConfigFile* cf, int section);
  	~multidriver(void){};
    virtual int ProcessMessage(QueuePointer & resp_queue, 
                               player_msghdr * hdr, 
                               void * data);
  private:
    // Main function for device thread.
    virtual void Main();
    virtual int MainSetup();
    virtual void MainQuit();

    // My position interface
    player_devaddr_t m_position_addr;
    // My laser interface
    player_devaddr_t m_laser_addr;

    // Address of and pointer to the laser device to which I'll subscribe
    player_devaddr_t laser_addr;
    Device* laser_dev;

  	virtual int Subscribe(QueuePointer &queue, player_devaddr_t addr){};
  	virtual int Unsubscribe(QueuePointer &queue, player_devaddr_t addr){};
  	Interface* LookupDevice(player_devaddr_t addr);
  protected:
  	std::vector<Interface*> devices;
};
class Interface {
public:
	Interface(player_devaddr_t addr, multidriver* driver, ConfigFile* cf,
			int section);
	virtual ~Interface(void) {
	}
	;

	player_devaddr_t addr;
	multidriver* driver; // the driver instance that created this device
	char PublishSign;

	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr_t* hdr,
			void* data) {
		return (-1);
	} // empty implementation
	virtual void Publish(void) {
	}
	; // do nothing
	virtual void Subscribe(void) {
	}
	; // do nothing
	virtual void Unsubscribe(void) {
	}
	; // do nothing
	virtual void Subscribe(QueuePointer &queue) {
	}
	; // do nothing
	virtual void Unsubscribe(QueuePointer &queue) {
	}
	;
};
typedef struct {
	int type;
} OpaqueConf;
//define type of laserConf
#define DIOMAX 11
#define DIOMIN 0
#define AIOMAX 7
#define AIOMIN 0
#define RS422MAX 1
#define RS422MIN 0
#define DONTKNOW 0
#define AIO 1
#define DIO 2
#define RS422 3
#define DIOOUT 4
#define DIOIN 5
#define infrProxSen 0 //#DIO：红外接近传感器端口号
#define colliSen 1  //#DIO：碰撞传感器端口号
#define soundSen 2  //#DIO：声音传感器端口号
#define gestSen 3   //#DIO：姿态传感器端口号
#define hallSen 4   //#DIO：霍尔接近传感器端口号
#define dout 5 //#DIO：输出端口端口号
#define infrDistSen 100 //#AIO：红外测距传感器端口号
#define tempSen 101 //#AIO：温度传感器端口号
#define graySen 102  //#AIO：灰度传感器端口号
#define lightSen 103  //#AIO：光强传感器端口号
#define RS422Sen 200     //RS422
typedef struct {
	int type;
	int portsNum;
	char ports[DIOMAX + 1];
	float fov;
	int sample_count;
	struct Bounds {
		double min;
		double max;
	} range_bounds;
	int scan_id;
} LaserConf;
//define type of position2dconf
#define POS2D8TOP 1
#define POS2D8BUTTON 2
#define POS2DOMNIDIR 3
#define POS2D4TIRES 4
typedef struct {
	int type;
	int frontL;
	int frontR;
	int rearL;
	int rearR;
	int servoFR;
	int servoFL;
	int servoRR;
	int servoRL;
	int rationSpeedToServo;
	int maxSpeed;
	int aToAngle;
} Position2dConf;
typedef struct {
	int type;
	int scan_width;
	int scan_height;
} BlobfinderConf;
class InterfaceOpaque: public Interface {
public:
	//InterfaceOpaque(player_devaddr_t addr, multidriver* driver, ConfigFile* cf,int section, const std::string& type);
	InterfaceOpaque(player_devaddr_t addr, multidriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceOpaque(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
public:
	virtual void Publish(void);
	OpaqueConf conf;
};
class InterfaceLaser: public Interface {
public:
	InterfaceLaser(player_devaddr_t addr, multidriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceLaser(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
public:
	virtual void Publish(void);
	LaserConf conf;
private:
	int AioDioRS422(int type);
	int StringToType(const char *p);
};
class InterfacePosition2d: public Interface {
public:
	InterfacePosition2d(player_devaddr_t addr, multidriver* driver,
			ConfigFile* cf, int section);
	virtual ~InterfacePosition2d(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
public:
	virtual void Publish(void);
	Position2dConf conf;
};
class InterfaceBlobfinder: public Interface {
public:
	InterfaceBlobfinder(player_devaddr_t addr, multidriver* driver,
			ConfigFile* cf, int section);
	virtual ~InterfaceBlobfinder(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
public:
	virtual void Publish(void);
	BlobfinderConf conf;
};
// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver* multidriver_Init(ConfigFile* cf, int section)
{
  // Create and return a new instance of this driver
  return ((Driver*) (new multidriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void multidriver_Register(DriverTable* table)
{
  table->AddDriver("multidriver", multidriver_Init);
}


////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C"
{
  int player_driver_init(DriverTable* table)
  {
    multidriver_Register(table);
    return(0);
  }
}


////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.
int initialTimes;
multidriver::multidriver(ConfigFile* cf, int section)
    : ThreadedDriver(cf, section)
{
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
			printf(
					"error: multi driver doesn't support interface type %d\n",
					player_addr.interf);
			this->SetError(-1);
			return;
		}
		if (ifsrc) {
			if (this->AddInterface(ifsrc->addr)) {
				printf("AddInterface() failed");
				this->SetError(-2);
				return;
			}
			devices.push_back(ifsrc);
		} else {
			printf("No Stage source found for interface %d:%d:%d",
					player_addr.robot, player_addr.interf, player_addr.index);
			this->SetError(-3);
			return;
		}
	}
/*
  if (cf->ReadDeviceAddr(&(this->m_position_addr), section,
                         "provides", PLAYER_POSITION2D_CODE, -1, NULL) != 0)
  {
    this->SetError(-1);
    return;
  }
  if (this->AddInterface(this->m_position_addr))
  {
    this->SetError(-1);
    return;
  }

  // Create my laser interface
  if (cf->ReadDeviceAddr(&(this->m_laser_addr), section,
                         "provides", PLAYER_LASER_CODE, -1, NULL) != 0)
  {
    this->SetError(-1);
    return;
  }
  if (this->AddInterface(this->m_laser_addr))
  {
    this->SetError(-1);
    return;
  }*/
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int multidriver::MainSetup()
{   
  return(0);
}


////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void multidriver::MainQuit()
{
	FOR_EACH(it, this->devices)
		(*it)->Unsubscribe();
	return;
}


////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void multidriver::Main()
{
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
			usleep(100000);
		}
		return;
}


int multidriver::ProcessMessage(QueuePointer & resp_queue,
                                player_msghdr * hdr, 
                                void * data)
{
  // Handle new data from the laser
  if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA, PLAYER_LASER_DATA_SCAN, 
                           this->laser_addr))
  {
    // Do someting with it
    return(0);
  }
  
  // Tell the caller that you don't know how to handle this message
  return(-1);
}


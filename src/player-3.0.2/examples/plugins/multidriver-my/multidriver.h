#ifndef multidriver_H_
#define multidriver_H_

#if !defined (WIN32)
#include <unistd.h>
#include <netinet/in.h>
#endif
#include <string.h>
#include <libplayercore/playercore.h>
#include "stage.hh"
#include "background.h"
#include "CYZXInter.h"

class Interface;
////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class multidriver: public ThreadedDriver {
public:
	multidriver(ConfigFile* cf, int section);
	~multidriver(void){};
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
private:
	// Main function for device thread.
	virtual void Main();
	virtual int MainSetup();
	virtual void MainQuit();
	virtual int Subscribe(QueuePointer &queue, player_devaddr_t addr);
	virtual int Unsubscribe(QueuePointer &queue, player_devaddr_t addr);
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
	}; // empty implementation
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
	//laser
	int scan_id = 0;
	int aioPortMin;
	int aioPortMax;
	int dioPortMin;
	int dioPortMax;
	char dio[12];
	char rs422p0;
	char rs422p1;
	//blobfinder
	//position2d
	int frontL;
	int frontR;
	int rearL;
	int rearR;
	int servoFR;
	int servoFL;
	int servoRR;
	int servoRL;
#ifdef WRITELOG
	//all messages count
	int mc;
	//don't know messages count
	int nmc;
	int aiomc;
	int naiomc;
	int diomc;
	int ndiomc;
	int posmc;
	int nposmc;
	int opaquemc;
	int nopaquemc;
	int opaquemfcapmc;
	int nopaquemfcapmc;
	int opaquediomc;
	int nopaquediomc;
	FILE *fl;
	time_t logtime;
#endif
} OpaqueConf;
//define type of laserConf
#define DIOMAX 11
#define DIOMIN 0
#define AIOMAX 7
#define AIOMIN 0
#define RS422MAX 1
#define RS422MIN 0
#define DONTKNOW 6
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
private:
	int dealPosMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealLaserMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealBlobFinderMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealOpaqueMFCAPCommand(myOpaqueSt *popa);
	int dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueBKComReq(comReqBk1 *popa, respBk1 *res);
	int dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaquePosition2d(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueBK(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueSR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void writeMessagehrToFile(player_msghdr * hdr, void * data);
	int dealOpaqueBKGETULTRASONICF(comReqBk1 *popa);
	int deaperDealAIO(int t, int type);
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
#endif

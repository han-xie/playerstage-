#ifndef _STAGE_PLAYER_DRIVER_H
#define _STAGE_PLAYER_DRIVER_H

#include <unistd.h>
#include <string.h>
#include <math.h>

#include <libplayercore/playercore.h>

#include "../libstage/stage.hh"

#include <getopt.h>
#include "websim.hh"
#include <stage.hh>
using namespace Stg;
#include "config.h"

#define DRIVER_ERROR(X) printf( "Stage driver error: %s\n", X )

// foward declare;
class Interface;
class StgTime;
class WebStage;
/*typedef struct OpaqueModel{
	char model[10];
	Stg::ModelPosition* positionModel;
	Stg::ModelLaser* laserModel;
	OpaqueModel *next;
}OpaqueModel;*/

class StgDriver: public Driver {
public:
	// Constructor; need that
	StgDriver(ConfigFile* cf, int section);

	// Destructor
	~StgDriver(void);

	// Must implement the following methods.
	virtual int Setup();
	virtual int Shutdown();
	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr * hdr,
			void * data);
	virtual int Subscribe(QueuePointer &queue, player_devaddr_t addr);
	virtual int Unsubscribe(QueuePointer &queue, player_devaddr_t addr);

	/// The server thread calls this method frequently. We use it to
	/// check for new commands and configs
	virtual void Update();

	/// all player devices share the same Stage world (for now)
	//static Stg::WorldGui* world;
	static Stg::World* world;
	static WebStage* ws;

	/// find the device record with this Player id
	Interface* LookupDevice(player_devaddr_t addr);

	Stg::Model* LocateModel(char* basename, player_devaddr_t* addr,
			const std::string& type);

protected:

	/// an array of pointers to Interface objects, defined below
	//GPtrArray* devices;
	std::vector<Interface*> devices;
};

class Interface {
public:
	Interface(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);

	virtual ~Interface(void) { /* TODO: clean up*/
	}
	;

	player_devaddr_t addr;
	double last_publish_time;
	double publish_interval_msec;

	StgDriver* driver; // the driver instance that created this device

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
	// do nothing};
};

class InterfaceSimulation: public Interface {
public:
	InterfaceSimulation(player_devaddr_t addr, StgDriver* driver,
			ConfigFile* cf, int section);
	virtual ~InterfaceSimulation(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
};

class InterfaceOpaque: public Interface {
public:
	InterfaceOpaque(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceOpaque(void) {
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
private:
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
};

// base class for all interfaces that are associated with a model
class InterfaceModel

: public Interface {
public:
	InterfaceModel(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section, const std::string& type);

	Stg::Model* mod;

	virtual ~InterfaceModel(void) { /* TODO: clean up*/
	}
	;

	virtual void Subscribe(void) {
		this->mod->Subscribe();
	}
	;
	virtual void Unsubscribe(void) {
		this->mod->Unsubscribe();
	}
	;
};

//define type of position2dconf
#define POS2D8TOP 1
#define POS2D8BUTTON 2
#define POS2DOMNIDIR 3
#define POS2D4TIRES 4
#define FPOS2D8BUTTON 5
#define FPOS2DOMNIDIR 6
#define FPOS2D4TIRES 7
typedef struct{
	int type;
	double redressAngle;
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
}Pos2dConf;
typedef struct{
	player_position2d_data_t pos2d;
	double redressAngle;
}player_position2d_data_t_redress;
class InterfacePosition: public InterfaceModel {
public:
	InterfacePosition(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfacePosition(void) { /* TODO: clean up*/
	}
	;
	virtual void Publish(void);
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
	virtual bool testCollision();
private:
	Pos2dConf conf;
	player_devaddr_t require_addr;
	Device* require_dev;
	//bool collisionS;
};

class InterfaceGripper: public InterfaceModel {
public:
	InterfaceGripper(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceGripper(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
	virtual void Publish(void);
};

class InterfaceWifi: public InterfaceModel {
public:
	InterfaceWifi(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceWifi(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
	virtual void Publish(void);
};

class InterfaceSpeech: public InterfaceModel {
public:
	InterfaceSpeech(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceSpeech(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
	virtual void Publish(void);
};
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
#define infrProxSensor 0 //#DIO：红外接近传感器端口号
#define colliSensor 1  //#DIO：碰撞传感器端口号
#define soundSensor 2  //#DIO：声音传感器端口号
#define gestSensor 3   //#DIO：姿态传感器端口号
#define hallSensor 4   //#DIO：霍尔接近传感器端口号
#define doutSensor 5 //#DIO：输出端口端口号
#define infrDistSensor 100 //#AIO：红外测距传感器端口号
#define tempSensor 101 //#AIO：温度传感器端口号
#define graySensor 102  //#AIO：灰度传感器端口号
#define lightSensor 103  //#AIO：光强传感器端口号
#define RS422Sensor 200     //RS422
typedef struct{
	int type;
	int macroName;
	int portsNum;
	char ports[DIOMAX + 1];
	float fov;
	int sample_count;
	struct Bounds {
		double min;
		double max;
	} range_bounds;
	int scan_id;
}LaserConf;
class InterfaceLaser: public InterfaceModel {
private:
	int scan_id;
	LaserConf conf;
	//player_devaddr_t opaque_addr;
	player_devaddr_t require_addr;
	Device* require_dev;
public:
	InterfaceLaser(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceLaser(void) { /* TODO: clean up*/
	}
	;
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
	virtual void Publish(void);
};

/*  class InterfaceAio : public InterfaceModel */
/* { */
/*  public: */
/*   InterfaceAio( player_devaddr_t addr, StgDriver* driver, ConfigFile* cf, int section ); */
/*   virtual ~InterfaceAio( void ){ /\* TODO: clean up*\/ }; */
/*   virtual int ProcessMessage(QueuePointer & resp_queue, */
/* 			      player_msghdr_t* hdr, */
/* 			      void* data); */
/*   virtual void Publish( void ); */
/* }; */

/* class InterfaceDio : public InterfaceModel */
/* { */
/* public: */
/* 	InterfaceDio(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf, int section); */
/* 	virtual ~InterfaceDio(); */
/* 	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr, void* data); */
/* 	virtual void Publish(); */
/* }; */

class InterfacePower: public InterfaceModel {
public:
	InterfacePower(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfacePower(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	virtual void Publish(void);
};

class InterfaceFiducial: public InterfaceModel {
public:
	InterfaceFiducial(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceFiducial(void) { /* TODO: clean up*/
	}
	;

	virtual void Publish(void);
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
};

class InterfaceActArray: public InterfaceModel {
public:
	InterfaceActArray(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceActArray(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	virtual void Publish(void);
};

class InterfaceBlobfinder: public InterfaceModel {
public:
	InterfaceBlobfinder(player_devaddr_t addr, StgDriver* driver,
			ConfigFile* cf, int section);
	virtual ~InterfaceBlobfinder(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	virtual void Publish(void);
};

class InterfacePtz: public InterfaceModel {
public:
	InterfacePtz(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfacePtz(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	virtual void Publish(void);
};

class InterfaceSonar: public InterfaceModel {
public:
	InterfaceSonar(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceSonar(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	virtual void Publish(void);
};

class InterfaceBumper: public InterfaceModel {
public:
	InterfaceBumper(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceBumper(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	virtual void Publish(void);
};

class InterfaceLocalize: public InterfaceModel {
public:
	InterfaceLocalize(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);

	virtual ~InterfaceLocalize(void) { /* TODO: clean up*/
	}
	;

	virtual void Publish(void);
	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr_t* hdr,
			void* data);
};

class InterfaceMap: public InterfaceModel {
public:
	InterfaceMap(player_devaddr_t addr, StgDriver* driver, ConfigFile* cf,
			int section);
	virtual ~InterfaceMap(void) { /* TODO: clean up*/
	}
	;

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	//virtual void Publish( void );

	// called by ProcessMessage to handle individual messages

	int HandleMsgReqInfo(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int HandleMsgReqData(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
};

class PlayerGraphics2dVis;
class InterfaceGraphics2d: public InterfaceModel {
public:
	InterfaceGraphics2d(player_devaddr_t addr, StgDriver* driver,
			ConfigFile* cf, int section);
	virtual ~InterfaceGraphics2d(void);

	void Subscribe(QueuePointer &queue);
	void Unsubscribe(QueuePointer &queue);

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	PlayerGraphics2dVis * vis;
};

class PlayerGraphics3dVis;
class InterfaceGraphics3d: public InterfaceModel {
public:
	InterfaceGraphics3d(player_devaddr_t addr, StgDriver* driver,
			ConfigFile* cf, int section);
	virtual ~InterfaceGraphics3d(void);

	void Subscribe(QueuePointer &queue);
	void Unsubscribe(QueuePointer &queue);

	virtual int ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	PlayerGraphics3dVis * vis;
};

/** Replaces Player's real time clock object */
class StTime: public PlayerTime {
private:
	StgDriver* driver;

public:
	// Constructor
	StTime(StgDriver* driver);

	// Destructor
	virtual ~StTime();

	// Get the simulator time
	int GetTime(struct timeval* time);
	int GetTimeDouble(double* time);
};

class WebStage: public websim::WebSim {
	Stg::World* world;

public:
	WebStage(Stg::World* world, const std::string& host,
			const unsigned short port) :
			websim::WebSim(host, port), world(world) {
	}

	virtual ~WebStage() {
	}

	virtual std::string IdentificationString() {
		return "WebStage";
	}

	virtual std::string VersionString() {
		return Stg::Version();
	}

	virtual bool ClockStart() {
		puts("[WebStage]  Clock start");
		world->Start();
		return true;
	}

	virtual bool ClockStop() {
		puts("[WebStage]  Clock stop");
		world->Stop();
		return true;
	}

	virtual bool ClockRunFor(uint32_t msec) {
		puts("[WebStage]  Clock tick");

		world->paused = true;
		world->sim_interval = msec * 1e-3; // usec
		world->Update();

		return true;
	}

	virtual bool GetModelType(const std::string& name, std::string& type) {
		Stg::Model* mod = world->GetModel(name.c_str());

		if (!mod)
			return false;

		type = mod->GetModelType();
		return true;
	}

	void Push(const std::string& name) {
		Stg::Model* mod = world->GetModel(name.c_str());

		if (mod) {
			websim::Pose p;
			websim::Velocity v;
			websim::Acceleration a;

			Stg::Pose sp = mod->GetPose();
			p.x = sp.x;
			p.y = sp.y;
			p.z = sp.z;
			p.a = sp.a;

			Stg::Velocity sv = mod->GetVelocity();
			v.x = sv.x;
			v.y = sv.y;
			v.z = sv.z;
			v.a = sv.a;

			SetPuppetPVA(name, p, v, a);
		} else
			printf(
					"Warning: attempt to push PVA for unrecognized model \"%s\"\n",
					name.c_str());
	}

	void Push() {
		for (std::map<std::string, Puppet*>::iterator it = puppets.begin();
				it != puppets.end(); it++) {
			Puppet* pup = it->second;
			assert(pup);

			Stg::Model* mod = world->GetModel(pup->name.c_str());
			assert(mod);

			websim::Pose p;
			websim::Velocity v;
			websim::Acceleration a;

			Stg::Pose sp = mod->GetPose();
			p.x = sp.x;
			p.y = sp.y;
			p.z = sp.z;
			p.a = sp.a;

			Stg::Velocity sv = mod->GetVelocity();
			v.x = sv.x;
			v.y = sv.y;
			v.z = sv.z;
			v.a = sv.a;

			pup->Push(p, v, a);
			printf("pushing puppet %s\n", pup->name.c_str());
		}
	}

	virtual bool GetModelChildren(const std::string& model,
			std::vector<std::string>& children) {
		std::vector<Model*> c;

		if (model == "") {
			c = world->GetChildren();
		} else {
			Stg::Model* mod = world->GetModel(model.c_str());
			if (mod) {

				c = mod->GetChildren();

			} else {
				printf(
						"Warning: Tried to get the children of unknown model:%s \n",
						model.c_str());
				return false;
			}

		}

		FOR_EACH( it, c )
		{
			children.push_back(std::string((*it)->Token()));
		}

		return true;

	}

	// Interface to be implemented by simulators
	virtual bool CreateModel(const std::string& name, const std::string& type,
			std::string& error) {
		printf("create model name:%s type:%s\n", name.c_str(), type.c_str());

		Model* mod = world->CreateModel(NULL, type.c_str()); // top level models only for now

		// rename the model and store it by the new name
		mod->SetToken(name.c_str());
		world->AddModel(mod);

		printf("done.");
		return true;
	}

	virtual bool DeleteModel(const std::string& name, std::string& error) {
		printf("delete model name:%s \n", name.c_str());
		return true;
	}

	virtual bool GetModelData(const std::string& name,

	std::string& response, websim::Format format, void* xmlparent) {
		std::string str;
		websim::Time t = GetTime();

		Model*mod = world->GetModel(name.c_str());
		if (mod) {
			std::string type = mod->GetModelType();
			if (type == "position") {

				websim::Pose p;
				websim::Velocity v;
				websim::Acceleration a;

				Stg::Pose sp = mod->GetPose();
				p.x = sp.x;
				p.y = sp.y;
				p.z = sp.z;
				p.a = sp.a;

				Stg::Velocity sv = mod->GetVelocity();
				v.x = sv.x;
				v.y = sv.y;
				v.z = sv.z;
				v.a = sv.a;

				WebSim::GetPVA(name, t, p, v, a, format, response, xmlparent);

			} else if (type == "laser") {

				uint32_t resolution;
				double fov;
				websim::Pose p;
				std::vector<double> ranges;

				ModelLaser* laser = (ModelLaser*) mod;
				uint32_t sample_count = 0;
				std::vector<ModelLaser::Sample> scan = laser->GetSamples();

				ModelLaser::Config cfg = laser->GetConfig();
				resolution = cfg.resolution;
				fov = cfg.fov;

				sample_count = scan.size();
				for (unsigned int i = 0; i < sample_count; i++)
					ranges.push_back(scan[i].range);

				WebSim::GetLaserData(name, t, resolution, fov, p, ranges,
						format, response, xmlparent);

			} else if (type == "ranger") {

				std::vector<websim::Pose> p;
				std::vector<double> ranges;

				ModelRanger* ranger = (ModelRanger*) mod;

				if (ranger) {
					for (unsigned int i = 0; i < ranger->sensors.size(); i++) {
						//char str[10];
						//sprintf(str,"size:%d",ranger->sensors.size());
						//puts(str);
						//puts("in the ranger loop");
						websim::Pose pos;
						Pose rpos;
						rpos = ranger->sensors[i].pose;
						pos.x = rpos.x;
						pos.y = rpos.y;
						pos.z = rpos.z;
						pos.a = rpos.a;
						p.push_back(pos);

						ranges.push_back(ranger->sensors[i].range);
					}
				}

				WebSim::GetRangerData(name, t, p, ranges, format, response,
						xmlparent);

			} else if (type == "fiducial") {

				ModelFiducial::Fiducial* fids;
				unsigned int n = 0;
				std::vector<websim::Fiducial> f;

				ModelFiducial* fiducial = (ModelFiducial*) mod;

				fids = fiducial->GetFiducials(&n);

				for (unsigned int i = 0; i < n; i++) {
					websim::Fiducial fid;
					fid.pos.x = fids[i].range * cos(fids[i].bearing);
					fid.pos.y = fids[i].range * sin(fids[i].bearing);
					//fid.range = fids[i].range;
					//fid.bearing = fids[i].bearing;
					fid.id = fids[i].id;

					f.push_back(fid);
					//printf("stage: fiducials:%d\n",f.size());
				}

				WebSim::GetFiducialData(name, t, f, format, response,
						xmlparent);

			} else {

				//printf("Warning: Unkown model type\n");
				return false;

			}
		} else {
			printf("Warning: tried to get the data of unkown model:%s .\n",
					name.c_str());
			return false;
		}
		return true;
	}

	virtual bool SetModelPVA(const std::string& name, const websim::Pose& p,
			const websim::Velocity& v, const websim::Acceleration& a,
			std::string& error) {
		//printf( "set model PVA name:%s\n", name.c_str() );

		Model* mod = world->GetModel(name.c_str());
		if (mod) {
			mod->SetPose(Stg::Pose(p.x, p.y, p.z, p.a));
			mod->SetVelocity(Stg::Velocity(v.x, v.y, v.z, v.a));
			// stage doesn't model acceleration
		} else
			printf(
					"Warning: attempt to set PVA for unrecognized model \"%s\"\n",
					name.c_str());

		return true;
	}

	virtual bool GetModelPVA(const std::string& name, websim::Time& t,
			websim::Pose& p, websim::Velocity& v, websim::Acceleration& a,
			std::string& error) {
		//printf( "get model name:%s\n", name.c_str() );

		t = GetTime();

		Model* mod = world->GetModel(name.c_str());
		if (mod) {
			Stg::Pose sp = mod->GetPose();
			p.x = sp.x;
			p.y = sp.y;
			p.z = sp.z;
			p.a = sp.a;

			Stg::Velocity sv = mod->GetVelocity();
			v.x = sv.x;
			v.y = sv.y;
			v.z = sv.z;
			v.a = sv.a;
		} else
			printf(
					"Warning: attempt to set PVA for unrecognized model \"%s\"\n",
					name.c_str());

		return true;
	}
	virtual bool GetModelGeometry(const std::string& name, double& x, double& y,
			double& z, websim::Pose& center, std::string& response) {
		if (name == "sim") {

			stg_bounds3d_t ext = world->GetExtent();

			x = ext.x.max - ext.x.min;
			y = ext.y.max - ext.y.min;
			z = ext.z.max - ext.z.min;

		} else {
			Model* mod = world->GetModel(name.c_str());
			if (mod) {
				Geom ext = mod->GetGeom();

				x = ext.size.x;
				y = ext.size.y;
				z = ext.size.z;
				center.x = ext.pose.x;
				center.y = ext.pose.y;
				center.a = ext.pose.a;
			} else {
				printf(
						"Warning: attemp to get the extent of unrecognized model \"%s\"\n",
						name.c_str());
				return false;
			}
		}

		return true;
	}

	static int CountRobots(Model * mod, int* n) {

		if (n && mod->GetModelType() == "position")
			(*n)++;

		return 0;
	}

	virtual bool GetNumberOfRobots(unsigned int& n) {

		world->ForEachDescendant((stg_model_callback_t) CountRobots, &n);
		return true;

	}
	virtual bool GetSayStrings(std::vector<std::string>& sayings) {
		unsigned int n = 0;
		this->GetNumberOfRobots(n);

		for (unsigned int i = 0; i < n; i++) {
			char temp[128];
			sprintf(temp, "position:%d", i);
			Model *mod = world->GetModel(temp);
			if (mod->GetSayString() != "") {

				std::string str = temp;
				str += " says: \" ";
				str += mod->GetSayString();
				str += " \" ";

				sayings.push_back(str);

			}
		}

		return true;
	}

	virtual websim::Time GetTime() {
		stg_usec_t stgtime = world->SimTimeNow();

		websim::Time t;
		t.sec = stgtime / 1e6;
		t.usec = stgtime - (t.sec * 1e6);
		return t;
	}

	// add an FLTK event loop update to WebSim's implementation
	virtual void Wait() {
		do {
			event_loop (EVLOOP_NONBLOCK);
			//Fl::check();
		} while (unacknowledged_ticks || unacknowledged_pushes
				|| ticks_remaining);
	}

};
/* options descriptor */
static struct option longopts[] = { { "gui", no_argument, NULL, 'g' }, { "port",
		required_argument, NULL, 'p' },
		{ "host", required_argument, NULL, 'h' }, { "federation",
				required_argument, NULL, 'f' }, { NULL, 0, NULL, 0 } };
#endif

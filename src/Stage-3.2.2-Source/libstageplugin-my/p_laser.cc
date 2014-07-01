/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004, 2005 Richard Vaughan
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * Desc: A plugin driver for Player that gives access to Stage devices.
 * Author: Richard Vaughan
 * Date: 10 December 2004
 * CVS: $Id: p_laser.cc,v 1.2 2008-01-15 01:25:42 rtv Exp $
 */

// DOCUMENTATION ------------------------------------------------------------
/** @addtogroup player
 @par Laser interface
 - PLAYER_LASER_DATA_SCAN
 - PLAYER_LASER_REQ_SET_CONFIG
 - PLAYER_LASER_REQ_GET_CONFIG
 - PLAYER_LASER_REQ_GET_GEOM
 */

// CODE ----------------------------------------------------------------------
#include "p_driver.h"
using namespace Stg;
#include <string.h>
#include <math.h>

extern uint32_t hallscount;
extern uint32_t soundcount;

//extern OpaqueModel opaquem;

InterfaceLaser::InterfaceLaser(player_devaddr_t addr, StgDriver* driver,
		ConfigFile* cf, int section) :
		InterfaceModel(addr, driver, cf, section, "laser") {
	this->scan_id = 0;
	/*std::string laserType = "infroProxSensor";
	laserType = cf->ReadString(section, "lasertype", laserType.data());
	const char *p = laserType.data();
	if (strcmp(p, "infroProxSen") == 0)
		this->conf.macroName = infrProxSensor;
	else if (strcmp(p, "colliSen") == 0)
		this->conf.macroName = colliSensor;
	else if (strcmp(p, "soundSen") == 0)
		this->conf.macroName = soundSensor;
	else if (strcmp(p, "gestSen") == 0)
		this->conf.macroName = gestSensor;
	else if (strcmp(p, "hallSen") == 0)
		this->conf.macroName = hallSensor;
	else if (strcmp(p, "dout") == 0)
		this->conf.macroName = doutSensor;
	else if (strcmp(p, "infrDistSen") == 0)
		this->conf.macroName = infrDistSensor;
	else if (strcmp(p, "tempSen") == 0)
		this->conf.macroName = tempSensor;
	else if (strcmp(p, "graySen") == 0)
		this->conf.macroName = graySensor;
	else if (strcmp(p, "lightSen") == 0)
		this->conf.macroName = lightSensor;
	else if (strcmp(p, "RS422Sen") == 0)
		this->conf.macroName = RS422Sensor;
	else
		this->conf.macroName = DONTKNOW;
	int type = this->conf.macroName;
	if (type == infrProxSensor)
		this->conf.type = DIOIN;
	else if (type == colliSensor)
		this->conf.type = DIOIN;
	else if (type == soundSensor)
		this->conf.type = DIOIN;
	else if (type == gestSensor)
		this->conf.type = DIOIN;
	else if (type == hallSensor)
		this->conf.type = DIOIN;
	else if (type == doutSensor)
		this->conf.type = DIOOUT;
	else if (type == infrDistSensor)
		this->conf.type = AIO;
	else if (type == tempSensor)
		this->conf.type = AIO;
	else if (type == graySensor)
		this->conf.type = AIO;
	else if (type == lightSensor)
		this->conf.type = AIO;
	else if (type == RS422Sensor)
		this->conf.type = RS422;
	else
		this->conf.type = DONTKNOW;*/
	//opaquem.laserModel = (Stg::ModelLaser*) this->mod;
	/*memset(&this->opaque_addr, 0, sizeof(player_devaddr_t));
	 cf->ReadDeviceAddr(&this->opaque_addr, section, "requires",
	 PLAYER_OPAQUE_CODE, -1, NULL);*/
}

void InterfaceLaser::Publish(void) {
	ModelLaser* mod = (ModelLaser*) this->mod;
	ModelLaser::Sample* samples = mod->GetSamples(NULL);
	ModelLaser::CYZXLaser* cyzxlc= mod->GetCYZXLaserConf(NULL);

	// don't publish anything until we have some real data
	if (samples == NULL || cyzxlc == NULL)
		return;

	player_laser_data_t pdata;
	memset(&pdata, 0, sizeof(pdata));

	ModelLaser::Config cfg = mod->GetConfig();
	pdata.min_angle = -cfg.fov / 2.0;
	pdata.max_angle = +cfg.fov / 2.0;
	//pdata.resolution = cfg.fov / cfg.sample_count;
	pdata.resolution = cfg.fov / cfg.cyzxlaserc_count;
	pdata.max_range = cfg.range_bounds.max;
	//pdata.ranges_count = pdata.intensity_count = cfg.sample_count;
	pdata.ranges_count = pdata.intensity_count = cfg.cyzxlaserc_count;
	pdata.id = this->scan_id++;

	pdata.ranges = new float[pdata.ranges_count];
	pdata.intensity = new uint8_t[pdata.ranges_count];

	/*for (unsigned int i = 0; i < cfg.sample_count; i++) {
		//printf( "range %d %d\n", i, samples[i].range);
		pdata.ranges[i] = samples[i].range;
		pdata.intensity[i] = (uint8_t) samples[i].reflectance;
	}*/
	for(unsigned int i=0;i<cfg.cyzxlaserc_count;i++){
		//if port have not laser connect with
		if(cyzxlc[i].type == ""){
			pdata.ranges[i] = 0;
			pdata.intensity[i] = 0;
			continue;
		}
		stg_meters_t stg1=cyzxlc[i].range_max;
		double d1;
		for(unsigned int j=cyzxlc[i].angle_from;j<cyzxlc[i].angle_to;j++){
			if(samples[j].range < stg1){
				stg1=samples[j].range;
				d1= samples[i].reflectance;
			}
		}
		pdata.ranges[i] = stg1;
		pdata.intensity[i] = (uint8_t) d1;

		if(strcmp(cyzxlc[i].type.data(),"infrProxSen")==0){
			if(pdata.ranges[i] <= cyzxlc[i].range_max && pdata.ranges[i] >= cyzxlc[i].range_min){
				pdata.ranges[i]=1;
			}else{
				pdata.ranges[i] = 0;
			}
		}else if(strcmp(cyzxlc[i].type.data(),"colliSen")==0){
			if(pdata.ranges[i] <= cyzxlc[i].range_max && pdata.ranges[i] >= cyzxlc[i].range_min){
				pdata.ranges[i]=1;
			}else{
				pdata.ranges[i] = 0;
			}

		}else if(strcmp(cyzxlc[i].type.data(),"hallSen")==0){
			if(hallscount > 0) pdata.ranges[i]=1;
			else pdata.ranges[i]=0;
		}else if (strcmp(cyzxlc[i].type.data(),"soundSen")==0 ) {
			if(soundcount > 0) pdata.ranges[i]=1;
			else pdata.ranges[i]=0;
		} else if (strcmp(cyzxlc[i].type.data(),"gestSen")==0 ) {

		} else if (strcmp(cyzxlc[i].type.data(),"dout")==0 ) {

		} else if (strcmp(cyzxlc[i].type.data(),"infrDistSen")==0 ) {

		} else if (strcmp(cyzxlc[i].type.data(),"tempSen")==0 ) {
			ModelLaser::FixModel temp=mod->GetTempModelPos();
			Model* pos=mod->Parent();
			if(pos!=NULL){
				Stg::Pose sp = pos->GetPose();
				pdata.ranges[i]=sqrt(pow(sp.x-temp.x,2)+pow(sp.y-temp.y,2));
			}
			else{
				pdata.ranges[i]=0;
			}
		} else if (strcmp(cyzxlc[i].type.data(),"graySen")==0 ) {

		} else if (strcmp(cyzxlc[i].type.data(),"lightSen")==0 ) {

		} else if (strcmp(cyzxlc[i].type.data(),"RS422Sen")==0 ) {

		}
	}

	/*
	 #define  infrProxSen 0 //#DIO：红外接近传感器端口号
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
	 */
	//0 -no data , 1-data

	// Write laser data
	this->driver->Publish(this->addr, PLAYER_MSGTYPE_DATA,
			PLAYER_LASER_DATA_SCAN, (void*) &pdata, sizeof(pdata), NULL);

	delete[] pdata.ranges;
	delete[] pdata.intensity;
}

int InterfaceLaser::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	ModelPosition* modpos2d = (ModelPosition*) this->mod;
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
			PLAYER_POSITION2D_DATA_STATE, this->require_addr)) {
		printf("--run here in Laser\n");
		if (this->conf.type == FPOS2D8BUTTON) {
			player_position2d_data_t_redress* pcmd =
					(player_position2d_data_t_redress*) data;
			modpos2d->GoTo(pcmd->pos2d.pos.px, pcmd->pos2d.pos.py,
					pcmd->pos2d.pos.pa - pcmd->redressAngle);
			return 0;
		}
		if (this->conf.type == FPOS2DOMNIDIR) {
			player_position2d_data_t* pcmd = (player_position2d_data_t*) data;
			modpos2d->GoTo(pcmd->pos.px, pcmd->pos.py, pcmd->pos.pa);
			return 0;
		}
		if (this->conf.type == FPOS2D4TIRES) {
			player_position2d_data_t* pcmd = (player_position2d_data_t*) data;
			modpos2d->GoTo(pcmd->pos.px, pcmd->pos.py, pcmd->pos.pa);
			return 0;
		}
	}

	ModelLaser* mod = (ModelLaser*) this->mod;

	// Is it a request to set the laser's config?
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_SET_CONFIG, this->addr)) {


		player_laser_config_t* plc = (player_laser_config_t*) data;

		if (hdr->size == sizeof(player_laser_config_t)) {
			// TODO
			// int intensity = plc->intensity;

			ModelLaser::Config cfg = mod->GetConfig();

			PRINT_DEBUG3( "laser config was: resolution %d, fov %.6f, interval %d\n",
					cfg.resolution, cfg.fov, cfg.interval );

			cfg.fov = plc->max_angle - plc->min_angle;
			cfg.resolution = (uint32_t) (cfg.fov
					/ (cfg.sample_count * plc->resolution));
			if (cfg.resolution < 1)
				cfg.resolution = 1;
			cfg.interval = (stg_usec_t) (1.0E6 / plc->scanning_frequency);

			PRINT_DEBUG3( "setting laser config: resolution %d, fov %.6f, interval %d\n",
					cfg.resolution, cfg.fov, cfg.interval );

			// Range resolution is currently locked to the world setting
			//  and intensity values are always read.  The relevant settings
			//  are ignored.

			mod->SetConfig(cfg);

			this->driver->Publish(this->addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_LASER_REQ_SET_CONFIG);
			return (0);
		} else {
			PRINT_ERR2("config request len is invalid (%d != %d)",
					(int)hdr->size, (int)sizeof(player_laser_config_t));

			return (-1);
		}
	}
	// Is it a request to get the laser's config?
	else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_CONFIG, this->addr)) {
		if (hdr->size == 0) {
			ModelLaser::Config cfg = mod->GetConfig();

			player_laser_config_t plc;
			memset(&plc, 0, sizeof(plc));
			plc.min_angle = -cfg.fov / 2.0;
			plc.max_angle = +cfg.fov / 2.0;
			plc.resolution = cfg.fov / (cfg.sample_count * cfg.resolution);
			plc.max_range = cfg.range_bounds.max;
			plc.range_res = 1.0; // todo
			plc.intensity = 1; // todo
			plc.scanning_frequency = 1.0E6 / cfg.interval;

			this->driver->Publish(this->addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_LASER_REQ_GET_CONFIG,
					(void*) &plc, sizeof(plc), NULL);
			return (0);
		} else {
			PRINT_ERR2("config request len is invalid (%d != %d)",
					(int)hdr->size, 0);
			return (-1);
		}
	}
	// Is it a request to get the laser's geom?
	else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_LASER_REQ_GET_GEOM, this->addr)) {
		if (hdr->size == 0) {
			Geom geom = this->mod->GetGeom();

			Pose pose = this->mod->GetPose();

			// fill in the geometry data formatted player-like
			player_laser_geom_t pgeom;
			pgeom.pose.px = pose.x;
			pgeom.pose.py = pose.y;
			pgeom.pose.pyaw = pose.a;
			pgeom.size.sl = geom.size.x;
			pgeom.size.sw = geom.size.y;

			this->driver->Publish(this->addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_LASER_REQ_GET_GEOM,
					(void*) &pgeom, sizeof(pgeom), NULL);
			return (0);
		} else {
			PRINT_ERR2("config request len is invalid (%d != %d)",
					(int)hdr->size, 0);
			return (-1);
		}
	}

	// Don't know how to handle this message.
	PRINT_WARN2( "stage laser doesn't support message %d:%d.", hdr->type,
			hdr->subtype);
	return (-1);
}

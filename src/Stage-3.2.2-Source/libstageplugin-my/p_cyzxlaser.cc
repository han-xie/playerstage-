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
 * CVS: $Id: p_cyzxlaser.cc,v 1.2 2008-01-15 01:25:42 rtv Exp $
 */

// DOCUMENTATION ------------------------------------------------------------
/** @addtogroup player
 @par cyzxLaser interface
 - PLAYER_CYZXLASER_DATA_SCAN
 - PLAYER_CYZXLASER_REQ_SET_CONFIG
 - PLAYER_CYZXLASER_REQ_GET_CONFIG
 - PLAYER_CYZXLASER_REQ_GET_GEOM
 */

// CODE ----------------------------------------------------------------------
#include "p_driver.h"
using namespace Stg;
#include <string.h>
#include <math.h>

extern bool PXAupdate[PXA270PORTS];
extern uint32_t PXAcount[PXA270PORTS];
extern float PXAvalue[PXA270PORTS];

//extern OpaqueModel opaquem;
struct myOpaqueHead {
	uint32_t type;
	uint32_t subtype;
};

InterfacecyzxLaser::InterfacecyzxLaser(player_devaddr_t addr, StgDriver* driver,
		ConfigFile* cf, int section) :
		InterfaceModel(addr, driver, cf, section, "cyzxlaser") {
	this->scan_id = 0;
}

void InterfacecyzxLaser::Publish(void) {
	ModelcyzxLaser* mod = (ModelcyzxLaser*) this->mod;

	player_cyzxlaser_data_t pdata;
	memset(&pdata, 0, sizeof(pdata));

	ModelcyzxLaser::Config cfg = mod->GetConfig();

	pdata.min_angle = -cfg.fov / 2.0;
	pdata.max_angle = +cfg.fov / 2.0;
	pdata.resolution = cfg.fov / cfg.sample_count;
	pdata.max_range = cfg.range_bounds.max;
	pdata.ranges_count = pdata.intensity_count = cfg.sample_count = PXA270PORTS;
	pdata.id = this->scan_id++;

	pdata.ranges = new float[pdata.ranges_count];
	pdata.intensity = new uint8_t[pdata.ranges_count];

	for (unsigned int i = 0; i < cfg.sample_count; i++) {
		pdata.ranges[i] = PXAvalue[i];
	}

	/*ModelcyzxLaser::FixModel temp = mod->GetTempModelPos();
	 Model* pos = mod->Parent();
	 if (pos != NULL) {
	 Stg::Pose sp = pos->GetPose();
	 pdata.ranges[i] = sqrt(
	 pow(sp.x - temp.x, 2) + pow(sp.y - temp.y, 2));
	 } else {
	 pdata.ranges[i] = maxAioValue;
	 }*/

	this->driver->Publish(this->addr, PLAYER_MSGTYPE_DATA,
			PLAYER_CYZXLASER_DATA_SCAN, (void*) &pdata, sizeof(pdata), NULL);

	delete[] pdata.ranges;
	delete[] pdata.intensity;
}

int InterfacecyzxLaser::ProcessMessage(QueuePointer & resp_queue,
		player_msghdr_t* hdr, void* data) {
	ModelPosition* modpos2d = (ModelPosition*) this->mod;
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
			PLAYER_POSITION2D_DATA_STATE, this->require_addr)) {
		printf("--run here in cyzxLaser\n");
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

	ModelcyzxLaser* mod = (ModelcyzxLaser*) this->mod;

	// Is it a request to set the cyzxlaser's config?
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_SET_CONFIG, this->addr)) {

		player_cyzxlaser_config_t* plc = (player_cyzxlaser_config_t*) data;

		if (hdr->size == sizeof(player_cyzxlaser_config_t)) {
			// TODO
			// int intensity = plc->intensity;

			ModelcyzxLaser::Config cfg = mod->GetConfig();

			PRINT_DEBUG3( "cyzxlaser config was: resolution %d, fov %.6f, interval %d\n",
					cfg.resolution, cfg.fov, cfg.interval );

			cfg.fov = plc->max_angle - plc->min_angle;
			cfg.resolution = (uint32_t) (cfg.fov
					/ (cfg.sample_count * plc->resolution));
			if (cfg.resolution < 1)
				cfg.resolution = 1;
			cfg.interval = (stg_usec_t) (1.0E6 / plc->scanning_frequency);

			PRINT_DEBUG3( "setting cyzxlaser config: resolution %d, fov %.6f, interval %d\n",
					cfg.resolution, cfg.fov, cfg.interval );

			// Range resolution is currently locked to the world setting
			//  and intensity values are always read.  The relevant settings
			//  are ignored.

			mod->SetConfig(cfg);

			this->driver->Publish(this->addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_CYZXLASER_REQ_SET_CONFIG);
			return (0);
		} else {
			PRINT_ERR2("config request len is invalid (%d != %d)",
					(int)hdr->size, (int)sizeof(player_cyzxlaser_config_t));

			return (-1);
		}
	}
	// Is it a request to get the cyzxlaser's config?
	else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_GET_CONFIG, this->addr)) {
		if (hdr->size == 0) {
			ModelcyzxLaser::Config cfg = mod->GetConfig();

			player_cyzxlaser_config_t plc;
			memset(&plc, 0, sizeof(plc));
			plc.min_angle = -cfg.fov / 2.0;
			plc.max_angle = +cfg.fov / 2.0;
			plc.resolution = cfg.fov / (cfg.sample_count * cfg.resolution);
			plc.max_range = cfg.range_bounds.max;
			plc.range_res = 1.0; // todo
			plc.intensity = 1; // todo
			plc.scanning_frequency = 1.0E6 / cfg.interval;

			this->driver->Publish(this->addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_CYZXLASER_REQ_GET_CONFIG,
					(void*) &plc, sizeof(plc), NULL);
			return (0);
		} else {
			PRINT_ERR2("config request len is invalid (%d != %d)",
					(int)hdr->size, 0);
			return (-1);
		}
	}
	// Is it a request to get the cyzxlaser's geom?
	else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_GET_GEOM, this->addr)) {
		if (hdr->size == 0) {
			Geom geom = this->mod->GetGeom();

			Pose pose = this->mod->GetPose();

			// fill in the geometry data formatted player-like
			player_cyzxlaser_geom_t pgeom;
			pgeom.pose.px = pose.x;
			pgeom.pose.py = pose.y;
			pgeom.pose.pyaw = pose.a;
			pgeom.size.sl = geom.size.x;
			pgeom.size.sw = geom.size.y;

			this->driver->Publish(this->addr, resp_queue,
					PLAYER_MSGTYPE_RESP_ACK, PLAYER_CYZXLASER_REQ_GET_GEOM,
					(void*) &pgeom, sizeof(pgeom), NULL);
			return (0);
		} else {
			PRINT_ERR2("config request len is invalid (%d != %d)",
					(int)hdr->size, 0);
			return (-1);
		}
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_CYZXLASER_CMD_SETWEIBO, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;
		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		//dealWifiSetweiboCMD(ip);
		mod->lcdswitch = 1;
		mod->lcdcount++;
		std::string display = "connect to ";
		std::string stemp = (char *) ip;
		display += stemp + " content : hello";
		mod->lcdcontent = display;
		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_CYZXLASER_CMD_SETLCD, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;

		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		int length = opaquedata->data_count - (8 + iplength) + 1;
		uint8_t *display = new uint8_t[length];
		to = display;
		for (int i = 0; i < length; i++) {
			*to = *from;
			from++;
			to++;
		}
		*to = 0x0;
		//dealWifiSetLCDCMD(ip, display);
		mod->lcdswitch = 1;
		mod->lcdcount++;
		/*std::string stemp;
		std::string sdisplay = "connect to ";
		stemp = (char*) ip;
		sdisplay += stemp + " content : ";
		stemp = (char*) display;
		sdisplay += stemp;
		mod->lcdcontent = sdisplay;*/
		std::string stemp;
		stemp = (char*) display;
		mod->lcdcontent = stemp;

		delete[] display;

		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
			PLAYER_CYZXLASER_CMD_SETSOUND, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;
		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		int length = opaquedata->data_count - (8 + iplength) + 1;
		uint8_t *type = new uint8_t[length];
		to = type;
		for (int i = 0; i < length; i++) {
			*to = *from;
			from++;
			to++;
		}
		*to = 0x0;
		//dealWifiSetSoundCMD(ip, type);
		mod->soundswitch = 1;
		mod->soundcount++;
		std::string sound = "connect to ";
		std::string stemp = (char *) ip;
		sound += stemp + " content : ";
		stemp = (char *) type;
		sound += stemp;
		mod->soundcontent = sound;

		delete[] type;

		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_CYZXLASER_REQ_GETWEIBO, this->addr)) {
		player_cyzxlaser_opaque_data_t *opaquedata =
				(player_cyzxlaser_opaque_data_t *) data;
		myOpaqueHead head;
		uint8_t *from, *to;
		from = (uint8_t*) opaquedata->data;
		to = (uint8_t*) (&head.type);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}
		to = (uint8_t*) (&head.subtype);
		for (int i = 0; i < 4; i++) {
			*to = *from;
			to++;
			from++;
		}

		//player_opaque_data_t opaqueRes;
		int iplength = 30;

		uint8_t ip[iplength];
		for (int i = 0; i < iplength; i++) {
			ip[i] = *from;
			from++;
		}
		ip[iplength] = 0x0;
		int length = opaquedata->data_count - (8 + iplength) + 1;
		uint8_t *cmd = new uint8_t[length];
		to = cmd;
		for (int i = 0; i < length; i++) {
			*to = *from;
			from++;
			to++;
		}
		*to = 0x0;
		//dealWifiGetWeibo(ip, cmd);

		/*int test=1;

		this->driver->Publish(this->addr, resp_queue, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_CYZXLASER_REQ_GETWEIBO, (void*) &test,
				sizeof(test), NULL);
       */

		player_cyzxlaser_opaque_data_t opaqueRes;
		std::string result = "weibo";
		opaqueRes.data_count = result.size();
		opaqueRes.data=(uint8_t *)result.c_str();
		this->driver->Publish(this->Interface::addr, resp_queue,PLAYER_MSGTYPE_RESP_ACK,
					PLAYER_CYZXLASER_REQ_GETWEIBO, (void *)&opaqueRes, sizeof(opaqueRes), NULL);

		delete[] cmd;

		return 0;
	}

	// Don't know how to handle this message.
	PRINT_WARN2( "stage cyzxlaser doesn't support message %d:%d.", hdr->type,
			hdr->subtype);
	return (-1);
}

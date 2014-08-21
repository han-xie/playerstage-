/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000-2003
 *     Brian Gerkey, Kasper Stoy, Richard Vaughan, & Andrew Howard
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
/********************************************************************
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ********************************************************************/

/*
 * $Id: cyzxlaserproxy.cc 4238 2007-11-01 23:55:17Z gerkey $
 */

#include "playerc++.h"
#include <cstring>
#include <cstdio>
#include <cmath>
#include <climits>

using namespace PlayerCc;


cyzxLaserProxy::cyzxLaserProxy(PlayerClient *aPc, uint32_t aIndex)
  : ClientProxy(aPc, aIndex),
  mDevice(NULL)
{
  Subscribe(aIndex);
  // how can I get this into the clientproxy.cc?
  // right now, we're dependent on knowing its device type
  mInfo = &(mDevice->info);
}

cyzxLaserProxy::~cyzxLaserProxy()
{
  Unsubscribe();
}

void
cyzxLaserProxy::Subscribe(uint32_t aIndex)
{
  scoped_lock_t lock(mPc->mMutex);
  mDevice = playerc_cyzxlaser_create(mClient, aIndex);
  if (NULL==mDevice)
    throw PlayerError("cyzxLaserProxy::cyzxLaserProxy()", "could not create");

  if (0 != playerc_cyzxlaser_subscribe(mDevice, PLAYER_OPEN_MODE))
    throw PlayerError("cyzxLaserProxy::cyzxLaserProxy()", "could not subscribe");
}

void
cyzxLaserProxy::Unsubscribe()
{
  assert(NULL!=mDevice);
  scoped_lock_t lock(mPc->mMutex);
  playerc_cyzxlaser_unsubscribe(mDevice);
  playerc_cyzxlaser_destroy(mDevice);
  mDevice = NULL;
}

void
cyzxLaserProxy::Configure(double min_angle,
                      double max_angle,
                      uint32_t scan_res,
                      uint32_t range_res,
                      bool intensity,
                      double aScanningFrequency)
{
  scoped_lock_t lock(mPc->mMutex);
  if (0 != playerc_cyzxlaser_set_config(mDevice, min_angle, max_angle,
                                    scan_res, range_res, intensity?1:0,aScanningFrequency))
    throw PlayerError("cyzxLaserProxy::Configure()", "error setting config");
}

void
cyzxLaserProxy::RequestConfigure()
{
  scoped_lock_t lock(mPc->mMutex);
  unsigned char temp_int;
  if (0 != playerc_cyzxlaser_get_config(mDevice, &min_angle, &max_angle,
                                     &scan_res, &range_res, &temp_int, &scanning_frequency))
    throw PlayerError("cyzxLaserProxy::RequestConfigure()", "error getting config");
  intensity = temp_int == 0 ? false : true;

  return;
}

void
cyzxLaserProxy::RequestID()
{
  scoped_lock_t lock(mPc->mMutex);
  if (0 != playerc_cyzxlaser_get_id(mDevice))
    throw PlayerError("cyzxLaserProxy::RequestConfigure()", "error getting id");

  return;
}

void
cyzxLaserProxy::RequestGeom()
{
  boost::mutex::scoped_lock lock(mPc->mMutex);
  if (0 != playerc_cyzxlaser_get_geom(mDevice))
    throw PlayerError("cyzxLaserProxy::RequestGeom()", "error getting geom");
  return;
}

std::ostream&
std::operator << (std::ostream &os, const PlayerCc::cyzxLaserProxy &c)
{
  os << "#min\tmax\tres\tcount\trange_res" << std::endl;
  os << RTOD(c.GetMinAngle()) << "\t"
     << RTOD(c.GetMaxAngle()) << "\t"
     << RTOD(c.GetScanRes()) << "\t"
     << c.GetCount() << "\t"
     << c.GetRangeRes() << std::endl;

  os << "#range\tbearing\tintensity" << std::endl;

  for(unsigned int i=0;i<c.GetCount();i++)
    os << c.GetRange(i) << "\t"
       << RTOD(c.GetBearing(i)) << "\t"
       << c.GetIntensity(i) << "\t";
  
  return os;
}


void cyzxLaserProxy::WifiSetWeibo(char *ip){
		player_cyzxlaser_opaque_data_t popa;
		uint32_t wifiID = 7;
		uint32_t subtype = 1;
		uint8_t temp[50];
		int iplength=30;
		for(int i=0;i<50;i++)
			temp[i]='\0';
		uint8_t *from,*to;
		to=temp;

		from=(uint8_t*)&wifiID;
		for(int i=0;i<4;i++){
			*to=*from;
			from++;
			to++;
		}
		from=(uint8_t*)&subtype;
		for(int i=4;i<8;i++){
			*to=*from;
			from++;
			to++;
		}
		if(ip!=NULL){
			from=(uint8_t*)ip;
			for(int i=0;i<iplength&&(*from)!='\0';i++){
				*to=*from;
				from++;
				to++;
			}
		}
		to=&temp[8+iplength];
		popa.data_count = 8+iplength;
		popa.data = (uint8_t *) &temp[0];
		playerc_cyzxlaser_WifiSetWeibo(mDevice,&popa);
		return;
}
void cyzxLaserProxy::WifiSetLCD(char *display,char *ip){
	player_cyzxlaser_opaque_data_t popa;
	uint32_t wifiID = 7;
	uint32_t subtype = 2;
	uint8_t temp[1024];
	int iplength=30;
	for(int i=0;i<1024;i++)
		temp[i]='\0';
	uint8_t *from,*to;
	to=temp;

	from=(uint8_t*)&wifiID;
	for(int i=0;i<4;i++){
		*to=*from;
		from++;
		to++;
	}
	from=(uint8_t*)&subtype;
	for(int i=4;i<8;i++){
		*to=*from;
		from++;
		to++;
	}
	if(ip!=NULL){
		from=(uint8_t*)ip;
		for(int i=0;i<iplength&&(*from)!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
	}/*else{
		for(int i=0;i<iplength;i++){
			*to=' ';
			to++;
		}
	}*/
	to=&temp[8+iplength];
	popa.data_count = 8+iplength;
	if(display){
		int i=0;
		from=(uint8_t*)display;
		for(i=0;i<900&&*from!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
		popa.data_count+=i;
	}
	popa.data = (uint8_t *) &temp[0];
	playerc_cyzxlaser_WifiSetLCD(mDevice,&popa);
}
void cyzxLaserProxy::WifiSetSound(char *type,char *ip){
	player_cyzxlaser_opaque_data_t popa;
		uint32_t wifiID = 7;
		uint32_t subtype = 3;
		int iplength=30;
		uint8_t temp[1024];
		for(int i=0;i<1024;i++)
			temp[i]='\0';
		uint8_t *from,*to;
		to=temp;

		from=(uint8_t*)&wifiID;
		for(int i=0;i<4;i++){
			*to=*from;
			from++;
			to++;
		}
		from=(uint8_t*)&subtype;
		for(int i=4;i<8;i++){
			*to=*from;
			from++;
			to++;
		}
		if(ip!=NULL){
			from=(uint8_t*)ip;
			for(int i=0;i<iplength&&*from!='\0';i++){
				*to=*from;
				from++;
				to++;
			}
		}
		to=&temp[8+iplength];
		popa.data_count = 8+iplength;
		if(type){
			int i=0;
			from=(uint8_t*)type;
			for(i=0;i<900&&*from!='\0';i++){
				*to=*from;
				from++;
				to++;
			}
			popa.data_count+=i;
		}
		popa.data = (uint8_t *) &temp[0];
		playerc_cyzxlaser_WifiSetSound(mDevice,&popa);

}
int  cyzxLaserProxy::WifiGetWeibo(std::string &content,
		char *cmd,char *ip){

	player_cyzxlaser_opaque_data_t popa;
	uint32_t wifiID = 7;
	uint32_t subtype = 4;
	int iplength=30;
	uint8_t temp[4096];
	for(int i=0;i<1024;i++)
		temp[i]='\0';
	uint8_t *from,*to;
	to=temp;

	from=(uint8_t*)&wifiID;
	for(int i=0;i<4;i++){
		*to=*from;
		from++;
		to++;
	}
	from=(uint8_t*)&subtype;
	for(int i=4;i<8;i++){
		*to=*from;
		from++;
		to++;
	}
	if(ip!=NULL){
		from=(uint8_t*)ip;
		for(int i=0;i<iplength&&*from!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
	}
	to=&temp[8+iplength];
	popa.data_count = 8+iplength;
	if(cmd){
		int i=0;
		from=(uint8_t*)cmd;
		for(i=0;i<900&&(*from!='\0');i++){
			*to=*from;
			from++;
			to++;
		}
		popa.data_count+=i;
	}
	popa.data = (uint8_t *) &temp[0];
	char rtemp[10240];
	//tempSizeForData((uint8_t*)rtemp);
	//this->opaquep->newSizeFormDevice(1024);
	player_cyzxlaser_opaque_data_t *aReply=new player_cyzxlaser_opaque_data_t();
	aReply->data=(uint8_t*)rtemp;
	//std::string *aReply;
	if(0 !=playerc_cyzxlaser_WifiGetWeibo(mDevice,&popa,aReply)){
		return -1;
	}
		/*player_cyzxlaser_opaque_data_t *aReply;
		if(0 !=playerc_cyzxlaser_WifiGetWeibo(mDevice,&popa,aReply)){
			return -1;
		}*/

	//memcpy(mDevice->data, aReply->data, aReply->data_count);
	//mDevice->data_count = aReply->data_count;
	content = (char*)aReply->data;

	//player_opaque_data_t_free(aReply);
	return 1;

}



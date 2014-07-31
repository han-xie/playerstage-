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



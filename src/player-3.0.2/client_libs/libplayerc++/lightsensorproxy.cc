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
 * $Id: lightsensorproxy.cc 4227 2007-10-24 22:32:04Z thjc $
 *
 * client-side lightsensor device
 */

#include "playerc++.h"

using namespace PlayerCc;

LightsensorProxy::LightsensorProxy(PlayerClient *aPc, uint32_t aIndex) :
		ClientProxy(aPc, aIndex), mDevice(NULL) {
	Subscribe(aIndex);
	// how can I get this into the clientproxy.cc?
	// right now, we're dependent on knowing its device type
	mInfo = &(mDevice->info);
}

LightsensorProxy::~LightsensorProxy() {
	Unsubscribe();
}

void LightsensorProxy::Subscribe(uint32_t aIndex) {
	scoped_lock_t lock(mPc->mMutex);
	mDevice = playerc_lightsensor_create(mClient, aIndex);
	if (NULL == mDevice)
		throw PlayerError("LightsensorProxy::LightsensorProxy()",
				"could not create");

	if (0 != playerc_lightsensor_subscribe(mDevice, PLAYER_OPEN_MODE))
		throw PlayerError("LightsensorProxy::LightsensorProxy()",
				"could not subscribe");
}

void LightsensorProxy::Unsubscribe() {
	assert(NULL != mDevice);
	scoped_lock_t lock(mPc->mMutex);
	playerc_lightsensor_unsubscribe(mDevice);
	playerc_lightsensor_destroy(mDevice);
	mDevice = NULL;
}

std::ostream&
std::operator <<(std::ostream &os, const PlayerCc::LightsensorProxy &c) {
	os << "#Lightsensor(" << c.GetInterface() << ":" << c.GetIndex() << ")"
			<< std::endl;
	os << "Count:" << c.GetCount() << std::endl;
	for (unsigned int i = 0; i < c.GetCount(); i++) {
		os << "  light " << i << ":" << std::endl;
		os << "               id: " << c.GetLight(i).id << std::endl;
		os << "             area: " << c.GetLight(i).area << std::endl;
		os << "                X: " << c.GetLight(i).x << std::endl;
		os << "                Y: " << c.GetLight(i).y << std::endl;
		os << "             Left: " << c.GetLight(i).left << std::endl;
		os << "            Right: " << c.GetLight(i).right << std::endl;
		os << "              Top: " << c.GetLight(i).top << std::endl;
		os << "           Bottom: " << c.GetLight(i).bottom << std::endl;
	}
	return os;
}

void LightsensorProxy::setInt(int value) {
	scoped_lock_t lock(mPc->mMutex);
	playerc_lightsensor_set_int_cmd(mDevice, value);
}

int LightsensorProxy::getInt() {
	boost::mutex::scoped_lock lock(mPc->mMutex);

	if (0 !=playerc_lightsensor_get_int_lights_count(mDevice))
		throw PlayerError("LightsensorProxy::RequestInt()", "error getting int");
	return 0;
}

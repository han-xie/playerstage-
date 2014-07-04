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
 * $Id: hallsensorproxy.cc 4227 2007-10-24 22:32:04Z thjc $
 *
 * client-side hallsensor device
 */

#include "playerc++.h"

using namespace PlayerCc;

HallsensorProxy::HallsensorProxy(PlayerClient *aPc, uint32_t aIndex) :
		ClientProxy(aPc, aIndex), mDevice(NULL) {
	Subscribe(aIndex);
	// how can I get this into the clientproxy.cc?
	// right now, we're dependent on knowing its device type
	mInfo = &(mDevice->info);
}

HallsensorProxy::~HallsensorProxy() {
	Unsubscribe();
}

void HallsensorProxy::Subscribe(uint32_t aIndex) {
	scoped_lock_t lock(mPc->mMutex);
	mDevice = playerc_hallsensor_create(mClient, aIndex);
	if (NULL == mDevice)
		throw PlayerError("HallsensorProxy::HallsensorProxy()",
				"could not create");

	if (0 != playerc_hallsensor_subscribe(mDevice, PLAYER_OPEN_MODE))
		throw PlayerError("HallsensorProxy::HallsensorProxy()",
				"could not subscribe");
}

void HallsensorProxy::Unsubscribe() {
	assert(NULL != mDevice);
	scoped_lock_t lock(mPc->mMutex);
	playerc_hallsensor_unsubscribe(mDevice);
	playerc_hallsensor_destroy(mDevice);
	mDevice = NULL;
}

std::ostream&
std::operator <<(std::ostream &os, const PlayerCc::HallsensorProxy &c) {
	os << "#Hallsensor(" << c.GetInterface() << ":" << c.GetIndex() << ")"
			<< std::endl;
	os << "Count:" << c.GetCount() << std::endl;
	for (unsigned int i = 0; i < c.GetCount(); i++) {
		os << "  hall " << i << ":" << std::endl;
		os << "               id: " << c.GetHall(i).id << std::endl;
		os << "             area: " << c.GetHall(i).area << std::endl;
		os << "                X: " << c.GetHall(i).x << std::endl;
		os << "                Y: " << c.GetHall(i).y << std::endl;
		os << "             Left: " << c.GetHall(i).left << std::endl;
		os << "            Right: " << c.GetHall(i).right << std::endl;
		os << "              Top: " << c.GetHall(i).top << std::endl;
		os << "           Bottom: " << c.GetHall(i).bottom << std::endl;
	}
	return os;
}

void HallsensorProxy::setInt(int value) {
	scoped_lock_t lock(mPc->mMutex);
	playerc_hallsensor_set_int_cmd(mDevice, value);
}

int HallsensorProxy::getInt() {
	boost::mutex::scoped_lock lock(mPc->mMutex);

	if (0 !=playerc_hallsensor_get_int_halls_count(mDevice))
		throw PlayerError("HallsensorProxy::RequestInt()", "error getting int");
	return 0;
}

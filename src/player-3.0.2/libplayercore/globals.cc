/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2000  Brian Gerkey   &  Kasper Stoy
 *                      gerkey@usc.edu    kaspers@robotics.usc.edu
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

#include <config.h>

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <libplayercore/globals.h>
#include <libplayercore/devicetable.h>
#include <libplayercore/drivertable.h>
#include <libplayercore/filewatcher.h>
#include <libplayercore/playertime.h>
#include <libplayercore/wallclocktime.h>

#if HAVE_PLAYERSD
  #include <libplayersd/playersd.h>
#endif

// this table holds all the currently *instantiated* devices
PLAYERCORE_EXPORT DeviceTable* deviceTable;

// this table holds all the currently *available* drivers
PLAYERCORE_EXPORT DriverTable* driverTable;

// the global PlayerTime object has a method
//   int GetTime(struct timeval*)
// which everyone must use to get the current time
PLAYERCORE_EXPORT PlayerTime* GlobalTime;

// global class for watching for changes in files and sockets
PLAYERCORE_EXPORT FileWatcher* fileWatcher;

PLAYERCORE_EXPORT char playerversion[32];

PLAYERCORE_EXPORT bool player_quit;
PLAYERCORE_EXPORT bool player_quiet_startup;
PLAYERCORE_EXPORT bool usegui;
PLAYERCORE_EXPORT bool wlocalhost;
PLAYERCORE_EXPORT int http;

//below global var use to control some hidden model
PLAYERCORE_EXPORT bool hallupdate;//霍尔接近传感器端口号
PLAYERCORE_EXPORT uint32_t hallscount;
PLAYERCORE_EXPORT bool grayupdate;//灰度传感器端口号
PLAYERCORE_EXPORT uint32_t grayscount;
PLAYERCORE_EXPORT bool soundupdate;//声音传感器端口号
PLAYERCORE_EXPORT uint32_t soundcount;
PLAYERCORE_EXPORT bool gestupdate;//姿态传感器端口号
PLAYERCORE_EXPORT uint32_t gestcount;
PLAYERCORE_EXPORT bool lightupdate;//光强传感器端口号
PLAYERCORE_EXPORT uint32_t lightcount;
PLAYERCORE_EXPORT bool tempupdate;//温度传感器端口号
PLAYERCORE_EXPORT uint32_t tempcount;
PLAYERCORE_EXPORT bool dioupdate;//输出端口端口号
PLAYERCORE_EXPORT uint32_t diocount;


// global access to the cmdline arguments
int player_argc;
char** player_argv;


#if HAVE_PLAYERSD
struct player_sd* globalSD;
#endif

void
player_globals_init()
{
  deviceTable = new DeviceTable();
  driverTable = new DriverTable();
  GlobalTime = new WallclockTime();
  fileWatcher = new FileWatcher();
  strncpy(playerversion, PLAYER_VERSION, sizeof(playerversion));
  player_quit = false;
  player_quiet_startup = false;
#if HAVE_PLAYERSD
  globalSD = player_sd_init();
#endif
}

void
player_globals_fini()
{
  delete deviceTable;
  delete driverTable;
  delete GlobalTime;
  delete fileWatcher;
#if HAVE_PLAYERSD
  if(globalSD)
    player_sd_fini(globalSD);
#endif
}

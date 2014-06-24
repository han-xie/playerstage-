/*
 *  libplayerc : a Player client library
 *  Copyright (C) Andrew Howard 2002-2003
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) Andrew Howard 2003
 *
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
 */
/***************************************************************************
 * Desc: Visual hall finder proxy
 * Author: Andrew Howard
 * Date: 24 May 2002
 * CVS: $Id: dev_hallsensor.c 4232 2007-11-01 22:16:23Z gerkey $
 **************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"


// Local declarations
void playerc_hallsensor_putmsg( playerc_hallsensor_t *device,
			        player_msghdr_t *header,
			        void *datap );

// Create a new hallsensor proxy
playerc_hallsensor_t *playerc_hallsensor_create(playerc_client_t *client, int index)
{
  playerc_hallsensor_t *device;

  device = malloc(sizeof(playerc_hallsensor_t));
  memset(device, 0, sizeof(playerc_hallsensor_t));

  playerc_device_init(&device->info, client, PLAYER_HALLSENSOR_CODE, index,
                      (playerc_putmsg_fn_t) playerc_hallsensor_putmsg);

  return device;
}


// Destroy a hallsensor proxy
void playerc_hallsensor_destroy(playerc_hallsensor_t *device)
{
  playerc_device_term(&device->info);
  free(device->halls);
  free(device);
}


// Subscribe to the hallsensor device
int playerc_hallsensor_subscribe(playerc_hallsensor_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}


// Un-subscribe from the hallsensor device
int playerc_hallsensor_unsubscribe(playerc_hallsensor_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}


// Process incoming data
void playerc_hallsensor_putmsg(playerc_hallsensor_t *device,
			       player_msghdr_t *header,
			       void* datap )
{
  if( header->type == PLAYER_MSGTYPE_DATA &&
      header->subtype == PLAYER_HALLSENSOR_DATA_HALLS)
    {
      player_hallsensor_data_t *data = (player_hallsensor_data_t*)datap;
      
      device->width  = data->width;
      device->height = data->height;
      
      // threshold the number of halls to avoid overunning the array
      device->halls_count =data->halls_count;
      device->halls = realloc(device->halls, device->halls_count * sizeof(device->halls[0]));
      memcpy(device->halls, data->halls, sizeof (player_hallsensor_hall_t)*device->halls_count);

    }

  return;
}


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
 * Desc: Visual gray finder proxy
 * Author: Andrew Howard
 * Date: 24 May 2002
 * CVS: $Id: dev_graysensor.c 4232 2007-11-01 22:16:23Z gerkey $
 **************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"


// Local declarations
void playerc_graysensor_putmsg( playerc_graysensor_t *device,
			        player_msghdr_t *header,
			        void *datap );

// Create a new graysensor proxy
playerc_graysensor_t *playerc_graysensor_create(playerc_client_t *client, int index)
{
  playerc_graysensor_t *device;

  device = malloc(sizeof(playerc_graysensor_t));
  memset(device, 0, sizeof(playerc_graysensor_t));

  playerc_device_init(&device->info, client, PLAYER_GRAYSENSOR_CODE, index,
                      (playerc_putmsg_fn_t) playerc_graysensor_putmsg);

  return device;
}


// Destroy a graysensor proxy
void playerc_graysensor_destroy(playerc_graysensor_t *device)
{
  playerc_device_term(&device->info);
  free(device->grays);
  free(device);
}


// Subscribe to the graysensor device
int playerc_graysensor_subscribe(playerc_graysensor_t *device, int access)
{
  return playerc_device_subscribe(&device->info, access);
}


// Un-subscribe from the graysensor device
int playerc_graysensor_unsubscribe(playerc_graysensor_t *device)
{
  return playerc_device_unsubscribe(&device->info);
}


// Process incoming data
void playerc_graysensor_putmsg(playerc_graysensor_t *device,
			       player_msghdr_t *header,
			       void* datap )
{
  if( header->type == PLAYER_MSGTYPE_DATA &&
      header->subtype == PLAYER_GRAYSENSOR_DATA_GRAYS)
    {
      player_graysensor_data_t *data = (player_graysensor_data_t*)datap;
      
      device->width  = data->width;
      device->height = data->height;
      
      // threshold the number of grays to avoid overunning the array
      device->grays_count =data->grays_count;
      device->grays = realloc(device->grays, device->grays_count * sizeof(device->grays[0]));
      memcpy(device->grays, data->grays, sizeof (player_graysensor_gray_t)*device->grays_count);

    }

  return;
}


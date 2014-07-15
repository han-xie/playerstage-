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
 * Desc: Visual light finder proxy
 * Author: Andrew Howard
 * Date: 24 May 2002
 * CVS: $Id: dev_lightsensor.c 4232 2007-11-01 22:16:23Z gerkey $
 **************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "playerc.h"
#include "error.h"

// Local declarations
void playerc_lightsensor_putmsg(playerc_lightsensor_t *device,
		player_msghdr_t *header, void *datap);

// Create a new lightsensor proxy
playerc_lightsensor_t *playerc_lightsensor_create(playerc_client_t *client,
		int index) {
	playerc_lightsensor_t *device;

	device = malloc(sizeof(playerc_lightsensor_t));
	memset(device, 0, sizeof(playerc_lightsensor_t));

	playerc_device_init(&device->info, client, PLAYER_LIGHTSENSOR_CODE, index,
			(playerc_putmsg_fn_t) playerc_lightsensor_putmsg);

	return device;
}

// Destroy a lightsensor proxy
void playerc_lightsensor_destroy(playerc_lightsensor_t *device) {
	playerc_device_term(&device->info);
	free(device->lights);
	free(device);
}

// Subscribe to the lightsensor device
int playerc_lightsensor_subscribe(playerc_lightsensor_t *device, int access) {
	return playerc_device_subscribe(&device->info, access);
}

// Un-subscribe from the lightsensor device
int playerc_lightsensor_unsubscribe(playerc_lightsensor_t *device) {
	return playerc_device_unsubscribe(&device->info);
}

// Process incoming data
void playerc_lightsensor_putmsg(playerc_lightsensor_t *device,
		player_msghdr_t *header, void* datap) {
	if (header->type == PLAYER_MSGTYPE_DATA
			&& header->subtype == PLAYER_LIGHTSENSOR_DATA_LIGHTS) {
		player_lightsensor_data_t *data = (player_lightsensor_data_t*) datap;

		device->width = data->width;
		device->height = data->height;

		// threshold the number of lights to avoid overunning the array
		device->lights_count = data->lights_count;
		device->lights = realloc(device->lights,
				device->lights_count * sizeof(device->lights[0]));
		memcpy(device->lights, data->lights,
				sizeof(player_lightsensor_light_t) * device->lights_count);

	}

	return;
}

int playerc_lightsensor_get_int_lights_count(playerc_lightsensor_t *device) {
	int *geom;

	if (playerc_client_request(device->info.client, &device->info,
			PLAYER_LIGHTSENSOR_REQ_GET_COLOR, NULL, (void**) &geom) < 0)
		return (-1);

	device->lights_count = *geom;
	free(geom);
	return (0);
}

int playerc_lightsensor_set_int_cmd(playerc_lightsensor_t *device, int v) {
	player_lightsensor_int_t cmd;

	memset(&cmd, 0, sizeof(cmd));
	cmd.i = v;

	return playerc_client_write(device->info.client, &device->info,
			PLAYER_LIGHTSENSOR_REQ_SET_COLOR, &cmd, NULL );
}

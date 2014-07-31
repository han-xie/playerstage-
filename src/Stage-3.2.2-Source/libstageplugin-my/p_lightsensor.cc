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
 * CVS: $Id: p_lightsensor.cc,v 1.2 2008-01-15 01:25:42 rtv Exp $
 */

// DOCUMENTATION

/** @addtogroup player
@par Lightsensor interface
- PLAYER_LIGHTSENSOR_DATA_LIGHTS
*/

// CODE

#include "p_driver.h"
using namespace Stg;


extern bool PXAupdate[PXA270PORTS];
extern uint32_t PXAcount[PXA270PORTS];
extern float PXAvalue[PXA270PORTS];


InterfaceLightsensor::InterfaceLightsensor( player_devaddr_t addr,
				StgDriver* driver,
				ConfigFile* cf,
				int section )
  : InterfaceModel( addr, driver, cf, section, "lightsensor" )
{
  // nothing to do for now
	ModelLightsensor* lightmod = (ModelLightsensor*)this->mod;
		PXAupdate[lightmod->port]=true;
		PXAcount[lightmod->port]=0;
		PXAvalue[lightmod->port]=32767;
}


void InterfaceLightsensor::Publish( void )
{
  player_lightsensor_data_t bfd;
  bzero( &bfd, sizeof(bfd) );

  ModelLightsensor* lightmod = (ModelLightsensor*)this->mod;
  
  uint32_t bcount = 0;
  const ModelLightsensor::Light* lights = lightmod->GetLights( &bcount );
  
  if(bcount > 0){
	  PXAcount[lightmod->port]=bcount;
	  PXAvalue[lightmod->port]=32767;
  }
  else{
	  PXAcount[lightmod->port] = 0;
	  PXAvalue[lightmod->port]=32767;
  }

  if ( bcount > 0 )
  {
	  // and set the image width * height
	  bfd.width = lightmod->scan_width;
	  bfd.height = lightmod->scan_height;
	  bfd.lights_count = bcount;

	  bfd.lights = new player_lightsensor_light_t[ bcount ];

	  // now run through the lights, packing them into the player buffer
	  // counting the number of lights in each channel and making entries
	  // in the acts header
	  unsigned int b;
	  for( b=0; b<bcount; b++ )
		{
		  // useful debug - leave in
		/*
		cout << "light "
		<< " left: " <<  lights[b].left
		<< " right: " <<  lights[b].right
		<< " top: " <<  lights[b].top
		<< " bottom: " <<  lights[b].bottom
		<< " color: " << hex << lights[b].color << dec
		<< endl;
		  */

			int dx = lights[b].right - lights[b].left;
			int dy = lights[b].top - lights[b].bottom;

		  bfd.lights[b].x      = lights[b].left + dx/2;
		  bfd.lights[b].y      = lights[b].bottom + dy/2;

		  bfd.lights[b].left   = lights[b].left;
		  bfd.lights[b].right  = lights[b].right;
		  bfd.lights[b].top    = lights[b].top;
		  bfd.lights[b].bottom = lights[b].bottom;

		  bfd.lights[b].color =
			 ((uint8_t)(lights[b].color.r*255.0) << 16) +
			 ((uint8_t)(lights[b].color.g*255.0) << 8) +
			 ((uint8_t)(lights[b].color.b*255.0));
			 
		  bfd.lights[b].area  = dx * dy;

		  bfd.lights[b].range = lights[b].range;
		  if(lights[b].range<PXAvalue[lightmod->port])
					  PXAvalue[lightmod->port]=lights[b].range;
		}
  }

  // should change player interface to support variable-lenght light data
  // size_t size = sizeof(bfd) - sizeof(bfd.lights) + bcount * sizeof(bfd.lights[0]);

  this->driver->Publish( this->addr,
								 PLAYER_MSGTYPE_DATA,
								 PLAYER_LIGHTSENSOR_DATA_LIGHTS,
								 &bfd, sizeof(bfd), NULL);
  if ( bfd.lights )
	  delete [] bfd.lights;
}

int InterfaceLightsensor::ProcessMessage( QueuePointer& resp_queue,
													  player_msghdr_t* hdr,
													  void* data )
{
  // todo: handle configuration requests

  //else
  {
    // Don't know how to handle this message.
    PRINT_WARN2( "stg_lightsensor doesn't support msg with type/subtype %d/%d",
		 hdr->type, hdr->subtype);
    return(-1);
  }
}


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
 * CVS: $Id: p_hallsensor.cc,v 1.2 2008-01-15 01:25:42 rtv Exp $
 */

// DOCUMENTATION

/** @addtogroup player
@par Hallsensor interface
- PLAYER_HALLSENSOR_DATA_HALLS
*/

// CODE

#include "p_driver.h"
using namespace Stg;

extern int hallscount;

InterfaceHallsensor::InterfaceHallsensor( player_devaddr_t addr,
				StgDriver* driver,
				ConfigFile* cf,
				int section )
  : InterfaceModel( addr, driver, cf, section, "hallsensor" )
{
  // nothing to do for now
}


void InterfaceHallsensor::Publish( void )
{
  player_hallsensor_data_t bfd;
  bzero( &bfd, sizeof(bfd) );

  ModelHallsensor* hallmod = (ModelHallsensor*)this->mod;
  
  uint32_t bcount = 0;
  const ModelHallsensor::Hall* halls = hallmod->GetHalls( &bcount );
  
  if(bcount > 0)
	  hallscount=bcount;
  else hallscount = 0;

  if ( bcount > 0 )
  {
	  // and set the image width * height
	  bfd.width = hallmod->scan_width;
	  bfd.height = hallmod->scan_height;
	  bfd.halls_count = bcount;

	  bfd.halls = new player_hallsensor_hall_t[ bcount ];

	  // now run through the halls, packing them into the player buffer
	  // counting the number of halls in each channel and making entries
	  // in the acts header
	  unsigned int b;
	  for( b=0; b<bcount; b++ )
		{
		  // useful debug - leave in
		/*
		cout << "hall "
		<< " left: " <<  halls[b].left
		<< " right: " <<  halls[b].right
		<< " top: " <<  halls[b].top
		<< " bottom: " <<  halls[b].bottom
		<< " color: " << hex << halls[b].color << dec
		<< endl;
		  */

			int dx = halls[b].right - halls[b].left;
			int dy = halls[b].top - halls[b].bottom;

		  bfd.halls[b].x      = halls[b].left + dx/2;
		  bfd.halls[b].y      = halls[b].bottom + dy/2;

		  bfd.halls[b].left   = halls[b].left;
		  bfd.halls[b].right  = halls[b].right;
		  bfd.halls[b].top    = halls[b].top;
		  bfd.halls[b].bottom = halls[b].bottom;

		  bfd.halls[b].color =
			 ((uint8_t)(halls[b].color.r*255.0) << 16) +
			 ((uint8_t)(halls[b].color.g*255.0) << 8) +
			 ((uint8_t)(halls[b].color.b*255.0));
			 
		  bfd.halls[b].area  = dx * dy;

		  bfd.halls[b].range = halls[b].range;
		}
  }

  // should change player interface to support variable-lenght hall data
  // size_t size = sizeof(bfd) - sizeof(bfd.halls) + bcount * sizeof(bfd.halls[0]);

  this->driver->Publish( this->addr,
								 PLAYER_MSGTYPE_DATA,
								 PLAYER_HALLSENSOR_DATA_HALLS,
								 &bfd, sizeof(bfd), NULL);
  if ( bfd.halls )
	  delete [] bfd.halls;
}

int InterfaceHallsensor::ProcessMessage( QueuePointer& resp_queue,
													  player_msghdr_t* hdr,
													  void* data )
{
  // todo: handle configuration requests

  //else
  {
    // Don't know how to handle this message.
    PRINT_WARN2( "stg_hallsensor doesn't support msg with type/subtype %d/%d",
		 hdr->type, hdr->subtype);
    return(-1);
  }
}


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
 * CVS: $Id: p_graysensor.cc,v 1.2 2008-01-15 01:25:42 rtv Exp $
 */

// DOCUMENTATION

/** @addtogroup player
@par Graysensor interface
- PLAYER_GRAYSENSOR_DATA_GRAYS
*/

// CODE

#include "p_driver.h"
using namespace Stg;

InterfaceGraysensor::InterfaceGraysensor( player_devaddr_t addr,
				StgDriver* driver,
				ConfigFile* cf,
				int section )
  : InterfaceModel( addr, driver, cf, section, "graysensor" )
{
  // nothing to do for now
}


void InterfaceGraysensor::Publish( void )
{
  player_graysensor_data_t bfd;
  bzero( &bfd, sizeof(bfd) );

  ModelGraysensor* graymod = (ModelGraysensor*)this->mod;
  
  uint32_t bcount = 0;
  const ModelGraysensor::Gray* grays = graymod->GetGrays( &bcount );
  
  if ( bcount > 0 )
  {
	  // and set the image width * height
	  bfd.width = graymod->scan_width;
	  bfd.height = graymod->scan_height;
	  bfd.grays_count = bcount;

	  bfd.grays = new player_graysensor_gray_t[ bcount ];

	  // now run through the grays, packing them into the player buffer
	  // counting the number of grays in each channel and making entries
	  // in the acts header
	  unsigned int b;
	  for( b=0; b<bcount; b++ )
		{
		  // useful debug - leave in
		/*
		cout << "gray "
		<< " left: " <<  grays[b].left
		<< " right: " <<  grays[b].right
		<< " top: " <<  grays[b].top
		<< " bottom: " <<  grays[b].bottom
		<< " color: " << hex << grays[b].color << dec
		<< endl;
		  */

			int dx = grays[b].right - grays[b].left;
			int dy = grays[b].top - grays[b].bottom;

		  bfd.grays[b].x      = grays[b].left + dx/2;
		  bfd.grays[b].y      = grays[b].bottom + dy/2;

		  bfd.grays[b].left   = grays[b].left;
		  bfd.grays[b].right  = grays[b].right;
		  bfd.grays[b].top    = grays[b].top;
		  bfd.grays[b].bottom = grays[b].bottom;

		  bfd.grays[b].color =
			 ((uint8_t)(grays[b].color.r*255.0) << 16) +
			 ((uint8_t)(grays[b].color.g*255.0) << 8) +
			 ((uint8_t)(grays[b].color.b*255.0));
			 
		  bfd.grays[b].area  = dx * dy;

		  bfd.grays[b].range = grays[b].range;
		}
  }

  // should change player interface to support variable-lenght gray data
  // size_t size = sizeof(bfd) - sizeof(bfd.grays) + bcount * sizeof(bfd.grays[0]);

  this->driver->Publish( this->addr,
								 PLAYER_MSGTYPE_DATA,
								 PLAYER_GRAYSENSOR_DATA_GRAYS,
								 &bfd, sizeof(bfd), NULL);
  if ( bfd.grays )
	  delete [] bfd.grays;
}

int InterfaceGraysensor::ProcessMessage( QueuePointer& resp_queue,
													  player_msghdr_t* hdr,
													  void* data )
{
  // todo: handle configuration requests

  //else
  {
    // Don't know how to handle this message.
    PRINT_WARN2( "stg_graysensor doesn't support msg with type/subtype %d/%d",
		 hdr->type, hdr->subtype);
    return(-1);
  }
}


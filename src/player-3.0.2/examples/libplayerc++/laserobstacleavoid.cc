/*
Copyright (c) 2005, Brad Kratochvil, Toby Collett, Brian Gerkey, Andrew Howard, ...
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Player Project nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * laserobstacleavoid.cc
 *
 * a simple obstacle avoidance demo
 *
 * @todo: this has been ported to libplayerc++, but not tested
 */

#include <libplayerc++/playerc++.h>
#include <iostream>

#include "args.h"

#define RAYS 32
const double minfrontdistance = 1.5;
const double cruisespeed = 0.5;
const double avoidspeed = 0;
const double avoidturn = 0.5;
const double avoidlrspeed=0.1;
double reward=false;

int
main(int argc, char **argv)
{
  parse_args(argc,argv);

  // we throw exceptions on creation if we fail
  try
  {
    using namespace PlayerCc;

    PlayerClient robot(gHostname, gPort);
    Position2dProxy pp(&robot, gIndex);
    LaserProxy lp(&robot, gIndex);

    //pp.SetMotorEnable (true);
    double minR = 2;
    double minL = 2;
    double newspeed = 0;
    double newturnrate = 0;

    // go into read-think-act loop
    for(;;)
    {
      // this blocks until new data comes; 10Hz by default
      robot.Read();


      if(lp.GetCount()==22){
          minL=lp.GetRange(1);
          minR=lp.GetRange(0);
      }

      // laser avoid (stolen from esben's java example)
      std::cout << "minR: " << minR
                << "minL: " << minL
                << std::endl;

      if(minL>minfrontdistance&&minR>minfrontdistance){
    	  newspeed=cruisespeed;
    	  newturnrate=0;
    	  if(reward){
    		  pp.SetSpeed(0, 3.14);
    		  sleep(1);
    		  reward=false;
    	  }
      }else if(minL<=minfrontdistance&&minR>minfrontdistance){
    	  newspeed=-avoidlrspeed;
    	  newturnrate=-avoidturn;
      }else if(minL>minfrontdistance&&minR<=minfrontdistance){
    	  newspeed=-avoidlrspeed;
    	  newturnrate=avoidturn;
      }else{
    	  newspeed=-cruisespeed;
    	  newturnrate=0;
    	  reward=true;
      }

      std::cout << "speed: " << newspeed
                << "turn: " << newturnrate
                << std::endl;

      // write commands to robot
      pp.SetSpeed(newspeed, newturnrate);
    }
  }
  catch (PlayerCc::PlayerError & e)
  {
    std::cerr << e << std::endl;
    return -1;
  }
}

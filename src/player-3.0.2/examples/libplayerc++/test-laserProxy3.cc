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

#include <iostream>
#include <libplayerc++/playerc++.h>

int
main(int argc, char *argv[])
{
  using namespace PlayerCc;

  PlayerClient    robot("localhost");
  LaserProxy      lp(&robot,0);
  LaserProxy lp2(&robot,1);
  Position2dProxy pp(&robot,0);

  for(;;)
  {
    double turnrate, speed;

    // read from the proxies
    printf("--5\n");
    robot.Read();
    printf("--6\n");

    printf("--------------\n");
    std::cout<< lp<< std::endl;
    printf("--------------\n");
    std::cout<<lp2<<std::endl;
    printf("--------------\n");

    /*
    // print out sonars for fun
    std::cout << sp << std::endl;

    // do simple collision avoidance
    if((sp[0] + sp[1]) < (sp[6] + sp[7]))
      turnrate = dtor(-20); // turn 20 degrees per second
    else
      turnrate = dtor(20);

    if(sp[3] < 0.500)
      speed = 0;
    else
      speed = 0.100;*/

    // command the motors
    std::cout<<"speed"<<std::endl;
    std::cin>>speed;
    std::cout<<"turn rate"<<std::endl;
    std::cin>>turnrate;
    printf("--3\n");
    pp.SetSpeed(speed, turnrate);
    printf("--4\n");
  }
}


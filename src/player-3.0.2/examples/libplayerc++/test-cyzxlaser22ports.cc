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

#define USAGE \
  "USAGE: goto [-x <x>] [-y <y>] [-h <host>] [-p <port>] [-m]\n" \
  "       -x <x>: set the X coordinate of the target to <x>\n"\
  "       -y <y>: set the Y coordinate of the target to <y>\n"\
  "       -h <host>: connect to Player on this host\n" \
  "       -p <port>: connect to Player on this TCP port\n" \
  "       -m       : turn on motors (be CAREFUL!)"

bool gMotorEnable(false);
bool gGotoDone(false);
std::string gHostname(PlayerCc::PLAYER_HOSTNAME);
uint32_t gPort(PlayerCc::PLAYER_PORTNUM);
uint32_t gIndex(0);
uint32_t gDebug(0);
uint32_t gFrequency(10);

player_pose2d_t gTarget = { 0, 0, 0 };

void print_usage(int argc, char** argv) {
	std::cout << USAGE << std::endl;
}


int parse_args(int argc, char** argv) {
	const char* optflags = "h:p:i:d:u:x:y:m";
	int ch;

	while (-1 != (ch = getopt(argc, argv, optflags))) {
		switch (ch) {
		/* case values must match long_options */
		case 'h':
			gHostname = optarg;
			break;
		case 'p':
			gPort = atoi(optarg);
			break;
		case 'i':
			gIndex = atoi(optarg);
			break;
		case 'd':
			gDebug = atoi(optarg);
			break;
		case 'u':
			gFrequency = atoi(optarg);
			break;
		case 'x':
			gTarget.px = atof(optarg);
			break;
		case 'y':
			gTarget.py = atof(optarg);
			break;
		case 'm':
			gMotorEnable = true;
			break;
		case '?':
		case ':':
		default:
			print_usage(argc, argv);
			return (-1);
		}
	}

	return (0);
}

int
main(int argc, char *argv[])
{
  using namespace PlayerCc;
  parse_args(argc, argv);

  PlayerClient    robot(gHostname, gPort);
  cyzxLaserProxy      lp(&robot,0);
  Position2dProxy pp(&robot,0);


  char i;

  for(;;)
  {
    double turnrate, speed;

    // read from the proxies
    printf("---continue?\n");
    std::cin>>i;
    if(i=='n') return 0;
    printf("--\n");
    robot.Read();
    printf("--\n");

    if(lp.GetCount()==22){
		printf("--------------\n");
		//std::cout<< lp<< std::endl;
		std::cout<<lp.GetCount() << "\t";
		for(unsigned int j=0;j<lp.GetCount();j++)
		  std::cout << lp.GetRange(j) << "\t";
		printf("--------------\n");
    }else continue;
  }
}


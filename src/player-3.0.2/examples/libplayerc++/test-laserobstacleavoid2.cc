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
int avoidcount=0,randcount=0;

const double cruisespeed = 0.4;
const double avoidspeed = 0.05;
const double avoidturn = 0.5;
const double minfrontdistance = 1.0; // 0.6
const bool verbose = false;
const double stopdist = 0.3;
const int avoidduration = 10;

int main(int argc, char **argv) {
	parse_args(argc, argv);

	// we throw exceptions on creation if we fail
	try {
		using namespace PlayerCc;

		PlayerClient robot(gHostname, gPort);
		Position2dProxy pp(&robot, gIndex);
		LaserProxy lp(&robot, gIndex);

		bool obstruction = false;
		bool stop = false;

		// find the closest distance to the left and right and check if
		// there's anything in front
		double minleft = 1e6;
		double minright = 1e6;

		robot.Read();
		if (lp.GetCount() == 22) {
			minleft = lp.GetRange(1);
			minright = lp.GetRange(0);
		}

		if (minleft < minfrontdistance && minright < minfrontdistance) {
			if (verbose)
				puts("  obstruction stop!");
			obstruction = true;
			stop = true;
		}

		if (verbose) {
			puts("");
			printf("minleft %.3f \n", minleft);
			printf("minright %.3f\n ", minright);
		}

		if (obstruction || stop || (avoidcount > 0)) {
			if (verbose)
				printf("Avoid %d\n", avoidcount);

			pp.SetSpeed(stop ? 0.0 : avoidspeed,0);

			/* once we start avoiding, select a turn direction and stick
			 with it for a few iterations */
			if (avoidcount < 1) {
				if (verbose)
					puts("Avoid START");
				avoidcount = random() % avoidduration + avoidduration;

				if (minleft < minright) {
					pp.SetSpeed(0,-avoidturn);
					if (verbose)
						printf("turning right %.2f\n", -avoidturn);
				} else {
					pp.SetSpeed(0,+avoidturn);
					if (verbose)
						printf("turning left %2f\n", +avoidturn);
				}
			}

			avoidcount--;
		} else {
			if (verbose)
				puts("Cruise");

			avoidcount = 0;
			pp.SetSpeed(cruisespeed,0);
		}
	} catch (PlayerCc::PlayerError & e) {
		std::cerr << e << std::endl;
		return -1;
	}
}

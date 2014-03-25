/*
 * test-CDS55XXX.cc
 *
 *  Created on: Jun 20, 2013
 *      Author: keevi7c3l
 */

#include <libplayerc++/playerc++.h>
#include <iostream>

int main(int argc, char **argv) {
	// we throw exceptions on creation if we fail
	try {
		using namespace PlayerCc;

		PlayerClient robot("192.168.0.111", 6665);
		Position2dProxy pp(&robot, 0);

		// go into read-think-act loop
		int id = 0;
		int speed = 0;
		for (;;) {

			//void SetSpeed(double aXSpeed, double aYawSpeed)
			/*because now not mature so
			 * void MFSetServoRotaSpd(int inID,int inSpeed);
			 * cmd.vel.px = inID
			 * cmd.vel.pa = inSpeed
			 * cmd.vel.px && cmd.vel.pa ==0 --> rot
			 */

			pp.SetSpeed((float) id, (float) speed);
			std::cout<<"id"<<std::endl;
			std::cin >> id;
			std::cout<<"speed"<<std::endl;
			std::cin>>speed;

		}
	} catch (PlayerCc::PlayerError e) {
		std::cout<<"wrong in control"<<std::endl;
		std::cerr << e << std::endl;
		return -1;
	}
}

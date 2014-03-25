/*
 * test-all.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

/*
 * test-position2d.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
#include "CYZXInter.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.110", 6666);
	OpaqueProxy opaquep(&robot, 0);

	CYZXInter cyzxic(&robot, &opaquep);

	int t = 0;
	int id;
	int mode;
	int pos;
	int speed;
	int seconds;

	while (1) {
		std::cout << "-----------------\n";
		//std::cout << "1 servoMode\n";
		std::cout << "2 pos-1-tire\n";
		std::cout << "3 speed-1-tire\n";
		std::cout << "4 action\n";
		std::cout<<"5 speedMode\n";
		std::cout<<"6 posMode\n";
		std::cout << "10 collide sensor\n";
		std::cout << "11 temprature sensor\n";
		std::cout << "20 4-tires-speed\n";
		std::cout << "21 4-tires-pos\n";
		std::cout << "-----------------\n";
		std::cin >> t;
		switch (t) {
		case 1: {
			std::cout << "id\n";
			std::cin >> id;
			std::cout << "mode\n";
			std::cin >> mode;
			//cyzxic.bkMFSetServoMode(id, mode);
			break;
		}
		case 6:{
			std::cout<<"id\n";
			std::cin>>id;
			cyzxic.bkMFSetServoModeServo(id);
			break;
		}
		case 5:{
			std::cout<<"id\n";
			std::cin>>id;
			cyzxic.bkMFSetServoModeMoto(id);
			break;
		}
		case 2: {
			std::cout << "id\n";
			std::cin >> id;
			std::cout << "pos\n";
			std::cin >> pos;
			std::cout << "speed\n";
			std::cin >> speed;
			cyzxic.bkMFSetServoPos(id, pos, speed);
			break;
		}
		case 3: {
			std::cout << "id\n";
			std::cin >> id;
			std::cout << "speed\n";
			std::cin >> speed;
			cyzxic.bkMFSetServoRotaSpd(id, speed);
			break;
		}
		case 4: {
			cyzxic.bkMFServoAction();
			break;
		}
		case 10: {
			printf("%d\n", cyzxic.colliSen());
			break;
		}
		case 11: {
			printf("%d\n", cyzxic.tempSen());
			break;
		}
		case 20: {
			int id1, id2, id3, id4;
			int speed1, speed2, speed3, speed4;
			std::cout << "id1 id2 id3 id4:\n";
			std::cin >> id1;
			std::cin >> id2;
			std::cin >> id3;
			std::cin >> id4;
			std::cout << "speed1 speed2 speed3 speed4\n";
			std::cin >> speed1;
			std::cin >> speed2;
			std::cin >> speed3;
			std::cin >> speed4;
			cyzxic.pos2dSet4Tires(id1, id2, id3, id4, speed1, speed2, speed3,
					speed4);
			break;
		}
		case 21: {
			int id1, id2, id3, id4, pos;
			std::cout << "id1 id2 id3 id4\n";
			std::cin >> id1;
			std::cin >> id2;
			std::cin >> id3;
			std::cin >> id4;
			std::cout << "pos\n";
			std::cin >> pos;
			std::cout << "speed\n";
			std::cin >> speed;
			cyzxic.pos2dSet4TiresPos(id1, id2, id3, id4, pos, speed);
			break;
		}
		default:
			return 0;
			break;
		}
	}

	return 0;
}

/*
 * my-position2d.cc
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include  "../background.h"
int main(int argc, char * argv[]) {
	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	int t = 0;
	int speed = 0;
	int angle = 0;
	int id=0;

	while (1) {
		std::cout << "choose one tire 6 7 8 9 10 11" << std::endl;
		std::cout << "20 exit" << std::endl;
		std::cout << "21 display" << std::endl;
		std::cout << "22 display" << std::endl;
		std::cout << "30 input id , input speed \n";
		std::cin >> t;
		if (6 == t) {
			MFSetServoMode(t, 0);
			std::cout << "speed :" << std::endl;
			std::cin >> speed;
			std::cout << "angle :" << std::endl;
			std::cin >> angle;
			MFSetServoPos(t, angle, speed);
			MFServoAction();
			std::cout << "angle : " << MFGetServoPos(8) << std::endl;
			std::cout << "angle : " << MFGetServoPos(9) << std::endl;
		} else if(30 == t){
			std::cout<<"id"<<std::endl;
			std::cin>>id;
			MFSetServoMode(id, 1);
			std::cout<<"speed"<<std::endl;
			std::cin>>speed;
			MFSetServoRotaSpd(id,speed);
			MFServoAction();
		}
		else if (7 == t) {
			MFSetServoMode(t, 0);
			std::cout << "speed :" << std::endl;
			std::cin >> speed;
			std::cout << "angle :" << std::endl;
			std::cin >> angle;
			MFSetServoPos(t, angle, speed);
			MFServoAction();
			std::cout << "angle : " << MFGetServoPos(8) << std::endl;
			std::cout << "angle : " << MFGetServoPos(9) << std::endl;
		} else if (8 == t) {
			MFSetServoMode(t, 0);
			std::cout << "speed :" << std::endl;
			std::cin >> speed;
			std::cout << "angle :" << std::endl;
			std::cin >> angle;
			MFSetServoPos(t, angle, speed);
			MFServoAction();
			std::cout << "angle : " << MFGetServoPos(8) << std::endl;
			std::cout << "angle : " << MFGetServoPos(9) << std::endl;
		} else if (9 == t) {
			MFSetServoMode(t, 0);
			std::cout << "speed :" << std::endl;
			std::cin >> speed;
			std::cout << "angle :" << std::endl;
			std::cin >> angle;
			MFSetServoPos(t, angle, speed);
			MFServoAction();
			std::cout << "angle : " << MFGetServoPos(8) << std::endl;
			std::cout << "angle : " << MFGetServoPos(9) << std::endl;
		} else if (10 == t) {
			MFSetServoMode(t, 0);
			std::cout << "speed :" << std::endl;
			std::cin >> speed;
			std::cout << "angle :" << std::endl;
			std::cin >> angle;
			MFSetServoPos(t, angle, speed);
			MFServoAction();
			std::cout << "angle : " << MFGetServoPos(10) << std::endl;
			std::cout << "angle : " << MFGetServoPos(11) << std::endl;
		} else if (11 == t) {
			MFSetServoMode(t, 0);
			std::cout << "speed :" << std::endl;
			std::cin >> speed;
			std::cout << "angle :" << std::endl;
			std::cin >> angle;
			MFSetServoPos(t, angle, speed);
			MFServoAction();
			std::cout << "angle : " << MFGetServoPos(10) << std::endl;
			std::cout << "angle : " << MFGetServoPos(11) << std::endl;
		} else if (20 == t) {
			break;
		} else if (21 == t) {
			MF2X4LCD("hello", 5);
		} else if (22 == t) {
			MF2X4LCD("world", 6);
		}
	}

	return 0;
}

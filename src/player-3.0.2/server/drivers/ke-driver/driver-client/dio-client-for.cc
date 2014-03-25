#include <iostream>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.111", 6665);
	DioProxy diop(&robot, 0);
	AioProxy aiop(&robot, 0);
	Position2dProxy pos2dp(&robot, 0);
	float speed = 0.0;

	for (;;) {
		for (int i = 0; i < 12; i++) {
			diop.SetOutput(i, i);

			// read from the proxies
			robot.Read();

			std::cout << diop.GetCount() << "  ";

			usleep(100000);

		}
		std::cout << std::endl;

		for (int i = 0; i < 8; i++) {
			aiop.SetVoltage(i, 1.0);

			robot.Read();

			std::cout << aiop.GetCount() << "  ";

			usleep(100);

		}
		std::cout << std::endl;

		std::cout << "set speed : ";
		std::cin >> speed;

		if (speed > 10.0)
			pos2dp.SetSpeed(1.0, speed);
		else
			pos2dp.SetSpeed(3.0, 0.0);

	}
}


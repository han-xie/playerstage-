#include <iostream>
#include <libplayerc++/playerc++.h>


int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.111",6665);
	DioProxy diop(&robot, 0);
	AioProxy aiop(&robot,0);
	Position2dProxy pos2dp(&robot,0);

	uint32_t port=0;

	for (;;) {
		std::cout<<"port :"<<std::endl;
		std::cin>>port;

		diop.SetOutput(port,0);
		aiop.SetVoltage(port,1.0);
		pos2dp.SetSpeed((double)port, (double)port);

		// read from the proxies
		robot.Read();

		std::cout<<"DIO port value : ";
		std::cout<<diop.GetCount()<<"   "<<diop.GetDigin()<<std::endl;

		std::cout<<"AIO port value :";
		std::cout<<aiop.GetCount()<<std::endl;


		//usleep(100000);
	}

}


#include <iostream>
#include <libplayerc++/playerc++.h>
#include "args.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;
	parse_args(argc, argv);
	PlayerClient robot(gHostname, gPort);
	cyzxLaserProxy cyzxic(&robot,gIndex);
	Position2dProxy pp(&robot,gIndex);


while(1){
	//forward
	pp.SetSpeed(0.3,0);
	sleep(1);
	pp.SetSpeed(0,0);
	sleep(1);
	//pp.SetSpeed(0,0);
	//sleep(3);
	cyzxic.WifiSetSound("play\n\0","192.168.0.99:8888");
	//cyzxic.WifiSetSound("play");
    sleep(1);
    cyzxic.WifiSetSound("stop\n\0","192.168.0.99:8888");
	sleep(1);

	cyzxic.WifiSetLCD("weather\t成都\t192.168.0.99\t8888\t\0","192.168.0.100:8899");
std::cout<<"goto display"<<std::endl;
	//cyzxic.WifiSetLCD("weather\t成都\t192.168.0.99\t8888\t\0");
	sleep(1);
	pp.SetSpeed(0,0.75);
	sleep(1);
	pp.SetSpeed(0.3,0);
	sleep(1);
	pp.SetSpeed(0,0);
	sleep(1);
	cyzxic.WifiSetSound("play\n\0","192.168.0.99:8888");
	//cyzxic.WifiSetSound("play");
    sleep(1);
    cyzxic.WifiSetSound("stop\n\0","192.168.0.99:8888");
	sleep(1);
	pp.SetSpeed(0,0);
	sleep(1);
std::cout<<"over"<<std::endl;
}

}
                
        
                  
        
                 
              
        
                 
        
                 
        
                 
        
        

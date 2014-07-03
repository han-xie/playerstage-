#include <iostream>
#include <libplayerc++/playerc++.h>

int
main(int argc, char *argv[])
{
  using namespace PlayerCc;

  PlayerClient    robot("localhost");
  LaserProxy      lp(&robot,0);
  LaserProxy      lp2(&robot,1);
  Position2dProxy pp(&robot,0);

  char i;
  int k=0;

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

    printf("--------------\n");
    //std::cout<< lp<< std::endl;
    std::cout<<lp.GetCount() << "\t";
    for(unsigned int j=0;j<lp.GetCount();j++){
      std::cout << lp.GetRange(j) << "  ";
    }
    printf("--------------\n");

    printf("*******************\n");
    //std::cout<< lp<< std::endl;
    std::cout<<lp2.GetCount() << "\t";
    for(unsigned int j=0;j<lp2.GetCount();j++){
      std::cout << lp2.GetRange(j) << "  ";
    }
    printf("*******************\n");
  }
}


#include <iostream>
#include <libplayerc++/playerc++.h>
#include "CYZXInter.h"

#include <iostream>
#include <stdlib.h> // for atof()
#if !defined (WIN32)
  #include <unistd.h>
#endif

#include <math.h>
#include <string>


/*void OpaqueProxy::newSizeFormDevice(int size) {
		if (NULL == this->mDevice->data)
			this->mDevice->data = new uint8_t[size];
}

void OpaqueProxy::newFormDevice(int size) {
}
*/
#define USAGE \
  "USAGE: goto [-x <x>] [-y <y>] [-h <host>] [-p <port>] [-m]\n" \
  "       -x <x>: set the X coordinate of the target to <x>\n"\
  "       -y <y>: set the Y coordinate of the target to <y>\n"\
  "       -h <host>: connect to Player on this host\n" \
  "       -p <port>: connect to Player on this TCP port\n" \
  "       -m       : turn on motors (be CAREFUL!)"

PlayerCc::PlayerClient* robot;
PlayerCc::Position2dProxy* pp;
PlayerCc::LaserProxy* lp;

bool         gMotorEnable(false);
bool         gGotoDone(false);
std::string  gHostname(PlayerCc::PLAYER_HOSTNAME);
uint32_t        gPort(PlayerCc::PLAYER_PORTNUM);
uint32_t        gIndex(0);
uint32_t        gDebug(0);
uint32_t        gFrequency(10); // Hz

player_pose2d_t gTarget = {0, 0, 0};

void
print_usage(int argc, char** argv)
{
  std::cout << USAGE << std::endl;
}

int
parse_args(int argc, char** argv)
{
  const char* optflags = "h:p:i:d:u:x:y:m";
  int ch;

  while(-1 != (ch = getopt(argc, argv, optflags)))
  {
    switch(ch)
    {
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

/*
 * very bad goto.  target is arg (as pos_t*)
 *
 * sets global 'gGotoDone' when it's done
 */
void
position_goto(player_pose2d_t target)
{
  using namespace PlayerCc;

  double dist, angle;

  dist = sqrt((target.px - pp->GetXPos())*
              (target.px - pp->GetXPos()) +
              (target.py - pp->GetYPos())*
              (target.py - pp->GetYPos()));

  angle = atan2(target.py - pp->GetYPos(),
                target.px - pp->GetXPos());

  double newturnrate = 0;
  double newspeed = 0;

  if (fabs(rtod(angle)) > 10.0)
  {
    newturnrate = limit((angle/M_PI) * 40.0, -40.0, 40.0);
    newturnrate = dtor(newturnrate);
  }
  else
    newturnrate = 0.0;

  if (dist > 0.05)
  {
    newspeed = limit(dist * 0.200, -0.2, 0.2);
  }
  else
    newspeed = 0.0;

  if (fabs(newspeed) < 0.01)
    gGotoDone = true;

  pp->SetSpeed(newspeed, newturnrate);

}

/*
 * sonar avoid.
 *   policy:
 *     if(object really close in front)
 *       backup and turn away;
 *     else if(object close in front)
 *       stop and turn away
 */
void
sonar_avoid(void)
{/*
  double min_front_dist = 0.500;
  double really_min_front_dist = 0.300;
  bool avoid = false;

  double newturnrate = 10.0;
  double newspeed = 10.0;

  if ((sp->GetScan(2) < really_min_front_dist) ||
      (sp->GetScan(3) < really_min_front_dist) ||
      (sp->GetScan(4) < really_min_front_dist) ||
      (sp->GetScan(5) < really_min_front_dist))
  {
    avoid = true;
    std::cerr << "really avoiding" << std::endl;
    newspeed = -0.100;
  }
  else if((sp->GetScan(2) < min_front_dist) ||
          (sp->GetScan(3) < min_front_dist) ||
          (sp->GetScan(4) < min_front_dist) ||
          (sp->GetScan(5) < min_front_dist))
  {
    avoid = true;
    std::cerr << "avoiding" << std::endl;
    newspeed = 0;
  }

  if(avoid)
  {
    if ((sp->GetScan(0) + sp->GetScan(1)) < (sp->GetScan(6) + sp->GetScan(7)))
      newturnrate = PlayerCc::dtor(30);
    else
      newturnrate = PlayerCc::dtor(-30);
  }

  if(newspeed < 10.0 && newturnrate < 10.0)
    pp->SetSpeed(newspeed, newturnrate);
*/
}

bool printfs=false;

template<typename T>
void
Print(T t)
{
	if(printfs){
  std::cout <<"-------"<< *t << std::endl;
  printfs=false;
	}
}

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

		parse_args(argc, argv);
	PlayerClient robot(gHostname, gPort);
	OpaqueProxy opaquep(&robot, 0);
	Position2dProxy pp(&robot,gIndex);
	CYZXInter cyzxic(&robot, &opaquep);


	//forward
	pp.SetSpeed(0.3,0);

	sleep(3);

	cyzxic.WifiSetLCD("weather\t成都\t192.168.0.99\t8888\t\0","192.168.1.101:7666");
	//cyzxic.WifiSetLCD("weather\t成都\t192.168.0.99\t8888\t\0");
	sleep(5);
	pp.SetSpeed(0,0.75);
	sleep(2);
	pp.SetSpeed(0.3,0);
	cyzxic.WifiSetSound("play","192.168.1.101:7666");
	//cyzxic.WifiSetSound("play");
	sleep(3);
	pp.SetSpeed(0,0);
	sleep(3);

}


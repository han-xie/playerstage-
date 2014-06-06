#include <iostream>
#include <libplayerc++/playerc++.h>
#include "args.h"
using namespace std;

int main(int argc, char **argv) 
{
	using namespace PlayerCc;

	parse_args(argc, argv);
	PlayerClient robot(gHostname, gPort);
	BlobfinderProxy bp(robot, gIndex);
	playerc_blobfinder_blob_t pbbt;
    sleep(2);

    while(1)
    {
        robot.Read();
	pbbt = bp.GetBlob(0);
        cout<<"中心点坐标："<<pbbt.x<<","<<pbbt.y<<endl;
    }
    return 0;
}

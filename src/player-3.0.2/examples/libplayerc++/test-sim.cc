#include <libplayerc++/playerc++.h>
#if !defined (WIN32)
#include <unistd.h>
#endif
#include <stdlib.h>
#include <iostream>

#include "args.h"

int main(int argc, char **argv) {
	parse_args(argc, argv);

	using namespace PlayerCc;

	/* Connect to Player server */
	PlayerClient robot(gHostname, gPort);
	SimulationProxy sp(&robot, gIndex);

	for (;;) {

		printf("char* identifier, double& x, double& y, double& a\n");
		char t[30];
		scanf("%s", t);
		double x, y, a;
		scanf("%lf", &x);
		scanf("%lf", &y);
		scanf("%lf", &a);

		sp.SetPose2d(t, x, y, a);
	}
	return (0);
}


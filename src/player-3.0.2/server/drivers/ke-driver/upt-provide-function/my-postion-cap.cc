/*
 * my-postion-cap.cc
 *
 *  Created on: Jul 9, 2013
 *      Author: keevi7c3l
 */

#include  "background.h"
int main(int argc, char * argv[]) {
	int x = 0;
	int y = 0;
	int id = 8;
	int pos = 0;
	MFCapOpen();
	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	MFSetServoMode(id, 0);

	while (1) {
		MFCapSetH(50, 0);
		MFCapSetS(255, 60);
		x = MFCapGetCenterX();
		y = MFCapGetCenterY();
		printf("x:%d   y:%d\n", x, y);
		printf("pos:%d\nnew pos:\n", MFGetServoPos(id));
		scanf("%d", &pos);

		MFSetServoPos(id, pos, 200);
		MFServoAction();

	}
}


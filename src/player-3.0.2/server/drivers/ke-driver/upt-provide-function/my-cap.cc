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
	int sum = 8;
	//int pos = 0;
	MFCapOpen();
	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	//MFSetServoMode(id, 0);

MFCapSetH(50, 0);
		MFCapSetS(255, 60);
sleep(3);
	while (1) {
		
		x = MFCapGetCenterX();
		y = MFCapGetCenterY();
sum = MFCapGetSum();
		printf("x:%d   y:%d  sum:%d\n", x, y,sum);
		//printf("pos:%d\nnew pos:\n", MFGetServoPos(id));
		//scanf("%d", &pos);

		//MFSetServoPos(id, pos, 200);
		//MFServoAction();
sleep(2);

	}
}


#include  "background.h"
#include <stdio.h>

int main(int argc, char * argv[]) {

	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	/*
	 printf("-------%d------\n", argc);
	 printf("-------%s------\n", argv[0]);
	 printf("-------%s------\n", argv[1]);

	 int t;
	 scanf("%d", &t);
	 */

	MF2X4LCD(argv[0], 16);

	sleep(2);

	while (1) {
		sleep(4);
	}

	return 0;

}

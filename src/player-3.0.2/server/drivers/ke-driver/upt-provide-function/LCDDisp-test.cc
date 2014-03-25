#include  "background.h"
#include <stdio.h>

int main(int argc, char * argv[]) {

	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);


	printf("-------%d------\n", argc);
	printf("-------%s------\n", argv[0]);
	printf("-------%s------\n", argv[1]);

	int t;
	while (1) {
		scanf("%d", &t);
		switch (t) {
		case 1: {
			MF2X4LCD(argv[0], 16);
			break;
		}
		case 2: {
			MF2X4LCD("1234567890123456", 16);
			break;
		}
		case 3: {
			char test[5] = { 'a', 'b', 'c', 'd', '\0' };
			const char *p = test;
			MF2X4LCD(p, 4);
			break;
		}
		}
	}
	return 0;

}

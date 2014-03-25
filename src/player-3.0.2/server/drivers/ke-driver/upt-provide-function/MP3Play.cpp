#include <iostream>
#include  "background.h"
int main(int argc, char * argv[]) {

	if (0 == argc) {
		printf("can't play because no song's name\n");
		return 0;
	}

	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	int i=0;
	MFMp3Play(argv[i],strlen(argv[i]));

	printf("------------%s  %d----------\n",argv[i],strlen(argv[i]));

	sleep(2);

	return 0;
}

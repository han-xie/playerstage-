#include <iostream>
#include  "background.h"
int main(int argc, char * argv[]) {

	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	MFMp3Stop();

	sleep(4);

	return 0;
}

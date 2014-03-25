#include  "background.h"
int main(int argc, char * argv[]) {

	MFInit();
	MFSetPortDirect(0x00000FFF);
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);

	DelayMS(100);
	while (1) {
		MF2X4LCD("hello world", 12);
		DelayMS(100);
	}
}

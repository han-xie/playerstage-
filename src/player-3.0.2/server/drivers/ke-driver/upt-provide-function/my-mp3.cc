#include  "background.h"
int main(int argc, char * argv[]) {

	MFInit();
	/*
	 MFSetPortDirect(0x00000FFF);
	 MFDigiInit(100);
	 DelayMS(100);
	 MFADInit(100);
	 MFSetServoMode(5,0);
	 */
	MFMp3Play((char*) &("song.mp3"), 8);

	printf(" mp3 have played \n");

}

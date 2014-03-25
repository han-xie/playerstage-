#include  "background.h"

int main(int argc, char * argv[]) {

	int io0 = 2;
	int io1 = 2;
	int io2 = 2;
	int io3 = 2;
	int io4 = 2;
	int io5 = 2;
	int io6 = 2;
	int io7 = 2;

	MFInit();
	MFSetPortDirect(0x00000000);
	for (int i = 0; i < 8; i++) {
		MFADEnable(i);
	}
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	MFSetServoMode(8, 1);
	MFSetServoMode(9, 1);

	printf("test before into while \n");

	while (1) {
		io0 = MFGetDigiInput(0);
		io1 = MFGetDigiInput(1);
		io2 = MFGetDigiInput(2);
		io3 = MFGetDigiInput(3);
		io4 = MFGetDigiInput(4);
		io5 = MFGetDigiInput(5);
		io6 = MFGetDigiInput(6);
		io7 = MFGetDigiInput(7);

		printf("%d %d %d %d %d %d %d %d\n", io0, io1, io2, io3, io4, io5, io6,
				io7);

		printf("---%d \n",MFGetAD(4));

		DelayMS(4000);
	}
}

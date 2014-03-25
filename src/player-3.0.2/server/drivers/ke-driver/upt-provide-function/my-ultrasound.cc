#include  "background.h"

int main(int argc, char * argv[]) {

	MFInit();
	MFUSonicEnable();
	DelayMS(100);

	while (1) {
		printf("---------%d----------\n", MFGetUltrasonic());
		DelayMS(400);
	}

	return 0;
}

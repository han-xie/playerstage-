#include  "background.h"
int main(int argc, char * argv[]) {
	char *display = "hello world";
	MFInit();
	DelayMS(100);
	//MF2X4LCD(display, strlen (display)+1);
	MF2X4LCD(display, 12);
	printf("displayer over\n");

	return 0;
}

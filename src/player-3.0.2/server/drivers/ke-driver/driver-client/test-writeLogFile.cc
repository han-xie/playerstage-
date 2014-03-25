#include <stdio.h>
#define LOGFILENAME "CYZXmultidriver.log"
int main(int argc, char *argv[]) {
	FILE *fl;
	if ((fl = (fopen(LOGFILENAME, "w"))) == NULL)
		return -1;
	fputs("hello world!", fl);
	fclose(fl);
	return -1;
}

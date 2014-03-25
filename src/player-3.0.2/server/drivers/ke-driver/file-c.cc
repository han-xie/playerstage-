#include <stdio.h>

#define WRITELOG 1
FILE *fr;

int openf() {
	if (WRITELOG) {
		if (NULL == (fr = fopen("kelog-c", "w+"))) {
			printf("open file error !");
		}
	}
}
int closef() {
	if (WRITELOG) {
		fclose(fr);
	}
}


int main() {
FILE *fr;
int i = 0;
long j = 98;
float k = 8.5;
fr = fopen("kelog-c", "w+");
fprintf(fr, "begin--\n");
fprintf(fr, "%d\n", i);
fprintf(fr, "%d\n", j);
fprintf(fr, "%f\n", k);
fclose(fr);
return 0;
}


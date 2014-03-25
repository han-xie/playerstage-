/*
 * t.cc
 *
 *  Created on: Oct 13, 2013
 *      Author: keevi7c3l
 */

#include <stdio.h>

char *itoa(int num, char *str, int radix) {
	char string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char* ptr = str;
	int denom = 0;  //余数
	int count = -1;
	int i;
	int j;

	if (num == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	while (num >= radix) {
		denom = num % radix;
		num /= radix;

		*ptr++ = string[denom];
		count++;
	}

	if (num) {
		*ptr++ = string[num];
		count++;
	}

	*ptr = '\0';
	j = count;

	for (i = 0; i < (count + 1) / 2; i++) {
		int temp = str[i];
		str[i] = str[j];
		str[j--] = temp;
	}

	return str;
}
int main(int argc, char *argv[]) {
	int i;
	char ts[40];
	while (1) {
		printf("input a number:\n");
		scanf("%d", &i);
		itoa(i, &ts[0], 10);
		printf("%d to string:%s,\n", i, ts);
	}
	return 0;
}

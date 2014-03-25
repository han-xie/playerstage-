/*
 * t.cc
 *
 *  Created on: Oct 13, 2013
 *      Author: keevi7c3l
 */

#include <stdio.h>

int strcmpI(const char *s1, const char * s2) {
	char *t = (char *) malloc(strlen(s1) + 1);
	int i = 0;
	while (*(s1 + i) != '\0') {
		*(t + i) = tolower(*(s1 + i));
		i++;
	}
	t[i] = '\0';
	return strcmp(t, s2);
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

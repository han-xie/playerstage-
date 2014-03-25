#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
//test

int main() {

	int outputlog = open("kelog", O_RDWR | O_CREAT | O_TRUNC);

	char buf[] = "porcessMessage PLAYER_DIO_CMD_STATE \n";
	printf("write bytes : %d \n",write(outputlog, buf, sizeof(buf)));
	close(outputlog);

	return 0;
}

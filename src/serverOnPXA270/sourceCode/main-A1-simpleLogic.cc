/*
 * main.cc
 *
 *  Created on: Sep 12, 2013
 *      Author: keevi7c3l
 */

#include "apue.h"
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h> // fork, close
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <libplayerc++/playerc++.h>
#include "CYZXInter.h"

#define MAXLINE 4096
#define DEBUGV 1
#define MAXWAITTIME 5

int inputNumber = 0;
int exitStatus = 0;
char commandline[4096];
pthread_t ntid;
pthread_mutex_t globalLock;
int listenfd, connfd;
struct sockaddr_in servaddr;
//char buff[4096];
int sockfdc;
char recvlinec[4096];
//char sendlinec[4096];
struct sockaddr_in servaddrc;

/*below all about CYZX*/
char ips[16];
int playerClientPort;
int opaqueProxyIndex;
CYZXInter *pcyzxic;

/*below all about  buffer*/
#define MAXBUFFERBLOCKUNMBER 20
#define OCCUPY 0           //'\0'
#define UNOCCUPY 1         //'\x01'
pthread_mutex_t mutexOccupyTable = PTHREAD_MUTEX_INITIALIZER;
char occupyT[MAXBUFFERBLOCKUNMBER];
int sock[MAXBUFFERBLOCKUNMBER];
pthread_mutex_t mutexBuffer[MAXBUFFERBLOCKUNMBER];
char buffer[MAXBUFFERBLOCKUNMBER][MAXLINE];

/*below all about demoF's data structure unblockList/blockList*/
//#define MAXLISTUNMBER 20 //MAXLISTUNMBER = MAXBUFFERBLOCKUNMBER
#define MAXLISTUNMBER  MAXBUFFERBLOCKUNMBER
int inputMutexListUNB[MAXLISTUNMBER + 1];
int inputMutexListB[MAXLISTUNMBER + 1];
int UNBHead;
int UNBTail;
int BHead;
int BTail;
pthread_mutex_t mutexListTable = PTHREAD_MUTEX_INITIALIZER;
#define BLOCKMODE 1
#define UNBLOCKMODE 0
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
int requestNum;

int inputDemoFList(int index, int blockOrNot);
int getOneFromDemoFList(int ListType);
int getOneFromDemoFList();

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

int strTpara(const char *s, int paraNum, int *pr) {
	int length = strlen(s);
	int i = 0, j = 0;

	for (i = 0; i < paraNum; i++) {
		while ((*(s + j) < '0') || (*(s + j) > '9')) {
			j++;
		}
		if (j > length)
			return -1;
		*(pr + i) = atoi((s + j));
#ifdef DEBUGV
		printf("%d parameter is :%d\n", i, *(pr + i));
#endif
		while ((*(s + j) >= '0') && (*(s + j) <= '9')) {
			j++;
		}
	}
	return 0;
}
//忽略大小写，s1转化成小写，s2匹配字符串所以都是小写
int strcmpI(const char *s1, const char * s2) {
	char *t = (char *) malloc(strlen(s1) + 1);
	int i = 0;
	while (*(s1 + i) != '\0') {
		*(t + i) = tolower(*(s1 + i));
		i++;
	}
	t[i] = '\0';
	i = strcmp(t, s2);
	free(t);
	return i;
}
int strcmpIA(const char *s1, int from, int to, const char *s2) {
	char *t = (char *) malloc(to - from + 2);
	if (t == NULL) {
		printf("malloc error!\n");
		exit(0);
	}
	t[to - from + 1] = '\0';
	int i = 0, j = 0;
	for (i = from; i <= to; i++, j++) {
		t[j] = s1[i];
	}
	i = strcmpI(t, s2);
	return i;
}

void CYZXInit() {

	//char ts[16] = { 'l', 'o', 'c', 'a', 'l', 'h', 'o', 's', 't', '\0' };
	//char ips[16] = { '1', '9', '2', '.', '1', '6', '8', '.', '0', '.', '1', '1','1', '\0' };
	ips[0] = '1';
	ips[1] = '9';
	ips[2] = '2';
	ips[3] = '.';
	ips[4] = '1';
	ips[5] = '6';
	ips[6] = '8';
	ips[7] = '.';
	ips[8] = '0';
	ips[9] = '.';
	ips[10] = '1';
	ips[11] = '1';
	ips[12] = '1';
	ips[13] = '\0';
	ips[14] = '\0';
	ips[15] = '\0';
	playerClientPort = 6666;
	opaqueProxyIndex = 0;

	/*
	 PlayerClient robot(ips, playerClientPort);
	 OpaqueProxy opaquep(&robot, opaqueProxyIndex);
	 cyzxic(&robot, &opaquep);
	 */
}

/*below is :
 * get one buffer block + put back one buffer block*/
void bufferInit() {
	pthread_mutex_lock(&mutexOccupyTable);
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		if (pthread_mutex_init(mutexBuffer + i, NULL) != 0) {
			printf("can't init the mutex exit!\n");
			exit(0);
		}
	}
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		occupyT[i] = UNOCCUPY;
	}
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		pthread_mutex_lock(mutexBuffer + i);
	}
	pthread_mutex_unlock(&mutexOccupyTable);
}
void bufferDestroy() {
	pthread_mutex_lock(&mutexOccupyTable);
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		pthread_mutex_destroy(mutexBuffer + i);
	}
	pthread_mutex_unlock(&mutexOccupyTable);
}
int getOneBuffer() {
	pthread_mutex_lock(&mutexOccupyTable);
	for (int i = 0; i < MAXBUFFERBLOCKUNMBER; i++) {
		if (UNOCCUPY == occupyT[i]) {
			occupyT[i] = OCCUPY;
			pthread_mutex_unlock(&mutexOccupyTable);
			return i;
		}
	}
	pthread_mutex_unlock(&mutexOccupyTable);
	return -1;
}
int putbackBuffer(int index) {

	pthread_mutex_lock(&mutexOccupyTable);

#ifdef DEBUGV
	printf("putbackBuffer get mutexOccupyTable,index=%d\n", index);
#endif
	if (UNOCCUPY == occupyT[index]) {
		printf("wrong in putbackBuffer ,index=%d\n", index);
		exit(0);
	}
	occupyT[index] = UNOCCUPY;

	pthread_mutex_unlock(&mutexOccupyTable);
	return 0;
}

/*below is demoFunction
 * get one from List no matter block/unblock
 * deal the command stream*/
int submitToDemoF(int index, int blockOrNot) {
	int i = 0;

	if (BLOCKMODE == blockOrNot) {
		while (-1 == inputDemoFList(index, BLOCKMODE)) {
			if (i > MAXWAITTIME)
				return -1;
			sleep(++i);
		}
#ifdef DEBUGV
		printf("block mode ,submit the request,index=%d\n", index);
#endif

		pthread_mutex_lock(&qlock);
		if (requestNum >= MAXLISTUNMBER) {
#ifdef DEBUGV
			printf("can't submitToDemoF because full,number=%d\n", requestNum);
#endif
			pthread_mutex_unlock(&qlock);
			pthread_cond_signal(&qready);
			return -1;
		}
		requestNum++;
#ifdef DEBUGV
		printf("submitToDemoF, now number is=%d\n", requestNum);
#endif
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&qready);

		pthread_mutex_lock(&mutexBuffer[index]);
#ifdef DEBUGV
		printf("block mode ,the request get response ,index=%d\n", index);
#endif
	} else if (UNBLOCKMODE == blockOrNot) {
		while (-1 == inputDemoFList(index, UNBLOCKMODE)) {
			if (i > MAXWAITTIME)
				return -1;
			sleep(++i);
		}
		pthread_mutex_lock(&qlock);
		if (requestNum >= MAXLISTUNMBER) {
#ifdef DEBUGV
			printf("can't submitToDemoF because full-unblock,number=%d\n",
					requestNum);
#endif
			pthread_mutex_unlock(&qlock);
			pthread_cond_signal(&qready);
			return -1;
		}
		requestNum++;
#ifdef DEBUGV
		printf("submitToDemoF-unblock, now number is=%d\n", requestNum);
#endif
		pthread_mutex_unlock(&qlock);
		pthread_cond_signal(&qready);
	} else {
		return -1;
	}

	return i;
}
void demoFListInit() {
	pthread_mutex_lock(&mutexListTable);
	UNBHead = 0;
	UNBTail = 0;
	BHead = 0;
	BTail = 0;
	requestNum = 0;
	pthread_mutex_unlock(&mutexListTable);

	return;
}
void demoFListDestroy() {
	return;
}
int inputDemoFList(int index, int blockOrNot) { //erro return -1;
	int temp = -1;
	pthread_mutex_lock(&mutexListTable);
	if (BLOCKMODE == blockOrNot) {
		if (((BTail + 1) % (MAXBUFFERBLOCKUNMBER + 1)) == BHead) {  //full
#ifdef DEBUGV
			printf("cycle-block-buffer full tail%d head%d\n", BTail, BHead);
#endif
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		} else {
			inputMutexListB[BTail] = index;
			BTail = (BTail + 1) % (MAXBUFFERBLOCKUNMBER + 1);
			temp = index;
#ifdef DEBUGV
			printf("in inputDemoFList BTail=%d,index=%d\n", BTail, index);
			printf("cycle-block-buffer not full\n");
#endif
		}
	} else if (UNBLOCKMODE == blockOrNot) {
		if (((UNBTail + 1) % (MAXBUFFERBLOCKUNMBER + 1)) == UNBHead) { //full
#ifdef DEBUGV
			printf("cycle-unblock-buffer full tail%d head%d\n", UNBTail,
					UNBHead);
#endif
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		} else {
			inputMutexListUNB[UNBTail] = index;
			UNBTail = (UNBTail + 1) % (MAXBUFFERBLOCKUNMBER + 1);
			temp = index;
#ifdef DEBUGV
			printf("in inputDemoFList UNBTail=%d,index=%d\n", UNBTail, index);
			printf("cycle-block-buffer not full\n");
#endif
		}
	} else {
		pthread_mutex_unlock(&mutexListTable);
		return -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return temp;
}
int getOneFromDemoFList() {  //erro return -1;
	int temp = -1;
	pthread_mutex_lock(&mutexListTable);
	if (BHead != BTail) {
#ifdef DEBUGV
		printf("get-one-block-buffer\n");
#endif
		temp = inputMutexListB[BHead];
		BHead = (BHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
	} else if (UNBHead != UNBTail) {
#ifdef DEBUGV
		printf("get-one-unblock-buffer\n");
#endif
		temp = inputMutexListUNB[UNBHead];
		UNBHead = (UNBHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
	} else {
#ifdef DEBUGV
		printf("not get-one-buffer\n");
#endif
		temp = -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return temp;
}
int getOneFromDemoFList(int ListType) { //erro return -1;
	int temp = -1;
#ifdef DEBUGV
	printf("in getOneFromDemoFList(int)\n");
#endif
	pthread_mutex_lock(&mutexListTable);
	if (UNBLOCKMODE == ListType) {
#ifdef DEBUGV
		printf(
				"in getOneFromDemoFList(int)-unblock,BHead=%d,BTail=%d,ListType=%d\n",
				BHead, BTail, ListType);
#endif
		if (UNBHead == UNBTail) {
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		}
		temp = inputMutexListUNB[UNBHead];
		UNBHead = (UNBHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
		pthread_mutex_unlock(&mutexListTable);
		return temp;
	} else if (BLOCKMODE == ListType) {
#ifdef DEBUGV
		printf(
				"in getOneFromDemoFList(int)-block,BHead=%d,BTail=%d,ListType=%d\n",
				BHead, BTail, ListType);
#endif

		if (BHead == BTail) {
			pthread_mutex_unlock(&mutexListTable);
			return -1;
		}
		temp = inputMutexListB[BHead];
		BHead = (BHead + 1) % (MAXBUFFERBLOCKUNMBER + 1);
		pthread_mutex_unlock(&mutexListTable);
		return temp;
	} else {
		pthread_mutex_unlock(&mutexListTable);
		return -1;
	}
	pthread_mutex_unlock(&mutexListTable);
	return -1;
}
int demoFCYZXIbkMFSetServo(int index) {
	/*
	 X void bkMFSetServoMode(int inID,int inMode);
	 X void bkMFSetServoPos(int inID,int inPos,int inSpeed);
	 X void bkMFSetServoRotaSpd(int inID,int inSpeed);
	 */
	int rv;

	if ((0 == strcmpIA(buffer[index], 17, 23, "rotaspd"))
			&& (buffer[index][24] == '(' || buffer[index][24] == ' ')) {
		int ti[2] = { 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 24, 2, tp);
		pcyzxic->bkMFSetServoRotaSpd(*(tp + 0), *(tp + 1));
		buffer[index][0] = '\0';
	} else if ((0 == strcmpIA(buffer[index], 17, 19, "pos"))
			&& (buffer[index][20] == '(' || buffer[index][20] == ' ')) {

		int ti[3] = { 0, 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 20, 3, tp);
		pcyzxic->bkMFSetServoPos(*(tp + 0), *(tp + 1), *(tp + 2));
		buffer[index][0] = '\0';
	} else if ((0 == strcmpIA(buffer[index], 17, 20, "mode"))
			&& (buffer[index][21] == '(' || buffer[index][21] == ' ')) {

		int ti[2] = { 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 21, 2, tp);
		pcyzxic->bkMFSetServoMode(*(tp + 0), *(tp + 1));
		buffer[index][0] = '\0';
	} else {
		rv = -1;
	}

	return rv;
}
int demoFCYZXIbk(int index) {
	int rv;

	/*
	 X void bkLCDdisp(const char*item); 	//LCD
	 X void bkUSonicEnable();    	//Ultrasonic
	 X int  bkGetUltrasonic();        //Ultrasonic
	 X void bkMp3Play(const char*item);          	//mp3
	 X void bkMFServoAction();
	 * */
	if (0 == strcmpIA(buffer[index], 7, 16, "mfsetservo")) {
		rv = demoFCYZXIbkMFSetServo(index);
	} else if ((0 == strcmpIA(buffer[index], 7, 19, "getultrasonic"))
			&& (buffer[index][20] == '(' || buffer[index][20] == ' ')) {
		int tdis = pcyzxic->bkGetUltrasonic();
		itoa(tdis, buffer[index], 10);
	} else if ((0 == strcmpIA(buffer[index], 7, 13, "mp3play"))
			&& (buffer[index][14] == '(' || buffer[index][14] == ' ')) {
		char pmp3title[40];
		int i = 14;
		while (buffer[index][i] != '\"') {
			i++;
		}
		int length = strlen(buffer[index]);
		int j = length - 1;
		while (buffer[index][j] != '\"') {
			j--;
		}
		int k;
		int k1 = 0;
		for (k = i + 1; k < j; k++, k1++)
			pmp3title[k1] = buffer[index][k];
		pmp3title[k1] = '\0';
#ifdef DEBUGV
		printf("mp3 title:%s\n", pmp3title);
#endif
		pcyzxic->bkMp3Play(pmp3title);
		buffer[index][0] = '\0';
	} else if ((buffer[index][7] == 'L' || buffer[index][7] == 'l')
			&& (buffer[index][8] == 'C' || buffer[index][8] == 'c')
			&& (buffer[index][9] == 'D' || buffer[index][9] == 'd')
			&& (buffer[index][10] == 'D' || buffer[index][10] == 'd')
			&& (buffer[index][11] == 'I' || buffer[index][11] == 'i')
			&& (buffer[index][12] == 'S' || buffer[index][12] == 's')
			&& (buffer[index][13] == 'P' || buffer[index][13] == 'p')
			&& (buffer[index][14] == '(' || buffer[index][14] == ' ')) {
		char lcddisptitle[20];
		int i = 14;
		while (buffer[index][i] != '\"') {
			i++;
		}
		int length = strlen(buffer[index]);
		int j = length - 1;
		while (buffer[index][j] != '\"') {
			j--;
		}
		int k;
		int k1 = 0;
		for (k = i + 1; k < j; k++, k1++)
			lcddisptitle[k1] = buffer[index][k];
		lcddisptitle[k1] = '\0';
#ifdef DEBUGV
		printf("mp3 title:%s\n", lcddisptitle);
#endif
		pcyzxic->bkLCDdisp(lcddisptitle);
		buffer[index][0] = '\0';
	} else if ((buffer[index][7] == 'M' || buffer[index][7] == 'm')
			&& (buffer[index][8] == 'F' || buffer[index][8] == 'f')
			&& (buffer[index][9] == 'S' || buffer[index][9] == 's')
			&& (buffer[index][10] == 'E' || buffer[index][10] == 'e')
			&& (buffer[index][11] == 'R' || buffer[index][11] == 'r')
			&& (buffer[index][12] == 'V' || buffer[index][12] == 'v')
			&& (buffer[index][13] == 'O' || buffer[index][13] == 'o')
			&& (buffer[index][14] == 'A' || buffer[index][14] == 'a')
			&& (buffer[index][15] == 'C' || buffer[index][15] == 'c')
			&& (buffer[index][16] == 'T' || buffer[index][16] == 't')
			&& (buffer[index][17] == 'I' || buffer[index][17] == 'i')
			&& (buffer[index][18] == 'O' || buffer[index][18] == 'o')
			&& (buffer[index][19] == 'N' || buffer[index][19] == 'n')
			&& (buffer[index][20] == '(' || buffer[index][20] == ' ')) {
		pcyzxic->bkMFServoAction();
		buffer[index][0] = '\0';
	} else if ((buffer[index][7] == 'U' || buffer[index][7] == 'u')
			&& (buffer[index][8] == 'S' || buffer[index][8] == 's')
			&& (buffer[index][9] == 'O' || buffer[index][9] == 'o')
			&& (buffer[index][10] == 'N' || buffer[index][10] == 'n')
			&& (buffer[index][11] == 'I' || buffer[index][11] == 'i')
			&& (buffer[index][12] == 'C' || buffer[index][12] == 'c')
			&& (buffer[index][13] == 'E' || buffer[index][13] == 'e')
			&& (buffer[index][14] == 'N' || buffer[index][14] == 'n')
			&& (buffer[index][15] == 'A' || buffer[index][15] == 'a')
			&& (buffer[index][16] == 'B' || buffer[index][16] == 'b')
			&& (buffer[index][17] == 'L' || buffer[index][17] == 'l')
			&& (buffer[index][18] == 'E' || buffer[index][18] == 'e')
			&& (buffer[index][19] == '(' || buffer[index][19] == ' ')) {
		pcyzxic->bkUSonicEnable();
		buffer[index][0] = '\0';
	} else {
		rv = -1;
	}

	return rv;
}
int demoFCYZXIdio(int index) {
	/*
	 void dioSetPortDirect(unsigned int inData);
	 X int dioGetDigiInput(int port);                  //can't get result return -1
	 X void dioDigiOutput(int port, int inVal);
	 */
	int rv;
	if ((0 == strcmpIA(buffer[index], 8, 17, "digioutput"))
			&& (buffer[index][18] == '(' || buffer[index][18] == ' ')) {
		int ti[2] = { 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 18, 2, tp);
		pcyzxic->dioDigiOutput(*(tp + 0), *(tp + 1));
		buffer[index][0] = '\0';
	} else if ((0 == strcmpIA(buffer[index], 8, 19, "getdigiinput"))
			&& (buffer[index][20] == '(' || buffer[index][20] == ' ')) {
		int ti[1] = { 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 20, 1, tp);
		int t = pcyzxic->dioGetDigiInput(*(tp + 0));
		itoa(t, buffer[index], 10);
	} else if ((0 == strcmpIA(buffer[index], 8, 20, "setportdirect"))
			&& (buffer[index][21] == '(' || buffer[index][21] == ' ')) {
		int ti[1] = { 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 21, 1, tp);
		pcyzxic->dioSetPortDirect(*(tp + 0));
		buffer[index][0] = '\0';
	} else {
		rv = -1;
	}
	return rv;
}
int demoFCYZXIaio(int index) {
	/*
	 * 	int aioComInfrared(int port);
	 int aioCom(int port);
	 */
	int rv;
	if ((0 == strcmpIA(buffer[index], 8, 10, "com"))
			&& (buffer[index][11] == '(' || buffer[index][11] == ' ')) {
		int ti[1] = { 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 11, 1, tp);
		int t = pcyzxic->aioCom(*(tp + 0));
		itoa(t, buffer[index], 10);
	} else if ((0 == strcmpIA(buffer[index], 8, 18, "cominfrared"))
			&& (buffer[index][19] == '(' || buffer[index][19] == ' ')) {
		int ti[1] = { 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 19, 1, tp);
		int t = pcyzxic->aioComInfrared(*(tp + 0));
		itoa(t, buffer[index], 10);
	} else {
		rv = -1;
	}

	return rv;
}
int demoFCYZXIpos2d(int index) {
	int rv;
	/*
	 void pos2dSet4Tires(int id1,int id2,int id3,int id4,int speed1,int speed2,int speed3,int speed4);
	 void pos2dSet4TiresPos(int id1,int id2,int id3,int id4,int pos,int speed);
	 */
	if ((0 == strcmpIA(buffer[index], 10, 18, "set4tires"))
			&& (buffer[index][19] == '(' || buffer[index][19] == ' ')) {

		int ti[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 19, 8, tp);
		pcyzxic->pos2dSet4Tires(*(tp + 0), *(tp + 1), *(tp + 2), *(tp + 3),
				*(tp + 4), *(tp + 5), *(tp + 6), *(tp + 7));
		buffer[index][0] = '\0';
	} else if ((0 == strcmpIA(buffer[index], 10, 21, "set4tirespos"))
			&& (buffer[index][22] == '(' || buffer[index][22] == ' ')) {
		int ti[6] = { 0, 0, 0, 0, 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 22, 6, tp);
		pcyzxic->pos2dSet4TiresPos(*(tp + 0), *(tp + 1), *(tp + 2), *(tp + 3),
				*(tp + 4), *(tp + 5));
		buffer[index][0] = '\0';
	} else {
		rv = -1;
	}

	return rv;
}
int demoFCYZXIcap(int index) {
	int rv;
	/*void capSetH(int inHmax, int inHmin);
	 void capSetS(int inSmax, int inSmin);
	 void capSetI(int inImax, int inImin);
	 int capGetCenterX();
	 int capGetCenterY();
	 int capGetSum();
	 */
	if (0 == strcmpIA(buffer[index], 8, 21, "capgetcenterx")) {
		int t = pcyzxic->capGetCenterX();
		itoa(t, buffer[index], 10);
	} else if (0 == strcmpIA(buffer[index], 8, 21, "capgetcentery")) {
		int t = pcyzxic->capGetCenterY();
		itoa(t, buffer[index], 10);
	} else if ((0 == strcmpIA(buffer[index], 8, 16, "capgetsum"))
			&& (buffer[index][17] == '(' || buffer[index][17] == ' ')) {
		int t = pcyzxic->capGetSum();
		itoa(t, buffer[index], 10);
	} else if ((0 == strcmpIA(buffer[index], 8, 14, "capseth"))
			&& (buffer[index][15] == '(' || buffer[index][15] == ' ')) {
		int ti[2] = { 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 15, 2, tp);
		pcyzxic->capSetH(*(tp + 0), *(tp + 1));
		buffer[index][0] = '\0';
	} else if ((0 == strcmpIA(buffer[index], 8, 14, "capsets"))
			&& (buffer[index][15] == '(' || buffer[index][15] == ' ')) {
		int ti[2] = { 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 15, 2, tp);
		pcyzxic->capSetS(*(tp + 0), *(tp + 1));
		buffer[index][0] = '\0';
	} else if ((0 == strcmpIA(buffer[index], 8, 14, "capseti"))
			&& (buffer[index][15] == '(' || buffer[index][15] == ' ')) {
		int ti[2] = { 0, 0 };
		int *tp = &ti[0];
		strTpara(buffer[index] + 15, 2, tp);
		pcyzxic->capSetI(*(tp + 0), *(tp + 1));
		buffer[index][0] = '\0';
	} else {
		rv = -1;
	}
	return rv;
}
int demoFCYZXI(int index) {
	int rv;
#ifdef DEBUGV
	printf("comming command(now in demoFCYZXI):%s\n", buffer[index]);
#endif

	if (0 == strcmpIA(buffer[index], 5, 6, "bk")) {
		rv = demoFCYZXIbk(index);
	} else if (0 == strcmpIA(buffer[index], 5, 6, "sr")) {
		rv = -1;
	} else if (0 == strcmpIA(buffer[index], 5, 7, "dio")) {
		rv = demoFCYZXIdio(index);
	} else if (0 == strcmpIA(buffer[index], 5, 7, "aio")) {
		rv = demoFCYZXIaio(index);
	} else if (0 == strcmpIA(buffer[index], 5, 9, "pos2d")) {
		rv = demoFCYZXIpos2d(index);
	} else if (0 == strcmpIA(buffer[index], 5, 7, "cap")) {
		rv = demoFCYZXIcap(index);
	} else {
		rv = -1;
	}
	return rv;
}
int demoF1(int index) {
	int rv;
#ifdef DEBUGV
	printf("comming command(now in demoF):%s\n", buffer[index]);
#endif
	/*if ((buffer[index][0] == 'C' || buffer[index][0] == 'c')
	 && (buffer[index][1] == 'Y' || buffer[index][1] == 'y')
	 && (buffer[index][2] == 'Z' || buffer[index][2] == 'z')
	 && (buffer[index][3] == 'X' || buffer[index][3] == 'x')
	 && (buffer[index][4] == 'I' || buffer[index][4] == 'i'))*/
	if (0 == strcmpIA(buffer[index], 0, 4, "cyzxi")) {
		rv = demoFCYZXI(index);
	} else {
		rv = -1;
	}

#ifdef DEBUGV
	printf("command done(now in demoF):%s\n", buffer[index]);
#endif
	return rv;
}

int demoF2(int index, int bm) {
	if (UNBLOCKMODE == bm) {
		int t = demoF1(index);
		putbackBuffer(index);
		return t;
	} else if (BLOCKMODE == bm) {
		return demoF1(index);
	} else {
		return demoF1(index);
	}
}

int demoF() {
	int index;
	while (1) {
		pthread_mutex_lock(&qlock);
		while (requestNum == 0)
			pthread_cond_wait(&qready, &qlock);
#ifdef DEBUGV
		printf("demoF get one command ,so to deal it ,requestNum=%d\n",
				requestNum);
#endif
		requestNum--;
		pthread_mutex_unlock(&qlock);
#ifdef DEBUGV
		printf("after get one command,requestNum=%d\n", requestNum);
#endif
		if ((index = getOneFromDemoFList(UNBLOCKMODE)) != -1) {
			demoF2(index, UNBLOCKMODE);
		} else if ((index = getOneFromDemoFList(BLOCKMODE)) != -1) {
			demoF2(index, BLOCKMODE);
#ifdef DEBUGV
			printf("unlock before\n");
#endif
			pthread_mutex_unlock(&mutexBuffer[index]);
#ifdef DEBUGV
			printf("unlock after\n");
#endif
		}
	}
	return 0;
}

void * acceptInput(void *arg) {
	printf("command:\n");
	while (1) {
		fgets(commandline, 4096, stdin);
		int t = strlen(commandline);
		commandline[t - 1] = '\0';
		if (0 == strcmp(commandline, "exit")) {
			break;
		} else {
			continue;
		}
	}
#ifdef DEBUGV
	printf("acceptInput thread terminate\n");
#endif

	return 0;
}

int serverDetermineBlock(void *arg) {
	/*below all is BLOCKMODE
	 * CYZXI
	 * bkGet+
	 * SRGet+
	 * dioGet+
	 * aio+
	 * pos2dGet+
	 * capGet+
	 */
	char *t = (char *) arg;
	int rv = UNBLOCKMODE;
	if ((*(t + 0) != 'C' && *(t + 0) != 'c')
			|| (*(t + 1) != 'Y' && *(t + 1) != 'y')
			|| (*(t + 2) != 'Z' && *(t + 2) != 'z')
			|| (*(t + 3) != 'X' && *(t + 3) != 'x')
			|| (*(t + 4) != 'I' && *(t + 4) != 'i')) {
		return UNBLOCKMODE;
	}
	if ((*(t + 5) == 'B' || *(t + 5) == 'b')
			&& (*(t + 6) == 'K' || *(t + 6) == 'k')
			&& (*(t + 7) == 'G' || *(t + 7) == 'g')
			&& (*(t + 8) == 'E' || *(t + 8) == 'e')
			&& (*(t + 9) == 'T' || *(t + 9) == 't')) {
		rv = BLOCKMODE;
	} else if ((*(t + 5) == 'S' || *(t + 5) == 's')
			&& (*(t + 6) == 'R' || *(t + 6) == 'r')
			&& (*(t + 7) == 'G' || *(t + 7) == 'g')
			&& (*(t + 8) == 'E' || *(t + 8) == 'e')
			&& (*(t + 9) == 'T' || *(t + 9) == 't')) {
		rv = BLOCKMODE;
	} else if ((*(t + 5) == 'D' || *(t + 5) == 'd')
			&& (*(t + 6) == 'I' || *(t + 6) == 'i')
			&& (*(t + 7) == 'O' || *(t + 7) == 'o')
			&& (*(t + 8) == 'G' || *(t + 8) == 'g')
			&& (*(t + 9) == 'E' || *(t + 9) == 'e')
			&& (*(t + 10) == 'T' || *(t + 10) == 't')) {
		rv = BLOCKMODE;
	} else if ((*(t + 5) == 'A' || *(t + 5) == 'a')
			&& (*(t + 6) == 'I' || *(t + 6) == 'i')
			&& (*(t + 7) == 'O' || *(t + 7) == 'o')) {
		rv = BLOCKMODE;
	} else if ((*(t + 5) == 'P' || *(t + 5) == 'p')
			&& (*(t + 6) == 'O' || *(t + 6) == 'o')
			&& (*(t + 7) == 'S' || *(t + 7) == 's') && (*(t + 8) == '2')
			&& (*(t + 9) == 'D' || *(t + 9) == 'd')
			&& (*(t + 10) == 'G' || *(t + 10) == 'g')
			&& (*(t + 11) == 'E' || *(t + 11) == 'e')
			&& (*(t + 12) == 'T' || *(t + 12) == 't')) {
		rv = BLOCKMODE;
	} else if ((*(t + 5) == 'C' || *(t + 5) == 'c')
			&& (*(t + 6) == 'A' || *(t + 6) == 'a')
			&& (*(t + 7) == 'P' || *(t + 7) == 'p')
			&& (*(t + 8) == 'G' || *(t + 8) == 'g')
			&& (*(t + 9) == 'E' || *(t + 9) == 'e')
			&& (*(t + 9) == 'T' || *(t + 9) == 't')) {
		rv = BLOCKMODE;
	} else {
		rv = UNBLOCKMODE;
	}

	return rv;
}
void * thr_serverTCP(void *arg) {
	int n;
	int err;
	int index;
	int tmpConnfd = *(int *) arg;
	int bmode;

#ifdef DEBUGV
	printf("tmpConnfd is :%d\n", tmpConnfd);
#endif

	//get one buffer
	while (-1 == (index = getOneBuffer())) {
		sleep(1);
	}
#ifdef DEBUGV
	printf("get one buffer:%d\n", index);
#endif

	while (1) {
		//recv message from client
		if ((n = recv(tmpConnfd, buffer[index], MAXLINE, 0)) > 0) {
			buffer[index][n] = '\0';
#ifdef DEBUGV
			printf("recv msg from client: %s\n", buffer[index]);
#endif
		} else {
			break;
		}

//submit to demoF to handle message
#ifdef DEBUGV
		printf("DemoF1\n");
#endif
		demoF1(index);
#ifdef DEBUGV
		printf("DemoF1 over !\n");
#endif

//so get the result
#ifdef DEBUGV
		printf("the result message after submit to demoF:%s\n", buffer[index]);
#endif
		if (buffer[index][0] != '\0') {
			if (send(tmpConnfd, buffer[index], strlen(buffer[index]), 0) < 0) {
				printf("send msg error: %s(errno: %d)\n", strerror(errno),
						errno);
			}
		}
	}

	//put back the buffer
#ifdef DEBUGV
	printf("putbackBuffer index=%d over\n", index);
#endif
	putbackBuffer(index);

	close(tmpConnfd);

	return ((void*) 0);
}
void * thr_serverWork(void *arg) {
	int err;
	int *pconnfd = &connfd;

	while (1) {
		if ((connfd = accept(listenfd, (struct sockaddr*) NULL, NULL)) == -1) {
			printf("accept socket error: %s(errno: %d)", strerror(errno),
					errno);
		}

#ifdef DEBUGV
		printf("connfd:%d\n", connfd);
#endif

		err = pthread_create(&ntid, NULL, thr_serverTCP, (void *) pconnfd);
		if (err != 0)
			printf("can't create thread:%s\n", strerror(err));
	}

	return NULL;
}

void * thr_clientWork(void *arg) {

	/*
	 memset(&servaddrc, 0, sizeof(servaddrc));
	 servaddrc.sin_family = AF_INET;
	 servaddrc.sin_port = htons(7667);
	 if (inet_pton(AF_INET, "192.168.1.131", &servaddrc.sin_addr) <= 0) {
	 printf("inet_pton error for %s\n", "192.168.1.131");
	 exit(0);
	 }

	 if (connect(sockfdc, (struct sockaddr*) &servaddrc, sizeof(servaddrc))
	 < 0) {
	 printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
	 exit(0);
	 }
	 */

	while (1) {
		printf("read data: %s\n", commandline);
		sleep(2);
		break;
	}

	return NULL;
}

void * thr_demoF(void *arg) {
	demoF();
	return NULL;
}

void * initialAtBegin(void *arg) {

	int err;

//initial for server
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(7666);

	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if (listen(listenfd, 10) == -1) {
		printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

#ifdef DEBUGV
	printf("======waiting for client's request======\n");
#endif

//initial for client
	if ((sockfdc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	bufferInit();
	demoFListInit();

	CYZXInit();

	return NULL;
}

void * beforeReturn(void *arg) {
	close(listenfd);
	close(sockfdc);
	bufferDestroy();
	return NULL;
}

int main(int argc, char * argv[]) {
	using namespace PlayerCc;

	int err;
	void *t;

	initialAtBegin (NULL);

//CYZXInit();
	PlayerClient robot(&ips[0], playerClientPort);
	OpaqueProxy opaquep(&robot, opaqueProxyIndex);
	CYZXInter cyzxic(&robot, &opaquep);
	pcyzxic = &cyzxic;

//do server thing
	err = pthread_create(&ntid, NULL, thr_serverWork, NULL);
	if (err != 0) {
		printf("can't create server thread:%s\n", strerror(err));
		exit(0);
	}

//do client thing
	err = pthread_create(&ntid, NULL, thr_clientWork, NULL);
	if (err != 0) {
		printf("can't create client thread:%s\n", strerror(err));
		exit(0);
	}

#ifdef DEBUGV
	printf("three thread creative over\n");
#endif

//deal input
	acceptInput(NULL);

	beforeReturn(NULL);

	return 0;
}

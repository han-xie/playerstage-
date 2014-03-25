/*
 * CYZXInter.cc
 *
 *  Created on: Jul 19, 2013
 *      Author: keevi7c3l
 */

#include "CYZXInter.h"

#include <stdio.h>

#include <libplayerc/playerc.h>

int main(int argc, const char **argv) {
	int i;
	playerc_client_t *client;
	playerc_opaque_t *opaquec;

	// Create a client and connect it to the server.
	client = playerc_client_create(NULL, "192.168.0.111", 6666);
	if (0 != playerc_client_connect(client))
		return -1;

	printf("connect\n");

	// Create and subscribe to a opaque device.
	opaquec = playerc_opaque_create(client, 0);
	if (playerc_opaque_subscribe(opaquec, PLAYER_OPEN_MODE))
		return -1;

	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEAIO;
	sendComd.subtype = AIOCOMTF;
	sendComd.p1 = 0;
	sendComd.p2 = whatKindAIOAIODEF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;

	//scoped_lock_t lock(mPc->mMutex);
	player_opaque_data_t *aReply;
	/*= new player_opaque_data_t;
	 aReply->data = new uint8_t[popa.data_count + 1];
	 aReply->data[popa.data_count] = '\0';
	 */
	printf("send req !\n");

	int result = playerc_opaque_req(opaquec, &popa, &aReply);
	printf("send req over !\n");
	printf("data_count %d\n", aReply->data_count);
	for (i = 0; i < aReply->data_count; i++) {
		printf("%x ", aReply->data[i]);
	}
	printf("\nmemcpy\n");
	opaquec->data = new uint8_t[aReply->data_count + 1];
	if (result == 0) {
		memcpy(opaquec->data, aReply->data, aReply->data_count);
		opaquec->data_count = aReply->data_count;
	}
	printf("mencpy over\n");
	player_opaque_data_t_free(aReply);

	printf("over");

	// Shutdown
	playerc_opaque_unsubscribe(opaquec);
	playerc_opaque_destroy(opaquec);
	playerc_client_disconnect(client);
	playerc_client_destroy(client);

	return 0;
}


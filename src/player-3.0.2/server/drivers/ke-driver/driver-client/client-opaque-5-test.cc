#include <iostream>
#include <libplayerc++/playerc++.h>
#include <unistd.h>
#include "myOpaqueSt.h"

int main(int argc, char *argv[]) {
	using namespace PlayerCc;

	PlayerClient robot("192.168.0.111", 6666);
	OpaqueProxy opaquep(&robot, 0);

	/*  /// Opaque data
	 void GetData(uint8_t* aDest) const
	 {
	 return GetVarByRef(mDevice->data,
	 mDevice->data+GetVar(mDevice->data_count),
	 aDest);
	 };

	 /// Send a command
	 void SendCmd(player_opaque_data_t* aData);

	 /// Send a request
	 int SendReq(player_opaque_data_t* aRequest);
	 * */

	/*type -1 opaque
	 *
	 *  void MFCapSetH(int inHmax,int inHmin);  -subtype 1
	 void MFCapSetS(int inSmax,int inSmin);  -2
	 void MFCapSetI(int inImax,int inImin);  -3
	 void MFCapSetMode(int inMode);          -4
	 int MFCapGetCenterX();                  -5
	 int MFCapGetCenterY();                  -6
	 int MFCapGetSum();                      -7
	 * */

	for (int i = 0; i < 100000; i++) {
	}

	myOpaqueSt topa;
	player_opaque_data_t popa;

	sleep(5);

	topa.type = 1;
	topa.subtype = 1;
	topa.p1 = 50;
	topa.p2 = 0;
	popa.data_count = sizeof(myOpaqueSt);
	popa.data = (uint8_t *) &topa;
	opaquep.SendCmd(&popa);
	for (int i = 0; i < popa.data_count; i++) {
		printf("%x ", popa.data[i]);
	}
	printf("\n");
	printf("MFCapSetH:command 1 1 50 0 size:%d\n", popa.data_count);

	sleep(3);

	topa.type = 1;
	topa.subtype = 2;
	topa.p1 = 255;
	topa.p2 = 60;
	popa.data = (uint8_t *) &topa;
	opaquep.SendCmd(&popa);
	for (int i = 0; i < popa.data_count; i++) {
		printf("%x ", popa.data[i]);
	}
	printf("\n");
	printf("MFCapSetS:command 1 2 255 60 size:%d\n", popa.data_count);

	sleep(2);

	printf("test opaque over\n");

}


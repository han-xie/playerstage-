/*
 * CYZXInter.cc
 *
 *  Created on: Jul 19, 2013
 *      Author: keevi7c3l
 */

#include "CYZXInter.h"
#include <iostream>
#include <libplayerc++/playerc++.h>
#include <libplayerc++/playerclient.h>
#include <libplayercore/configfile.h>

using namespace PlayerCc;

char ConvertIntToChar[51] = { '\0', '\x01', '\x02', '\x03', '\x04', '\x05',
		'\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e',
		'\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
		'\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', '\x20',
		'\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28', '\x29',
		'\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f', '\x30', '\x31', '\x32' };

/*
 * class mlist{
 public:
 mlist();
 mlist(int v);
 void push_back(int v);
 int at(int index);
 private:
 int val;
 mlist *next;
 };
 * */
mlist::mlist() {
	this->val = -1;
	this->next = NULL;
}
mlist::mlist(int v) {
	this->val = v;
	this->next = NULL;
}
void mlist::push_back(int v) {
	mlist *trav = this;
	if (this->val == -1) {
		this->val = v;
		return;
	}
	mlist *mlt = new mlist(v);
	while (trav->next != NULL) {
		trav = trav->next;
	}
	trav->next = mlt;
}
int mlist::at(int index) {
	mlist *trav = this;
	int i;
	for (i = 0; i < index; i++) {
		if (trav->next == NULL)
			break;
		trav = trav->next;
	}
	if ((i == index) && (trav != NULL))
		return trav->val;
	return -1;
}

/*
 class mConfigFile{
 public:
 mConfigFile();
 int load(const char *filename);
 int GetTupleCount(const char *s);
 int ReadTupleInt(const char *s,int index);
 int ReadString(const char *s,char *rs);
 private:
 std::map<std::string,std::string,classcomp> tsadports;
 };
 */

mConfigFile::mConfigFile() {

	/*std::map<std::string, std::string>::iterator it = this->tsadports.begin();

	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("infrProxSen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("colliSen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("soundSen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("gestSen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("hallSen", "(-1)"));

	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "dout", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO0", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO1", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO2", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO3", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO4", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO5", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("DIO6", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO7", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO8", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO9", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO10", "in"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>( "DIO11", "in"));

	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("infrDistSen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("tempSen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("graySen", "(-1)"));
	 this->tsadports.insert(it,
	 std::pair<std::string, std::string>("lightSen", "(-1)"));
	 */

	this->tsadports.insert(
			std::pair<std::string, std::string>("infrProxSen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("colliSen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("soundSen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("gestSen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("hallSen", "(-1)"));

	this->tsadports.insert(std::pair<std::string, std::string>("dout", "(-1)"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO0", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO1", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO2", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO3", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO4", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO5", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO6", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO7", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO8", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO9", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO10", "in"));
	this->tsadports.insert(std::pair<std::string, std::string>("DIO11", "in"));

	this->tsadports.insert(
			std::pair<std::string, std::string>("infrDistSen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("tempSen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("graySen", "(-1)"));
	this->tsadports.insert(
			std::pair<std::string, std::string>("lightSen", "(-1)"));

	this->tsadports.insert(
			std::pair<std::string, std::string>("servoDirect", "0"));
}

int mConfigFile::dealAline(const char *s) {
	int i = 0;
	while (*(s + i) == ' ') {
		i++;
	}
	if (*(s + i) == '#') {
		return 0;
	}
	char ts[20];
	int j = 0;
	while (*(s + i) != ' ') {
		ts[j] = *(s + i);
		j++;
		i++;
	}
	ts[j] = 0;
	std::map<std::string, std::string>::iterator it;
	if ((it = this->tsadports.find(ts)) != tsadports.end()) {
		it->second = (std::string) (s + i);
#ifdef WRITELOGC
		printf("%s , %s\n",&ts[0],s+i);
#endif
		return 0;
	} else {
		if (strlen(&ts[0]) > 3)
			printf("%s don't exist\n", &ts[0]);
	}

	return -1;
}

int mConfigFile::load(const char *filename) {
	FILE *fl;
	if ((fl = fopen(SENSCFGNAME, "r")) == NULL) {
		printf("open file ");
		printf(SENSCFGNAME);
		printf(" failed\n");
		return -1;
	}
	char ts[100];
	while (fgets(&ts[0], 100, fl) != NULL) {
		this->dealAline(&ts[0]);
	}

	fclose(fl);
	return 1;
}
int mConfigFile::count(std::string s) {
	int i = 0;
	int count = 1;
	int slength = s.length();
	while ((s[i] == ' ') && (i < slength)) {
		i++;
	}
	if (s[i] == '(') {
		i++;
		while ((s[i] != ')') && (i < slength)) {
			if (s[i] == ',')
				count++;
			i++;
		}
		return count;
	} else {
		printf("format wrong : param begin with (\n");
	}
	return -1;
}
int mConfigFile::GetTupleCount(const char *s) {
	std::map<std::string, std::string>::iterator it;
	if ((it = this->tsadports.find(s)) != tsadports.end()) {
		return (this->count(it->second));
	} else {
		printf("can't find the %s\n", s);
	}
	return -1;
}
int mConfigFile::atoiIndex(const char *s, int index) {
	int i = 0;
	while ((*(s + i) != '(') && (*(s + i) != '\0'))
		i++;
	int j = 0;
	for (j = 0; j < index; j++) {
		i++;
		while (*(s + i) != ',')
			i++;
	}
	i++;
	return atoi(s + i);
}
int mConfigFile::ReadTupleInt(const char *s, int index) {
	std::map<std::string, std::string>::iterator it;
	if ((it = this->tsadports.find(s)) != tsadports.end()) {
		return (this->atoiIndex(it->second.data(), index));
	} else {
		printf("can't find the %s\n", s);
	}
	return -1;
}
char* mConfigFile::ReadString(const char *s, char *rs) {
	const char *ts;
	int i = 0;
	int j = 0;
	std::map<std::string, std::string>::iterator it;
	if ((it = this->tsadports.find(s)) != tsadports.end()) {
		ts = it->second.data();
		for (i = 0; i < strlen(ts); i++) {
			if ((*(ts + i) != ' ') && (*(ts + i) != '\"')
					&& (*(ts + i) != '\n')) {
				rs[j] = ts[i];
				j++;
			}
		}
		return rs;
	} else {
		printf("can't find the %s\n", s);
	}
	return rs;
}

void OpaqueProxy::newSizeFormDevice(int size) {
	if (NULL == this->mDevice->data)
		this->mDevice->data = new uint8_t[size];
}

void OpaqueProxy::newFormDevice(int size) {
}

CYZXInter::CYZXInter() {
	this->robot = NULL;
	this->diop = NULL;
	this->aiop = NULL;
	this->opaquep = NULL;
	this->posp = NULL;
	this->DIOPortDirect = 0x0;
	/*
	this->pc("localhost",6665);
	this->robot=&(this->pc);
	this->op(this->robot,this->pc,0);
	this->opaquep=&(this->op);
	*/

}
CYZXInter::CYZXInter(PlayerClient *probot) {
	this->robot = probot;
	this->DIOPortDirect = 0x0;

}
CYZXInter::CYZXInter(DioProxy *pdiop) {
	this->diop = pdiop;
	this->DIOPortDirect = 0x0;

}
CYZXInter::CYZXInter(AioProxy *paiop) {
	this->aiop = paiop;
	this->DIOPortDirect = 0x0;

}
CYZXInter::CYZXInter(OpaqueProxy *popaquep) {
	this->opaquep = popaquep;
	this->DIOPortDirect = 0x0;

}
CYZXInter::CYZXInter(Position2dProxy *pposp) {
	this->posp = pposp;
	this->DIOPortDirect = 0x0;

}
CYZXInter::CYZXInter(PlayerClient *probot, OpaqueProxy *popaquep) {
	this->robot = probot;
	this->opaquep = popaquep;

	this->frontl = 1;
	this->frontr = 1;
	this->rearl = 1;
	this->rearr = 1;

	int i = 0, j = 0, k = 0;
	mlist *pml = (mlist *) new mlist();
	this->DIOoutport = pml;
	for (i = 0; i < SENSNUM + 1; i++) {
		mlist *pml = (mlist *) new mlist();
		this->senl[i] = pml;
	}
	for (i = 0; i < SENSAIONUM + 1; i++) {
		mlist *pml = (mlist *) new mlist();
		this->senlAIO[i] = pml;
	}

	mConfigFile cf;
	int section = 0;

	char temps[10];
	int tempi = 0;
	char* sens[SENSNUM];
	sens[0] = (char*) SENS0;
	sens[1] = (char*) SENS1;
	sens[2] = (char*) SENS2;
	sens[3] = (char*) SENS3;
	sens[4] = (char*) SENS4;
	//sens[5][0] = '\0';
	char *sensAIO[SENSAIONUM];
	sensAIO[0] = (char*) SENSAIO0;
	sensAIO[1] = (char*) SENSAIO1;
	sensAIO[2] = (char*) SENSAIO2;
	sensAIO[3] = (char*) SENSAIO3;
	//sensAIO[4][0] = '\0';
	if (cf.load(SENSCFGNAME)) {
		//read DIO configure
		for (i = 0; i < SENSNUM; i++) {
			j = cf.GetTupleCount(sens[i]);
#ifdef WRITELOG
			printf("%s is %d\n", sens[i], j);
#endif
			for (k = 0; k < j; k++) {
				tempi = -1;
				tempi = cf.ReadTupleInt(sens[i], k);
				if (tempi != -1)
					this->senl[i]->push_back(tempi);
#ifdef WRITELOG
				printf("%s is %d\n", sens[i], tempi);
#endif
			}
		}

		//read AIO configure
		for (i = 0; i < SENSAIONUM; i++) {
			j = cf.GetTupleCount(sensAIO[i]);
#ifdef WRITELOG
			printf("%s is %d\n", sensAIO[i], j);
#endif
			for (k = 0; k < j; k++) {
				tempi = -1;
				tempi = cf.ReadTupleInt(sensAIO[i], k);
				if (tempi != -1)
					this->senlAIO[i]->push_back(tempi);
#ifdef WRITELOG
				printf("%s is %d\n", sensAIO[i], tempi);
#endif
			}
		}

		//read DIO port director
		i = 0;
		this->DIOPortDirect = 0x0;
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO0", &temps[0]), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 1;
			this->DIOoutport->push_back(0);
		}
		temps[0] = '\0';

		if (0 != strcmp(cf.ReadString("DIO1", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 2;
			this->DIOoutport->push_back(1);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO2", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 4;
			this->DIOoutport->push_back(2);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO3", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 8;
			this->DIOoutport->push_back(3);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO4", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 16;
			this->DIOoutport->push_back(4);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO5", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 32;
			this->DIOoutport->push_back(5);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO6", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 64;
			this->DIOoutport->push_back(6);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO7", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 128;
			this->DIOoutport->push_back(7);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO8", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 256;
			this->DIOoutport->push_back(8);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO9", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 512;
			this->DIOoutport->push_back(9);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO10", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 1024;
			this->DIOoutport->push_back(10);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO11", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 2048;
			this->DIOoutport->push_back(11);
		}

		//read servoDirect
		temps[0] = '\0';
		cf.ReadString("servoDirect", temps);
		this->servoDirect = atoi(temps);

#ifdef WRITELOG
		printf("portDirect is %d\n", this->DIOPortDirect);
		printf("servoDirect is %d\n", this->servoDirect);
#endif

	} else {
		printf("load configure file failed,exit\n");
		exit(0);
	}

}
/*CYZXInter::CYZXInter(PlayerClient *probot, DioProxy *pdiop, AioProxy *paiop,
		OpaqueProxy *popaquep, Position2dProxy *pposp) {
	this->robot = probot;
	this->diop = pdiop;
	this->aiop = paiop;
	this->opaquep = popaquep;
	this->posp = pposp;
	this->DIOPortDirect = 0x0;

}*/

CYZXInter::CYZXInter(const std::string aHostname, uint32_t aPort,
		unsigned int index) {
	/*this->pc(aHostname.data(), aPort);
	 this->robot = &(this->pc);
	 this->op(&(this->pc),index);
	 this->opaquep = &(this->op);
	 */

	this->frontl = 1;
	this->frontr = 1;
	this->rearl = 1;
	this->rearr = 1;

	int i = 0, j = 0, k = 0;
	mlist *pml = (mlist *) new mlist();
	this->DIOoutport = pml;
	for (i = 0; i < SENSNUM + 1; i++) {
		mlist *pml = (mlist *) new mlist();
		this->senl[i] = pml;
	}
	for (i = 0; i < SENSAIONUM + 1; i++) {
		mlist *pml = (mlist *) new mlist();
		this->senlAIO[i] = pml;
	}

	mConfigFile cf;
	int section = 0;

	char temps[10];
	int tempi = 0;
	char* sens[SENSNUM];
	sens[0] = (char *) SENS0;
	sens[1] = (char *) SENS1;
	sens[2] = (char *) SENS2;
	sens[3] = (char *) SENS3;
	sens[4] = (char *) SENS4;
	//sens[5][0] = '\0';
	char *sensAIO[SENSAIONUM];
	sensAIO[0] = (char *) SENSAIO0;
	sensAIO[1] = (char *) SENSAIO1;
	sensAIO[2] = (char *) SENSAIO2;
	sensAIO[3] = (char *) SENSAIO3;
	//sensAIO[4][0] = '\0';
	if (cf.load(SENSCFGNAME)) {
		//read DIO configure
		for (i = 0; i < SENSNUM; i++) {
			j = cf.GetTupleCount(sens[i]);
#ifdef WRITELOG
			printf("%s is %d\n", sens[i], j);
#endif
			for (k = 0; k < j; k++) {
				tempi = -1;
				tempi = cf.ReadTupleInt(sens[i], k);
				if (tempi != -1)
					this->senl[i]->push_back(tempi);
#ifdef WRITELOG
				printf("%s is %d\n", sens[i], tempi);
#endif
			}
		}

		//read AIO configure
		for (i = 0; i < SENSAIONUM; i++) {
			j = cf.GetTupleCount(sensAIO[i]);
#ifdef WRITELOG
			printf("%s is %d\n", sensAIO[i], j);
#endif
			for (k = 0; k < j; k++) {
				tempi = -1;
				tempi = cf.ReadTupleInt(sensAIO[i], k);
				if (tempi != -1)
					this->senlAIO[i]->push_back(tempi);
#ifdef WRITELOG
				printf("%s is %d\n", sensAIO[i], tempi);
#endif
			}
		}

		//read DIO port director
		i = 0;
		this->DIOPortDirect = 0x0;
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO0", &temps[0]), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 1;
			this->DIOoutport->push_back(0);
		}
		temps[0] = '\0';

		if (0 != strcmp(cf.ReadString("DIO1", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 2;
			this->DIOoutport->push_back(1);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO2", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 4;
			this->DIOoutport->push_back(2);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO3", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 8;
			this->DIOoutport->push_back(3);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO4", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 16;
			this->DIOoutport->push_back(4);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO5", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 32;
			this->DIOoutport->push_back(5);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO6", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 64;
			this->DIOoutport->push_back(6);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO7", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 128;
			this->DIOoutport->push_back(7);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO8", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 256;
			this->DIOoutport->push_back(8);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO9", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 512;
			this->DIOoutport->push_back(9);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO10", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 1024;
			this->DIOoutport->push_back(10);
		}
		temps[0] = '\0';
		if (0 != strcmp(cf.ReadString("DIO11", temps), "in")) {
			this->DIOPortDirect = (this->DIOPortDirect) | 2048;
			this->DIOoutport->push_back(11);
		}

		//read servoDirect
		temps[0] = '\0';
		cf.ReadString("servoDirect", temps);
		this->servoDirect = atoi(temps);

#ifdef WRITELOG
		printf("portDirect is %d\n", this->DIOPortDirect);
		printf("servoDirect is %d\n", this->servoDirect);
#endif

	} else {
		printf("load configure file failed,exit\n");
		exit(0);
	}

}
CYZXInter::~CYZXInter() {
	//delete this->opaquep;
	//delete this->robot;
}
/*
 void CYZXInter::dioCom(int port) {
 if (this->diop != NULL) {
 this->diop->SetOutput(port, port);
 }
 return;
 }

 uint32_t CYZXInter::dioRes() {
 if (this->robot != NULL) {
 this->robot->Read();
 return this->diop->GetCount();
 }
 return 0;
 }
 */

int CYZXInter::aioComInfrared(int port) {
	return aioComT(port, whatKindAIOAIOINFRAREDR);
}
int CYZXInter::aioCom(int port) {
	return aioComT(port, whatKindAIOAIODEF);
}
int CYZXInter::aioComT(int port, int whatKindAIO) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEAIO;
	sendComd.subtype = AIOCOMTF;
	sendComd.p1 = port;
	sendComd.p2 = whatKindAIO;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respAIO) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}

//dio
void CYZXInter::dioSetPortDirect(unsigned int inData) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIOSETPORTDIRECTF;
	sendComd.p1 = inData;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::dioSetPortDirectOutput(unsigned int from, unsigned int to) { //others is input
	unsigned int inData = 0x00000000;
	if (from < DIOPORTSMIN)
		from = DIOPORTSMIN;
	if (to > DIOPORTSMAX)
		to = DIOPORTSMAX;
	for (int i = from; i <= to; i++) {
		inData |= (1 << i);
	}
#ifdef WRITELOGC
	printf("%x\n",inData);
#endif
	dioSetPortDirect(inData);
}
int CYZXInter::dioGetDigiInput(int port) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIOGETDIGIINPUTF;
	sendComd.p1 = port;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respDIO) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
void CYZXInter::dioDigiOutput(int port, int inVal) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEDIO;
	sendComd.subtype = DIODIGIOUTPUTF;
	sendComd.p1 = port;
	sendComd.p2 = inVal;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}

int CYZXInter::infrProxSen(unsigned int index) {
	int portindex = -1;
	portindex = (this->senl[infrProxSenLI])->at(index);
#ifdef WRITELOG
	printf("portindex : %d\n", portindex);
#endif
	if (portindex != -1) {
#ifdef DEBUGTEMP
		return this->dioGetDigiInput(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::colliSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senl[colliSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->dioGetDigiInput(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::lightSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senlAIO[lightSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->aioCom(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::soundSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senl[soundSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->dioGetDigiInput(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::gestSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senl[gestSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->dioGetDigiInput(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::graySen(unsigned int index) {
	int portindex = -1;
	portindex = this->senlAIO[graySenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d", portindex);
#endif
#ifdef DEBUGTEMP
		return this->aioCom(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::tempSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senlAIO[tempSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->aioCom(portindex);
#endif
	} else
		return -1;
	return -1;
}

int CYZXInter::hallSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senl[hallSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->dioGetDigiInput(portindex);
#endif
	} else
		return -1;
	return -1;
}

void CYZXInter::dioout(int value, unsigned int index) {
	int portindex = -1;
	portindex = this->DIOoutport->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		this->dioDigiOutput(portindex, value);
#endif
	}
}

int CYZXInter::infrDistSen(unsigned int index) {
	int portindex = -1;
	portindex = this->senlAIO[infrDistSenLI]->at(index);
	if (portindex != -1) {
#ifdef WRITELOG
		printf("portindex : %d\n", portindex);
#endif
#ifdef DEBUGTEMP
		return this->aioComInfrared(portindex);
#endif
	} else
		return -1;
	return -1;
}

//position2d
void CYZXInter::pos2dSet4TiresPos(int id1, int id2, int id3, int id4, int pos,
		int speed) {
	player_opaque_data_t popa;
	myOpaqueSt4TiresPos sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSET4TIRESPOSF;
	sendComd.pos = pos;
	sendComd.speed = speed;
	sendComd.tires[0] = ConvertIntToChar[id1];
	sendComd.tires[1] = ConvertIntToChar[id2];
	sendComd.tires[2] = ConvertIntToChar[id3];
	sendComd.tires[3] = ConvertIntToChar[id4];

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSet4Tires(int id1, int id2, int id3, int id4, int sp1,
		int sp2, int sp3, int sp4) {
	player_opaque_data_t popa;
	myOpaqueSt4Tires sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSET4TIRESF;
	sendComd.tires[0] = ConvertIntToChar[id1];
	sendComd.tires[1] = ConvertIntToChar[id2];
	sendComd.tires[2] = ConvertIntToChar[id3];
	sendComd.tires[3] = ConvertIntToChar[id4];
	sendComd.speed1 = sp1;
	sendComd.speed2 = sp2;
	sendComd.speed3 = sp3;
	sendComd.speed4 = sp4;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dServoModeServo() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSERVOMODESERVOF;
	sendComd.p1 = 0;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dServoModeMoto() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSERVOMODEMOTOF;
	sendComd.p1 = 1;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dForward(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt4 sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DFORWARDF;
	int fls, frs, rls, rrs;
	fls = speed;
	frs = speed;
	rls = speed;
	rrs = speed;

	int temp;
	if ((this->servoDirect < 0) || (this->servoDirect > 15)) {
		this->servoDirect = 0;
	}
	temp = (this->servoDirect) & 8;
	if (temp == 8) {
		fls = -fls;
	}
	temp = (this->servoDirect) & 4;
	if (temp == 4) {
		frs = -frs;
	}
	temp = (this->servoDirect) & 2;
	if (temp == 2) {
		rls = -rls;
	}
	temp = (this->servoDirect) & 1;
	if (temp == 1) {
		rrs = -rrs;
	}

	sendComd.p1 = fls;
	sendComd.p2 = frs;
	sendComd.p3 = rls;
	sendComd.p4 = rrs;

#ifdef WRITELOG
	printf("p1:%d,p2:%d,p3:%d,p4:%d\n", sendComd.p1, sendComd.p2, sendComd.p3,
			sendComd.p4);
#endif

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dBackward(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt4 sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DBACKWARDF;
	int fls, frs, rls, rrs;
	fls = speed;
	frs = speed;
	rls = speed;
	rrs = speed;

	int temp;
	if ((this->servoDirect < 0) || (this->servoDirect > 15)) {
		this->servoDirect = 0;
	}
	temp = (this->servoDirect) & 8;
	if (temp == 8) {
		fls = -fls;
	}
	temp = (this->servoDirect) & 4;
	if (temp == 4) {
		frs = -frs;
	}
	temp = (this->servoDirect) & 2;
	if (temp == 2) {
		rls = -rls;
	}
	temp = (this->servoDirect) & 1;
	if (temp == 1) {
		rrs = -rrs;
	}

	sendComd.p1 = fls;
	sendComd.p2 = frs;
	sendComd.p3 = rls;
	sendComd.p4 = rrs;

#ifdef WRITELOG
	printf("p1:%d,p2:%d,p3:%d,p4:%d\n", sendComd.p1, sendComd.p2, sendComd.p3,
			sendComd.p4);
#endif

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dTurnLeft(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt4 sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DTURNLEFTF;
	int fls, frs, rls, rrs;
	fls = speed;
	frs = speed;
	rls = speed;
	rrs = speed;

	int temp;
	if ((this->servoDirect < 0) || (this->servoDirect > 15)) {
		this->servoDirect = 0;
	}
	temp = (this->servoDirect) & 8;
	if (temp == 8) {
		fls = -fls;
	}
	temp = (this->servoDirect) & 4;
	if (temp == 4) {
		frs = -frs;
	}
	temp = (this->servoDirect) & 2;
	if (temp == 2) {
		rls = -rls;
	}
	temp = (this->servoDirect) & 1;
	if (temp == 1) {
		rrs = -rrs;
	}

#ifdef WRITELOG
	printf("p1:%d,p2:%d,p3:%d,p4:%d\n", sendComd.p1, sendComd.p2, sendComd.p3,
			sendComd.p4);
#endif

	sendComd.p1 = fls;
	sendComd.p2 = frs;
	sendComd.p3 = rls;
	sendComd.p4 = rrs;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dTurnRight(int speed) {
	player_opaque_data_t popa;
	myOpaqueSt4 sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DTURNRIGHT;
	int fls, frs, rls, rrs;
	fls = speed;
	frs = speed;
	rls = speed;
	rrs = speed;

	int temp;
	if ((this->servoDirect < 0) || (this->servoDirect > 15)) {
		this->servoDirect = 0;
	}
	temp = (this->servoDirect) & 8;
	if (temp == 8) {
		fls = -fls;
	}
	temp = (this->servoDirect) & 4;
	if (temp == 4) {
		frs = -frs;
	}
	temp = (this->servoDirect) & 2;
	if (temp == 2) {
		rls = -rls;
	}
	temp = (this->servoDirect) & 1;
	if (temp == 1) {
		rrs = -rrs;
	}

	sendComd.p1 = fls;
	sendComd.p2 = frs;
	sendComd.p3 = rls;
	sendComd.p4 = rrs;
#ifdef WRITELOG
	printf("p1:%d,p2:%d,p3:%d,p4:%d\n", sendComd.p1, sendComd.p2, sendComd.p3,
			sendComd.p4);
#endif

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dStop() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSTOP;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetFrontL(unsigned int fl) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETFRONTL;
	sendComd.p1 = fl;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetFrontR(unsigned int fr) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETFRONTR;
	sendComd.p1 = fr;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetRearL(unsigned int rl) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETREARL;
	sendComd.p1 = rl;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetRearR(unsigned int rr) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETREARR;
	sendComd.p1 = rr;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::pos2dGetFrontL() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETFRONTL;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::pos2dGetFrontR() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETFRONTR;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::Pos2dGetRearL() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETREARL;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::pos2dGetRearR() {
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETREARR;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
void CYZXInter::pos2dSetTires(unsigned int fl, unsigned int fr, unsigned int rl,
		unsigned int rr) {

	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETTIRES;

	unsigned int ttire = 0x00000000;
	ttire |= (fl & 0x0000FFFF) | ((fr << 16) & 0xFFFF0000);
	sendComd.p1 = ttire;
	ttire = 0x00000000;
	ttire |= (rl & 0x0000FFFF) | ((rr << 16) & 0xFFFF0000);
	sendComd.p2 = ttire;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

}
void CYZXInter::pos2dSetServoTurn(int angle, int inSpeed) { //void MFSetServoPos(int inID,int inPos,int inSpeed);
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DSETSERVOTURN;

	sendComd.p1 = angle;
	sendComd.p2 = inSpeed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::pos2dSetServoTurnA(int angle, int inSpeed) {

}

int CYZXInter::pos2dGetTires(unsigned int *ptire) { //fl fr rl rr
	player_opaque_data_t popa;
	comReqPos2d sendComd;

	sendComd.type = MYOPAQUESTTYPEPOS;
	sendComd.subtype = POS2DGETTIRES;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respPos2d) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "get 4 tire id req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respPos2d recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	*ptire = (recRes.p1) & 0x0000FFFF;
	*(ptire + 1) = (recRes.p1) >> 16;
	*(ptire + 2) = (recRes.p2) & 0x0000FFFF;
	*(ptire + 3) = (recRes.p2) >> 16;

	return 0;
}
//MFCap
void CYZXInter::capSetH(int inHmax, int inHmin) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETHF;
	sendComd.p1 = inHmax;
	sendComd.p2 = inHmin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetS(int inSmax, int inSmin) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETSF;
	sendComd.p1 = inSmax;
	sendComd.p2 = inSmin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetI(int inImax, int inImin) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETIF;
	sendComd.p1 = inImax;
	sendComd.p2 = inImin;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	sleep(2);
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::capSetMode(int inMode) {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPSETMODEF;
	sendComd.p1 = inMode;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::capGetCenterX() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETCENTERXF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respMFCap) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::capGetCenterY() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETCENTERYF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respMFCap) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
int CYZXInter::capGetSum() {
	player_opaque_data_t popa;
	myOpaqueSt sendComd;

	sendComd.type = MYOPAQUESTTYPEMFCAP;
	sendComd.subtype = CAPGETSUMF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respMFCap) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	myOpaqueSt recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
void CYZXInter::capSetYellow() {
	this->capSetH(110, 20);
	this->capSetS(200, 60);
}
void CYZXInter::capSetRed() {
	this->capSetH(50, 0);
	this->capSetS(255, 100);
}
void CYZXInter::capSetGreen() {

}

//background special
void CYZXInter::bkMFSetServoRotaSpdTime(int inID, int inSpeed, int seconds) {
	player_opaque_data_t popa;
	myOpaqueStTime sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMFSETSERVOROTASPDTIMEF;
	sendComd.p1 = inID;
	sendComd.p2 = inSpeed;
	sendComd.time = seconds;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::bkMp3Play(const char*item) {          	//mp3
	player_opaque_data_t popa;
	myOpaqueStMp3 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMP3PLAYF;
	for (int i = 0; i < MP3MAXLENGTH; i++)
		sendComd.mp3Title[i] = '\0';
	for (int i = 0; i < strlen(item) && i < MP3MAXLENGTH; i++)
		sendComd.mp3Title[i] = *(item + i);

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::bkMp3Stop() {                                    //mp3
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMP3STOPF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::bkLCDdisp(const char*item) {
	player_opaque_data_t popa;
	myOpaqueStLCD sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKLCDDISF;
	for (int i = 0; i < LCDMAXLENGTH; i++)
		sendComd.disp[i] = '\x20';
	for (int i = 0; i < strlen(item) && i < LCDMAXLENGTH; i++) {
		sendComd.disp[i] = *(item + i);
	}

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::bkMFSetServoPos(int inID, int inPos, int inSpeed) {
	player_opaque_data_t popa;
	myOpaqueSt3 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMFSETSERVOPOSF;
	sendComd.p1 = inID;
	sendComd.p2 = inPos;
	sendComd.p3 = inSpeed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
void CYZXInter::bkMFSetServoMode(int inID, int inMode) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMFSETSERVOMODEF;
	sendComd.p1 = inID;
	sendComd.p2 = inMode;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
void CYZXInter::bkMFSetServoModeServo(int inID) {
	this->bkMFSetServoMode(inID, 0);
	return;
}
void CYZXInter::bkMFSetServoModeMoto(int inID) {
	this->bkMFSetServoMode(inID, 1);
	return;
}
//void bkMFSetServoPos(int inID,int inPos,int inSpeed);
void CYZXInter::bkMFSetServoRotaSpd(int inID, int inSpeed) {
	player_opaque_data_t popa;
	myOpaqueSt2 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMFSETSERVOROTASPDF;
	sendComd.p1 = inID;
	sendComd.p2 = inSpeed;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
//int bkMFGetServoPos(int inID);
void CYZXInter::bkMFServoAction() {
	player_opaque_data_t popa;
	myOpaqueSt0 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKMFSERVOACTIONF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
	return;
}
void CYZXInter::bkDelayMS(int inMS) {
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKDELAYMSF;
	sendComd.p1 = inMS;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}

void CYZXInter::bkUSonicEnable() {   	//Ultrasonic
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKUSONICENABLEF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->SendCmd(&popa);
}
int CYZXInter::bkGetUltrasonic() {       //Ultrasonic
	player_opaque_data_t popa;
	comReqBk1 sendComd;

	sendComd.type = MYOPAQUEBk;
	sendComd.subtype = BKGETULTRASONICF;

	popa.data_count = sizeof(sendComd);
	popa.data = (uint8_t *) &sendComd;
	this->opaquep->newSizeFormDevice(sizeof(respBk1) + 20);
	if (0 != this->opaquep->SendReq(&popa))
		return -1;
#ifdef WRITELOGC
	int t = opaquep->GetCount();
	std::cout << "aio req size %d" << t << "\n";
	uint8_t *p = new uint8_t[t];
	opaquep->GetData(p);
	for (int i = 0; i < t; i++)
	printf("%x ", p[i]);
	std::cout << "\n";
#endif
	respBk1 recRes;
	this->opaquep->GetData((uint8_t *) &recRes);
	return recRes.p1;
}
void CYZXInter::bkComSend(const void *pBuffer, const int iLength) { //SerialCom

}

//SpeechRecongn
void CYZXInter::SRStart() {

}
void CYZXInter::SRStop() {

}
bool CYZXInter::SRPause() {
	return false;
}
bool CYZXInter::SRContinue() {
	return false;
}
bool CYZXInter::SRInsertText(int index, const char*item, int insize) {
	return false;
}
int CYZXInter::SRGetResIndex() {
	return -1;
}
void CYZXInter::SRClearItems() {

}
bool CYZXInter::SRisStarted() {
	return false;
}

void CYZXInter::WifiSetWeibo(char *ip){
	player_opaque_data_t popa;
	uint32_t wifiID = WIFIPERIPHERAL;
	uint32_t subtype = WIFISETWEIBOF;
	uint8_t temp[50];
	int iplength=30;
	for(int i=0;i<50;i++)
		temp[i]='\0';
	uint8_t *from,*to;
	to=temp;

	from=(uint8_t*)&wifiID;
	for(int i=0;i<4;i++){
		*to=*from;
		from++;
		to++;
	}
	from=(uint8_t*)&subtype;
	for(int i=4;i<8;i++){
		*to=*from;
		from++;
		to++;
	}
	if(ip!=NULL){
		from=(uint8_t*)ip;
		for(int i=0;i<iplength&&(*from)!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
	}else{
		for(int i=0;i<iplength;i++){
			*to=' ';
			to++;
		}
	}
	popa.data_count = 8+iplength;
	popa.data = (uint8_t *) &temp[0];
	this->opaquep->SendCmd(&popa);

	return;
}
void CYZXInter::WifiSetLCD(char *display,char *ip){
	player_opaque_data_t popa;
	uint32_t wifiID = WIFIPERIPHERAL;
	uint32_t subtype = WIFISETLCDF;
	uint8_t temp[1024];
	int iplength=30;
	for(int i=0;i<1024;i++)
		temp[i]='\0';
	uint8_t *from,*to;
	to=temp;

	from=(uint8_t*)&wifiID;
	for(int i=0;i<4;i++){
		*to=*from;
		from++;
		to++;
	}
	from=(uint8_t*)&subtype;
	for(int i=4;i<8;i++){
		*to=*from;
		from++;
		to++;
	}
	if(ip!=NULL){
		from=(uint8_t*)ip;
		for(int i=0;i<iplength&&(*from)!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
	}else{
		for(int i=0;i<iplength;i++){
			*to=' ';
			to++;
		}
	}
	popa.data_count = 8+iplength;
	if(display){
		int i=0;
		from=(uint8_t*)display;
		for(i=0;i<900&&*from!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
		popa.data_count+=i;
	}
	popa.data = (uint8_t *) &temp[0];
	this->opaquep->SendCmd(&popa);
}
void CYZXInter::WifiSetSound(char *type,char *ip){
	player_opaque_data_t popa;
	uint32_t wifiID = WIFIPERIPHERAL;
	uint32_t subtype = WIFISETSOUNDF;
	int iplength=30;
	uint8_t temp[1024];
	for(int i=0;i<1024;i++)
		temp[i]='\0';
	uint8_t *from,*to;
	to=temp;

	from=(uint8_t*)&wifiID;
	for(int i=0;i<4;i++){
		*to=*from;
		from++;
		to++;
	}
	from=(uint8_t*)&subtype;
	for(int i=4;i<8;i++){
		*to=*from;
		from++;
		to++;
	}
	if(ip!=NULL){
		from=(uint8_t*)ip;
		for(int i=0;i<iplength&&*from!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
	}else{
		for(int i=0;i<iplength;i++){
			*to=' ';
			to++;
		}
	}
	popa.data_count = 8+iplength;
	if(type){
		int i=0;
		from=(uint8_t*)type;
		for(i=0;i<900&&*from!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
		popa.data_count+=i;
	}
	popa.data = (uint8_t *) &temp[0];
	this->opaquep->SendCmd(&popa);
}

int  CYZXInter::WifiGetWeibo(std::string &content,char *cmd,char *ip){
	player_opaque_data_t popa;
	uint32_t wifiID = WIFIPERIPHERAL;
	uint32_t subtype = WIFIGETWEIBOF;
	int iplength=30;
	uint8_t temp[4096];
	for(int i=0;i<1024;i++)
		temp[i]='\0';
	uint8_t *from,*to;
	to=temp;

	from=(uint8_t*)&wifiID;
	for(int i=0;i<4;i++){
		*to=*from;
		from++;
		to++;
	}
	from=(uint8_t*)&subtype;
	for(int i=4;i<8;i++){
		*to=*from;
		from++;
		to++;
	}
	if(ip!=NULL){
		from=(uint8_t*)ip;
		for(int i=0;i<iplength&&*from!='\0';i++){
			*to=*from;
			from++;
			to++;
		}
	}else{
		for(int i=0;i<iplength;i++){
			*to=' ';
			to++;
		}
	}
	popa.data_count = 8+iplength;
	if(cmd){
		int i=0;
		from=(uint8_t*)cmd;
		for(i=0;i<900&&(*from!='\0');i++){
			*to=*from;
			from++;
			to++;
		}
		popa.data_count+=i;
	}
	popa.data = (uint8_t *) &temp[0];
	this->opaquep->newSizeFormDevice(1024);
	if(0 !=this->opaquep->SendReq(&popa)){
		return -1;
	}

	uint8_t *receivedata=new uint8_t[this->opaquep->GetCount()];
	this->opaquep->GetData(receivedata);
	content=(char*)receivedata;
	return 1;
}

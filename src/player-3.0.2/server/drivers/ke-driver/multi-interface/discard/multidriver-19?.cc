/*
 * A simple DIO of how to write a driver that will be built as a
 * shared object.
 */

// ONLY if you need something that was #define'd as a result of configure
// (e.g., HAVE_CFMAKERAW), then #include <config.h>, like so:
/*
 #include <config.h>
 */

#if !defined (WIN32)
#include <unistd.h>
#endif
#include <string.h>
#include <sys/wait.h>

#include <libplayercore/playercore.h>
//#include <libplayerinterface/player_interfaces.h>
#include "../driver-client/CYZXInter.h"

#include "background.h"
#include <stdio.h>
#include <fcntl.h>

//below three for alarm
/*#include <ctime>

 void timing(double x) {
 printf("x %f\n", x);
 clock_t cha = (clock_t) (x * 1000000);
 clock_t start, finish;
 start = clock();
 while (1) {
 finish = clock();
 if (cha <= (finish - start))
 break;
 }
 printf("finish\n");
 }
 static void sig_alrm(int signo) {
 }
 unsigned int sleep2(unsigned int nsecs) {
 if (signal(SIGALRM, sig_alrm) == SIG_ERR )
 return (nsecs);
 alarm(nsecs);
 pause();
 return (alarm(0));
 }*/
/*
 static jmp_buf env_alrm;
 static void sig_alrm(int signo) {
 longjmp(env_alrm, 1);
 }
 unsigned int sleep2(unsigned int nsecs) {
 if (signal(SIGALRM, sig_alrm) == SIG_ERR )
 return (nsecs);
 if (setjmp(env_alrm) == 0) {
 alarm(nsecs);
 pause();
 }
 return (alarm(0));
 }*/

////////////////////////////////////////////////////////////////////////////////
// The class for the driver
class multidriver: public ThreadedDriver {
public:

	// Constructor; need that
	multidriver(ConfigFile* cf, int section);

	// This method will be invoked on each incoming message
	virtual int ProcessMessage(QueuePointer &resp_queue, player_msghdr * hdr,
			void * data);

private:

	// Main function for device thread.
	virtual void Main();
	virtual int MainSetup();
	virtual void MainQuit();

	// My position interface
	player_devaddr_t m_dio_addr;
	player_devaddr_t m_aio_addr;
	player_devaddr_t m_position_addr;
	player_devaddr_t m_opaque_addr;

	//uint32_t getDIOValue(uint32_t port);
	//uint32_t getAIOValue(uint32_t port);
	uint32_t getDIOValue(player_dio_cmd_t *pdioc, player_dio_data_t *pr);
	uint32_t getAIOValue(player_aio_cmd_t *paioc, player_aio_data_t *pr);

	int dealDIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealAIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealPOSmessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueMessages(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealOpaqueMFCAPCommand(myOpaqueSt *popa);
	int dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *res);
	int dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res);

	int dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *res);

	int dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res);

	int dealOpaqueBKComReq(myOpaqueSt1 *popa, respBk1 *res);

	int dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	int dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaquePosition2d(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueBK(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	int dealOpaqueSR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);

	void writeMessagehrToFile(player_msghdr * hdr, void * data);

	int dealOpaqueBKGETULTRASONICF(myOpaqueSt1 *popa);

	void writeLog(char *logstr);

	int frontl;
	int frontr;
	int behindl;
	int behindr;

#ifdef WRITELOG
	//all messages count
	int mc;
	//don't know messages count
	int nmc;
	int aiomc;
	int naiomc;
	int diomc;
	int ndiomc;
	int posmc;
	int nposmc;
	int opaquemc;
	int nopaquemc;
	int opaquemfcapmc;
	int nopaquemfcapmc;
	int opaquediomc;
	int nopaquediomc;
	FILE *fl;
#endif

	//the ptr to function
	void ((*fDealAllComReq)(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data))[MAXCOMMANDNUMBER];

void	CYZXSRSTARTF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXSRSTOPF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXSRPAUSEF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXSRCONTINUEF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXSRINSERTTEXTF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXSRGETRESINDEXF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXSRCLEARITEMSF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXSRISSTARTEDF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKDELAYMSF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKLCDDISF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKUSONICENABLEF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKGETULTRASONICF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKCOMSENDF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMP3PLAYF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMP3STOPF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMFSETSERVOMODEF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMFSETSERVOPOSF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMFSETSERVOROTASPDF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMFGETSERVOPOSF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMFSERVOACTIONF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXBKMFSETSERVOROTASPDTIMEF(QueuePointer & resp_queue,
			player_msghdr * hdr, void * data);
	void CYZXAIOCOMINFRAREDF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXAIOCOMF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXAIOCOMTF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXwhatKindAIOAIODEF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXwhatKindAIOAIOINFRAREDR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXDIOSETPORTDIRECTF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXDIOGETDIGIINPUTF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXDIODIGIOUTPUTF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSERVOMODESERVOF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSERVOMODEMOTOF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DFORWARDF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DBACKWARDF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DTURNLEFTF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DTURNRIGHT(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSTOP(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETFRONTL(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETFRONTR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETREARL(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETREARR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DGETFRONTL(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DGETFRONTR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DGETREARL(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DGETREARR(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETTIRES(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DGETTIRES(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETSERVOTURN(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSETSERVOTURNA(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSET4TIRESF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXPOS2DSET4TIRESPOSF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPSETHF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXCAPSETSF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXCAPSETIF(QueuePointer & resp_queue, player_msghdr * hdr, void * data);
	void CYZXCAPSETMODEF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPGETCENTERXF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPGETCENTERYF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPGETSUMF(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPSETYELLOW(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPSETRED(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
	void CYZXCAPSETGREEN(QueuePointer & resp_queue, player_msghdr * hdr,
			void * data);
};

// A factory creation function, declared outside of the class so that it
// can be invoked without any object context (alternatively, you can
// declare it static in the class).  In this function, we create and return
// (as a generic Driver*) a pointer to a new instance of this driver.
Driver*
multidriver_Init(ConfigFile* cf, int section) {
	// Create and return a new instance of this driver
#ifdef WRITELOG
	printf("multidriver_Init begin !!!\n");
#endif

	return ((Driver*) (new multidriver(cf, section)));
}

// A driver registration function, again declared outside of the class so
// that it can be invoked without object context.  In this function, we add
// the driver into the given driver table, indicating which interface the
// driver can support and how to create a driver instance.
void multidriver_Register(DriverTable* table) {
#ifdef WRITELOG
	printf("multidriver_Register begin !!! \n");
#endif

	table->AddDriver("multidriver", multidriver_Init);
#ifdef WRITELOG
	printf("multidriver_Register end !!!\n");
#endif

}

////////////////////////////////////////////////////////////////////////////////
// Constructor.  Retrieve options from the configuration file and do any
// pre-Setup() setup.

/*
 * if (cf->ReadDeviceAddr(&(this->m_position_addr), section,
 "provides", PLAYER_POSITION2D_CODE, -1, NULL) != 0)
 {
 this->SetError(-1);
 return;
 }
 if (this->AddInterface(this->m_position_addr))
 {
 this->SetError(-1);
 return;
 }

 // Create my laser interface
 if (cf->ReadDeviceAddr(&(this->m_laser_addr), section,
 "provides", PLAYER_LASER_CODE, -1, NULL) != 0)
 {
 this->SetError(-1);
 return;
 }
 if (this->AddInterface(this->m_laser_addr))
 {
 this->SetError(-1);
 return;
 }
 * */
void multidriver::CYZXSRSTARTF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRSTOPF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRPAUSEF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRCONTINUEF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRINSERTTEXTF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRGETRESINDEXF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRCLEARITEMSF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXSRISSTARTEDF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}

void multidriver::CYZXBKDELAYMSF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;

	DelayMS(popa->p1);

#ifdef WRITELOG
	if (NULL != this->fl) {
		fprintf(this->fl, "bkDelayMs %d\n", popa->p1);
	}
#endif

	return;
}
void multidriver::CYZXBKLCDDISF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueStLCD *popa = (myOpaqueStLCD*) opaquedata->data;

	int pid;
	if ((pid = fork()) < 0) {
		printf("fork error\n");
	} else if (pid == 0) {
		char lcdDisp[LCDMAXLENGTH];
		for (int i = 0; i < LCDMAXLENGTH; i++)
			lcdDisp[i] = '\x20';
		for (int i = 0; i < LCDMAXLENGTH; i++) {
			lcdDisp[i] = popa->disp[i];
		}
		if (execl("/mnt/yaffs/test/LCDDisp", lcdDisp, (char *) 0) < 0)
			printf("exe error\n");
	}

#ifdef WRITELOG
	if (NULL != this->fl) {
		fprintf(this->fl, "LCDDisplay %s\n", popa->disp);
	}
#endif

	return;
}

void multidriver::CYZXBKUSONICENABLEF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt0 *popa = (myOpaqueSt0*) opaquedata->data;

	MFUSonicEnable();

#ifdef WRITELOG
	if (NULL != this->fl) {
		fprintf(this->fl, "ultrasonic enable\n");
	}
#endif

	return;
}
void multidriver::CYZXBKGETULTRASONICF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt0 *popa = (myOpaqueSt0*) opaquedata->data;

	int res = MFGetUltrasonic();

	player_opaque_data_t opaqueRes;

	opaqueRes.data_count = sizeof(res);
	uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
			+ opaqueRes.data_count;
	opaqueRes.data = new uint8_t[opaqueRes.data_count];
	memcpy(opaqueRes.data, &res, opaqueRes.data_count);

	Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
			PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);

#ifdef WRITELOG
	if (NULL != this->fl) {
		fprintf(this->fl, "get ultrasonic distance %d\n", res);
	}
#endif

	return;
}
void multidriver::CYZXBKCOMSENDF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXBKMP3PLAYF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueStMp3 *popa = (myOpaqueStMp3*) opaquedata->data;

	int pid;
	if ((pid = fork()) < 0) {
		printf("fork error\n");
	} else if (pid == 0) {
		char mp3Title[MP3MAXLENGTH + 1];
		for (int i = 0; i < MP3MAXLENGTH + 1; i++)
			mp3Title[i] = '\0';
		for (int i = 0; i < MP3MAXLENGTH; i++)
			mp3Title[i] = popa->mp3Title[i];
		if (execl("/mnt/yaffs/test/MP3Play", mp3Title, (char *) 0) < 0)
			printf("exe error\n");
	}

	return;
}
void multidriver::CYZXBKMP3STOPF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt0 *popa = (myOpaqueSt0*) opaquedata->data;

	int pid;
	if ((pid = fork()) < 0) {
		printf("fork error\n");
	} else if (pid == 0) {
		if (execl("/mnt/yaffs/test/MP3Stop", "MP3STOP", (char *) 0) < 0)
			printf("exe error\n");
	}

	return;
}
void multidriver::CYZXBKMFSETSERVOMODEF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt2 *popa = (myOpaqueSt2*) opaquedata->data;

	MFSetServoMode(popa->p1, popa->p2);

	return;
}
void multidriver::CYZXBKMFSETSERVOPOSF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt2 *popa = (myOpaqueSt2*) opaquedata->data;


	return;
}
void multidriver::CYZXBKMFSETSERVOROTASPDF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXBKMFGETSERVOPOSF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXBKMFSERVOACTIONF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXBKMFSETSERVOROTASPDTIMEF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXAIOCOMINFRAREDF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXAIOCOMF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXAIOCOMTF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXwhatKindAIOAIODEF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXwhatKindAIOAIOINFRAREDR(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXDIOSETPORTDIRECTF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXDIOGETDIGIINPUTF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXDIODIGIOUTPUTF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSERVOMODESERVOF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSERVOMODEMOTOF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DFORWARDF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DBACKWARDF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DTURNLEFTF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DTURNRIGHT(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSTOP(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETFRONTL(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETFRONTR(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETREARL(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETREARR(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DGETFRONTL(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DGETFRONTR(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DGETREARL(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DGETREARR(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETTIRES(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DGETTIRES(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETSERVOTURN(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSETSERVOTURNA(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSET4TIRESF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXPOS2DSET4TIRESPOSF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETHF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETSF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETIF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETMODEF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPGETCENTERXF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPGETCENTERYF(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPGETSUMF(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETYELLOW(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETRED(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}
void multidriver::CYZXCAPSETGREEN(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt1 *popa = (myOpaqueSt1*) opaquedata->data;
	return;
}

multidriver::multidriver(ConfigFile* cf, int section) :
		ThreadedDriver(cf, section) {
#ifdef WRITELOG
	printf("multidriver initial begin\n");
#endif

	// Read an option from the configuration file
	/*if (cf->ReadDeviceAddr(&(this->m_dio_addr), section, "provides",
	 PLAYER_DIO_CODE, -1, NULL) != 0) {
	 this->SetError(-1);
	 return;
	 }
	 if (this->AddInterface(this->m_dio_addr)) {
	 this->SetError(-1);
	 return;
	 }

	 if (cf->ReadDeviceAddr(&(this->m_aio_addr), section, "provides",
	 PLAYER_AIO_CODE, -1, NULL) != 0) {
	 this->SetError(-1);
	 return;
	 }
	 if (this->AddInterface(this->m_aio_addr)) {
	 this->SetError(-1);
	 return;
	 }
	 if (cf->ReadDeviceAddr(&(this->m_position_addr), section, "provides",
	 PLAYER_POSITION2D_CODE, -1, NULL) != 0) {
	 this->SetError(-1);
	 return;
	 }
	 if (this->AddInterface(this->m_position_addr)) {
	 this->SetError(-1);
	 return;
	 }*/
	if (cf->ReadDeviceAddr(&(this->m_opaque_addr), section, "provides",
			PLAYER_OPAQUE_CODE, -1, NULL) != 0) {
		this->SetError(-1);
		return;
	} else {
		if (this->AddInterface(this->m_opaque_addr)) {
			this->SetError(-1);
			return;
		}
	}

#ifdef WRITELOG
	printf("DIO driver initial end\n");
#endif

#ifdef WRITELOG
	this->mc = 0;
	this->nmc = 0;
	this->diomc = 0;
	this->ndiomc = 0;
	this->aiomc = 0;
	this->naiomc = 0;
	this->posmc = 0;
	this->nposmc = 0;
	this->opaquemc = 0;
	this->nopaquemc = 0;
	this->opaquemfcapmc = 0;
	this->nopaquemfcapmc = 0;
	this->opaquediomc = 0;
	this->nopaquediomc = 0;
	this->fl = NULL;
	if (NULL == (this->fl = fopen(LOGFILENAME, "w+"))) {
		printf(LABLESTRING);
		printf("can't open log file\n");
	}

#endif

	this->frontl = cf->ReadInt(section, "frontL", 8);
	this->frontr = cf->ReadInt(section, "frontR", 6);
	this->behindl = cf->ReadInt(section, "rearL", 7);
	this->behindr = cf->ReadInt(section, "rearR", 9);

#ifdef WRITELOG
	printf(LABLESTRING);
	printf("MFInit\n");
#endif

	// Here you do whatever is necessary to setup the device, like open and
	// configure a serial port.

	MFCapOpen();
	MFInit();
	MFSetPortDirect(0x00000000);
	for (int i = 0; i < 8; i++) {
		MFADEnable(i);
	}
	MFDigiInit(100);
	DelayMS(100);
	MFADInit(100);
	MFSetServoMode(this->frontl, 1);
	MFSetServoMode(this->frontr, 1);
	MFSetServoMode(this->behindl, 1);
	MFSetServoMode(this->behindr, 1);
	DelayMS(100);
	MFUSonicEnable();
	DelayMS(100);

#ifdef WRITELOG
	printf(LABLESTRING);
	printf("MFInit end\n");
#endif

	/*below initial the deal ComReq-packet function*/
	for (int i = 0; i < MAXCOMMANDNUMBER; i++)
		this->fDealAllComReq[i] = NULL;
	this->fDealAllComReq[SRSTARTF] = CYZXSRSTARTF;
	this->fDealAllComReq[SRSTOPF] = CYZXSRSTOPF;
	this->fDealAllComReq[SRPAUSEF] = CYZXSRPAUSEF;
	this->fDealAllComReq[SRCONTINUEF] = CYZXSRCONTINUEF;
	this->fDealAllComReq[SRINSERTTEXTF] = CYZXSRINSERTTEXTF;
	this->fDealAllComReq[SRGETRESINDEXF] = CYZXSRGETRESINDEXF;
	this->fDealAllComReq[SRCLEARITEMSF] = CYZXSRCLEARITEMSF;
	this->fDealAllComReq[SRISSTARTEDF] = CYZXSRISSTARTEDF;
	this->fDealAllComReq[BKDELAYMSF] = CYZXBKDELAYMSF;
	this->fDealAllComReq[BKLCDDISF] = CYZXBKLCDDISF;
	this->fDealAllComReq[BKUSONICENABLEF] = CYZXBKUSONICENABLEF;
	this->fDealAllComReq[BKGETULTRASONICF] = CYZXBKGETULTRASONICF;
	this->fDealAllComReq[BKCOMSENDF] = CYZXBKCOMSENDF;
	this->fDealAllComReq[BKMP3PLAYF] = CYZXBKMP3PLAYF;
	this->fDealAllComReq[BKMP3STOPF] = CYZXBKMP3STOPF;
	this->fDealAllComReq[BKMFSETSERVOMODEF] = CYZXBKMFSETSERVOMODEF;
	this->fDealAllComReq[BKMFSETSERVOPOSF] = CYZXBKMFSETSERVOPOSF;
	this->fDealAllComReq[BKMFSETSERVOROTASPDF] = CYZXBKMFSETSERVOROTASPDF;
	this->fDealAllComReq[BKMFGETSERVOPOSF] = CYZXBKMFGETSERVOPOSF;
	this->fDealAllComReq[BKMFSERVOACTIONF] = CYZXBKMFSERVOACTIONF;
	this->fDealAllComReq[BKMFSETSERVOROTASPDTIMEF] =
			CYZXBKMFSETSERVOROTASPDTIMEF;
	this->fDealAllComReq[AIOCOMINFRAREDF] = CYZXAIOCOMINFRAREDF;
	this->fDealAllComReq[AIOCOMF] = CYZXAIOCOMF;
	this->fDealAllComReq[AIOCOMTF] = CYZXAIOCOMTF;
	this->fDealAllComReq[whatKindAIOAIODEF] = CYZXwhatKindAIOAIODEF;
	this->fDealAllComReq[whatKindAIOAIOINFRAREDR] = CYZXwhatKindAIOAIOINFRAREDR;
	this->fDealAllComReq[DIOSETPORTDIRECTF] = CYZXDIOSETPORTDIRECTF;
	this->fDealAllComReq[DIOGETDIGIINPUTF] = CYZXDIOGETDIGIINPUTF;
	this->fDealAllComReq[DIODIGIOUTPUTF] = CYZXDIODIGIOUTPUTF;
	this->fDealAllComReq[POS2DSERVOMODESERVOF] = CYZXPOS2DSERVOMODESERVOF;
	this->fDealAllComReq[POS2DSERVOMODEMOTOF] = CYZXPOS2DSERVOMODEMOTOF;
	this->fDealAllComReq[POS2DFORWARDF] = CYZXPOS2DFORWARDF;
	this->fDealAllComReq[POS2DBACKWARDF] = CYZXPOS2DBACKWARDF;
	this->fDealAllComReq[POS2DTURNLEFTF] = CYZXPOS2DTURNLEFTF;
	this->fDealAllComReq[POS2DTURNRIGHT] = CYZXPOS2DTURNRIGHT;
	this->fDealAllComReq[POS2DSTOP] = CYZXPOS2DSTOP;
	this->fDealAllComReq[POS2DSETFRONTL] = CYZXPOS2DSETFRONTL;
	this->fDealAllComReq[POS2DSETFRONTR] = CYZXPOS2DSETFRONTR;
	this->fDealAllComReq[POS2DSETREARL] = CYZXPOS2DSETREARL;
	this->fDealAllComReq[POS2DSETREARR] = CYZXPOS2DSETREARR;
	this->fDealAllComReq[POS2DGETFRONTL] = CYZXPOS2DGETFRONTL;
	this->fDealAllComReq[POS2DGETFRONTR] = CYZXPOS2DGETFRONTR;
	this->fDealAllComReq[POS2DGETREARL] = CYZXPOS2DGETREARL;
	this->fDealAllComReq[POS2DGETREARR] = CYZXPOS2DGETREARR;
	this->fDealAllComReq[POS2DSETTIRES] = CYZXPOS2DSETTIRES;
	this->fDealAllComReq[POS2DGETTIRES] = CYZXPOS2DGETTIRES;
	this->fDealAllComReq[POS2DSETSERVOTURN] = CYZXPOS2DSETSERVOTURN;
	this->fDealAllComReq[POS2DSETSERVOTURNA] = CYZXPOS2DSETSERVOTURNA;
	this->fDealAllComReq[POS2DSET4TIRESF] = CYZXPOS2DSET4TIRESF;
	this->fDealAllComReq[POS2DSET4TIRESPOSF] = CYZXPOS2DSET4TIRESPOSF;
	this->fDealAllComReq[CAPSETHF] = CYZXCAPSETHF;
	this->fDealAllComReq[CAPSETSF] = CYZXCAPSETSF;
	this->fDealAllComReq[CAPSETIF] = CYZXCAPSETIF;
	this->fDealAllComReq[CAPSETMODEF] = CYZXCAPSETMODEF;
	this->fDealAllComReq[CAPGETCENTERXF] = CYZXCAPGETCENTERXF;
	this->fDealAllComReq[CAPGETCENTERYF] = CYZXCAPGETCENTERYF;
	this->fDealAllComReq[CAPGETSUMF] = CYZXCAPGETSUMF;
	this->fDealAllComReq[CAPSETYELLOW] = CYZXCAPSETYELLOW;
	this->fDealAllComReq[CAPSETRED] = CYZXCAPSETRED;
	this->fDealAllComReq[CAPSETGREEN] = CYZXCAPSETGREEN;

	return;
}

////////////////////////////////////////////////////////////////////////////////
// Set up the device.  Return 0 if things go well, and -1 otherwise.
int multidriver::MainSetup() {

	return (0);
}

////////////////////////////////////////////////////////////////////////////////
// Shutdown the device
void multidriver::MainQuit() {
	// Here you would shut the device down by, for DIO, closing a
	// serial port.
#ifdef WRITELOG
	printf(
			"all messages count %d , aio message %d , dio message %d , position message %d , opaque message %d \n",
			this->mc, this->aiomc, this->diomc, this->posmc, this->opaquemc);
	printf(
			"not messages %d , not aio %d , not dio %d , not pos %d , not opaque %d \n",
			this->nmc, this->naiomc, this->ndiomc, this->nposmc,
			this->nopaquemc);
#endif

#ifdef WRITELOG
	printf(LABLESTRING);
	printf("mainQuit\n");
#endif
}

uint32_t multidriver::getDIOValue(player_dio_cmd_t *pdioc,
		player_dio_data_t *pr) {
#ifdef WRITELOG
	printf("comming dio message : %d , %d\n", pdioc->count, pdioc->digout);
#endif
	pr->count = MFGetDigiInput(pdioc->count);
#ifdef WRITELOG
	printf("outing dio message : %d , %d\n", pr->count, pr->bits);
#endif
	return 0;
}

void multidriver::writeMessagehrToFile(player_msghdr * hdr, void * data) {
#ifdef WRITELOG
	char *buf;

#endif
}

uint32_t multidriver::getAIOValue(player_aio_cmd_t *paioc,
		player_aio_data_t *pr) {
#ifdef WRITELOG
	printf("comming aio message : %d , %f\n", paioc->id, paioc->voltage);
#endif
	pr->voltages_count = MFGetAD(paioc->id) / 10;
	float t = 0.0;
	pr->voltages = &t;

#ifdef WRITELOG
	printf("outing aio message : %d , %f\n", pr->voltages_count, pr->voltages);
#endif
	return 0;
}

int multidriver::dealAIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_AIO_CMD_STATE,
			*(player_devaddr_t *) &(this->m_aio_addr))) {
		player_aio_data_t aiod;
		player_aio_cmd_t *aioc = (player_aio_cmd_t*) data;
		this->getAIOValue(aioc, &aiod);
		Publish(this->m_aio_addr, resp_queue, PLAYER_MSGTYPE_DATA,
				PLAYER_AIO_DATA_STATE, (void*) &aiod, sizeof(aiod), NULL);
		return 0;
	} else {
#ifdef WRITELOG
		this->naiomc++;
#endif
		return 0;
	}
	return -1;
}

int multidriver::dealOpaqueMFCAPCommand(myOpaqueSt *popa) {
#ifdef WRITELOG
	printf("coming opaque command : %d %d %d %d\n", popa->type, popa->subtype,
			popa->p1, popa->p2);
#endif
	switch (popa->subtype) {
	case 1: {
		MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case 2: {
		MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case 3: {
		MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case 4: {
		MFCapSetMode(popa->p1);
		break;
	}
	default:
		break;
	}
	return 0;
}
int multidriver::dealOpaqueBKComReq(myOpaqueSt1 *popa, respBk1 *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;

	if (BKDELAYMSF == popa->subtype) {
		DelayMS(popa->p1);
	} else if (BKLCDDISF == popa->subtype) {

	} else if (BKUSONICENABLEF == popa->subtype) {
		MFUSonicEnable();
	} else if (BKGETULTRASONICF == popa->subtype) {

	} else if (BKCOMSENDF == popa->subtype) {

	} else if (BKMP3PLAYF == popa->subtype) {

	} else if (BKMP3STOPF == popa->subtype) {
		MFMp3Stop();
	} else
		return -1;
	return 0;
}
int multidriver::dealOpaqueBKGETULTRASONICF(myOpaqueSt1 *popa) {
	player_opaque_data_t opaqueRes;
	respBk1 res;

#ifdef WRITELOG
	printf("req opaque get ultrasonic ");
	printf("%d %d %d\n", popa->type, popa->subtype, popa->p1);
#endif

	res.type = popa->type;
	res.subtype = popa->subtype;
	res.p1 = MFGetUltrasonic();

	opaqueRes.data_count = sizeof(res);
	uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
			+ opaqueRes.data_count;
	opaqueRes.data = new uint8_t[opaqueRes.data_count];
	memcpy(opaqueRes.data, &res, opaqueRes.data_count);

#ifdef WRITELOG
	printf("req opaque ultrasonic response data_count %d , size %d\n",
			opaqueRes.data_count, size);
	printf("outing ultrasonic message %d %d %d\n", res.type, res.subtype,
			res.p1);
#endif

	Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
			PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
	return 0;
}
int multidriver::dealOpaqueMFCAPComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case CAPSETHF: {
		MFCapSetH(popa->p1, popa->p2);
		break;
	}
	case CAPSETSF: {
		MFCapSetS(popa->p1, popa->p2);
		break;
	}
	case CAPSETIF: {
		MFCapSetI(popa->p1, popa->p2);
		break;
	}
	case CAPSETMODEF: {
		MFCapSetMode(popa->p1);
		break;
	}
	case CAPGETCENTERXF: {
		pr->p1 = MFCapGetCenterX();
		break;
	}
	case CAPGETCENTERYF: {
		pr->p1 = MFCapGetCenterY();
		break;
	}
	case CAPGETSUMF: {
		pr->p1 = MFCapGetSum();
		break;
	}
	default: {
		break;
	}
	}
	return 0;
}
int multidriver::dealOpaqueMFCAPReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	switch (popa->subtype) {
	case 5: {
		pr->p1 = MFCapGetCenterX();
		break;
	}
	case 6: {
		pr->p1 = MFCapGetCenterY();
		break;
	}
	case 7: {
		pr->p1 = MFCapGetSum();
		break;
	}
	default: {
		break;
	}
	}
#ifdef WRITELOG
	printf("comming opaque req : %d %d %d %d \n", popa->type, popa->subtype,
			popa->p1, popa->p2);
	printf("outing opaque message : %d %d %d %d\n", pr->type, pr->subtype,
			pr->p1, pr->p2);
#endif
	return 0;
}

int multidriver::dealOpaqueMFCAP(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueMFCap size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueMFCAPComReq(popa, &res);
		if ((popa->subtype == CAPGETCENTERXF)
				|| (popa->subtype == CAPGETCENTERYF)
				|| (popa->subtype == CAPGETSUMF)) {
			opaqueRes.data_count = sizeof(myOpaqueSt);
			uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
					+ opaqueRes.data_count;
			opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
			printf("opaqueMFCap response data_count %d , size %d\n",
					opaqueRes.data_count, size);
			printf("outing MFCap message %d %d %d %d\n", res.type, res.subtype,
					res.p1, res.p2);
#endif
			Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
					PLAYER_OPAQUE_DATA_STATE,
					reinterpret_cast<void*>(&opaqueRes), size, NULL);
		}
		return 0;
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_OPAQUE_REQ_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueMFCap size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueMFCAPComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaqueMFCap response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing MFCap message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);

		return 0;
	} else {
#ifdef WRITELOG
		this->nopaquemc++;
#endif
		return -1;
	}
	return 0;
}
int multidriver::dealOpaqueBK(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	pid_t pid;
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	if (opaquedata->data_count == sizeof(myOpaqueStTime)) {
		myOpaqueStTime *popa = (myOpaqueStTime *) opaquedata->data;
		switch (popa->subtype) {
		case BKMFSETSERVOROTASPDTIMEF: {
			MFSetServoRotaSpd(popa->p1, popa->p2);
			MFServoAction();
#ifdef WRITELOG
			printf("begin to sleep,%d,%d,%f\n", popa->p1, popa->p2, popa->time);
#endif
			sleep2(popa->time);
			//timing(popa->time);
#ifdef WRITELOG
			printf("end sleep\n");
#endif
			MFSetServoRotaSpd(popa->p1, 0);
			MFServoAction();
			return 0;
			break;
		}
		default:
			break;
		}

	}
	if (opaquedata->data_count == sizeof(myOpaqueStMp3)) {
		myOpaqueStMp3 *popa = (myOpaqueStMp3 *) opaquedata->data;
		switch (popa->subtype) {
		case BKMP3PLAYF: {
			if ((pid = fork()) < 0) {
				printf("fork error\n");
			} else if (pid == 0) {
				char mp3Title[MP3MAXLENGTH + 1];
				for (int i = 0; i < MP3MAXLENGTH + 1; i++)
					mp3Title[i] = '\0';
				for (int i = 0; i < MP3MAXLENGTH; i++)
					mp3Title[i] = popa->mp3Title[i];
				if (execl("/mnt/yaffs/test/MP3Play", mp3Title, (char *) 0) < 0)
					printf("exe error\n");
			}

			/*
			 if (waitpid(pid, NULL, 0) < 0) {
			 printf("wait error\n");
			 }*/

			return 0;
			break;
		}
		default:
			break;
		}
	}
	if (opaquedata->data_count == sizeof(myOpaqueStLCD)) {
		myOpaqueStLCD *popa = (myOpaqueStLCD*) opaquedata->data;
		switch (popa->subtype) {
		case BKLCDDISF: {
			if (pid = fork() < 0) {
				printf("fork error\n");
			} else if (pid == 0) {
				char lcdDisp[LCDMAXLENGTH];
				for (int i = 0; i < LCDMAXLENGTH; i++)
					lcdDisp[i] = '\x20';
				for (int i = 0; i < LCDMAXLENGTH; i++) {
					lcdDisp[i] = popa->disp[i];
				}
				if (execl("/mnt/yaffs/test/LCDDisp", lcdDisp, (char *) 0) < 0)
					printf("exe error\n");
			}
			/*
			 if (waitpid(pid, NULL, 0) < 0) {
			 printf("wait error\n");
			 }
			 */
			return 0;
			break;
		}
		default:
			break;
		}
	}
	if (opaquedata->data_count == sizeof(myOpaqueSt1)) {
		myOpaqueSt1 *popa = (myOpaqueSt1 *) opaquedata->data;
		switch (popa->head.subtype) {
		case BKDELAYMSF: {
			DelayMS(popa->p1);
			break;
		}
		case BKLCDDISF: {
			break;
		}
		case BKUSONICENABLEF: {
			MFUSonicEnable();
			break;
		}
		case BKGETULTRASONICF: {
			if (-1 == dealOpaqueBKGETULTRASONICF(popa))
				return -1;
			break;
		}
		case BKCOMSENDF: {
			break;
		}
		case BKMP3PLAYF: {
			break;
		}
		case BKMP3STOPF: {
			//MFMp3Stop();

			if ((pid = fork()) < 0) {
				printf("fork error\n");
			} else if (pid == 0) {
				if (execl("/mnt/yaffs/test/MP3Stop", "MP3STOP", (char *) 0) < 0)
					printf("exe error\n");
			}

			/*
			 if (waitpid(pid, NULL, 0) < 0) {
			 printf("wait error\n");
			 }*/

			break;
		}
		default:
			break;
		}
	} else if (opaquedata->data_count == sizeof(myOpaqueSt2)) {
		myOpaqueSt2 *popa = (myOpaqueSt2 *) opaquedata->data;
		switch (popa->subtype) {
		case BKMFSETSERVOMODEF: {
			MFSetServoMode(popa->p1, popa->p2);
			break;
		}
		case BKMFSETSERVOROTASPDF: {
			MFSetServoRotaSpd(popa->p1, popa->p2);
			//MFServoAction();
			break;
		}
		default:
			break;
		}
	} else if (opaquedata->data_count == sizeof(myOpaqueSt0)) {
		myOpaqueSt0 *popa = (myOpaqueSt0 *) opaquedata->data;
		switch (popa->subtype) {
		case BKMFSERVOACTIONF: {
			MFServoAction();
			break;
		}
		default:
			break;
		}

	} else if (opaquedata->data_count == sizeof(myOpaqueSt3)) {
		myOpaqueSt3 *popa = (myOpaqueSt3 *) opaquedata->data;
		switch (popa->subtype) {
		case BKMFSETSERVOPOSF: {
			MFSetServoPos(popa->p1, popa->p2, popa->p3);
			break;
		}
		default:
			break;
		}
	} else {
		return -1;
	}
	return 0;
}

int multidriver::dealOpaqueSR(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	return -1;
}
int multidriver::dealOpaqueAIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueAIO size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueAIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaqueAIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing AIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
				PLAYER_OPAQUE_DATA_STATE, reinterpret_cast<void*>(&opaqueRes),
				size, NULL);
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_OPAQUE_REQ_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("req opaqueAIO size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueAIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = new uint8_t[opaqueRes.data_count];
		memcpy(opaqueRes.data, &res, opaqueRes.data_count);

#ifdef WRITELOG
		printf("req opaqueAIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing AIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		/*
		 Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
		 PLAYER_OPAQUE_DATA_STATE, reinterpret_cast<void*>(&opaqueRes),
		 size, NULL);
		 */
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
	} else
		return -1;
	return -1;
}
int multidriver::dealOpaquePosition2d(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	comReqPos2d *popa = (comReqPos2d *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		respPos2d res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaquePosition2d size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif
		dealOpaquePosition2dComReq(popa, &res);
	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_OPAQUE_REQ_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		respPos2d res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaquePosition2d size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaquePosition2dComReq(popa, &res);

		opaqueRes.data_count = sizeof(res);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaquePosition2d response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing position2d message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);
		return 0;
	} else
		return -1;
	return 0;
}

int multidriver::dealOpaqueDIO(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;
	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;

	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_OPAQUE_CMD_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {
		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueDIO size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueDIOComReq(popa, &res);

		if (popa->subtype == DIOGETDIGIINPUTF) {
			opaqueRes.data_count = sizeof(myOpaqueSt);
			uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
					+ opaqueRes.data_count;
			opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
			printf("opaqueDIO response data_count %d , size %d\n",
					opaqueRes.data_count, size);
			printf("outing DIO message %d %d %d %d\n", res.type, res.subtype,
					res.p1, res.p2);
#endif
			Publish(this->m_opaque_addr, PLAYER_MSGTYPE_DATA,
					PLAYER_OPAQUE_DATA_STATE,
					reinterpret_cast<void*>(&opaqueRes), size, NULL);
		}

	} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
			PLAYER_OPAQUE_REQ_DATA,
			*(player_devaddr_t *) &(this->m_opaque_addr))) {

		player_opaque_data_t opaqueRes;
		myOpaqueSt res;
#ifdef WRITELOG
		int t = sizeof(myOpaqueSt);
		printf("opaqueDIO size %d,coming message  ", t);
		printf("%d %d %d %d\n", popa->type, popa->subtype, popa->p1, popa->p2);
#endif

		dealOpaqueDIOComReq(popa, &res);

		opaqueRes.data_count = sizeof(myOpaqueSt);
		uint32_t size = sizeof(opaqueRes) - sizeof(opaqueRes.data)
				+ opaqueRes.data_count;
		opaqueRes.data = (uint8_t *) &res;
#ifdef WRITELOG
		printf("opaqueDIO response data_count %d , size %d\n",
				opaqueRes.data_count, size);
		printf("outing DIO message %d %d %d %d\n", res.type, res.subtype,
				res.p1, res.p2);
#endif
		Publish(this->m_opaque_addr, PLAYER_MSGTYPE_RESP_ACK,
				PLAYER_OPAQUE_REQ_DATA, &opaqueRes, size, NULL);

	}

	if (POS2DSET4TIRESF == popa->subtype) {
		if (opaquedata->data_count == sizeof(myOpaqueSt4Tires)) {
			myOpaqueSt4Tires *popa4tires = (myOpaqueSt4Tires *) opaquedata->data;
			int id1 = popa4tires->tires[0];
			int id2 = popa4tires->tires[1];
			int id3 = popa4tires->tires[2];
			int id4 = popa4tires->tires[3];
			int speed1 = popa4tires->speed1;
			int speed2 = popa4tires->speed2;
			int speed3 = popa4tires->speed3;
			int speed4 = popa4tires->speed4;
#ifdef WRITELOG
			printf("coming  pos2dset4Tires : %d %d %d %d, %d %d %d %d\n", id1,
					id2, id3, id4, speed1, speed2, speed3, speed4);
#endif
			MFSetServoRotaSpd(id1, speed1);
			MFSetServoRotaSpd(id2, speed2);
			MFSetServoRotaSpd(id3, speed3);
			MFSetServoRotaSpd(id4, speed4);
			return 0;
		} else {
#ifdef WRITELOG
			printf("coming worng pos2dset4Tires !\n");
#endif
			return -1;
		}
	} else if (POS2DSET4TIRESPOSF == popa->subtype) {

		if (opaquedata->data_count == sizeof(myOpaqueSt4TiresPos)) {
			myOpaqueSt4TiresPos *popa4TiresPos =
					(myOpaqueSt4TiresPos*) opaquedata->data;
			int id1 = popa4TiresPos->tires[0];
			int id2 = popa4TiresPos->tires[1];
			int id3 = popa4TiresPos->tires[2];
			int id4 = popa4TiresPos->tires[3];
			int speed = popa4TiresPos->speed;
			int pos = popa4TiresPos->pos;
#ifdef WRITELOG
			printf("coming pos2dset4TiresPos :pos %d,speed %d,%d %d %d %d\n",
					pos, speed, id1, id2, id3, id4);
#endif
			MFSetServoPos(id1, pos, speed);
			MFSetServoPos(id2, pos, speed);
			MFSetServoPos(id3, pos, speed);
			MFSetServoPos(id4, pos, speed);
			return 0;
		} else {
#ifdef WRITELOG
			printf("coming worng pos2dset4TiresPos !\n");
#endif
			return -1;
		}
		return 0;
	}

	return 0;
}
inline int deaperDealAIO(int t, int type) {
	if (type == whatKindAIOAIOINFRAREDR) {
		int dis = 1;
#ifdef WRITELOG
#endif
		if (t > 500) {
			return 1;
		} else if (t <= 500 && t > 470) {
			return 2;
		} else if (t <= 470 && t > 400) {
			return 5;
		} else if (t <= 400 && t > 350) {
			return 6;
		} else if (t <= 350 && t > 320) {
			return 7;
		} else if (t <= 320 && t > 230) {
			return 10;
		} else if (t <= 235 && t > 115) {
			return (20 - (t - 115) / 12);
		} else if (t <= 115 && t > 72) {
			return (30 - (t - 72) / 4);
		} else if (t <= 72 && t > 52) {
			return (40 - (t - 52) / 2);
		} else if (t <= 52 && t > 42) {
			return (92 - t);
		} else if (t <= 42 && t > 33) {
			return (60 - (10 * t - 330) / 9);
		} else if (t <= 33 && t > 29) {
			return (70 - (10 * t - 290) / 4);
		} else if (t <= 29 && t > 23) {
			return (80 - (10 * t - 230) / 6);
		} else
			return 80;
		return dis;
	} else {
		return t;
	}
}

inline int deaperDealAIOOld(int t, int type) {
	if (type == whatKindAIOAIOINFRAREDR) {
		float volt = 0.0;
		t *= 5;
		float ft = (float) t;
		volt = ft / 1023;
#ifdef WRITELOG
		printf("t %d , ft %f , volt %f\n", t, ft, volt);
#endif

		int tmp = 0;

		if (volt > 2.45)
			return 10;
		else if (volt > 1.4) {
			ft = 20 - 10 * ((volt - 1.4) / 1.05);
			return (int) ft;
		} else if (volt > 1.0) {
			ft = 30 - 10 * ((volt - 1.0) / 0.4);
			return (int) ft;
		} else if (volt > 0.75) {
			ft = 40 - 10 * ((volt - 0.75) / 0.25);
			return (int) ft;
		} else if (volt > 0.6) {
			ft = 50 - 10 * ((volt - 0.6) / 0.15);
			return (int) ft;
		} else if (volt > 0.53) {
			ft = 60 - 10 * ((volt - 0.53) / 0.07);
			return (int) ft;
		} else if (volt > 0.45) {
			ft = 70 - 10 * ((volt - 0.45) / 0.08);
			return (int) ft;
		} else if (volt > 0, 4) {
			ft = 80 - 10 * ((volt - 0.4) / 0.05);
			return (int) ft;
		} else
			return 80;
	} else {
		return t;
	}
}
int multidriver::dealOpaqueAIOComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	res->p1 = MFGetAD(popa->p1);
	if (popa->p2 != whatKindAIOAIODEF)
		res->p1 = deaperDealAIO(res->p1, popa->p2);
	return 0;
}
int multidriver::dealOpaquePosition2dComReq(myOpaqueSt *popa, myOpaqueSt *res) {
	res->type = popa->type;
	res->subtype = popa->subtype;
	if (POS2DFORWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, (popa->p1));
		MFSetServoRotaSpd(this->frontr, -(popa->p1));
		MFSetServoRotaSpd(this->behindl, -(popa->p1));
		MFSetServoRotaSpd(this->behindr, popa->p1);
		MFServoAction();
	} else if (POS2DBACKWARDF == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, -(popa->p1));
		MFSetServoRotaSpd(this->frontr, (popa->p1));
		MFSetServoRotaSpd(this->behindl, popa->p1);
		MFSetServoRotaSpd(this->behindr, -(popa->p1));
		MFServoAction();
	} else if (POS2DTURNLEFTF == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, -(popa->p1));
		MFSetServoRotaSpd(this->frontr, -(popa->p1));
		MFSetServoRotaSpd(this->behindl, -(popa->p1));
		MFSetServoRotaSpd(this->behindr, -(popa->p1));
		MFServoAction();
	} else if (POS2DTURNRIGHT == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, popa->p1);
		MFSetServoRotaSpd(this->frontr, popa->p1);
		MFSetServoRotaSpd(this->behindl, popa->p1);
		MFSetServoRotaSpd(this->behindr, popa->p1);
		MFServoAction();
	} else if (POS2DSTOP == popa->subtype) {
		MFSetServoRotaSpd(this->frontl, 0);
		MFSetServoRotaSpd(this->frontr, 0);
		MFSetServoRotaSpd(this->behindl, 0);
		MFSetServoRotaSpd(this->behindr, 0);
		MFServoAction();
	} else if (POS2DSERVOMODESERVOF == popa->subtype) {
		MFSetServoMode(this->frontl, 0);
		MFSetServoMode(this->frontr, 0);
		MFSetServoMode(this->behindl, 0);
		MFSetServoMode(this->behindr, 0);
	} else if (POS2DSERVOMODEMOTOF == popa->subtype) {
		MFSetServoMode(this->frontl, 1);
		MFSetServoMode(this->frontr, 1);
		MFSetServoMode(this->behindl, 1);
		MFSetServoMode(this->behindr, 1);
	} else if (POS2DSETSERVOTURN == popa->subtype) {
		MFSetServoPos(this->frontl, popa->p1, popa->p2);
		MFSetServoPos(this->frontr, popa->p1, popa->p2);
		MFSetServoPos(this->behindl, popa->p1, popa->p2);
		MFSetServoPos(this->behindr, popa->p1, popa->p2);
		MFServoAction();
	} else if (POS2DSETFRONTL == popa->subtype) {
		this->frontl = popa->p1;
	} else if (POS2DSETFRONTR == popa->subtype) {
		this->frontr = popa->p1;
	} else if (POS2DSETREARL == popa->subtype) {
		this->behindl = popa->p1;
	} else if (POS2DSETREARR == popa->subtype) {
		this->behindr = popa->p1;
	} else if (POS2DGETFRONTL == popa->subtype) {
		res->p1 = this->frontl;
	} else if (POS2DGETFRONTR == popa->subtype) {
		res->p1 = this->frontr;
	} else if (POS2DGETREARL == popa->subtype) {
		res->p1 = this->behindl;
	} else if (POS2DGETREARR == popa->subtype) {
		res->p1 = this->behindr;
	} else if (POS2DSETTIRES == popa->subtype) {
		this->frontl = (popa->p1) & 0x0000FFFF;
		this->frontr = (popa->p1) >> 16;
		this->behindl = (popa->p2) & 0x0000FFFF;
		this->behindr = (popa->p2) >> 16;
	} else if (POS2DGETTIRES == popa->subtype) {
		unsigned int ttire = 0x00000000;
		ttire |= ((this->frontl & 0x0000FFFF)
				| ((this->frontr << 16) & 0xFFFF0000));
#ifdef WRITELOG
		printf("int size %d\n", sizeof(int));
		printf("test tire id %x , %x , %x\n", (this->frontl & 0x0000FFFF),
				(this->frontr << 16) & 0xFFFF0000, ttire);
#endif
		res->p1 = ttire;
		ttire = 0x00000000;
		ttire |= ((this->behindl & 0x0000FFFF)
				| ((this->behindr << 16) & 0xFFFF0000));
		res->p2 = ttire;
#ifdef WRITELOG
		printf("test tire id %x , %x , %x\n", (this->behindl & 0x0000FFFF),
				(this->behindr << 16) & 0xFFFF0000, ttire);
#endif
	} else {
		return -1;
	}
	return 0;
}
int multidriver::dealOpaqueDIOComReq(myOpaqueSt *popa, myOpaqueSt *pr) {
	pr->type = popa->type;
	pr->subtype = popa->subtype;
	if (DIOSETPORTDIRECTF == popa->subtype) {
		MFSetPortDirect(popa->p1);
	} else if (DIOGETDIGIINPUTF == popa->subtype) {
		pr->p1 = MFGetDigiInput(popa->p1);
	} else if (DIODIGIOUTPUTF == popa->subtype) {
		MFDigiOutput(popa->p1, popa->p2);
	} else {
		return -1;
	}
	return 0;
}

int multidriver::dealOpaqueMessages(QueuePointer & resp_queue,
		player_msghdr * hdr, void * data) {
	/*
	 * struct myOpaqueSt{
	 int type;
	 int subtype;
	 int p1;
	 int p2;
	 };
	 * */
	player_opaque_data_t *opaquedata = (player_opaque_data_t *) data;

	myOpaqueSt *popa = (myOpaqueSt *) opaquedata->data;
	if (popa->type == MYOPAQUESTTYPEMFCAP) {
		dealOpaqueMFCAP(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEAIO) {
		dealOpaqueAIO(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEDIO) {
		dealOpaqueDIO(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESTTYPEPOS) {
		dealOpaquePosition2d(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUEBk) {
		dealOpaqueBK(resp_queue, hdr, data);
		return 0;
	} else if (popa->type == MYOPAQUESR) {
		dealOpaqueSR(resp_queue, hdr, data);
		return 0;
	} else {
#ifdef WRITELOG
		printf("coming opaque messages type wrong ! \n");
#endif
		return -1;
	}

}

int multidriver::dealDIOmessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
	if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, PLAYER_DIO_CMD_VALUES,
			*(player_devaddr_t *) &(this->m_dio_addr))) {

		player_dio_cmd_t *pdc = (player_dio_cmd_t *) data;
		player_dio_data_t pdd;
		this->getDIOValue(pdc, &pdd);

		Publish(this->m_dio_addr, resp_queue, PLAYER_MSGTYPE_DATA,
				PLAYER_DIO_DATA_VALUES, (void*) &pdd, sizeof(pdd), NULL);

		return 0;
	} else {
#ifdef WRITELOG
		this->ndiomc++;
#endif
		return 0;
	}
	return -1;
}

int multidriver::dealPOSmessages(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {

	if (hdr->type == (uint8_t) PLAYER_MSGTYPE_CMD) {
		player_position2d_cmd_vel *pposc = (player_position2d_cmd_vel *) data;
#ifdef WRITELOG
		printf("--postion-- %f , %f , %f \n", pposc->vel.px, pposc->vel.py,
				pposc->vel.pa);
#endif
		/*px
		 * 1.0 - forward
		 * 2.0 - backward
		 * 3.0 - stop
		 * 4.0 - left round
		 * 5.0 - right round
		 *
		 * pa - velocity
		 */
		if (1.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, (int) pposc->vel.pa);
			MFServoAction();
		} else if (2.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, -(int) pposc->vel.pa);
			MFServoAction();
		} else if (3.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, 0);
			MFSetServoRotaSpd(this->frontr, 0);
			MFSetServoRotaSpd(this->behindl, 0);
			MFSetServoRotaSpd(this->behindr, 0);
			MFServoAction();
		} else if (4.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, -(int) pposc->vel.pa);
			MFServoAction();
		} else if (5.0 == pposc->vel.px) {
			MFSetServoRotaSpd(this->frontl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->frontr, (int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindl, -(int) pposc->vel.pa);
			MFSetServoRotaSpd(this->behindr, (int) pposc->vel.pa);
			MFServoAction();
		} else {
#ifdef WRITELOG
			this->nposmc++;
#endif
			return -1;
		}

		return 0;
	}
#ifdef WRITELOG
	this->nposmc++;
#endif
	return -1;
}

int multidriver::ProcessMessage(QueuePointer & resp_queue, player_msghdr * hdr,
		void * data) {
// Process messages here.  Send a response if necessary, using Publish().
// If you handle the message successfully, return 0.  Otherwise,
// return -1, and a NACK will be sent for you, if a response is required.

	myOpaqueHI *popaqueHI = (myOpaqueHI*) data;
	if ((this->fDealAllComReq)[popaqueHI->head.subtype])
		((this->fDealAllComReq)[popaqueHI->head.subtype])(resp_queue, hdr,
				data);
	return -1;
}

//0->don't need to exec , 1->need to exec
inline int recordCommandStatue(motoRecord *pmr, int id, int st) {
	int ret = 0;
	id -= 1;
	if (st == (pmr + id)->statue)
		return ret;
	else {
		(pmr + id)->statue = st;
		return 1;
	}
}
inline int recordCommandValue(motoRecord *pmr, int id, int val) {
	int ret = 0;
	id -= 1;
	if (val == (pmr + id)->value)
		return ret;
	else {
		(pmr + id)->value = val;
		return 1;
	}
}

inline void initialMotoRecordTable(motoRecord *pmr, int size) {
	for (int i = 0; i < size; i++) {
		(pmr + i)->statue = SEVOMODE;
		(pmr + i)->value = 0;
	}
	return;
}

void multidriver::writeLog(char *strLog) {
	if (NULL != this->fl) {
		fputs(strLog, this->fl);
	}
	return;
}
////////////////////////////////////////////////////////////////////////////////
// Main function for device thread
void multidriver::Main() {
// The main loop; interact with the device here
	/*
	 #ifdef WRITELOG
	 printf(LABLESTRING);
	 printf("In Main , begin to processMessages !\n");
	 int i = 0;
	 #endif
	 */
	motoRecord mr[MAXTIRENUMBER];
	initialMotoRecordTable(mr, MAXTIRENUMBER);

	int maxmsgs = 0;
	for (;;) {
		// test if we are supposed to cancel
		pthread_testcancel();

		// Process incoming messages.  multidriver::ProcessMessage() is
		// called on each message.
		//ProcessMessages();

		TestCancel();
		maxmsgs = 0;
		// See if we have any pending messages and process them
		maxmsgs = this->InQueue->GetLength();
		Message** msg = new (Message*)(maxmsgs);
		if (NULL == msg) {
			continue;
		}
		for (int i = 0; i < maxmsgs; i++)
			msg[i] = NULL;
		for (int i = 0; i < maxmsgs; i++) {
			msg[i] = this->InQueue->Pop();
		}
		player_msghdr **hdr = new (player_msghdr*)(maxmsgs);
		for (int i = 0; i < maxmsgs; i++) {
			hdr[i] = msg[i]->GetHeader();
		}
		void ** data = new (void*)(maxmsgs);
		for (int i = 0; i < maxmsgs; i++) {
			data[i] = msg[i]->GetPayload();
		}

		//first time do the moto command
		for (int i = 0; i < maxmsgs; i++) {
			if (NULL == msg[i])
				continue;
			//1 . moto command
			myOpaqueHI *receivMsg = (myOpaqueHI*) data[i];
			if (COMTYPE1 != (receivMsg->head.type)) {
				continue;
			}

			//2. wether duplicate

			int ret = this->ProcessMessage(msg[i]->Queue, hdr[i], data[i]);

			delete msg[i];
		}
		//second time do others command + delete the duplicate command
		for (int i = 0; i < maxmsgs; i++) {
			//1.except the NULL message
			if (NULL == msg[i])
				continue;
			//2. do first + delete the duplicate(if have new command arrived)  ->unfinish

			int ret = this->ProcessMessage(msg[i]->Queue, hdr[i], data[i]);
			if (ret < 0) {
				// Check if it's an internal message, if that doesn't handle it, give a warning
				if (ProcessInternalMessages(msg[i]->Queue, hdr[i], data[i])
						!= 0) {
					PLAYER_WARN7("Unhandled message for driver "
					"device=%d:%d:%s:%d type=%s subtype=%d len=%d\n",
							hdr[i]->addr.host, hdr[i]->addr.robot,
							interf_to_str(hdr[i]->addr.interf),
							hdr[i]->addr.index, msgtype_to_str(hdr[i]->type),
							hdr[i]->subtype, hdr[i]->size);

					// If it was a request, reply with an empty NACK
					if (hdr[i]->type == PLAYER_MSGTYPE_REQ)
						this->Publish(hdr[i]->addr, msg[i]->Queue,
								PLAYER_MSGTYPE_RESP_NACK, hdr[i]->subtype, NULL,
								0, NULL);
				}
			}
			delete msg[i];
		}

		TestCancel();
		/*
		 #ifdef WRITELOG
		 printf(LABLESTRING);
		 printf("processMessages times : %d\n", i++);
		 #endif
		 */

		//printf("ProcessMessages() excuted\n");
		// Interact with the device, and push out the resulting data, using
		// Driver::Publish()
		// Sleep (you might, for DIO, block on a read() instead)
		//??????????????????????????????????
		//usleep(100000);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Extra stuff for building a shared object.

/* need the extern to avoid C++ name-mangling  */
extern "C" {
int player_driver_init(DriverTable* table) {
#ifdef WRITELOG
	puts("multidriver initializing");
#endif
	multidriver_Register(table);
	return (0);
}
}

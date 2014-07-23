#ifndef JinjSlave_H
#define JinjSlave_H
#include "amswcom.h"

typedef unsigned short u_short;

#define NTDRS 24 //maximum number of TDRs (for Jinfs)


class ConfPars{

public:
	int type;
	u_short JLV1flash;
  u_short JINFflash;
  u_short TDRflash; 
  u_short delay; 
  char DATAPATH[255];
  char CALPATH[255];
  int Ntdrs;
  u_short mode[NTDRS];
  unsigned int refmask;
  int ReadConfig(char* filename);
	ConfPars();
	int SLowTrash[NTDRS];
	int SHighTrash[NTDRS];
	int KLowTrash[NTDRS];
	int KHighTrash[NTDRS];
	int SigmaRowTrash[NTDRS];
	int DeltaTdoubTrig[NTDRS];
};


class JinjSlave{

public:
  // FILE * logfile;
  char myname[20];
  int broadcastadd;
  int  selfaddress;
  AMSWcom *node;
  SlaveMask mask;
  JinjSlave(char* name,char* conf_file,int address,AMSWcom* node);
  ~JinjSlave();
	ConfPars *CPars;
	int jinfevents;
  int tdrevents[24];
	int jlv1events;
	virtual int Init(){};
	virtual int SetDelay(){};
	virtual int SetMode(int jinjslavemode){};
	virtual int GetEventNumber(){};
	virtual char* PrintAllEventNumber(int log=1,int jinjslavenum=0){};
	virtual int EventReset(){};

private:
 
};

#endif
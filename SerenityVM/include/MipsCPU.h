#ifndef __MIPSCPU__
#define __MIPSCPU__

#include <MemoryManageUnit.h>
#include <fstream>
#include <stdio.h>
#include <KeyCode.h>

#define MMU_SIZE 0x5202
#define INITSP 0x2FFE
#define VMADR 0x3000
#define SYSADR 0x4000
#define TVTADR 0x4500
#define INTADR 0x4600
#define IVTADR 0x4B00
#define INTENTRY 0x4F00
#define BKADR 0x5000
#define DADDR 0x5001
#define DCOMM 0x5002
#define DSIGN 0x5003
#define DCONT 0x5100

#define D_COMM_NONE 0
#define D_COMM_WRITE 1
#define D_COMM_READ 2

using namespace std;
class MipsCPU
{
public:
	MipsCPU();
	~MipsCPU();
	void boot(ifstream &fin);
	void printReg();
	void run();
	void setDebug(bool d);
private:
	FILE* disk;
	int rgf[32];
	int cpf[32];
	MemoryManageUnit MMU;
	int PC;
	int IR;
	int	op, rd, rs, rt, sft, fun, dat, adr;
	bool debug;
	string operation;
	bool chkInt();
};
#endif
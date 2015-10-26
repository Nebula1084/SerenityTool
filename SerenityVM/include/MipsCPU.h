#ifndef __MIPSCPU__
#define __MIPSCPU__

#include <MemoryManageUnit.h>
#include <fstream>

#define MMU_SIZE 0x5000
#define INITSP 0x2FFE
#define VMADR 0x3000
#define SYSADR 0x4000
#define TVTADR 0x4500
#define INTADR 0x4600
#define IVTADR 0x4B00
#define INTENTRY 0x4F00

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
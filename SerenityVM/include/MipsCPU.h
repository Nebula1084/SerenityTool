#ifndef __MIPSCPU__
#define __MIPSCPU__

#include <MemoryManageUnit.h>
#include <fstream>

#define MMU_SIZE 0x1000
#define CRTADR 0xc00
using namespace std;
class MipsCPU
{
public:
	MipsCPU();
	~MipsCPU();
	void boot(ifstream &fin);
	void printReg();
	void run();
private:
	int rgf[32];
	MemoryManageUnit MMU;
	int PC;
	int IR;
	int	op, rd, rs, rt, sft, fun, dat, adr;
	string operation;
};
#endif
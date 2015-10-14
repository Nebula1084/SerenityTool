#ifndef __ASSEMBLEINFO__
#define __ASSEMBLEINFO__

#include <CommonTypedef.h>
#include <map>

typedef enum{RegIns,ImmIns,SpcIns,JumpIns,LoadSaveIns} InsType;

class AssembleInfo
{
public:
	map<AssembleIns,Instruction> opcode;//指令模版
	map<Register,Instruction> reg;//寄存器编码
	map<AssembleIns, map<RegType,bool> > InsRegList;//各个指令需要填的寄存器
	map<AssembleIns, InsType> insType;//指令类型，你们用不到的
	AssembleInfo();
	/* data */
};
#endif
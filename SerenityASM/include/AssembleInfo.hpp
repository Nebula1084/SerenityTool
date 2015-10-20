//
//  AssembleInfo.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef AssembleInfo_hpp
#define AssembleInfo_hpp

#include "CommonTypedef.hpp"
#include <map>

class AssembleInfo
{
public:
    map<OpName, MachineCode> opcode;//指令模版
    map<Register, MachineCode> reg;//寄存器编码
//    map<OpName, map<RegType,bool> > InsRegList;//各个指令需要填的寄存器
    map<OpName, InsType> insType;//指令类型
    map<Register, MachineCode> coreg; // 协寄存器编码
    AssembleInfo();
    /* data */
};

#endif /* AssembleInfo_hpp */

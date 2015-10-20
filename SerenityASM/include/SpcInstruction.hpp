//
//  SpcInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef SpcInstruction_hpp
#define SpcInstruction_hpp

#include "Instruction.hpp"

class SpcInstruction: public Instruction
{
public:
    SpcInstruction(Instruction &instruction);
    SpcInstruction();
    ~SpcInstruction();
    MachineCode assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine);  // 汇编，正式转机器码
};

#endif /* SpcInstruction_hpp */

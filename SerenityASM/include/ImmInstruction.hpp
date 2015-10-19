//
//  ImmInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef ImmInstruction_hpp
#define ImmInstruction_hpp

#include "Instruction.hpp"

class ImmInstruction: public Instruction
{
public:
    ImmInstruction(Instruction &instruction);
    ImmInstruction();
    ~ImmInstruction();
    MachineCode assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine);
};

#endif /* ImmInstruction_hpp */

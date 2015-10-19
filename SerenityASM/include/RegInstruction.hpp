//
//  RegInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef RegInstruction_hpp
#define RegInstruction_hpp

#include "Instruction.hpp"

class RegInstruction: public Instruction
{
public:
    RegInstruction(Instruction &instruction);
    RegInstruction();
    ~RegInstruction();
    MachineCode assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine);
};

#endif /* RegInstruction_hpp */

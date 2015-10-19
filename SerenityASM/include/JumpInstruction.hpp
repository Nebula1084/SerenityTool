//
//  JumpInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef JumpInstruction_hpp
#define JumpInstruction_hpp

#include "Instruction.hpp"

class JumpInstruction: public Instruction
{
public:
    JumpInstruction(Instruction &instruction);
    JumpInstruction();
    ~JumpInstruction();
    MachineCode assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine);
};

#endif /* JumpInstruction_hpp */

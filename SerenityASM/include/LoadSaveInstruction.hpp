//
//  LoadSaveInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef LoadSaveInstruction_hpp
#define LoadSaveInstruction_hpp

#include "Instruction.hpp"

class LoadSaveInstruction: public Instruction
{
public:
    LoadSaveInstruction(Instruction &instruction);
    LoadSaveInstruction();
    ~LoadSaveInstruction();
    MachineCode assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine);
};

#endif /* LoadSaveInstruction_hpp */

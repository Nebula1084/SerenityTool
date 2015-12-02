//
//  FormatInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef FormatInstruction_hpp
#define FormatInstruction_hpp

#include "Instruction.hpp"

class FormatInstruction: public Instruction
{
public:
    FormatInstruction(Instruction &instruction);
    FormatInstruction();
    ~FormatInstruction();
    void assemble(AssembleInfo &assembleInfo, ofstream &fout, map<Label, Address> &labelTable);
    void print(MachineCode machineCode, ofstream &fout);
};

#endif /* FormatInstruction_hpp */

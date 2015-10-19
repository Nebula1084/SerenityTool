//
//  PseudoInstruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef PseudoInstruction_hpp
#define PseudoInstruction_hpp

#include "Instruction.hpp"
#include <map>

class PseudoInstruction: public Instruction
{
public:
    PseudoInstruction(Instruction &instruction);
    PseudoInstruction();
    ~PseudoInstruction();
    void translate(vector<Instruction *> &instructionSet, map<Label, Address> &labelTable);
};

#endif /* PseudoInstruction_hpp */

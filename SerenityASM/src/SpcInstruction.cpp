//
//  SpcInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "SpcInstruction.hpp"
#include "AssembleInfo.hpp"

SpcInstruction::SpcInstruction(Instruction &instruction): Instruction(instruction)
{}

SpcInstruction::SpcInstruction()
{}

SpcInstruction::~SpcInstruction()
{}

MachineCode SpcInstruction::assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine)
{
    OpName opName = getOpName();
    MachineCode ins = assembleInfo.opcode[opName];
    if (opName == "eret") {  // eret
        if (numOfOperand() != 0)   // eret无操作数
            printErrorInfo(The_amount_of_operand_is_wrong);
    }
    else if (opName == "mul") {  // mul rd,rs,rt
        if (numOfOperand() != 3)  // mul有3个操作数
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1) && islegalreg(2)))  // 3个操作数都是寄存器
            printErrorInfo(No_such_register);
        ins |= assembleInfo.reg[getOperand(0)] << 11;
        ins |= assembleInfo.reg[getOperand(1)] << 21;
        ins |= assembleInfo.reg[getOperand(2)] << 16;
    }
    else {
        // mfc0,mtc0
        // op rt,sa
        if (numOfOperand() != 2)  // 两个操作数
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))  // 第一个是主寄存器
            printErrorInfo(No_such_register);
        if (!islegalcoreg(1))  // 第二个是协寄存器
            printErrorInfo(No_such_coregister);
        ins |= assembleInfo.reg[getOperand(0)] << 16;
        ins |= assembleInfo.coreg[getOperand(1)] << 11;
    }
    return ins;
}


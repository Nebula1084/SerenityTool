//
//  LoadSaveInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "LoadSaveInstruction.hpp"

LoadSaveInstruction::LoadSaveInstruction(Instruction &instruction): Instruction(instruction)
{}

LoadSaveInstruction::LoadSaveInstruction()
{}

LoadSaveInstruction::~LoadSaveInstruction()
{}

MachineCode LoadSaveInstruction::assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine)
/*
 lb,lh,lw,lbu,lhu,sb,sh,sw
 op rt,imme(rs)
 */
{
    OpName opName = getOpName();
    MachineCode ins = assembleInfo.opcode[opName];
    if (numOfOperand() != 3)
        printErrorInfo(The_amount_of_operand_is_wrong);
    if (!(islegalreg(0) && islegalreg(2)))
        printErrorInfo(No_such_register);
    ins |= assembleInfo.reg[getOperand(0)] << 16;  // 寄存器
    ins |= assembleInfo.reg[getOperand(2)] << 21;
    int imme = immatoi(1);
    if (imme == errorIns)
        printErrorInfo(Wrong_immediate_number_or_offset);
    if (imme >= 0)
        ins |= imme;
    else ins |= (imme & 0x0000ffff);
    return ins;
}
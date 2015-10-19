//
//  ImmInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "ImmInstruction.hpp"

ImmInstruction::ImmInstruction(Instruction &instruction): Instruction(instruction)
{}

ImmInstruction::ImmInstruction()
{}

ImmInstruction::~ImmInstruction()
{}

MachineCode ImmInstruction::assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine)
{
    OpName opName = getOpName();
    MachineCode ins = assembleInfo.opcode[opName];
    if (opName == "lui") {
        // lui rt,imme
        if (numOfOperand() != 2)
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        int imme = immatoi(1);
        if (imme == errorIns)  // imme违法（违法字符或超出16位）
            printErrorInfo(Wrong_immediate_number_or_offset);
        ins |= assembleInfo.reg[getOperand(0)] << 16;
        if (imme >= 0)  // 符号扩展
            ins |= imme;
        else ins |= (imme & 0x0000ffff);
    }
    else {
        /*
         addi、addiu、slti、sltiu、andi、ori、xori
         op rt,rs,imme
         addi和slti中imme是有符号数符号扩展，其余为无符号数，用零扩展
         */
        if (numOfOperand() != 3)  //  三个操作数
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))  // 两个寄存器
            printErrorInfo(No_such_register);
        ins |= assembleInfo.reg[getOperand(0)] << 16;
        ins |= assembleInfo.reg[getOperand(1)] << 21;
        if (opName == "addi" || opName == "slti") {  // 有符号立即数
            int imme = immatoi(2);
            if (imme == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
            if (imme >= 0)
                ins |= imme;
            else ins |= (imme & 0x0000ffff);
        }
        else {  // 无符号立即数
            unsigned int imme = immatoi(2, 16, false);
            if (imme == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
            ins |= imme;
        }
    }
    return ins;
}
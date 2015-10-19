//
//  RegInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "RegInstruction.hpp"

RegInstruction::RegInstruction(Instruction &instruction) : Instruction(instruction)
{}

RegInstruction::RegInstruction()
{}

RegInstruction::~RegInstruction()
{}

MachineCode RegInstruction::assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine)
{
    OpName opName = getOpName();
    MachineCode ins = assembleInfo.opcode[opName];
    if (opName == "syscall") {
        if (numOfOperand() != 0)
            printErrorInfo(The_amount_of_operand_is_wrong);
    }
    else if (opName == "jr" || opName == "mthi" || opName == "mtlo" || opName == "mfhi" || opName == "mflo") {
        // 一个操作数
        if (numOfOperand() != 1)
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        if (opName == "mfhi" || opName == "mflo")  // op rd
            ins |= assembleInfo.reg[getOperand(0)] << 11;
        else ins |= assembleInfo.reg[getOperand(0)] << 21;  // op rs
    }
    else if (opName == "jalr" || opName == "mult" || opName == "multu" || opName == "div" || opName == "divu") {
        // 两个操作数
        if (numOfOperand() != 2)
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        ins |= assembleInfo.reg[getOperand(0)] << 21;
        if (opName == "jalr")   // op rs,rd
            ins |= assembleInfo.reg[getOperand(1)] << 11;
        else ins |= assembleInfo.reg[getOperand(1)] << 16; // op rs,rt
    }
    else {  // 三个操作数
        if (numOfOperand() != 3)
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        ins |= assembleInfo.reg[getOperand(0)] << 11; // 第一个操作数均为rd
        if (opName == "sll" || opName == "srl" || opName == "sra") {
            // op rd,rt,sa
            if (!islegalreg(1))
                printErrorInfo(No_such_register);
            ins |= assembleInfo.reg[getOperand(1)] << 16;
            int sa = immatoi(2, 5, false);
            if (sa == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
            ins |= (sa << 6);
        }
        else if (opName == "sllv" || opName == "srlv" || opName == "srav") {
            // op rd,rt,rs
            if (!(islegalreg(1) && islegalreg(2)))
                printErrorInfo(The_amount_of_operand_is_wrong);
            ins |= assembleInfo.reg[getOperand(1)] << 16;
            ins |= assembleInfo.reg[getOperand(2)] << 21;
        }
        else {
        // add、addu、sub、subu、slt、sltuand、or、xor、nor
        // op rd,rs,rt
            if (!(islegalreg(1) && islegalreg(2)))
                printErrorInfo(The_amount_of_operand_is_wrong);
            ins |= assembleInfo.reg[getOperand(1)] << 21;
            ins |= assembleInfo.reg[getOperand(2)] << 16;
        }
    }
    return ins;
}










//
//  JumpInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "JumpInstruction.hpp"
#include "global.hpp"

JumpInstruction::JumpInstruction(Instruction &instruction) : Instruction(instruction)
{}

JumpInstruction::JumpInstruction()
{}

JumpInstruction::~JumpInstruction()
{}

MachineCode JumpInstruction::assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine)
{
    OpName opName = getOpName();
    MachineCode ins = assembleInfo.opcode[opName];
    int imme;
    
    if (opName == "j" || opName == "jal") {  // j adr
        if (numOfOperand() != 1)
            printErrorInfo(The_amount_of_operand_is_wrong);
        string addr = getOperand(0);
        if (labelTable.find(addr) == labelTable.end()) {  // 存在该label
            imme = immatoi(0, 26, false);  // 26位imme
            if (imme == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
        }
        else imme = labelTable[addr] / 2;
        ins |= (imme & 0x03ffffff);
    }
    else if (opName == "beq" || opName == "bne") { // beq rs,rt,imme
        if (numOfOperand() != 3)
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        string addr = getOperand(2);
        if (labelTable.find(addr) == labelTable.end()) {
            imme = immatoi(2, 16);
            if (imme == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
        }
        else {
            imme = (labelTable[addr] - actualLine - 1) * 2;
            if (imme < -32768 || imme > 32767)  // imme 是否过大
                printErrorInfo(Wrong_immediate_number_or_offset);
        }
        ins |= assembleInfo.reg[getOperand(0)] << 21;
        ins |= assembleInfo.reg[getOperand(1)] << 16;
        ins |= (imme & 0x0000ffff);
    }
    else {
        // bltz、bgtz、blez、bgez、blezal、bgezal
        // op rs,imme
        if (numOfOperand() != 2)
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        string addr = getOperand(1);
        if (labelTable.find(addr) == labelTable.end()) {
            imme = immatoi(1, 16);
            if (imme == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
        }
        else {
            imme = (labelTable[addr] - actualLine - 1) * 2;
            if (imme < -32768 || imme > 32767)  // imme 是否过大
                printErrorInfo(Wrong_immediate_number_or_offset);
        }
        ins |= assembleInfo.reg[getOperand(0)] << 21;
        ins |= (imme & 0x0000ffff);
    }
    return ins;
}

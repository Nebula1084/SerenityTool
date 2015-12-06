//
//  PseudoInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "PseudoInstruction.hpp"
#include "RegInstruction.hpp"
#include "ImmInstruction.hpp"
#include "LoadSaveInstruction.hpp"
#include "JumpInstruction.hpp"
#include "global.hpp"
#include <iostream>

PseudoInstruction::PseudoInstruction(Instruction &instruction): Instruction(instruction)
{
}

PseudoInstruction::PseudoInstruction() {}

PseudoInstruction::~PseudoInstruction() {}

void PseudoInstruction::translate(vector<Instruction *> &instructionSet, map<Label, Address> &labelTable)
{
    OpName opName = getOpName();
    Instruction *newLine;
    AssemblyCode assemblyCode;
    
    if (opName == "move") {
        // move rd,rs  ->  add rd,rs,$zero
        if (!checkOperand(2))   // 检查操作数数量
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))  // 检查寄存器是否合法
            printErrorInfo(No_such_register);
        newLine = new RegInstruction;
        if (hasLabel())  // 原行若有label
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "add " + getOperand(0) + "," + getOperand(1) + ",$zero";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "not") {
        // not rt,rs -> nor rt,rs,$zero
        if (!checkOperand(2))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        newLine = new RegInstruction;
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "nor " + getOperand(0) + "," + getOperand(1) + ",$zero";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "neg") {
        // neg rt,rs -> sub rt,$zero,rs
        if (!checkOperand(2))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        newLine = new RegInstruction;
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "sub " + getOperand(0) + ",$zero," + getOperand(1);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "push") {
        // push $r1 -> addi $sp,$sp,-2
        //             sw $r1,0($sp)
        // push $r1, $r2, ...
        if (checkOperand(0))
            printErrorInfo(The_amount_of_operand_is_wrong);
        unsigned long num = numOfOperand();
        newLine = new ImmInstruction;   // addi $sp,$sp,-2*num
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        int imm = num * -2;
        assemblyCode = assemblyCode + "addi $sp,$sp," + immitoa(imm);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        for (int i = 0; i < num; i++) {
            if (!islegalreg(i))
                printErrorInfo(No_such_register);
            newLine = new LoadSaveInstruction;  // sw $r1,i * 2($sp)
            assemblyCode = "sw " + getOperand(i) + "," + immitoa(i * 2) + "($sp)";
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
        }
    }
    else if (opName == "pop") {
        // pop $r1 -> lw $r1,0($sp)
        //            addi $sp,$sp,2
        // pop $r1, $r2, ...
        if (checkOperand(0))
            printErrorInfo(The_amount_of_operand_is_wrong);
        unsigned long num = numOfOperand();
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        newLine = new LoadSaveInstruction;  // lw $r1,i * 2($sp)
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "lw " + getOperand(0) + ",0($sp)";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        for (int i = 1; i < num; i++) {
            newLine = new LoadSaveInstruction;
            assemblyCode = "lw " + getOperand(i) + "," + immitoa(i * 2) + "($sp)";
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
        }
        newLine = new ImmInstruction;  // addi $sp,$sp,2 * num
        assemblyCode = "addi $sp,$sp," + immitoa(2 * num);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "blt") {
        // blt rs,rt,RR -> slt $at,rs,rt
        //                 bne $at,$zero,RR
        if (!checkOperand(3))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        if (labelTable.find(getOperand(2)) == labelTable.end())
            printErrorInfo(No_such_label);
        newLine = new RegInstruction;  // slt $at,rs,rt
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "slt $at," + getOperand(0) + "," + getOperand(1);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new JumpInstruction;  // bne $at,$zero,RR
        assemblyCode = "bne $at,$zero," + getOperand(2);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "ble") {
        // ble rs,rt,RR -> slt $at,rt,rs
        //                 beq $at,$zero,RR
        if (!checkOperand(3))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        if (labelTable.find(getOperand(2)) == labelTable.end())
            printErrorInfo(No_such_label);
        newLine = new RegInstruction;  // slt $at,rt,rs
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "slt $at," + getOperand(1) + "," + getOperand(0);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new JumpInstruction;  // beq $at,$zero,RR
        assemblyCode = "beq $at,$zero," + getOperand(2);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "bgt") {
        // bgt rs,rt,RR -> slt $at,rt,rs
        //                 bne $at,$zero,RR
        if (!checkOperand(3))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        if (labelTable.find(getOperand(2)) == labelTable.end())
            printErrorInfo(No_such_label);
        newLine = new RegInstruction;  // slt $at,rt,rs
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "slt $at," + getOperand(1) + "," + getOperand(0);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new JumpInstruction;  // bne $at,$zero,RR
        assemblyCode = "bne $at,$zero," + getOperand(2);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "bge") {
        // bge rs,rt,RR -> slt $at,rs,rt
        //                 beq $at,$zero,RR
        if (!checkOperand(3))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        if (labelTable.find(getOperand(2)) == labelTable.end())
            printErrorInfo(No_such_label);
        newLine = new RegInstruction;  // slt $at,rs,rt
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "slt $at," + getOperand(0) + "," + getOperand(1);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new JumpInstruction;  // beq $at,$zero,RR
        assemblyCode = "beq $at,$zero," + getOperand(2);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "abs") {
        // abs rt,rs -> sra $at,rs,31
        //              xor rt,rs,$at
        //              sub rt,rt,$at
        if (!checkOperand(2))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        newLine = new RegInstruction;  // sra $at,rs,31
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "sra $at," + getOperand(1) + ",31";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction;  // xor rt,rs,$at
        assemblyCode = "xor " + getOperand(0) + "," + getOperand(1) + ",$at";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction; // sub rt,rt,$at
        assemblyCode = "sub " + getOperand(0) + "," + getOperand(0) + ",$at";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "swap") {
        // swap $r1,$r2 -> xor $r1,$r1,$r2
        //                 xor $r2,$r1,$r2
        //                 xor $r1,$r1,$r2
        if (!checkOperand(2))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1)))
            printErrorInfo(No_such_register);
        newLine = new RegInstruction; // xor $r1,$r1,$r2
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "xor " + getOperand(0) + "," + getOperand(0) + "," +getOperand(1);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction; // xor $r2,$r1,$r2
        assemblyCode = "xor " + getOperand(1) + "," + getOperand(0) + "," +getOperand(1);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction;  // xor $r1,$r1,$r2
        assemblyCode = "xor " + getOperand(0) + "," + getOperand(0) + "," +getOperand(1);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "sne") {
        // sne rd,rs,rt -> sub $at,rs,rt
        //                 sltu rd,$zero,$at
        if (!checkOperand(3))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1) && islegalreg(2)))
            printErrorInfo(No_such_register);
        newLine = new RegInstruction;  // sub $at,rs,rt
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "sub $at," + getOperand(1) + "," + getOperand(2);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction;  // sltu rd,$zero,$at
        assemblyCode = "sltu " + getOperand(0) + ",$zero,$at";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "seq") {
        // seq rd,rs,rt -> sub $at,rs,rt
        //                 sltiu rd,$at,1
        if (!checkOperand(3))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!(islegalreg(0) && islegalreg(1) && islegalreg(2)))
            printErrorInfo(No_such_register);
        newLine = new RegInstruction;  // sub $at,rs,rt
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "sub $at," + getOperand(1) + "," + getOperand(2);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new ImmInstruction;  // sltiu rd,$at,1
        assemblyCode = "sltiu " + getOperand(0) + ",$at,1";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "li") {
        /*
         li $r,imme ->
         1.imme能用16bit表示
         addi $r,$zero,imme
         2.imme不能用16bit表示
         lui $r,HIGH
         ori $r,$r,LOW
         */
        if (!checkOperand(2))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        int imme = immatoi(1);
        if (imme != errorIns) {   // imme可用16位表示
            newLine = new ImmInstruction; // addi $r,$zero,imme
            if (hasLabel())
                assemblyCode = getLabel() + ": ";
            else assemblyCode = "";
            assemblyCode = assemblyCode + "ori " + getOperand(0) + ",$zero," + getOperand(1);
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
            newLine = new RegInstruction;  // 增加空指令 add $s0,$s0,$zero
            assemblyCode = "add $s0,$s0,$zero";
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
        }
        else {
            imme = immatoi(1, 32);
            if (imme != errorIns) {
                newLine = new ImmInstruction;  // lui $r,HIGH
                if (hasLabel())
                    assemblyCode = getLabel() + ": ";
                else assemblyCode = "";
                assemblyCode = assemblyCode + "lui " + getOperand(0) + "," + immitoa((imme & 0xffff0000) >> 16);
                newLine->setAssemblyCode(assemblyCode);
                newLine->setLine(getLine());
                newLine->split();
                instructionSet.push_back(newLine);
                newLine = new ImmInstruction; // ori $r,$r,LOW
                assemblyCode = "ori " + getOperand(0) + "," + getOperand(0) + "," + immitoa(imme & 0x0000ffff);
                newLine->setAssemblyCode(assemblyCode);
                newLine->setLine(getLine());
                newLine->split();
                instructionSet.push_back(newLine);
            }
            else printErrorInfo(Wrong_immediate_number_or_offset);
        }
    }
    else if (opName == "la") {
        /*
         la $r,RR ->
         imme表示RR地址, li $r,imme
         */
        if (!checkOperand(2))
            printErrorInfo(The_amount_of_operand_is_wrong);
        if (!islegalreg(0))
            printErrorInfo(No_such_register);
        if (labelTable.find(getOperand(1)) == labelTable.end())
            printErrorInfo(No_such_label);
        int imme = labelTable[getOperand(1)] * 2 + base / 2;
        if (imme == (imme & 0x0000ffff))  { // imme可用16bit表示
            newLine = new ImmInstruction; // addi $r,$zero,imme
            if (hasLabel())
                assemblyCode = getLabel() + ": ";
            else assemblyCode = "";
            assemblyCode = assemblyCode + "ori " + getOperand(0) + ",$zero," + immitoa(imme);
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
            newLine = new RegInstruction;  // 增加空指令 add $s0,$s0,$zero
            assemblyCode = "add $s0,$s0,$zero";
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
        }
        else {
            newLine = new ImmInstruction;  // lui $r,HIGH
            if (hasLabel())
                assemblyCode = getLabel() + ": ";
            else assemblyCode = "";
            assemblyCode = assemblyCode + "lui " + getOperand(0) + "," + immitoa(imme & 0xffff0000);
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
            newLine = new ImmInstruction; // ori $r,$r,LOW
            assemblyCode = "ori " + getOperand(0) + "," + getOperand(0) + "," + immitoa(imme & 0x0000ffff);
            newLine->setAssemblyCode(assemblyCode);
            newLine->setLine(getLine());
            newLine->split();
            instructionSet.push_back(newLine);
        }
        newLine = new RegInstruction;
        assemblyCode = "add " + getOperand(0) + "," + getOperand(0) + ",$gp";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "jal") {
        if (numOfOperand() != 1)
            printErrorInfo(The_amount_of_operand_is_wrong);
        string addr = getOperand(0);
        int imme;
        if (labelTable.find(addr) == labelTable.end()) {  // 不存在该label
            imme = immatoi(0, 26, false);  // 26位imme
            if (imme == errorIns)
                printErrorInfo(Wrong_immediate_number_or_offset);
        }
        else imme = labelTable[addr];
        imme += base / 2;
        
        newLine = new ImmInstruction; // lui $r, HIGH
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "lui $at," + immitoa((imme & 0xffff0000) >> 16);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new ImmInstruction; // ori $r,$r,LOW
        assemblyCode = "ori $at,$at," + immitoa(imme & 0x0000ffff);
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction;
        assemblyCode = "add $at,$at,$gp";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
        newLine = new RegInstruction;
        assemblyCode = "jalr $at,$ra";
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
    else if (opName == "jd") {
        if (numOfOperand() != 1)
            printErrorInfo(The_amount_of_operand_is_wrong);
        string label = getOperand(0);
        if (labelTable.find(label) == labelTable.end())
            printErrorInfo(Wrong_formation);
        newLine = new JumpInstruction;
        if (hasLabel())
            assemblyCode = getLabel() + ": ";
        else assemblyCode = "";
        assemblyCode = assemblyCode + "beq $zero,$zero," + label;
        newLine->setAssemblyCode(assemblyCode);
        newLine->setLine(getLine());
        newLine->split();
        instructionSet.push_back(newLine);
    }
}




















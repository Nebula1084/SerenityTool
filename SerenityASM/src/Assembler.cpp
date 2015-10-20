//
//  Assembler.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "Assembler.hpp"
#include "RegInstruction.hpp"
#include "JumpInstruction.hpp"
#include "ImmInstruction.hpp"
#include "SpcInstruction.hpp"
#include "LoadSaveInstruction.hpp"
#include "PseudoInstruction.hpp"
#include "FormatInstruction.hpp"
#include "global.hpp"
#include <set>
#include <iostream>
#include <iomanip>

Assembler::Assembler() {}
Assembler::~Assembler() {}

bool Assembler::openFile(string &fileName)
{
    fin.open(fileName.c_str());
    if(fin.is_open()){
        return true;
    }
    else
        return false;
}

void Assembler::generateBinaryFile(string &fileName)
{
    try {
        Instructions instructions;
        InstructionSet instructionSet;
        formation(instructions);  // 格式处理
        getLabelTable(instructions);  // 获得labeltable
        dealWithPseudo(instructions, instructionSet);  // 处理伪指令
            
        fout.open(fileName.c_str());
        MachineCode machineCode;  // 汇编后的机器码
        int curLineNumber = 0;  // 当前行号
        
        vector<Instruction *>::iterator itr = instructionSet.begin();
        if (itr != instructionSet.end()) {      // 处理第一行的.origin
            Instruction *firstLine = *itr;
            if (firstLine->getOpName() == ".origin") {  // 第一行是.origin
                base = firstLine->immatoi(0, 32, false);
                if (base == errorIns)
                    firstLine->printErrorInfo(Illegal_origin_address);
                curLineNumber++;
                itr++;
            }
            else base = 0;
            cout << "0x" << uppercase << hex << setfill('0') << setw(8) << base << endl;
            for (int i = 3; i >= 0; --i)
                fout.write(((char *)&base) + i, 1);
        }

        for (; itr != instructionSet.end(); itr++, curLineNumber++) {
            Instruction *curLine = *itr;
            if (assembleInfo.insType[curLine->getOpName()] != FormatIns) {
                machineCode = curLine->assemble(assembleInfo, labelTable, curLineNumber);
                if (machineCode == errorIns)
                    curLine->printErrorInfo(Can_not_assemble);
                cout << "0x" << uppercase << hex << setfill('0') << setw(8) << machineCode << endl;
                for (int i = 3; i >= 0; --i)
                    fout.write(((char *)&machineCode) + i, 1);
            }
            else curLine->assemble(assembleInfo, fout);
        }
    } catch (...) {
        if (fout.is_open())
            fout.close();
    }
    if (fout.is_open())
        fout.close();
}

void Assembler::formation(Instructions &instructions)  // 格式处理
{
    AssemblyCode assembleIns;  // 读入的汇编码
    int line = 0;   // 当前行数，用于报错
    bool mergeFlag;  // 是否需要合并（一行只有label的Ins要与下一行合并）
    Instruction assembleLine;  // 一条指令对象
    set<Label> labelSet;  // Label集合，用于判断label是否重复
    
    while (!fin.eof()) {
        line++;
        getline(fin, assembleIns);
        if (!assembleIns.size())    // 空行
            continue;
        mergeFlag = false;   // 不需要合并
        assembleLine.setLine(line);
        assembleLine.setAssemblyCode(assembleIns);
        if (!assembleLine.split())    // 将汇编码按label、opName、operand分离
            assembleLine.printErrorInfo(Wrong_formation);
        if (assembleLine.isEmpty())  // 空行（只有注释)
            continue;
        if (assembleLine.hasLabel()) {  // 有label
            if (!assembleLine.checkLabel())   // label有非法字符
                assembleLine.printErrorInfo(Illegal_characters_in_label);
            if (labelSet.find(assembleLine.getLabel()) != labelSet.end())    // 重复label
                assembleLine.printErrorInfo(Redefined_label);
            labelSet.insert(assembleLine.getLabel());  // 插入label集合
            if (!instructions.empty())    // 不是第一行
                if (instructions.back().onlyLabel())   // 上一行只有label，即有连续重复的label出现
                    assembleLine.printErrorInfo(Label_duplication);
        }
        else if (!instructions.empty())
            if (instructions.back().onlyLabel()) {  // 上一行只有label，需合并
                instructions.back() =  instructions.back() + assembleLine;
                mergeFlag = true;
            }
        if (!mergeFlag)  
            instructions.push_back(assembleLine);
    }
}

void Assembler::getLabelTable(Instructions &instructions)
//  获得labeltable
//  labelTable[label] = addr中addr不是实际pc，而是指令行数，即pc的一半
{
    Address addr = 0;
    for (vector<Instruction>::iterator itr = instructions.begin(); itr != instructions.end(); itr++) {
        Instruction ins = *itr;
        if (ins.hasLabel()) {
            Label label = ins.getLabel();
            if (labelTable.find(label) != labelTable.end())  // label重定义
                ins.printErrorInfo(Redefined_label);
            else labelTable[label] = addr;
        }
        addr += ins.actualLinage();
    }
}

void Assembler::dealWithPseudo(Instructions &instructions, InstructionSet &instructionSet) // 处理伪指令
{
    for (vector<Instruction>::iterator itr = instructions.begin(); itr != instructions.end(); itr++) {
        Instruction curLine = *itr;
        OpName opName = curLine.getOpName();
        if (assembleInfo.insType.find(opName) == assembleInfo.insType.end())
            curLine.printErrorInfo(No_such_instruction_or_pseudo_instruction);
        switch (assembleInfo.insType[opName]) {
            case RegIns: {
                Instruction *insPointer = new RegInstruction(curLine);
                instructionSet.push_back(insPointer);
                break;
            }
            case JumpIns: {
                Instruction *insPointer = new JumpInstruction(curLine);
                instructionSet.push_back(insPointer);
                break;
            }
            case ImmIns: {
                Instruction *insPointer = new ImmInstruction(curLine);
                instructionSet.push_back(insPointer);
                break;
            }
            case SpcIns: {
                Instruction *insPointer = new SpcInstruction(curLine);
                instructionSet.push_back(insPointer);
                break;
            }
            case LoadSaveIns: {
                Instruction *insPointer = new LoadSaveInstruction(curLine);
                instructionSet.push_back(insPointer);
                break;
            }
            case PseudoIns: {
                Instruction *insPointer = new PseudoInstruction(curLine);
                insPointer->translate(instructionSet, labelTable);
                break;
            }
            case FormatIns: {
                Instruction *insPointer = new FormatInstruction(curLine);
                instructionSet.push_back(insPointer);
                break;
            }
            default: {
                curLine.printErrorInfo(No_such_instruction_or_pseudo_instruction);
                break;
            }
        }
    }
}


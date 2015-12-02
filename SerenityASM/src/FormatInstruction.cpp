//
//  FormatInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "FormatInstruction.hpp"
#include "global.hpp"
#include <iomanip>
#include <iostream>

FormatInstruction::FormatInstruction(Instruction &instruction): Instruction(instruction)
{}

FormatInstruction::FormatInstruction()
{}

FormatInstruction::~FormatInstruction()
{}

void FormatInstruction::assemble(AssembleInfo &assembleInfo, ofstream &fout, map<Label, Address> &labelTable)
{
    OpName opName = getOpName();
    if (opName == ".orgin")
        printErrorInfo(reduplicated_origin);
    else if (opName == ".asciiz") {  // 字符串，以0结尾
        for (vector<AssemblyCode>::iterator itr = operand.begin(); itr != operand.end(); itr++) {
            AssemblyCode str = *itr;
            for (int i = 0; i < str.size() / 2; i++) {
                MachineCode strCode;
                strCode = str.at(2 * i) - '\0';
                strCode = strCode << 16;
                strCode = strCode | (str.at(2 * i + 1) - '\0');
                print(strCode, fout);
            }
            if (str.size() % 2 == 0)
                print(0, fout);
            else {
                MachineCode strCode;
                strCode = str.at(str.size() - 1) << 16;
                print(strCode, fout);
            }
        }
    }
    else if (opName == ".2byte") {  // 2字节变量
        for (vector<AssemblyCode>::iterator itr = operand.begin(); itr != operand.end(); itr++) {
            AssemblyCode str = *itr;
            if (str.at(0) == 's')
                for (int i = 4; i < str.size(); i++)
                    print(str.at(i) - '\0', fout);
            else if (str.at(0) == 'i')
                print(immatoi(str.substr(4, str.size() - 4)), fout);
            else {
                string label = str.substr(4, str.size() - 4);
                if (labelTable.find(label) == labelTable.end())
                    printErrorInfo(No_such_label);
                int addr = labelTable[label] + base / 2;
                print(addr, fout);
            }
        }
    }
    else if (opName == ".space")  {
        int num = immatoi(0);
        if (num == errorIns)
            printErrorInfo(Wrong_immediate_number_or_offset);
        for (int i = 0; i < num; i++)
            print(0, fout);
    }
}

void FormatInstruction::print(MachineCode machineCode, ofstream &fout)
{
    cout << "0x" << uppercase << hex << setfill('0') << setw(8) << machineCode << endl;
    for (int i = 3; i >= 0; --i)
        fout.write(((char *)&machineCode) + i, 1);
}
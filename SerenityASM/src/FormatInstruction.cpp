//
//  FormatInstruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "FormatInstruction.hpp"
#include <iomanip>
#include <iostream>

FormatInstruction::FormatInstruction(Instruction &instruction): Instruction(instruction)
{}

FormatInstruction::FormatInstruction()
{}

FormatInstruction::~FormatInstruction()
{}

void FormatInstruction::assemble(AssembleInfo &assembleInfo, ofstream &fout)
{
    OpName opName = getOpName();
    if (opName == ".orgin")
        printErrorInfo(reduplicated_origin);
    else if (opName == ".asciiz") {  // 字符串，以0结尾
        for (vector<AssemblyCode>::iterator itr = operand.begin(); itr != operand.end(); itr++) {
            AssemblyCode str = *itr;
            for (int i = 0; i < str.size(); i++)
                print(str.at(i) - '\0', fout);
            print(0, fout);
        }
    }
    else if (opName == ".2byte") {  // 2字节变量
        for (vector<AssemblyCode>::iterator itr = operand.begin(); itr != operand.end(); itr++) {
            AssemblyCode str = *itr;
            if (str.at(0) == 's')
                for (int i = 4; i < str.size(); i++)
                    print(str.at(i) - '\0', fout);
            else print(immatoi(str.substr(4, str.size() - 4)), fout);
        }
    }
}

void FormatInstruction::print(MachineCode machineCode, ofstream &fout)
{
    cout << "0x" << uppercase << hex << setfill('0') << setw(8) << machineCode << endl;
    for (int i = 3; i >= 0; --i)
        fout.write(((char *)&machineCode) + i, 1);
}
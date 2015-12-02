//
//  Assembler.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef Assembler_hpp
#define Assembler_hpp

#include "AssembleInfo.hpp"
#include "Instruction.hpp"
#include <fstream>


class Assembler
{
    ifstream fin;
    map<Label, Address> labelTable;
    AssembleInfo assembleInfo;
public:
    Assembler();
    ~Assembler();
    bool openFile(string &fileName);
    void generateBinaryFile(string &fileName);
    void formation(Instructions &instructions);   // 格式处理
    void getLabelTable(Instructions &instructions);  // 获得labeltable
    void dealWithPseudo(Instructions &instructions, InstructionSet &instructionSet);  // 处理伪指令
};


#endif /* Assembler_hpp */

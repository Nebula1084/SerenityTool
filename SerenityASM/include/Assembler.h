//
//  Assembler.h
//  Assembler
//
//  Created by scn3 on 15/9/21.
//  Copyright (c) 2015年 scn3. All rights reserved.
//

#ifndef __Assembler__Assembler__
#define __Assembler__Assembler__

#include <CommonTypedef.h>
#include <AssemblerLine.h>
#include <fstream>
#include <string>
#include <iostream>
#include <set>
#include <iomanip>


class Assembler
{
    ifstream fin;
    ofstream fout;
    map<Label, Address> labelTable;
    AssembleInfo assembleInfo;
    
public:
    Assembler();
    ~Assembler();
    ErrorInfo generateBinaryFile(string fileName);
    bool openFile(string fileName);
    bool isPseudo(string &opName);
    ErrorInfo formation(AssembleSet &assembleSet);
    ErrorInfo pseudoOp(AssembleSet &ASin, AssembleSet &ASout);
    ErrorInfo getLabelTable(AssembleSet &assembleSet);
    Instruction regInsAsm(AssembleLine &assembleLine);
    Instruction jumpInsAsm(AssembleLine &assembleLine, int curLine);
    Instruction immInsAsm(AssembleLine &assembleLine);
    Instruction spcInsAsm(AssembleLine &assembleLine);
    Instruction loadSaveInsAsm(AssembleLine &assembleLine);
    int immatoi(string immStr);
};


#endif /* defined(__Assembler__Assembler__) */

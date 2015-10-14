//
//  AssemblerLine.h
//  Assembler
//
//  Created by scn3 on 15/9/26.
//  Copyright (c) 2015年 scn3. All rights reserved.
//

#ifndef __Assembler__AssemblerLine__
#define __Assembler__AssemblerLine__

#include <CommonTypedef.h>
#include <AssembleInfo.h>
#include <utility>

class AssembleLine{
    AssembleIns assembleIns;
    int line;
    Label label;
    string opName;
    vector<string> operand;
    
    char toLowerCase(char ch);
    void skipBlanks(int &pos);
public:
    AssembleLine(AssembleIns instruction = "", int lineNumber = 0);
    ~AssembleLine();
    void setAssembleIns(AssembleIns &instruction);
    void setLine(int lineNumber);
    AssembleIns getAssembleIns();
    int getLine() const;
    Label getLabel();
    string getOpName();
    string getOperand(int pos);
    unsigned long numOfOperand() const;
    bool split();
    bool isEmpty() const;
    bool onlyLabel() const;
    bool hasLabel() const;
    bool checkLabel() const;
    bool islegalins() const;
    bool islegalreg(int n) const;
    ErrorInfo checkOperand(int num) const;
    AssembleLine operator+(AssembleLine &addendLine);
};

typedef vector<AssembleLine> AssembleSet;

#endif /* defined(__Assembler__AssemblerLine__) */

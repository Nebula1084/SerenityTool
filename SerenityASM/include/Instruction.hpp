//
//  Instruction.hpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#ifndef Instruction_hpp
#define Instruction_hpp

#include <vector>
#include <map>
#include "CommonTypedef.hpp"
#include "AssembleInfo.hpp"

using namespace std;

class Instruction
{
protected:
    int line;
   // int end = 0; // 若没有注释end=assemblyCode.size(),否则end为第一个#的位置
    AssemblyCode assemblyCode;
    Label label;
    OpName opName;
    vector<AssemblyCode> operand;
 //   int len = 0;  // 若为格式指令，则len=数据段长度，单位为行，即4字节
    
    char toLowerCase(char ch);
    void skip(int &pos);
    void rskip(int &pos);
public:
    Instruction(AssemblyCode instruction = "", int lineNumber = 0);
    Instruction(Instruction const &instruction);   // 拷贝构造
    ~Instruction();
    bool split();   // 分离各部分
    void setAssemblyCode(AssemblyCode &instruction);  // 以下7行为setter、getter
    AssemblyCode getAssembleCode() const;
    void setLine(int lineNumber);
    int getLine() const;
    Label getLabel() const;
    OpName getOpName() const;
    AssemblyCode getOperand(int pos) const;
    unsigned long numOfOperand() const;  // 操作数数目
    bool isEmpty();  // assemblyCode是否为空
    bool onlyLabel() const;  // 只有label
    bool hasLabel() const;  // 有label
    bool checkLabel() const;  // 检查label是否有非法字符
    bool islegalins() const;  // 检查指令名是否合法
    bool islegalreg(int n) const;  // 检查第n个操作数的寄存器名是否合法
    bool islegalcoreg(int n) const; // 检查第n个操作数的协寄存器名是否合法
    bool checkOperand(int num) const;  // 检查操作数数目是否为num
    Instruction operator+(Instruction &addendLine);  // 两行合并
    int actualLinage();  // 该行实际占的行数（伪指令）
    int immatoi(int n, int len = 16, bool issigned = true);   // 第n个操作数立即数(长度为len bit)或地址string转int
    int immatoi(string str, int len = 32, bool issigned = true);
    string immitoa(int imme);  // 数字转字符串
    void printErrorInfo(ErrorInfo errorInfo);  // 打印错误信息并结束程序
    virtual void translate(vector<Instruction *> &instructionSet, map<Label, Address> &labelTable);  // 伪指令翻译为标准指令
  //  virtual MachineCode assemble(AssembleInfo assembleInfo);  // 正式转机器码
    virtual MachineCode assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine);  // 正式转机器码
    virtual void assemble(AssembleInfo &assembleInfo, ofstream &fout);  // 为FormationIns重载
};

typedef vector<Instruction> Instructions;
typedef vector<Instruction *> InstructionSet;

#endif /* Instruction_hpp */

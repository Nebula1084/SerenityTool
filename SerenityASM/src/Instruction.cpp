//
//  Instruction.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "Instruction.hpp"
#include "AssembleInfo.hpp"
#include <set>
#include <iostream>
#include <fstream>

Instruction::Instruction(AssemblyCode instruction, int lineNumber): assemblyCode(instruction), line(lineNumber)
{
    operand.clear();
    line = 0;
    assemblyCode = "";
    opName = "";
}

Instruction::Instruction(Instruction const &instruction)
{
    line = instruction.line;
    assemblyCode = instruction.assemblyCode;
    label = instruction.label;
    opName = instruction.opName;
    operand = instruction.operand;
}

Instruction::~Instruction()
{
    operand.clear();
}

void Instruction::setAssemblyCode(AssemblyCode &instruction)
{
    assemblyCode = instruction;
}

AssemblyCode Instruction::getAssembleCode() const
{
    return assemblyCode;
}

void Instruction::setLine(int lineNumber)
{
    line = lineNumber;
}

int Instruction::getLine() const
{
    return line;
}

Label Instruction::getLabel() const
{
    return label;
}

OpName Instruction::getOpName() const
{
    return opName;
}

AssemblyCode Instruction::getOperand(int pos) const
{
    return operand[pos];
}

unsigned long Instruction::numOfOperand() const
{
    return operand.size();
}

bool Instruction::isEmpty()
// label为空（可以包含空格有\t）返回true，否则返回false
{
    int pos = 0;
    skip(pos);
    return (pos == assemblyCode.size());
}

bool Instruction::onlyLabel() const
{
    return opName == "";
}

bool Instruction::hasLabel() const
{
    return label != "";
}

bool Instruction::checkLabel() const
// label仅包含数字、大小写和下划线返回true，否则返回false
{
    for (int i = 0; i < label.size(); i++)
    {
        char ch = label.at(i);
        if (ch < 48)
            return false;
        if (ch > 57 && ch < 65)
            return false;
        if (ch > 90 && ch < 97 && ch != 95)
            return false;
        if (ch > 122)
            return false;
    }
    return true;
}

bool Instruction::islegalins() const   // 合法的指令（判断指令名是否存在）
{
    AssembleInfo assembleInfo;
    if (assembleInfo.insType.find(opName) == assembleInfo.insType.end())
        return false;
    return true;
}

bool Instruction::islegalreg(int n) const  // 判断寄存器名是否存在
{
    AssembleInfo assembleInfo;
    if (assembleInfo.reg.find(operand[n]) == assembleInfo.reg.end())
        return false;
    return true;
}

bool Instruction::islegalcoreg(int n) const  // 判断协寄存器名是否存在
{
    AssembleInfo assembleInfo;
    if (assembleInfo.coreg.find(operand[n]) == assembleInfo.coreg.end())
        return false;
    return true;
}

bool Instruction::checkOperand(int num) const  // 判断操作数的数量
{
    if (numOfOperand() != num)
        return false;
    return true;
}

Instruction Instruction::operator+(Instruction &addendLine)  // 两行合并
{
    Instruction sumLine;
    sumLine.assemblyCode = assemblyCode + addendLine.assemblyCode;  // 汇编码字符直接合并
    sumLine.line = addendLine.line;  // 行数为下一行
    sumLine.label = label;  // label为上一行的label
    sumLine.opName = addendLine.opName;
    sumLine.operand = addendLine.operand;
    return sumLine;
}

char Instruction::toLowerCase(char ch)
{
    if (ch <= 'Z' && ch >= 'A')
        return ch - 'A' + 'a';
    return ch;
}

void Instruction::skip(int &pos)  // 过滤空格和\t
{
    while (pos < assemblyCode.size())
        if (assemblyCode.at(pos) == ' ' || assemblyCode.at(pos) == '\t')
            pos++;
        else break;
}

void Instruction::rskip(int &pos)  // 反向过滤
{
    while (pos >= 0)
        if (assemblyCode.at(pos) == ' ' || assemblyCode.at(pos) == '\t')
            pos--;
        else break;
}

bool Instruction::split()
{
    operand.clear();
    
    if (assemblyCode.find('#') != string::npos) {  // 有注释，去注释
        unsigned long start = assemblyCode.find_first_of('#');
        assemblyCode = assemblyCode.erase(start, assemblyCode.size() - start);
    }
    if (isEmpty())
        return true;
    
    int i = 0;
    skip(i);  // 过滤行首空格以及\t
    label = "";
    if (assemblyCode.find(':') != string::npos) {// 该行有label
        for (; i < assemblyCode.size() && assemblyCode.at(i) != ':'; i++) // 获得label
            label += toLowerCase(assemblyCode.at(i));
        i++;
    }
    if (i >= assemblyCode.size())    // 只有label
        return true;
    skip(i);
    
    opName = "";
    for (; i < assemblyCode.size() && assemblyCode.at(i) != ' '; i++) // 获得opName
        opName += toLowerCase(assemblyCode.at(i));
    if (i >= assemblyCode.size()) {   // 只有指令类型，无操作数
        if (opName != "eret" && opName != "syscall")  // 且不是eret或syscall指令
            return false;
        else return true;
    }
    skip(i);
    if (i >= assemblyCode.size()) {   // 只有指令类型，无操作数
        if (opName != "eret" && opName != "syscall")  // 且不是eret或syscall指令
            return false;
        else return true;
    }
    if (assemblyCode.at(i) == ',')   // 指令类型和操作数间出现逗号
        return false;
    
    if (opName == ".2byte") { // 格式指令特殊处理
        bool quote = false;   // 双引号是否已配对
        bool blankSlant = false;  // 之前的字符是否为反斜杠
        bool comma = true; // 两个字符串间是否出现过逗号
        bool lastIsStr = false; // 上一个参数是字符串
        string str = "";
        
        for (; i < assemblyCode.size(); i++) {
            char ch = toLowerCase(assemblyCode.at(i));
            if (quote) {  //  引号内内容
                if (blankSlant) {  // 前一字符为转义符
                    switch (ch) {
                        case '"':
                            str += '"';
                            break;
                        case '\\':
                            str += '\\';
                            break;
                        case 't':
                            str += '\t';
                            break;
                        case 'n':
                            str += '\n';
                            break;
                        case 'r':
                            str += '\r';
                            break;
                        default:
                            str += ch;
                            break;
                    }
                }
                else if (ch == '"') {  // 引号结束字符串
                    operand.push_back("str_" + str);
                    str = "";
                    quote = false;
                    comma = false;
                }
                else if (ch != '\\')
                    str += ch;
            }
            else {
                if (ch == '"') {
                    if (!comma)
                        return false;
                    quote = true;
                    str = "";
                    lastIsStr = true;
                }
                else if (ch == ',') {
                    if (comma)
                        return false;
                    comma = true;
                    if (!lastIsStr) {
                        if (immatoi(str) == errorIns)
                            return false;
                        operand.push_back("int_" + str);
                        str = "";
                    }
                }
                else if (ch != ' ' && ch != '\t') {
                    str += ch;
                    comma = false;
                    lastIsStr = false;
                }
            }
            if (!blankSlant)
                blankSlant = (ch == '\\');
            else if (ch == '\\')
                blankSlant = false;
            else blankSlant = (ch == '\\');
        }
        if (comma || quote)   // 最后逗号后无字符串或双引号为匹配
            return false;
        if (str != "") {
            if (immatoi(str) == errorIns)
                return false;
            operand.push_back("int_" + str);
        }
        return true;
    }
    if (opName == ".asciiz") {
        bool quote = false;   // 双引号是否已配对
        bool blankSlant = false;  // 之前的字符是否为反斜杠
        bool comma = true; // 两个字符串间是否出现过逗号
        string str = "";
        
        for (; i < assemblyCode.size(); i++) {
            char ch = toLowerCase(assemblyCode.at(i));
            if (quote) {  //  引号内内容
                if (blankSlant) {  // 前一字符为转义符
                    switch (ch) {
                        case '"':
                            str += '"';
                            break;
                        case '\\':
                            str += '\\';
                            break;
                        case 't':
                            str += '\t';
                            break;
                        case 'n':
                            str += '\n';
                            break;
                        case 'r':
                            str += '\r';
                            break;
                        default:
                            str += ch;
                            break;
                    }
                }
                else if (ch == '"') {  // 引号结束字符串
                    operand.push_back(str);
                    str = "";
                    quote = false;
                    comma = false;
                }
                else if (ch != '\\')
                    str += ch;
            }
            else {
                if (ch == '"') {
                    if (!comma)   //  两个字符串间未出现逗号
                        return false;
                    quote = true;
                }
                else if (ch == ',') {
                    if (comma)  // 重复的逗号
                        return false;
                    comma = true;
                }
                else if (ch != ' ' && ch != '\t')
                    return false;
            }
            if (!blankSlant)
                blankSlant = (ch == '\\');
            else if (ch == '\\')
                blankSlant = false;
            else blankSlant = (ch == '\\');
        }
        if (comma || quote)   // 最后逗号后无字符串或双引号为匹配
            return false;
        return true;
    }
    
    string singleOperand;
    singleOperand = "";
    
    bool separatorFlag = true;  // 分隔符是否出现，包括,（）
    short bracketFlag = 0;  // 0:无括号出现；1:一个左括号出现；2:一对括号出现
    bool rightBracket = false; // 最后一个分隔符是否是）
    
    for (; i < assemblyCode.size() && assemblyCode.at(i) != '\r'; i++){
        char ch = assemblyCode.at(i);
        if (ch != ' ' && ch != ',' && ch != '(' && ch != ')' && ch != '\t')
        {
            if (singleOperand == "" && !separatorFlag)  // 操作数与操作数间无逗号
                return false;
            singleOperand += toLowerCase(ch);
            separatorFlag = false;
        }
        else
        {
            if (singleOperand != "")
                operand.push_back(singleOperand);
            if (ch == ',') {
                if (separatorFlag)  // 重复的分隔符
                    return false;
                if (bracketFlag == 1)  // 逗号出现在括号间
                    return false;
                separatorFlag = true;
                rightBracket = false;
            }
            else if (ch == '(') {
                if (!bracketFlag) {
                    bracketFlag = 1;
                    separatorFlag = true;
                    rightBracket = false;
                }
                else return false;  // 已出现过括号，再出现左括号出错
            }
            else if (ch == ')') {
                if (bracketFlag != 1)  // 已出现过一对括号或还未出现左括号
                    return false;
                else {     // 括号成功匹配
                    bracketFlag = 2;
                    separatorFlag = true;
                    if (singleOperand == "")  // 括号间无operand
                        return false;
                    //else std::swap(operand[operand.size() - 2], operand[operand.size() - 1]);
                    rightBracket = true;
                }
            }
            singleOperand = "";
        }
    }
    if (separatorFlag && !rightBracket)   // 除)外的分隔符后无操作数
        return false;
    if (singleOperand != "")
        operand.push_back(singleOperand);
    return true;
}


void Instruction::printErrorInfo(ErrorInfo errorInfo)
{
    cout << "Line " << line << ": " << getAssembleCode() << endl;
    switch (errorInfo) {
        case No_errors:
            cout << "No error" << endl;
            break;
        case Wrong_formation:
            cout << "Wrong formation" << endl;
            break;
        case Illegal_characters_in_label:
            cout << "Illegal characters in label" << endl;
            break;
        case Label_duplication:
            cout << "Label duplication" << endl;
            break;
        case The_amount_of_operand_is_wrong:
            cout << "The amount of operand is wrong" << endl;
            break;
        case No_such_register:
            cout << "No such register" << endl;
            break;
        case No_such_instruction_or_pseudo_instruction:
            cout << "No such instruction or pseudo instruction" << endl;
            break;
        case Redefined_label:
            cout << "Redefined label" << endl;
            break;
        case Can_not_assemble:
            cout << "Can not assemble" << endl;
            break;
        case Wrong_immediate_number_or_offset:
            cout << "Wrong immediate number or offset" << endl;
            break;
        case No_such_label:
            cout << "No such label" << endl;
            break;
        case No_such_coregister:
            cout << "No such coregister" << endl;
            break;
        case Illegal_origin_address:
            cout << "Illegal origin address" << endl;
            break;
        case reduplicated_origin:
            cout << "reduplicated .origin" << endl;
            break;
        default:
            cout << "Unknown error" << endl;
            break;
    }
    throw errorIns;
}


int Instruction::actualLinage()   // 伪指令实际行数
{
    if (opName == ".origin")
        return 0;
    if (opName == "push" || opName == "pop" || opName == "blt" || opName == "bgt" || opName == "ble" || opName == "bge")
        return 2;
    if (opName == "abs" || opName == "swap" || opName == "sne" || opName == "seq")
        return 3;
    if (opName == "li" || opName == "la") {
        if (immatoi(1) != errorIns)
            return 1;
        else return 2;
    }
    if (opName == ".asciiz") {
        int linage = 0;
        for (vector<AssemblyCode>::iterator itr = operand.begin(); itr != operand.end(); itr++) {
            AssemblyCode str = *itr;
            linage += (str.size() + 1);
        }
        return linage;
    }
    if (opName == ".2byte") {
        int linage = 0;
        for (vector<AssemblyCode>::iterator itr = operand.begin(); itr != operand.end(); itr++) {
            AssemblyCode str = *itr;
            if (str.at(0) == 's')
                linage += (str.size() - 4);
            else linage += 1;
        }
        return linage;
    }
    if (opName == ".space") {
        int linage = immatoi(0);
        if (linage == errorIns)
            printErrorInfo(Wrong_immediate_number_or_offset);
        return linage;
    }
    return 1;
}

int Instruction::immatoi(int n, int len, bool issigned)
/*
 将第n个操作数（立即数，长度为len bit）由string转int返回
 如果该数是由符号数，issigned = true, 否则 issigned = false
 支持如下格式：  -n（负数十进制）、n（无符号十进制）、#n（无符号十进制）、bn（二进制）、0xn（十六进制）
*/
{
    string imma = operand[n];
    int immi = 0;
    int start = 0;  // 数字开始的位置（除去-、#、b、0x后）
    int radix;  // 进制
    bool isneg = false;  // 是否有负号
    
    int end = imma.size() - 1;
    while (end >= 0)        // 反向过滤空格和\t，确定数字结尾
        if (imma.at(end) == ' ' || imma.at(end) == '\t')
            end--;
        else break;
    
    if (imma.at(0) == '-') {    // -n
        if (!issigned)           // 无符号数带负号
            return errorIns;
        radix = 10;
        start = 1;
        isneg = true;
    }
    else if (imma.at(0) == '#') {  // #n
        radix = 10;
        start = 1;
    }
    else if (imma.at(0) == 'b') {  // bn
        radix = 2;
        start = 1;
    }
    else if (imma.at(0) == '0') {
        if (imma.size() > 1)
            if (imma.at(1) == 'x') {// 0xn
                radix = 16;
                start = 2;
            }
            else {
                radix = 10;
                start = 1;
            }
            else {
                radix = 10;
                start = 1;
            }
    }
    else if (imma.at(0) >= '0' && imma.at(0) <= '9') {  // n
        radix = 10;
        start = 0;
    }
    else return errorIns;
    if (radix != 16) {   // 非十六进制
        for (int i = start; i <= end; i++) {
            char ch = imma.at(i);
            if (ch >= '0' && ch <= '0' + radix - 1)
                immi = immi * radix + ch - '0';
            else return errorIns;   // 非法字符
        }
        if (isneg)  // 有负号
            immi = -immi;
        else if (issigned)  // 是有符号数
            immi = immi - 2 * (immi & (1 << (len -1)));  // 无符号数转有符号数，减去最高位的两倍（如果最高位是1）
    }
    else {
        for (int i = start; i <= end; i++) {
            char ch = imma.at(i);
            if (ch >= '0' && ch <= '9')
                immi = immi * radix + ch - '0';
            else if (ch >= 'a' && ch <= 'f')
                immi = immi * radix + ch - 'a' + 10;
            else return errorIns;
        }
        if (issigned)
            immi = immi - 2 * (immi & (1 << (len -1)));
    }
    if (len < 32) {
        if (!issigned) {    // 无符号数溢出检查
            if (immi < 0 || immi >= (1 << len))
                return errorIns;
        }
        else if (immi < -(1 << (len - 1)) || immi >= (1 << (len - 1)) )  // 有符号数溢出检查
            return errorIns;
    }
    return immi;
}

int Instruction::immatoi(string str, int len, bool issigned)
{
    string imma = str;
    int immi = 0;
    int start = 0;  // 数字开始的位置（除去-、#、b、0x后）
    int radix;  // 进制
    bool isneg = false;  // 是否有负号
    
    int end = imma.size() - 1;
    while (end >= 0)        // 反向过滤空格和\t，确定数字结尾
        if (imma.at(end) == ' ' || imma.at(end) == '\t')
            end--;
        else break;
    
    if (imma.at(0) == '-') {    // -n
        if (!issigned)           // 无符号数带负号
            return errorIns;
        radix = 10;
        start = 1;
        isneg = true;
    }
    else if (imma.at(0) == '#') {  // #n
        radix = 10;
        start = 1;
    }
    else if (imma.at(0) == 'b') {  // bn
        radix = 2;
        start = 1;
    }
    else if (imma.at(0) == '0' && imma.at(1) == 'x') {  // 0xn
        radix = 16;
        start = 2;
    }
    else if (imma.at(0) >= '0' && imma.at(0) <= '9') {  // n
        radix = 10;
        start = 0;
    }
    else return errorIns;
    if (radix != 16) {   // 非十六进制
        for (int i = start; i <= end; i++) {
            char ch = imma.at(i);
            if (ch >= '0' && ch <= '0' + radix - 1)
                immi = immi * radix + ch - '0';
            else return errorIns;   // 非法字符
        }
        if (isneg)  // 有负号
            immi = -immi;
        else if (issigned)  // 是有符号数
            immi = immi - 2 * (immi & (1 << (len -1)));  // 无符号数转有符号数，减去最高位的两倍（如果最高位是1）
    }
    else {
        for (int i = start; i <= end; i++) {
            char ch = imma.at(i);
            if (ch >= '0' && ch <= '9')
                immi = immi * radix + ch - '0';
            else if (ch >= 'a' && ch <= 'f')
                immi = immi * radix + ch - 'a' + 10;
            else return errorIns;
        }
        if (issigned)
            immi = immi - 2 * (immi & (1 << (len -1)));
    }
    if (len < 32) {
        if (!issigned) {    // 无符号数溢出检查
            if (immi < 0 || immi >= (1 << len))
                return errorIns;
        }
        else if (immi < -(1 << (len - 1)) || immi >= (1 << (len - 1)) )  // 有符号数溢出检查
            return errorIns;
    }
    return immi;
}

string Instruction::immitoa(int imme)
{
    string imma = "";
    string tmp = "";
    if (imme == 0x80000000)   // 特殊处理
        return "0x80000000";
    else if (imme == 0)
        return "0";
    if (imme < 0) {
        imma = "-";
        imme = -imme;
    }
    while (imme > 0) {
        tmp += imme % 10 + '0';
        imme /= 10;
    }
    for (int i = tmp.size() - 1; i >= 0; i--)
        imma += tmp.at(i);
    return imma;
}

void Instruction::translate(vector<Instruction *> &instructionSet, map<Label, Address> &labelTable)
{}

/*
MachineCode Instruction::assemble(AssembleInfo assembleInfo)
{
    return errorIns;
}
 */

MachineCode Instruction::assemble(AssembleInfo &assembleInfo, map<Label, Address> &labelTable, int actualLine)
{
    return errorIns;
}

void Instruction::assemble(AssembleInfo &assembleInfo, ofstream &fout)
{}























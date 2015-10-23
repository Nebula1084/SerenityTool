//
//  AssemblerLine.cpp
//  Assembler
//
//  Created by scn3 on 15/9/26.
//  Copyright (c) 2015年 scn3. All rights reserved.
//

#include <AssemblerLine.h>

AssembleLine::AssembleLine(AssembleIns instruction, int lineNumber): assembleIns(instruction), line(lineNumber) {}

AssembleLine::~AssembleLine()
{
    operand.clear();
}

char AssembleLine::toLowerCase(char ch)
{
    if (ch <= 'Z' && ch >= 'A')
        return ch - 'A' + 'a';
    return ch;
}

void AssembleLine::skipBlanks(int &pos)
{
    while (assembleIns.at(pos) == ' ' && pos < assembleIns.size()) pos++;
}


void AssembleLine::setAssembleIns(AssembleIns &instruction)
{
    assembleIns = instruction;
}

void AssembleLine::setLine(int lineNumber)
{
    line = lineNumber;
}

AssembleIns AssembleLine::getAssembleIns()
{
    return assembleIns;
}

int AssembleLine::getLine() const
{
    return line;
}

Label AssembleLine::getLabel()
{
    return label;
}

string AssembleLine::getOpName()
{
    return opName;
}

string AssembleLine::getOperand(int pos)
{
    return operand[pos];
}

unsigned long AssembleLine::numOfOperand() const
{
    return operand.size();
}


bool AssembleLine::split()
// 将指令各部分各自分离，并将大写转为小写，若失败或是有简单格式错误（如少逗号、无操作数），则返回false
{
    int i = 0;
    string singleOperand;
    
    
    skipBlanks(i);  // 过滤行首空格
    label = "";
    if (assembleIns.find(':') != string::npos) {// 该行有label
        for (; i < assembleIns.size() && assembleIns.at(i) != ':'; i++) // 获得label
            label += toLowerCase(assembleIns.at(i));
        i++;
    }
    if (i >= assembleIns.size())    // 只有label
        return true;
    skipBlanks(i);
    
    opName = "";
    for (; i < assembleIns.size() && assembleIns.at(i) != ' '; i++) // 获得opName
        opName += toLowerCase(assembleIns.at(i));
    if (i >= assembleIns.size())    // 只有指令类型，无操作数
        return false;
    skipBlanks(i);
    if (i >= assembleIns.size())    // 只有指令类型，无操作数
        return true;
    if (assembleIns.at(i) == ',')   // 指令类型和操作数间出现逗号
        return false;
    
    operand.clear();
    singleOperand = "";
    
    bool separatorFlag = true;  // 分隔符是否出现，包括,（）
    short bracketFlag = 0;  // 0:无括号出现；1:一个左括号出现；2:一对括号出现
    bool rightBracket = false; // 最后一个分隔符是否是）
    
    for (; i < assembleIns.size() && assembleIns.at(i) != '\r'; i++){
        char ch = assembleIns.at(i);
        if (ch != ' ' && ch != ',' && ch != '(' && ch != ')')
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
                    else std::swap(operand[operand.size() - 2], operand[operand.size() - 1]);
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


bool AssembleLine::isEmpty() const
{
    return assembleIns == "";
}

bool AssembleLine::onlyLabel() const
{
    return opName == "";
}

bool AssembleLine::hasLabel() const
{
    return label != "";
}

bool AssembleLine::checkLabel() const
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

bool AssembleLine::islegalins() const
{
    AssembleInfo assembleInfo;
    if (assembleInfo.insType.find(opName) == assembleInfo.insType.end())
        return false;
    return true;
}

bool AssembleLine::islegalreg(int n) const
{
    AssembleInfo assembleInfo;
    if (assembleInfo.reg.find(operand[n]) == assembleInfo.reg.end())
        return false;
    return true;
}

ErrorInfo AssembleLine::checkOperand(int num) const
{
    if (numOfOperand() != num)
        return  The_amount_of_operand_is_wrong;
    for (int i = 0; i < numOfOperand(); i++)
        if (!islegalreg(i))
            return No_such_register;
    return correct;
}

AssembleLine AssembleLine::operator+(AssembleLine &addendLine)
{
    AssembleLine sumLine;
    sumLine.assembleIns = assembleIns + addendLine.assembleIns;
    sumLine.line = addendLine.line;
    sumLine.label = label;
    sumLine.opName = addendLine.opName;
    sumLine.operand = addendLine.operand;
    return sumLine;
}

//
//  Assembler.cpp
//  Assembler
//
//  Created by scn3 on 15/9/21.
//  Copyright (c) 2015年 scn3. All rights reserved.
//

#include "Assembler.h"

Assembler::Assembler() {}
Assembler::~Assembler() {}

ErrorInfo Assembler::generateBinaryFile(string fileName)
{
    AssembleSet assembleSet1, assembleSet2;
    ErrorInfo errorInfo;
    
    errorInfo = formation(assembleSet1);
    if (errorInfo != correct)
        return errorInfo;
    errorInfo = pseudoOp(assembleSet1, assembleSet2);
    if (errorInfo != correct)
        return errorInfo;
    errorInfo = getLabelTable(assembleSet2);
    if (errorInfo != correct)
        return errorInfo;
    
    fout.open(fileName.c_str());
    Instruction instruction;
    int curLine = 0;
    for (vector<AssembleLine>::iterator itr = assembleSet2.begin(); itr != assembleSet2.end(); itr++, curLine++)
    {
        AssembleLine currentLine = *itr;
        string opName = currentLine.getOpName();
        switch (assembleInfo.insType[opName]) {
            case RegIns:
                instruction = regInsAsm(currentLine);
                break;
            case ImmIns:
                instruction = immInsAsm(currentLine);
                break;
            case SpcIns:
                instruction = spcInsAsm(currentLine);
                break;
            case JumpIns:
                instruction = jumpInsAsm(currentLine, curLine);
                break;
            case  LoadSaveIns:
                instruction = loadSaveInsAsm(currentLine);
                break;
            default:
                instruction = errorIns;
                break;
        }
        cout << "0x" << uppercase << hex << setfill('0') << setw(8) << instruction << endl;
        if (instruction == errorIns) {
            cout << "Line " << currentLine.getLine() << ": ";
            fout.close();
            return Can_not_assemble;
        }
        for (int i = 3; i >= 0; --i)
            fout.write(((char *)&instruction) + i, 1);
    }
    fout.close();
    return correct;
}

bool Assembler::openFile(string fileName)
{
    fin.open(fileName.c_str());
    if(fin.is_open()){
        return true;
    }
    else
        return false;
}

bool Assembler::isPseudo(string &opName)
{
    static set<string> pseudo;
    pseudo.insert("move");
    pseudo.insert("push");
    pseudo.insert("pop");
    if (pseudo.find(opName) == pseudo.end())
        return false;
    return true;
}

ErrorInfo Assembler::formation(AssembleSet &assembleSet)
// 格式标准化
{
    AssembleIns assembleIns;
    int line = 0;
    bool mergeFlag;

    AssembleIns newline;
    AssembleLine assembleLine;
    
    while (!fin.eof()) {
        line++;
        getline(fin, assembleIns);
        if (!assembleIns.size())    // 空行
            continue;
        mergeFlag = false;
        assembleLine.setLine(line);
        assembleLine.setAssembleIns(assembleIns);
        if (!assembleLine.split()) {
            cout << "Line " << line << ": ";    // 格式错误
            return Wrong_formation;
        }
        if (assembleLine.hasLabel()) {  // 有label
            if (!assembleLine.checkLabel()) {  // label有非法字符
                cout << "Line " << line << ": ";
                return Illegal_characters_in_label;
            }
            if (!assembleSet.empty())    // 不是第一行
                if (assembleSet.back().onlyLabel()) {  // 上一行只有label，即有连续重复的label出现
                    cout << "Line " << line << ": ";
                    return Label_duplication;
                }
        }
        else if (!assembleSet.empty())
            if (assembleSet.back().onlyLabel()) {
                assembleSet.back() =  assembleSet.back() + assembleLine;
                mergeFlag = true;
            }
        if (!mergeFlag)
            assembleSet.push_back(assembleLine);
    }
    /*
    while (!fin.eof()) {
        line++;
        getline(fin, assembleIns);
        if (!assembleIns.size())     // 空行
            continue;
        if (!labelFlag)
            newline = "";
        i = j = 0;
        if (assembleIns.find(':') != string::npos)  {// 该行有label
            if (labelFlag) {     // 重复的label
                errorInfo << "Line " << line << ": Repeated labels"<< endl;
                return errorInfo.str();
            }
            while (assembleIns.at(i) == ' ') i++;  // 过滤行首空格
            for (; i < assembleIns.size(); i++)  {// 扫label
                ch = assembleIns.at(i);
                if (ch == ':')    // label结束
                    break;
                if (ch < 48) {   // label只允许大小写、下划线和数字
                    errorInfo << "Line " << line << ": Illegal characters in label" << endl;
                    return errorInfo.str();
                }
                if (ch > 57 && ch < 65) {
                    errorInfo << "Line " << line << ": Illegal characters in label" << endl;
                    return errorInfo.str();
                }
                if (ch > 90 && ch < 97 && ch != 95) {
                    errorInfo << "Line " << line << ": Illegal characters in label" << endl;
                    return errorInfo.str();
                }
                if (ch > 122) {
                    errorInfo << "Line " << line << ": Illegal characters in label" << endl;
                    return errorInfo.str();
                }
                newline += toLowerCase(ch);
            }
            newline += ": ";
            for (i++; i < assembleIns.size() && assembleIns.at(i) == ' '; i++);  // 过滤label后空格
            if (i == assembleIns.size()) {    // 该行只有label，没有指令
                labelFlag = true;
                continue;
            }
        }
        insFlag = blankFlag = false;
        for (; i < assembleIns.size(); i++) {
            ch = assembleIns.at(i);
            if (ch == ' ') {
                if (insFlag && !blankFlag) {
                    newline += ch;
                    blankFlag = true;
                }
                continue;
            }
            insFlag = true;
            newline += toLowerCase(ch);
        }
        if (insFlag) {
            assembleLine.assembleIns = newline;
            assembleLine.line = line;
            assembleSet.push_back(assembleLine);
            labelFlag = false;
        }
    }
     */
    return correct;
}

ErrorInfo Assembler::pseudoOp(AssembleSet &ASin, AssembleSet &ASout)
// 处理伪指令
{
    AssembleLine newLine;
    AssembleIns assembleIns;
    ErrorInfo errorInfo;
    string opName;
    
    for (vector<AssembleLine>::iterator asitr = ASin.begin(); asitr != ASin.end(); asitr++) {
        AssembleLine oldLine = *asitr;
        AssembleIns assembleIns = oldLine.getAssembleIns();
        
        opName = oldLine.getOpName();
        
        if (oldLine.islegalins())  // 有这一opName的真指令
            ASout.push_back(oldLine);
        else if (isPseudo(opName)) {    // 检查是否伪指令
            if (opName == "move") {
                errorInfo = oldLine.checkOperand(2);
                if (errorInfo != correct) {
                    cout << "Line " << oldLine.getLine() << ": ";
                    return errorInfo;
                }
                if (oldLine.hasLabel())
                    assembleIns = oldLine.getLabel() + ": ";
                else assembleIns = "";
                assembleIns = assembleIns + "add " + oldLine.getOperand(0) + "," + oldLine.getOperand(1) + ",$zero";
                newLine.setAssembleIns(assembleIns);
                newLine.setLine(oldLine.getLine());
                newLine.split();
                ASout.push_back(newLine);
             }
            else if (opName == "push") {
                errorInfo = oldLine.checkOperand(1);
                if (errorInfo != correct) {
                    cout << "Line " << oldLine.getLine() << ": ";
                    return errorInfo;
                }
                if (oldLine.hasLabel())
                    assembleIns = oldLine.getLabel() + ": ";
                else assembleIns = "";
                assembleIns = assembleIns + "addi $sp,$sp,-2";
                newLine.setAssembleIns(assembleIns);
                newLine.setLine(oldLine.getLine());
                newLine.split();
                ASout.push_back(newLine);
                assembleIns = "sw " + oldLine.getOperand(0) + ",$sp,0";
                newLine.setAssembleIns(assembleIns);
                newLine.setLine(oldLine.getLine());
                newLine.split();
                ASout.push_back(newLine);
            }
            else if (opName == "pop") {
                errorInfo = oldLine.checkOperand(1);
                if (errorInfo != correct) {
                    cout << "Line " << oldLine.getLine() << ": ";
                    return errorInfo;
                }
                if (oldLine.hasLabel())
                    assembleIns = oldLine.getLabel() + ": ";
                else assembleIns = "";
                assembleIns = assembleIns + "lw " + oldLine.getOperand(0) + ",$sp,0";
                newLine.setAssembleIns(assembleIns);
                newLine.setLine(oldLine.getLine());
                newLine.split();
                ASout.push_back(newLine);
                assembleIns = "addi $sp,$sp,2";
                newLine.setAssembleIns(assembleIns);
                newLine.setLine(oldLine.getLine());
                newLine.split();
                ASout.push_back(newLine);
            }
        }
        else {
            cout << "Line " << oldLine.getLine() << ": ";
            return No_such_instruction_or_pseudo_instruction;
        }
    }
    return correct;
}

ErrorInfo Assembler::getLabelTable(AssembleSet &assembleSet)
{
    Address addr = 0;
    for (vector<AssembleLine>::iterator itr = assembleSet.begin(); itr != assembleSet.end(); itr++, addr++)
    {
        AssembleLine currentLine = *itr;
        if (currentLine.hasLabel()) {
            Label label = currentLine.getLabel();
            if (labelTable.find(label) != labelTable.end()) {
                cout << "Line " << currentLine.getLine() << ": ";
                return Redefined_label;
            }
            else labelTable[label] = addr;
        }
    }
    return correct;
}

Instruction Assembler::regInsAsm(AssembleLine &assembleLine)  // 基本完善
{
    string opName = assembleLine.getOpName();
    Instruction ins = assembleInfo.opcode[opName];
    
    if (opName == "srav" || opName == "srlv") {
        if (assembleLine.numOfOperand() != 3)
            return errorIns;
        ins += assembleInfo.reg[assembleLine.getOperand(0)] << 11;
        ins += assembleInfo.reg[assembleLine.getOperand(1)] << 16;
        ins += assembleInfo.reg[assembleLine.getOperand(2)] << 21;
        return ins;
    }
    if (opName == "jalr") {
        if (assembleLine.numOfOperand() != 2)
            return errorIns;
        ins += assembleInfo.reg[assembleLine.getOperand(0)] << 21;
        ins += assembleInfo.reg[assembleLine.getOperand(1)] << 11;
        return ins;
    }
    
    vector<string> regToBeChecked;
    regToBeChecked.push_back("RD");
    regToBeChecked.push_back("RS");
    regToBeChecked.push_back("RT");
    regToBeChecked.push_back("SFT");
    size_t count =0;
    for (vector<string>::iterator regTypeitr = regToBeChecked.begin(); regTypeitr != regToBeChecked.end(); regTypeitr++) {
        string regType;
        regType = *regTypeitr;
        if (assembleInfo.InsRegList[opName][regType])
            count ++;
    }
    if (count != assembleLine.numOfOperand())
        return errorIns;
    int k = 0;
    for (vector<string>::iterator regTypeitr = regToBeChecked.begin(); regTypeitr != regToBeChecked.end(); regTypeitr++) {
        string regType = *regTypeitr;
        if (assembleInfo.InsRegList[opName][regType]) {
            if (k == assembleLine.numOfOperand())
                break;
            Instruction regTmp = assembleInfo.reg[assembleLine.getOperand(k)];
            if (regType == "SFT")
                ins += regTmp << 6;
            else if (regType == "RD")
                ins += regTmp << 11;
            else if (regType == "RT")
                ins += regTmp << 16;
            else if (regType == "RS")
                ins += regTmp << 21;
            k++;
        }
    }
    return ins;}

Instruction Assembler::jumpInsAsm(AssembleLine &assembleLine, int curLine)  // 待完善
{
    // bgez、bgezal、bltz、bltzal、blez、bgtz待加
    string opName = assembleLine.getOpName();
    Instruction ins = assembleInfo.opcode[opName];
    int imm;
    if (opName == "j" || opName == "jal") {
        if (assembleLine.numOfOperand() != 1)
            return errorIns;
        string addr = assembleLine.getOperand(0);
        if (labelTable.find(addr) == labelTable.end()) {
            imm = immatoi(addr, 26);
            if (imm == errorIns)
                return errorIns;
            if (imm > 0x03ffffff || imm < 0)
                return errorIns;
        }
        else imm = labelTable[addr] * 2;
        ins = ins | (imm & 0x03ffffff);
    }
    else if (opName == "beq" || opName == "bne") {
        if (assembleLine.numOfOperand() != 3)
            return errorIns;
        string addr = assembleLine.getOperand(2);
        if (labelTable.find(addr) == labelTable.end()) {
            imm = immatoi(addr, 16);
            if (imm == errorIns)
                return errorIns;
            if (imm < -32768 || imm > 32767)
                return errorIns;
        }
        else imm = (labelTable[addr] - curLine - 1) * 2;
        if (assembleInfo.InsRegList[opName]["RS"])
            ins = ins | (assembleInfo.reg[assembleLine.getOperand(0)] << 21);
        if (assembleInfo.InsRegList[opName]["RT"])
            ins = ins | (assembleInfo.reg[assembleLine.getOperand(1)] << 16);
        ins = ins | (imm & 0x0000ffff);
    }
    else {
        if (assembleLine.numOfOperand() != 2)
            return errorIns;
        string addr = assembleLine.getOperand(1);
        if (labelTable.find(addr) == labelTable.end()) {
            imm = immatoi(addr, 16);
            if (imm == errorIns)
                return errorIns;
            if (imm < -32768 || imm > 32767)
                return errorIns;
        }
        else imm = (labelTable[addr] - curLine - 1) * 2;
        if (opName == "bgez")
            ins |= 1 << 16;
        else if (opName == "bgezal")
            ins |= 17 << 16;
        else if (opName == "bltzal")
            ins |= 16 << 16;
        ins |= assembleInfo.reg[assembleLine.getOperand(0)] << 21;
        ins |= imm & 0x0000ffff;
    }
    return ins;
}

Instruction Assembler::immInsAsm(AssembleLine &assembleLine)  // 基本完善
{
    // lui仅有一个寄存器操作数
    string opName = assembleLine.getOpName();
    Instruction ins = assembleInfo.opcode[opName];
    int imm;
    if (opName == "lui") {   // lui指令仅有RT寄存器
        if (assembleLine.numOfOperand() != 2)
            return errorIns;
        imm = immatoi(assembleLine.getOperand(1), 16);
        if (imm == errorIns)
            return errorIns;
     //   if (imm < -32768 || imm > 32767)
       //     return errorIns;
        ins |= assembleInfo.reg[assembleLine.getOperand(0)] << 16;
        ins |= imm & 0x0000ffff;
    }
    else {   // 其余有RT、RS寄存器
        if (assembleLine.numOfOperand() != 3)
            return errorIns;
        imm = immatoi(assembleLine.getOperand(2), 16);
        if (imm == errorIns)
            return errorIns;
        ins |= assembleInfo.reg[assembleLine.getOperand(0)] << 16;
        ins |= assembleInfo.reg[assembleLine.getOperand(1)] << 21;
        if (opName == "andi" || opName == "ori" || opName == "xori") {  // 0扩展
            if (imm > 65535)
                return errorIns;
            unsigned unimm = (unsigned)imm;
            ins |= unimm & 0x0000ffff;
        }
        else {
            if (imm < -32768 || imm > 32767)  // 符号位扩展
                return errorIns;
            ins |= imm & 0x0000ffff;
        }
    }
    return ins;
}

Instruction Assembler::spcInsAsm(AssembleLine &assembleLine)  // 基本完善
{
    string opName = assembleLine.getOpName();
    Instruction ins = assembleInfo.opcode[opName];
    
    if (opName == "mul") {  // mul 有3个操作数   ???
        if (assembleLine.numOfOperand() != 3)
            return errorIns;
        ins |= assembleInfo.reg[assembleLine.getOperand(0)] << 11;
        ins |= assembleInfo.reg[assembleLine.getOperand(1)] << 21;
        ins |= assembleInfo.reg[assembleLine.getOperand(2)] << 16;
        ins |= 2;
    }
    else if (opName == "eret") {
        if (assembleLine.numOfOperand() != 0)  // eret无操作数
            return errorIns;
        ins |= 16 << 21;
        ins |= 0x18;
    }
    else {
        if (assembleLine.numOfOperand() != 2)
            return errorIns;
        ins |= assembleInfo.reg[assembleLine.getOperand(0)] << 21;
        ins |= assembleInfo.reg[assembleLine.getOperand(1)] << 16;
    }
    return ins;
}

Instruction Assembler::loadSaveInsAsm(AssembleLine &assembleLine)  // 待完善
{
    string opName = assembleLine.getOpName();
    Instruction ins = assembleInfo.opcode[opName];
    int offset;
    
    if (assembleLine.numOfOperand() == 3) {
        offset = immatoi(assembleLine.getOperand(2), 16);
        if (offset == errorIns)
            return errorIns;
    //    if (offset < -32768 || offset > 32767)
      //      return errorIns;
        ins |= assembleInfo.reg[assembleLine.getOperand(0)] << 16;
        ins |= assembleInfo.reg[assembleLine.getOperand(1)] << 21;
        ins |= offset & 0x0000ffff;
    }
    else if (assembleLine.numOfOperand() == 2) {   // LW $S1,LABEL?? 暂不考虑
        return errorIns;
    }
    else return errorIns;
    return ins;
}

int Assembler::immatoi(string immStr, int bits)
{
    int i = 0, radix = 10;
    bool isNegative = false;
    if (immStr.at(0) == '-') {
        i++;
        isNegative = true;
    }
    if (immStr.size() == i)
        return errorIns;
    if (immStr.at(i) == 'b') {
        i++;
        radix = 2;
    }
    else if (immStr.at(i) == '#') {
        i++;
        radix = 10;
    }
    else if (immStr.size() > i + 1)
        if (immStr.at(i) == '0' && immStr.at(i + 1) == 'x') {
            i += 2;
            radix = 16;
        }
    if (immStr.size() == i)
        return errorIns;
    int imm = 0;
    for (; i < immStr.size(); i++) {
        if (immStr.at(i) >= '0' && immStr.at(i) <= '9')
            imm = imm * radix + immStr.at(i) - '0';
        else if (immStr.at(i) >= 'a' && immStr.at(i) <= 'f')
            imm = imm * radix + immStr.at(i) - 'f';
        else return errorIns;
    }
    if (radix != 10) {
        if (imm >= 2 << bits) {
            return errorIns;
        }
    }
    if (isNegative)
        return -imm;
    else return imm;
}











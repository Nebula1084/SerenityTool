//
//  main.cpp
//  Assembler
//
//  Created by scn3 on 15/9/20.
//  Copyright (c) 2015年 scn3. All rights reserved.
//

#include "Assembler.h"
#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{
    ifstream fin;
    ofstream fout;

    Assembler assembler;
    string fileName;
    cout << "enter your file name:" << endl;
    cin >> fileName;
    if (!assembler.openFile(fileName))
        cout << "Can't open" << endl;
    else cout << "Open file successfully" << endl;
    
    int pos = fileName.rfind('.');
    fileName.replace(pos + 1, fileName.length() - pos -1, "bit", 3);
    ErrorInfo errorInfo = assembler.generateBinaryFile(fileName);
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
        default:
            cout << "Unknow error" << endl;
            break;
    }
    return 0;
}

//
//  main.cpp
//  Assembler
//
//  Created by scn3 on 15/10/17.
//  Copyright © 2015年 scn3. All rights reserved.
//

#include "Assembler.hpp"
#include <iostream>
using namespace std;

int main(int argc, const char * argv[])
{
    
    ifstream fin;
    ofstream fout;
    
    Assembler assembler;
    string fileName;
    
    if (argc<2)
		cout << "Please enter correct parameters" << endl;    	
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == string("-p")){
			
		} else {
			fileName = argv[i];
		}
	}
    //cin >> fileName;
    if (!assembler.openFile(fileName)) {
        cout << "Can't open" << endl;
        return -1;
    }
    else cout << "Open file successfully" << endl;
    
    int pos = fileName.rfind('.');
    fileName.replace(pos + 1, fileName.length() - pos -1, "bit", 3);
    assembler.generateBinaryFile(fileName);
    return 0;
}
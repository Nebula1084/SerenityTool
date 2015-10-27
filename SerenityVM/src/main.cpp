#include <MipsCPU.h>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;
int main(int argc, char const *argv[])
{
	MipsCPU mipsCPU;
	char fileName[50] = "boot.bit";
	if (argc<2)
		cout << "Please enter correct parameters" << endl;
	for (int i = 1; i < argc; ++i) {
		if (argv[i] == string("-d")){
			mipsCPU.setDebug(true);
		} else {
			strcpy(fileName, argv[i]); 
		}
	}	
	ifstream fin(fileName, ios_base::binary);
	while(!fin.is_open()){
		cout << "Open Error!" << endl;
		cout << "Please input binary file name:" << endl;
		cin >> fileName;
		fin.open(fileName);
	}
	mipsCPU.boot(fin);	
	mipsCPU.run();
	fin.close();
	return 0;
}
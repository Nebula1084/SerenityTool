#include <MipsCPU.h>
#include <fstream>
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
	MipsCPU mipsCPU;
	char fileName[50] = "boot.bit";
	cout << "Please input binary file name:" << endl;
	cin >> fileName;
	ifstream fin(fileName);
	while(!fin.is_open()){
		cout << "Open Error!" << endl;
		cout << "Please input binary file name:" << endl;
		cin >> fileName;
		fin.open(fileName);
	}
	mipsCPU.boot(fin);
	cout << "before run" << endl;
	mipsCPU.run();
	cout << "after run" << endl;
	return 0;
}
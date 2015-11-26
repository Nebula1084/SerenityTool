#include "KeyCode.h"

int KeyCode::toVKC(int f, int s){
	using namespace std;
	ofstream fout;
	fout.open("log.txt", ios::app);
	fout << f << " " << s << endl;
	fout.close();
	if (f == 224){
		return s + 100;
	} else {
		return s;
	}
}
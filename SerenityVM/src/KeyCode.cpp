#include "KeyCode.h"

int KeyCode::toVKC(int f, int s){
	using namespace std;	
	int mask = 0x00010000;	
	// logFile << hex << f << " " << s << endl;	
	if (f == 224){
		switch(s){
			case 0x48: //UP
			return mask | 0x26;
			case 0x50: //DOWN
			return mask | 0x28;
			case 0x4b: //LEFT
			return mask | 0x25;
			case 0x4d: //RIGTH
			return mask | 0x27;
		}		
	} else {
		return s;
	}
}
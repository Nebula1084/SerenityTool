#ifndef __MEMORY_MANAGE_UNIT__
#define __MEMORY_MANAGE_UNIT__
#include <windows.h>
#define WIDTH 40
#define HEIGHT 25
#define REFRESH_FREQUENCE 10
class MemoryManageUnit
{
public:
	MemoryManageUnit(int m,int crtadr): size(m),CRTadr(crtadr),memory(new short[m]), time(0){//set the capacity and display memory address
		QueryPerformanceCounter(&start_time);
		QueryPerformanceFrequency(&frequency);/*获取CPU内部时钟频率*/
        for (int i = 0; i < size; ++i){
            if (i % 2 == 0)
                memory[i] = 0x1000;
            else
                memory[i] = 0xFFFF;
        }
		for (int i = CRTadr; i < size; ++i)
		{
			memory[i]=0x20;
		}
	};
	~MemoryManageUnit(){
		delete[] memory;
	}
	int lw(int adr);
	void sw(int adr, int dat);
	int lh(int adr);
	void sh(int adr, int dat);
	bool adrChk(int adr);
	void print();
	bool refresh;
	short getData(int adr);
	short* getMemory(){
		return memory;
	}
private:
	int size;
	int CRTadr;//the beginning address of display memory
	short* memory;
	LARGE_INTEGER start_time,stop_time,frequency;
	double time;
};
#endif
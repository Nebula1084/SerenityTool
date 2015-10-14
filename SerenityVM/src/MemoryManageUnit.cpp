#include <MemoryManageUnit.h>
#include <iostream>
// #include "stdio.h"
using namespace std;
int MemoryManageUnit::lw(int adr)   //Big_Endian
    {
        adrChk(adr);
        adrChk(adr+1);
        return ((int)memory[adr]<<16)|((int)memory[adr+1]&0xFFFF);
    }
void MemoryManageUnit::sw(int adr, int dat)
    {
        adrChk(adr);
        adrChk(adr+1);
        memory[adr+0]=(short)(dat>>16);
        memory[adr+1]=(short)(dat&0xFFFF);
    }
int MemoryManageUnit::lh(int adr)
    {
        adrChk(adr);        
        return memory[adr];
    }
void MemoryManageUnit::sh(int adr, int dat)
    {
        adrChk(adr);        
        memory[adr]=(short)(dat&0xFFFF);
        if(adr>=CRTadr)
            refresh=true;
    }
bool MemoryManageUnit::adrChk(int adr){
    if (0 <= adr && adr < size)
        return true;
    cout << "we couldn't access memory at " << adr << "." << endl;
        exit(-1);
}

void MemoryManageUnit::print(){
    QueryPerformanceCounter(&stop_time);
    double now = ((double)(stop_time.QuadPart-start_time.QuadPart))/frequency.QuadPart;
    if(now - time > 1.0/REFRESH_FREQUENCE){
        COORD coord = {0, 0};
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        short ch;
        char* gbk_buf = new char[2*WIDTH*HEIGHT];
        for (int i = 0; i < WIDTH*HEIGHT; ++i)
        {
            ch = memory[i + CRTadr];
            if ( ch & 0xFF00 ){
                int q,w;
                q = (ch - 0x2000)/94;
                w = (ch - 0x2000)%94;
                gbk_buf[2*i] = q + 0xA1;
                gbk_buf[2*i+1] = w + 0xA1;
            }
            else {
                gbk_buf[2*i] = ch;
                gbk_buf[2*i+1] = ' ';
            }
        }
        unsigned long count = 0;
        WriteConsoleOutputCharacter(hOut, gbk_buf, 2*WIDTH*HEIGHT, coord, &count);
        // printf("%c%c%c%c\n", gbk_buf[0], gbk_buf[1], gbk_buf[2], gbk_buf[3]);
        delete[] gbk_buf;
        time = now;
    }
    // printf("time = %f\n", time);
}
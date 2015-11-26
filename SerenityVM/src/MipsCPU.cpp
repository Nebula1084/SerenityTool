#include "MipsCPU.h"
#include <conio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "SysPara.h"
#include "KeyCode.h"

using namespace std;
MipsCPU::MipsCPU(): MMU( MMU_SIZE, VMADR){
	disk = fopen("SVD.vhd","rb+");
	if (disk == NULL) {
		cout << "open failed" << endl;
		exit(-1);
	}
	rgf[0]=0;	//$zero
	PC=0;
	MMU.print();
	debug=false;
	cpf[$STATE] = 0x00000001;
}

MipsCPU::~MipsCPU(){
	fclose(disk);
}

void MipsCPU::boot(ifstream &fin)
{
	for (int i=0; i<4; i++) fin.get();
	for(int i = 0; !fin.eof(); ++i){
		unsigned short byte = 0;
		byte = byte | (fin.get() << 8);
		byte = byte | (fin.get() << 0);
		MMU.sh(i,byte);		
	}
}

void MipsCPU::run(){
	int mask;
	int keycode;
	char c = 'r';
	char numstr[20];
	int comm;
	int sector;
	int p=0;
	while(c!='q'){
		if ((comm=MMU.getData(DCOMM))!=D_COMM_NONE){
			MMU.sh(DSIGN, 0);
			sector = MMU.getData(DADDR);
			fseek(disk, 512*sector, SEEK_SET);
			switch(comm){				
				case D_COMM_WRITE:					
					fwrite(MMU.getMemory()+DCONT, 512, 1, disk);
				break;
				case D_COMM_READ:
					fread(MMU.getMemory()+DCONT, 512, 1, disk);					
				break;
			}
			MMU.sh(DSIGN, 1);
			MMU.sh(DCOMM, 0);
			MMU.sh(DCONT+256+1, 0);
		}
		if (kbhit()){
			keycode = getch();
			if (keycode==224){
				int ff = keycode;
				keycode = getch();
				keycode = KeyCode::toVKC(ff, keycode);
			} else {
				keycode = KeyCode::toVKC(0, keycode);
			}
			MMU.sh(BKADR, keycode);
			cpf[$CAUSE] = $KBINT;
			cpf[$STATE] = cpf[$STATE] | 0x00000002;
		}
		if (chkInt()) {			
			cpf[$EPC] = PC;
			cpf[$STATE] = cpf[$STATE] & 0xFFFFFFFE;
			PC = INTENTRY;			
		}		
		IR=MMU.lw(PC);
		// cout << hex << PC<<"========" << IR << endl;
		PC+=2;						//16-bit/byte
	//R:	op:6,rs:5,rt:5,rd:5,sft:5,fun:6
	//I:	op:6,$rs:5,$rt:5,dat:16
	//J:	op:6,adr:26		
		op=(IR>>26)&63;
		rs=(IR>>21)&31;
		rt=(IR>>16)&31;
		rd=(IR>>11)&31;
		sft=(IR>>6)&31;
		fun=IR & 63;
		dat=(int)(short)(IR&0xFFFF);
		adr=IR&0x3FFFFFF;		
		switch(op){
		case 0:		//R-type
			switch(fun){
			case 32:	//ADD
				rgf[rd]=rgf[rs]+rgf[rt];				
				operation = "ADD "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				break;
			case 33:	//ADDU
				rgf[rd]=rgf[rs]+rgf[rt];
				operation = "ADDU "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				break;
			case 34:	//SUB
				operation = "SUB "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=rgf[rs]-rgf[rt];
				break;
			case 35:	//SUBU
				operation = "SUBU "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=rgf[rs]-rgf[rt];
				break;
			case 36:	//AND
				operation = "AND "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=rgf[rs]&rgf[rt];
				break;
			case 37:	//OR
				operation = "OR "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=rgf[rs]|rgf[rt];
				break;
			case 38:	//XOR
				operation = "XOR "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=rgf[rs]^rgf[rt];
				break;
			case 39:	//NOR
				operation = "NOR "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=~(rgf[rs]|rgf[rt]);
				break;
			case 42:	//SLT
				operation = "SLT "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=0;
				if (rgf[rs] < rgf[rt])
					rgf[rd]=1;
				break;
			case 43:	//SLTU
				operation = "SLTU "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];
				rgf[rd]=0;
				if ((unsigned int)rgf[rs] < (unsigned int)rgf[rt])
					rgf[rd]=1;
				break;
			case 0:		//SLL
				itoa(sft, numstr, 10);
				operation = "SLL "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+numstr;				
				rgf[rd]=rgf[rt]<<sft;
				break;
			case 2:		//SRL				
				itoa(sft, numstr, 10);
				operation = "SRL "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+numstr;				
				mask;
				if (rt==0) break;
				mask = 0x7fffffff;
				mask = mask>>(sft-1);									
				rgf[rd]=rgf[rt]>>sft;
				rgf[rd]=rgf[rd]&&mask;
				break;
			case 3:		//SRA
				itoa(sft, numstr, 10);
				operation = "SRA "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+numstr;				
				rgf[rd]=rgf[rt]>>sft;
				break;
			case 4:		//SLLV				
				operation = "SLLV "+SysPara::rgNm[rd]+","+SysPara::rgNm[rt]+","+SysPara::rgNm[rs];
				rgf[rd]=rgf[rt]<<rgf[rs];
				break;
			case 6:		//SRLV
				operation = "SRLV "+SysPara::rgNm[rd]+","+SysPara::rgNm[rt]+","+SysPara::rgNm[rs];
				mask;
				if (rt==0) break;
				mask = 0x7fffffff;
				mask = mask>>(rgf[rs]-1);									
				rgf[rd]=rgf[rt]>>rgf[rs];
				rgf[rd]=rgf[rd]&&mask;
				break;
			case 7:		//SRAV				
				operation = "SRAV "+SysPara::rgNm[rd]+","+SysPara::rgNm[rt]+","+SysPara::rgNm[rs];
				rgf[rd]=rgf[rt]>>rgf[rs];
				break;
			case 8:		//JR				
				operation = "JR "+SysPara::rgNm[rs];
				PC=rgf[rs];
				break;
			case 9:		//JALR				
				operation = "JALR "+SysPara::rgNm[rs]+","+SysPara::rgNm[rd];
				rgf[rd] = PC;
				PC = rgf[rs];
				break;
			case 12:	//SYSCALL
				operation = "SYSCALL";
				cpf[$CAUSE] = $SYSCALL;
				cpf[$STATE] = cpf[$STATE] | 0x00000002;
				break;
			default:
				operation = "ERROR!!!";
				cout << "\nError!\n" << endl;
				cout << "fun is " << fun << endl;
				cout << operation << " opcode:" << hex << op << " rd:" << rd << " rs:" << rs << " rt:" << rt << " sft:" << sft << " data:" << dat << " address:"<< adr << endl;
				getch();
				break;
			}
			break;
		case 16:	//coprocessor relative
			switch(rs){
			case 0:				
				operation = "MFC0 "+SysPara::rgNm[rt]+","+SysPara::coNm[rd];
				rgf[rt] = cpf[rd];
				break;
			case 4:
				operation = "MTC0 "+SysPara::rgNm[rt]+","+SysPara::coNm[rd];				
				cpf[rd] = rgf[rt];
				break;
			case 16:
				operation = "ERET";
				PC = cpf[$EPC];				
				cpf[$STATE] = cpf[$STATE] | 0x00000001;
				break;
			default:
				operation = "ERROR!!!";
				// cout << "\nError!\n" << endl;
				// cout << "op is " << op << endl;
				// cout << "rs is " << rs << endl;
				// cout << operation << " opcode:" << hex << op << " rd:" << rd << " rs:" << rs << " rt:" << rt << " sft:" << sft << " data:" << dat << " address:"<< adr << endl;
				getch();
				break;
			}
			break;
		case 35:	//LW
			rgf[rt]=MMU.lw(rgf[rs]+dat);
			itoa(dat, numstr, 10);
			operation = "LW "+SysPara::rgNm[rt]+","+numstr+"("+SysPara::rgNm[rs]+")";			
			break;
		case 43:	//SW
			MMU.sw(rgf[rs]+dat, rgf[rt]);
			itoa(dat, numstr, 10);
			operation = "SW "+SysPara::rgNm[rt]+","+numstr+"("+SysPara::rgNm[rs]+")";		
			break;
		case 33: //lh
			rgf[rt]=MMU.lh(rgf[rs]+dat);
			itoa(dat, numstr, 10);
			operation = "LH "+SysPara::rgNm[rt]+","+numstr+"("+SysPara::rgNm[rs]+")";			
			break;
		case 41: //sh
			MMU.sh(rgf[rs]+dat, rgf[rt]);
			itoa(dat, numstr, 10);
			operation = "SH "+SysPara::rgNm[rt]+","+numstr+"("+SysPara::rgNm[rs]+")";			
			break;
		case 4:		//BEQ
			if(rgf[rs]==rgf[rt])
				PC+=(dat<<1);
			itoa(dat, numstr, 10);
			operation = "BEQ "+SysPara::rgNm[rs]+","+SysPara::rgNm[rt]+","+numstr;			
			break;
		case 5:	//BNE
			if(rgf[rs]!=rgf[rt])
				PC+=(dat<<1);
			itoa(dat, numstr, 10);
			operation = "BNE "+SysPara::rgNm[rs]+","+SysPara::rgNm[rt]+","+numstr;						
			break;
		case 2:		//J
			PC=(PC&0xF8000000)|(adr<<1);
			itoa(adr, numstr, 10);
			operation = "J ";
			operation += numstr;
			break;
		case 3:		//JAL
			rgf[$ra] = PC;
			PC=(PC&0xF8000000)|(adr<<1);
			itoa(adr, numstr, 10);
			operation = "JAL ";
			operation += numstr;			
			break;
		case 8:   	//addi
			rgf[rt] = rgf[rs] + dat;
			itoa(dat, numstr, 10);
			operation = "ADDI "+SysPara::rgNm[rt]+","+SysPara::rgNm[rs]+","+numstr;					
            break;
		case 9:		//addiu
			rgf[rt] = rgf[rs] + dat;
			itoa(dat, numstr, 10);
			operation = "ADDIU "+SysPara::rgNm[rt]+","+SysPara::rgNm[rs]+","+numstr;					
            break;
		case 10:	//SLTI			
			itoa(dat, numstr, 10);
			operation = "SLTI "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+numstr;		
			rgf[rd]=0;
			if (rgf[rs] < dat)
				rgf[rd]=1;				
			break;
		case 11:	//SLTIU
			itoa(dat, numstr, 10);
			operation = "SLTIU "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+numstr;				
			rgf[rd]=0;
			if ((unsigned int)rgf[rs] < (unsigned int)dat)
				rgf[rd]=1;				
			break;
		case 12:	//ANDI			
			itoa(dat, numstr, 10);
			operation = "ANDI "+SysPara::rgNm[rt]+","+SysPara::rgNm[rs]+","+numstr;
			rgf[rt]=rgf[rs]&dat;
			break;
		case 13:	//ORI			
			itoa(dat, numstr, 10);
			operation = "ORI "+SysPara::rgNm[rt]+","+SysPara::rgNm[rs]+","+numstr;
			rgf[rt]=rgf[rs]|dat;
			break;
		case 14:	//XORI
			itoa(dat, numstr, 10);
			operation = "XORI "+SysPara::rgNm[rt]+","+SysPara::rgNm[rs]+","+numstr;			
			rgf[rt]=rgf[rs]^dat;			
			break;
		case 15:	//LUI
			itoa(dat, numstr, 10);
			operation = "LUI "+SysPara::rgNm[rt]+","+numstr;			
			rgf[rt]=dat<<16;
			break;
		case 28:	//MUL
			itoa(dat, numstr, 10);
			operation = "MUL "+SysPara::rgNm[rd]+","+SysPara::rgNm[rs]+","+SysPara::rgNm[rt];			
			rgf[rd]=rgf[rs]*rgf[rt];
			break;
		default:
			cout << "\nError!" << endl; 
			operation = "ERROR!!!";
			cout << "op is " << op << endl;
			break;
		}
		MMU.print();
		c = 'r';
		if (debug){
			c = getch();
			if (c == 'm'){
				int showAdr;			
				scanf("%x", &showAdr);				
				while (getch() != 'q' && showAdr < MMU_SIZE){
					cout << "0x" << hex << setw(8) << showAdr << ":" << setw(4) << hex <<MMU.getData(showAdr) << endl;
					showAdr++;
				}
			} else {
				printReg();	
			}			
		}
	}//end_for
}

bool MipsCPU::chkInt(){
	int EN = cpf[$STATE] & 0x00000001;
	int INT = cpf[$STATE] & 0x00000002;
	if (!EN) return false;
	if (INT) return true;
	return false;
}

void MipsCPU::printReg(){
		cout << endl;
		cout << operation << " opcode:" << hex << op << " rd:" << rd << " rs:" << rs << " rt:" << rt << " sft:" << sft << " data:" << dat << " address:"<< adr << endl;
		cout << "code:0x" << hex << setw(8) << setfill('0') << IR << endl;
		cout << "PC:0x" << hex << setw(8) << PC << endl;
		cout << "$zero:0x" << setw(8) << rgf[0] << "\t";
		cout << "at:0x" << setw(8) << rgf[1] << "\t";
		cout << "v0:0x" << setw(8) << rgf[2] << "\t";
		cout << "v1:0x" << setw(8) << rgf[3] << "\t" << endl;
		cout << "a0:0x" << setw(8) << rgf[4] << "\t";
		cout << "a1:0x" << setw(8) << rgf[5] << "\t";
		cout << "a2:0x" << setw(8) << rgf[6] << "\t";
		cout << "a3:0x" << setw(8) << rgf[7] << "\t" << endl;
		cout << "t0:0x" << setw(8) << rgf[8] << "\t";
		cout << "t1:0x" << setw(8) << rgf[9] << "\t";
		cout << "t2:0x" << setw(8) << rgf[10] << "\t";
		cout << "t3:0x" << setw(8) << rgf[11] << "\t" << endl;
		cout << "t4:0x" << setw(8) << rgf[12] << "\t";
		cout << "t5:0x" << setw(8) << rgf[13] << "\t";
		cout << "t6:0x" << setw(8) << rgf[14] << "\t";
		cout << "t7:0x" << setw(8) << rgf[15] << "\t" << endl;
		cout << "s0:0x" << setw(8) << rgf[16] << "\t";
		cout << "s1:0x" << setw(8) << rgf[17] << "\t";
		cout << "s2:0x" << setw(8) << rgf[18] << "\t";
		cout << "s3:0x" << setw(8) << rgf[19] << "\t" << endl;
		cout << "s4:0x" << setw(8) << rgf[20] << "\t";
		cout << "s5:0x" << setw(8) << rgf[21] << "\t";
		cout << "s6:0x" << setw(8) << rgf[22] << "\t";
		cout << "s7:0x" << setw(8) << rgf[23] << "\t" << endl;
		cout << "t8:0x" << setw(8) << rgf[24] << "\t";
		cout << "t9:0x" << setw(8) << rgf[25] << "\t";
		cout << "k0:0x" << setw(8) << rgf[26] << "\t";
		cout << "k1:0x" << setw(8) << rgf[27] << "\t" << endl;
		cout << "gp:0x" << setw(8) << rgf[28] << "\t";
		cout << "sp:0x" << setw(8) << rgf[29] << "\t";
		cout << "fp:0x" << setw(8) << rgf[30] << "\t";
		cout << "ra:0x" << setw(8) << rgf[31] << "\t" << endl;
		cout << "epc:0x" << setw(8) << cpf[0] << "\t";
		cout << "cause:0x" << setw(8) << cpf[1] << "\t";
		cout << "state:0x" << setw(8) << cpf[2] << "\t" << endl;		
}

void MipsCPU::setDebug(bool d){
	debug = d;
}
#include "MipsCPU.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "SysPara.h"

using namespace std;
MipsCPU::~MipsCPU(){}
MipsCPU::MipsCPU(): MMU( MMU_SIZE, VMADR){
	rgf[0]=0;	//$zero
	PC=0;
	MMU.print();
	debug=false;
	cpf[$STATE] = 0x00000001;
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
	char c = 'r';
	while(c!='q'){
		if (chkInt()) {
			cpf[$EPC] = PC;
			cpf[$STATE] = cpf[$STATE] & 0xFFFFFFFE;
			PC = INTENTRY;
			cout << PC << endl;
		}		
		IR=MMU.lw(PC);		
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
				operation = "ADD $rd,$rs,$rt";
				break;
			case 33:	//ADDU
				rgf[rd]=rgf[rs]+rgf[rt];
				operation = "ADDU $rd,$rs,$rt";				
				break;
			case 34:	//SUB
				operation = "SUB $rd,$rs,$rt";
				rgf[rd]=rgf[rs]-rgf[rt];
				break;
			case 35:	//SUBU
				operation = "SUBU $rd,$rs,$rt";
				rgf[rd]=rgf[rs]-rgf[rt];
				break;
			case 36:	//AND
				operation = "AND $rd,$rs,$rt";
				rgf[rd]=rgf[rs]&rgf[rt];
				break;
			case 37:	//OR
				operation = "OR $rd,$rs,$rt";
				rgf[rd]=rgf[rs]|rgf[rt];
				break;
			case 38:	//XOR
				operation = "XOR $rd,$rs,$rt";
				rgf[rd]=rgf[rs]^rgf[rt];
				break;
			case 39:	//NOR
				operation = "NOR $rd,$rs,$rt";
				rgf[rd]=~(rgf[rs]|rgf[rt]);
				break;
			case 42:	//SLT
				operation = "SLT $rd,$rs,$rt";
				rgf[rd]=0;
				if (rgf[rs] < rgf[rt])
					rgf[rd]=1;
				break;
			case 43:	//SLTU
				operation = "SLTU $rd,$rs,$rt";
				rgf[rd]=0;
				if ((unsigned int)rgf[rs] < (unsigned int)rgf[rt])
					rgf[rd]=1;
				break;
			case 0:		//SLL
				operation = "SLL $rd,$rt,sft";
				rgf[rd]=rgf[rt]<<sft;
				break;
			case 2:		//SRL
				operation = "SRL $rd,$rt,sft";
				mask;
				if (rt==0) break;
				mask = 0x7fffffff;
				mask = mask>>(sft-1);									
				rgf[rd]=rgf[rt]>>sft;
				rgf[rd]=rgf[rd]&&mask;
				break;
			case 3:		//SRA
				operation = "SRA $rd,$rt,sft";
				rgf[rd]=rgf[rt]>>sft;
				break;
			case 4:		//SLLV
				operation = "SLLV $rd,$rt,$rs";
				rgf[rd]=rgf[rt]<<rgf[rs];
				break;
			case 6:		//SRLV
				operation = "SRLV $rd,$rt,$rs";
				mask;
				if (rt==0) break;
				mask = 0x7fffffff;
				mask = mask>>(rgf[rs]-1);									
				rgf[rd]=rgf[rt]>>rgf[rs];
				rgf[rd]=rgf[rd]&&mask;
				break;
			case 7:
				operation = "SRAV $rd,$rt,$rs";
				rgf[rd]=rgf[rt]>>rgf[rs];
				break;
			case 8:		//JR
				operation = "JR $rs";
				PC=rgf[rs];
				break;
			case 9:		//JALR
				operation = "JALR $rs,$rd";
				rgf[rd] = PC+4;
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
				break;
			}
			break;
		case 16:	//coprocessor relative
			switch(rs){
			case 0:
				operation = "MFC0 $rt,$rd";
				rgf[rt] = cpf[rd];
				break;
			case 4:
				operation = "MTC0 $rt,$rd";
				cpf[rd] = rgf[rt];
				break;
			case 16:
				operation = "ERET";
				PC = cpf[$EPC];
				cpf[$STATE] = cpf[$STATE] | 0x00000001;
				break;
			}
			break;
		case 35:	//LW
			rgf[rt]=MMU.lw(rgf[rs]+dat);
			operation = "LW $rt,data($rs)";
			break;
		case 43:	//SW
			MMU.sw(rgf[rs]+dat, rgf[rt]);
			operation = "SW $rt,data($rs)";
			break;
		case 33: //lh
			rgf[rt]=MMU.lh(rgf[rs]+dat);
			operation = "LH $rt,data($rs)";
			break;
		case 41: //sh
			MMU.sh(rgf[rs]+dat, rgf[rt]);
			operation = "SH $rt,data($rs)";
			break;
		case 4:		//BEQ
			if(rgf[rs]==rgf[rt])
				PC+=(dat<<1);
			operation = "BEQ $rs,$rt,data";
			break;
		case 2:		//J
			PC=(PC&0xF8000000)|(adr<<1);
			operation = "J address";
			break;
		case 3:		//JAL
			rgf[$ra] = PC+4;
			PC=(PC&0xF8000000)|(adr<<1);
			operation = "JAL address";
			break;
		case 8:   	//addi
			rgf[rt] = rgf[rs] + dat;
			operation = "ADDI $rt,$rs,data";
            break;
		case 9:		//addiu
			rgf[rt] = rgf[rs] + dat;
			operation = "ADDIU $rt,$rs,data";
            break;
		case 10:	//SLTI
			operation = "SLTI $rd,$rs,imm";
			rgf[rd]=0;
			if (rgf[rs] < dat)
				rgf[rd]=1;				
			break;
		case 11:	//SLTI
			operation = "SLTIU $rd,$rs,imm";
			rgf[rd]=0;
			if ((unsigned int)rgf[rs] < (unsigned int)dat)
				rgf[rd]=1;				
			break;
		case 12:	//ANDI
			operation = "ANDI $rd,$rs,imm";
			rgf[rd]=rgf[rs]&dat;
			break;
		case 13:	//ORI
			operation = "ORI $rd,$rs,imm";
			rgf[rd]=rgf[rs]|dat;
			break;
		case 14:	//XORI
			operation = "XORI $rd,$rs,imm";
			rgf[rd]=rgf[rs]^dat;			
			break;
		case 15:	//LUI
			operation = "LUI $rt,imm";
			rgf[rt]=dat<<16;
			break;
		case 28:	//MUL
			operation = "MUL $rd,$rs,$rt";
			rgf[rd]=rgf[rs]*rgf[rt];
			break;
		default:
			cout << "\nError!" << endl; 
			operation = "ERROR!!!";
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
					cout << "0x" << setw(8) << showAdr << ":" << setw(4) <<MMU.getData(showAdr) << endl;
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
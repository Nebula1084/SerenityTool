`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:25:53 06/10/2015 
// Design Name: 
// Module Name:    ControlUnit 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ControlUnit(
input clk,
input[5:0] opcode,
input[5:0] func,
input[4:0] RS,
input  MemOK,
input[31:0] C0State,

output MemRead,
output PCWrite,
output PCWriteCond,
output IorD,
output MemWrite,
output[2:0] WriteData,
output IRWrite,
output[1:0] ALUSrcA,
output RegWrite,
output[1:0] RegDst,
output SaveHalf,
output LoadHalf,
output[1:0] ALUop, 
output[2:0] ALUSrcB, 
output[2:0] PCSource,
output[5:0] CUState,
output[5:0] ALUFunc,
output C0Write,
output[1:0] C0Dst,
output[2:0] C0Src,
output CauseSource,
output SyscallInte,
output IsBne
    );
	 
reg [5:0] state;
assign CUState = state;
assign InteEn = C0State[0];
assign INTE = C0State[1];
initial begin
	state <= 0;
end

assign R           =(opcode==6'h00);
assign SLL_SRL_SRA =(opcode==6'h00 && func[5:2]==4'h0);
assign JR          =(opcode==6'h00 && func==6'h8);
assign JALR        =(opcode==6'h00 && func==6'h9);
assign SYSCALL     = (opcode==6'h00 && func==6'hC);
assign J           =(opcode==6'h02);
assign JAL         =(opcode==6'h03);
assign BEQ         =(opcode==6'h04);
assign BNE         =(opcode==6'h05);

assign ADDI        =(opcode==6'h08);
assign ADDIU       =(opcode==6'h09);
assign SLTI        =(opcode==6'h0A);
assign SLTIU       =(opcode==6'h0B);

assign ANDI        =(opcode==6'h0C);
assign ORI 	       =(opcode==6'h0D);
assign XORI        =(opcode==6'h0E);

assign LUI         =(opcode==6'h0F);
assign MFC0        =(opcode==6'h10 && RS==5'h0);
assign MTC0        =(opcode==6'h10 && RS==5'h4);
assign ERET        =(opcode==6'h10 && RS==5'h10 && func==6'h18);
assign MUL         =(opcode==6'h1c);
assign LH          =(opcode==6'h21);
assign LW          =(opcode==6'h23);
assign SH          =(opcode==6'h29);
assign SW          =(opcode==6'h2B);


always @(posedge clk) begin
	case(state)
		0: begin 
			if(INTE && InteEn) begin
				state <= 25;
			end
			else begin
				state <= 23;
			end
		end
		1: begin 
			if(LW | SW | LH | SH) begin
				state <= 2;
			end
			
			else if (R)begin
				if(JR) begin
					state <= 14;
				end
				else if(JALR) begin
					state <= 15;
				end
				else if(SLL_SRL_SRA) begin
					state <= 19;
				end
				else if(SYSCALL) begin
					state <= 29;
				end
				else begin
					state <= 6;
				end
			end
			
			else if (BEQ) begin
				state <= 8;
			end
			
			else if (J) begin
				state <= 9;
			end 
			
			else if(JAL) begin
				state <= 13;
			end
			
			else if(MFC0) begin
				state <= 16;
			end
			
			else if(MTC0) begin
				state <= 17;
			end
			
			else if(LUI) begin
				state <= 18;
			end 
			
			else if(ADDI | SLTI) begin
				state <= 20;
			end
			
			else if(ANDI | ORI |XORI | ADDIU | SLTIU) begin
				state <= 21;
			end
			
			else if(MUL) begin
				state <= 22;
			end
			
			else if(ERET) begin
				state <= 32;
			end
			
			else if(BNE) begin
				state <= 34;
			end
		end
		2: begin 
			if (LW)begin
				state <= 3;
			end
			else if (SW)begin
				state <= 5;
			end
			else if (SH)begin
				state <= 11;
			end
			else if (LH)begin
				state <= 10;
			end
		end
		3: begin
			state <= 4;
		end
		4: begin 
			if(MemOK) begin
				state <= 0;
			end
		end
		5: begin 
			state <= 33;
		end
		6: begin 
			state <= 7;
		end
		7: begin 
			state <= 0;
		end
		8: begin 
			state <= 0;
		end
		9: begin 
			state <= 0;
		end
		10: begin 
			state <= 4;
		end
		11: begin 
			state <= 33;
		end
		12: begin 
			state <= 0;
		end
		13: begin
			state <=0;
		end
		14: begin
			state <= 0;
		end
		15: begin
			state <= 0;
		end
		16: begin
			state <= 0;
		end
		17: begin
			state <= 0;
		end
		18: begin
			state <= 0;
		end
		19: begin
			state <= 7;
		end
		20: begin
			state <= 24;
		end
		21: begin
			state <= 24;
		end
		22: begin
			state <= 7;
		end
		23: begin
			if(MemOK) begin
				state <= 1;
			end
		end
		24: begin
			state <= 0;
		end
		25: begin
			state <= 26;
		end
		26: begin
			state <= 27;
		end
		27: begin
			state <= 0;
		end
		28: begin
			state <= 31;
		end
		29: begin
			state <= 30;
		end
		30: begin
			state <= 0;
		end
		31: begin
			state <= 0;
		end
		32: begin
			state <= 28;
		end
		33: begin
			if(MemOK) begin
				state <= 0;
			end
		end
		34: begin
			state <= 0;
		end
		default: begin
			state <= 0;
		end
	endcase
end

assign state0 = (state == 0);
assign state1 = (state == 1);
assign state2 = (state == 2);
assign state3 = (state == 3);
assign state4 = (state == 4);
assign state5 = (state == 5);
assign state6 = (state == 6);
assign state7 = (state == 7);
assign state8 = (state == 8);
assign state9 = (state == 9);
assign state10 = (state == 10);
assign state11 = (state == 11);
assign state13 = (state == 13);
assign state14 = (state == 14);
assign state15 = (state == 15);
assign state16 = (state == 16);
assign state17 = (state == 17);
assign state18 = (state == 18);
assign state19 = (state == 19);
assign state20 = (state == 20);
assign state21 = (state == 21);
assign state22 = (state == 22);
assign state23 = (state == 23);
assign state24 = (state == 24);
assign state25 = (state == 25);
assign state26 = (state == 26);
assign state27 = (state == 27);
assign state28 = (state == 28);
assign state29 = (state == 29);
assign state30 = (state == 30);
assign state31 = (state == 31);
assign state32 = (state == 32);
assign state33 = (state == 33);
assign state34 = (state == 34);

assign PCWrite = state9 | state13 | state14 | state15 | (state23 & MemOK)|state27 | state28;
assign PCWriteCond = state8 | state34;
assign IorD = state3 | state5 | state10 | state11;
assign MemRead = state0 | state3 | state10;
assign MemWrite = state5 | state11;
assign WriteData[2] = state18;
assign WriteData[1] = state13 | state15 | state16;
assign WriteData[0] = state4 | state16;
assign IRWrite = state23 & MemOK;
assign ALUop[1] = state6 | state19 | state20 | state21 | state22;
assign ALUop[0] = state8 | state34;
assign ALUSrcA[1] = state19;
assign ALUSrcA[0] = state2 | state6 | state8 | state20 | state21 | state22 | state34;
assign ALUSrcB[2] = state21;
assign ALUSrcB[1] = state1 | state2 | state20;
assign ALUSrcB[0] = state1 | state23;
assign RegWrite = (state4 & MemOK) | state7 | state13 | state15 | state16 | state18 | state24;
assign RegDst[1] = state13;
assign RegDst[0] = state7 | state15;
assign SaveHalf = state11;
assign LoadHalf = state10;
assign PCSource[2] = state27 | state28;
assign PCSource[1] = state9 | state13 | state14 | state15;
assign PCSource[0] = state8 | state14 | state15 | state27 | state34;
assign IsBne = state34;
assign ALUFunc = 
	{6{R}} & func |
	{6{ADDI}}  & 6'h20 |
	{6{ADDIU}} & 6'h21 |
	{6{SLTI}}  & 6'h2A |
	{6{SLTIU}} & 6'h2B |
	{6{ANDI}}  & 6'h24 |
	{6{ORI}}   & 6'h25 |
	{6{XORI}}  & 6'h26 |
	{6{MUL}}   & 6'h3f;
assign C0Write = state17 | state25 | state26 | state29 | state31;
assign C0Dst[1]= state0 | state25 | state29 | state28 | state31;
assign C0Dst[0]= state26 | state29 | state32;
assign C0Src[2]= state31;
assign C0Src[1]= state25 | state29;
assign C0Src[0]= state26 | state29;
assign CauseSource = state30;
assign SyscallInte = state30;
endmodule

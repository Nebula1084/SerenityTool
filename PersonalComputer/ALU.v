`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:12:42 05/20/2015 
// Design Name: 
// Module Name:    ALU 
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
module ALU(
input[31:0] A,
input[31:0] B,
input[1:0] ALUop,
input[5:0] Func,
output[31:0] result,
output ZF
    );

//adition or subtraction
wire AddOrSub,SF,OF;
wire[63:0] AddOrSubResult;
assign AddOrSub = 
	(ALUop[1]==0) & ALUop[0] |
	(ALUop[1]==1) & Func[1];
AdderAndSubber64 adder64 (
    .A({ {32{A[31]}}, A}), 
    .B({ {32{B[31]}}, B}), 
    .mode(AddOrSub), 
    .result(AddOrSubResult), 
    .OF(OF), 
    .SF(SF), 
    .ZF(ZF), 
    .CF(CF)/*, 
    .PF(PF)*/
    );
	 
//rotation
wire[31:0] RotatorOut;
Rotator rotator (
    .RotatorIn(B), 
    .RotatorBitNum(A[4:0]), 
    .RotatorOp(Func[1:0]), 
    .RotatorOut(RotatorOut)
    );
	 
//mul
wire[31:0] mulResult;
assign mulResult = A*B;


assign result = 
	{32{ALUop[1]==0}} & AddOrSubResult |
	{32{ALUop[1]==1}} & (
		{32{Func[5:3]==3'b000}} & RotatorOut | // sll srl sra sllv srlv srav
		{32{Func[5:2]==4'b1000}} & AddOrSubResult | //sub or and
		{32{Func[5:0]==6'b100100}} & (A & B) |//and
		{32{Func[5:0]==6'b100101}} & (A | B) |//or
		{32{Func[5:0]==6'b100110}} & (A ^ B) |//xor
		{32{Func[5:0]==6'b100111}} & ~(A | B) |//nor
		{32{Func[5:0]==6'b101010}} & {31'h0,SF^OF} |//slt
		{32{Func[5:0]==6'b101011}} & {31'h0,~CF} |//sltu
		{32{Func[5:0]==6'b111111}} & mulResult
	);
	
endmodule

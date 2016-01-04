`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:42:20 10/14/2015 
// Design Name: 
// Module Name:    rotator 
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
module Rotator(
	input[31:0] RotatorIn,
	input[4:0] RotatorBitNum,
	input[1:0] RotatorOp,
	output[31:0] RotatorOut
    );
/*
reg[31:0] RotatorIn;
reg[4:0] RotatorBitNum;
reg[1:0] RotatorOp;

initial begin
	RotatorIn = 32'h00001998;
	RotatorBitNum = 5'b11001;
	RotatorOp = 0;
	#100
	RotatorOp = 2;
	#100
	RotatorOp = 3;
end*/
assign RotatorOut = 
	{32{RotatorOp==2'b00}} & (RotatorIn[31:0] << {1'b0,RotatorBitNum[4:0]}) |
	{32{RotatorOp==2'b10}} & ({32'h0,RotatorIn[31:0]} >> {1'b0,RotatorBitNum[4:0]}) |
	{32{RotatorOp==2'b11}} & ({{32{RotatorIn[31]}}, RotatorIn[31:0]} >> {1'b0,RotatorBitNum[4:0]}) ;
	
endmodule

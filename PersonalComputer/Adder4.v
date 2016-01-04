`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:32:28 03/24/2015 
// Design Name: 
// Module Name:    Adder4 
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
module Adder4(
    input [3:0] A,
    input [3:0] B,
    input c0,
    output [3:0] p,
    output [3:0] g,
    output [3:0] sum,
    output SF,
    output CF,
    output OF,
    output PF,
    output ZF
    );
wire[4:0] c;

assign 	p=A|B,
			g=A&B;
ParallelCarry4 PC(p,g,c0,c);

assign 	sum[0] = A[0]^B[0]^c[0],
	sum[1] = A[1]^B[1]^c[1],
	sum[2] = A[2]^B[2]^c[2],
	sum[3] = A[3]^B[3]^c[3];

assign 	SF=sum[3],
	CF=c[4],
	OF=c[4]^c[3],
	PF=sum[0]^sum[1]^sum[2]^sum[3],
	ZF= ~|sum[3:0];

endmodule

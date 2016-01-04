`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:05:27 03/24/2015 
// Design Name: 
// Module Name:    ParallelCarry4 
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
module ParallelCarry4(
    input [3:0] P,
    input [3:0] G,
    input C0,
    output [4:0] C
    );
assign 	C[0]=C0,
	C[1]=G[0]|P[0]&C[0],
	C[2]=G[1]|P[1]&G[0]|P[1]&P[0]&C[0],
	C[3]=G[2]|P[2]&G[1]|P[2]&P[1]&G[0]|P[2]&P[1]&P[0]&C[0],
	C[4]=G[3]|P[3]&G[2]|P[3]&P[2]&G[1]|P[3]&P[2]&P[1]&G[0]|P[3]&P[2]&P[1]&P[0]&C[0];

endmodule

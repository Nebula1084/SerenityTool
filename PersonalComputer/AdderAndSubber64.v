`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:17:48 03/31/2015 
// Design Name: 
// Module Name:    AdderAndSubber64 
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

//将adder封装一下，加上减法功能，去掉PG的输出，PG是为了扩展256bit加法器而留的接口
module AdderAndSubber64(
    input [63:0] A,
    input [63:0] B,
    input mode,
    output [63:0] result,
    output SF,
    output CF,
    output OF,
    output PF,
    output ZF
    );
wire [3:0] P,G;
wire[63:0] tmp={64{mode}}^B;
Adder64 Adder(A,tmp,mode,P,G,result,SF,CF,OF,PF,ZF);

endmodule

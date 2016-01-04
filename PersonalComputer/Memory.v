`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:12:01 05/13/2015 
// Design Name: 
// Module Name:    Memory 
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
module Memory(
input clk,
input[14:0] adrA,
input[14:0] adrB,
input we,
input[31:0] data,
input lh,	//when lh==0, big-endian. when lh==1, sign extension.
input sh,
output[31:0] outA,
output[15:0] outB
    );
wire weA,weB;
wire[13:0] addrAA,addrAB,addrBA,addrBB;
wire[15:0] doutAA,doutAB,doutBA,doutBB;
wire[15:0] dinA,dinB;
assign weA = we & ( ~sh | ~adrA[0] );
assign weB = we & ( ~sh | adrA[0] );
assign addrAA = adrA[0] ? (adrA[14:1]+1) : adrA[14:1];
assign addrBA = adrA[14:1];
assign dinA = sh ? data[15:0] : (adrA[0]? data[15:0] : data[31:16]);
assign dinB = sh ? data[15:0] : (adrA[0]? data[31:16] : data[15:0]);
assign outA = lh ? (adrA[0]? { {16{doutBA[15]}} ,doutBA} : { {16{doutAA[15]}}, doutAA}) : 
                   (adrA[0]? {doutBA,doutAA} : {doutAA,doutBA});

assign addrAB = adrB[14:1];
assign addrBB = adrB[14:1];
assign outB = adrB[0]? doutBB : doutAB;
BankA banka (
	.clka(clk),
	.wea(weA), // Bus [0 : 0] 
	.addra(addrAA), // Bus [13 : 0] 
	.dina(dinA), // Bus [15 : 0] 
	.douta(doutAA), // Bus [15 : 0] 
	.clkb(clk),
	.web(1'b0), // Bus [0 : 0] 
	.addrb(addrAB), // Bus [13 : 0] 
	.dinb(16'b0), // Bus [15 : 0] 
	.doutb(doutAB)); // Bus [15 : 0] 
	
BankB bankb (
	.clka(clk),
	.wea(weB), // Bus [0 : 0] 
	.addra(addrBA), // Bus [13 : 0] 
	.dina(dinB), // Bus [15 : 0] 
	.douta(doutBA), // Bus [15 : 0] 
	.clkb(clk),
	.web(1'b0), // Bus [0 : 0] 
	.addrb(addrBB), // Bus [13 : 0] 
	.dinb(16'b0), // Bus [15 : 0] 
	.doutb(doutBB)); // Bus [15 : 0] 	
endmodule

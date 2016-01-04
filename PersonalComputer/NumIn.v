`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:16:04 05/14/2015 
// Design Name: 
// Module Name:    NumIn 
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
module NumIn(
input clk,
input[7:0] addFlag,
output reg[31:0] number
    );
wire[7:0] btn_out;
	pbdebounce p0(clk,addFlag[0],btn_out[0]);
	pbdebounce p1(clk,addFlag[1],btn_out[1]);
	pbdebounce p2(clk,addFlag[2],btn_out[2]);
	pbdebounce p3(clk,addFlag[3],btn_out[3]);
	pbdebounce p4(clk,addFlag[4],btn_out[4]);
	pbdebounce p5(clk,addFlag[5],btn_out[5]);
	pbdebounce p6(clk,addFlag[6],btn_out[6]);
	pbdebounce p7(clk,addFlag[7],btn_out[7]);
	
	always @(posedge btn_out[0]) number[ 3: 0] <= number[ 3: 0] + 4'd1;
	always @(posedge btn_out[1]) number[ 7: 4] <= number[ 7: 4] + 4'd1;
	always @(posedge btn_out[2]) number[11: 8] <= number[11: 8] + 4'd1;
	always @(posedge btn_out[3]) number[15:12] <= number[15:12] + 4'd1;
	always @(posedge btn_out[4]) number[19:16] <= number[19:16] + 4'd1;
	always @(posedge btn_out[5]) number[23:20] <= number[23:20] + 4'd1;
	always @(posedge btn_out[6]) number[27:24] <= number[27:24] + 4'd1;
	always @(posedge btn_out[7]) number[31:28] <= number[31:28] + 4'd1;
endmodule

module pbdebounce
	(input wire clk,
	input wire button,
	output reg pbreg);
	
	reg [7:0] pbshift;
	wire clk_1ms;
	timer_1ms m0(clk, clk_1ms);
	always@(posedge clk_1ms) begin
	pbshift <=pbshift<<1;
	pbshift[0] <=button;
	if (pbshift==0)
	pbreg <=0;
	if (pbshift==8'hFF)
	pbreg <=1;
end
endmodule

module timer_1ms
	(input wire clk,
	output reg clk_1ms);
	
	reg [15:0] cnt;
	initial begin
	cnt [15:0] <=0;
	clk_1ms <= 0;
	end
	always@(posedge clk)
	if(cnt>=25000) begin
	cnt<=0;
	clk_1ms <= ~clk_1ms;
	end
	else begin
	cnt<=cnt+1;
	end
endmodule

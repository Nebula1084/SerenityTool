`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:25:45 06/10/2015 
// Design Name: 
// Module Name:    Top 
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
module Top(
input clk_50M,
//input btn,
input rst_n,
input RegNumPlus,

//vga
output hsync, 
output vsync, 
output vga_r,
output vga_g,
output vga_b,

//lcd
output lcd_rs,
output lcd_rw,
output lcd_e,
output[3:0] lcd_d,
output flash_ce,

//keyboard
input PS2C, 
input PS2D,

//Serial Port
input RxD,
output TxD
    );
wire clk;
assign clk = clk_50M;
/*
pbdebounce pb1 (
    .clk(clk_50M), 
    .button(btn), 
    .pbreg(clk)
    );*/
	 
//RegNumControl
reg[4:0] RegNum;
pbdebounce pb2 (
    .clk(clk_50M), 
    .button(RegNumPlus), 
    .pbreg(rnp)
    );
always@(posedge rnp) begin
	RegNum <= RegNum + 1;
end
	 
//MainBoard
wire[31:0] RegData,ProgramCounter,IR;
wire[31:0] ExtraOut;
MainBoard mb(
    //clk
    .clk(clk), 
    .clk_50M(clk_50M), 
	 
	 //for debug
    .RegNum(RegNum), 
    .RegData(RegData), 
    .ProgramCounter(ProgramCounter), 
	 .IR(IR),
	 .ExtraOut(ExtraOut),
	 
	 //vag
    .hsync(hsync), 
    .vsync(vsync), 
    .vga_r(vga_r), 
    .vga_g(vga_g), 
    .vga_b(vga_b), 
	 
	 //keyboard
    .PS2C(PS2C), 
    .PS2D(PS2D),
	 
	 //Serial Port
    .RxD(RxD),
    .TxD(TxD)
    );
//LCD_Display
wire[127:0] num128;
assign num128 = {
	ProgramCounter | {3'h0,RegNum,24'h0}, 
	RegData, 
	ExtraOut, 
	IR
	};
LCD_dis lcd (
    .clk(clk_50M), 
    .num(num128), 
    .reset(rst_n), 
    .lcd_rs(lcd_rs), 
    .lcd_rw(lcd_rw), 
    .lcd_e(lcd_e), 
    .lcd_d(lcd_d), 
    .flash_ce(flash_ce)
    );
	 
endmodule

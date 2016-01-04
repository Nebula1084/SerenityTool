`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:27:44 05/20/2015 
// Design Name: 
// Module Name:    RegisterFile 
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
module RegisterFile(
input clk,
input[4:0] R1,
input[4:0] R2,
input[4:0] WR,
input[31:0] Wdata,
input we,
output[31:0] Rdata1,
output[31:0] Rdata2,
input[4:0] R3,
output[31:0] Rdata3
    );
reg[31:0] regs[31:0];
assign Rdata1 = regs[R1[4:0]];
assign Rdata2 = regs[R2[4:0]];
assign Rdata3 = regs[R3[4:0]];

initial begin
$readmemh("RegisterFile.coe", regs);
end

always@(posedge clk) begin
	if(we)
		regs[WR] <= Wdata;
end

endmodule

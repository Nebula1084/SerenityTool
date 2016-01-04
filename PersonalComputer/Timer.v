`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:37:15 12/16/2015 
// Design Name: 
// Module Name:    Timer 
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
module Timer(
input clk,
output TimerInte
    );
reg[31:0] count = 1;
assign TimerInte = (count == 0);

always@(posedge clk) begin
	if(count == 32'd50000000) begin
		count <= 0;
	end
	else begin
		count <= count+1;
	end
end


endmodule

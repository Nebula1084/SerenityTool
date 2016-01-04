`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:26:10 09/30/2015 
// Design Name: 
// Module Name:    ZBziku 
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
module ZBziku(
    input[11:0] addra,
    output[15:0] douta
    );
reg[15:0] regs[0:4095];

initial begin
    $readmemh("ZBzk.coe", regs);
end

assign douta = regs[addra];

endmodule

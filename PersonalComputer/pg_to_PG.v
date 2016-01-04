`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:58:10 03/31/2015 
// Design Name: 
// Module Name:    pg_to_bpbg 
// bproject Name: 
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
module pg_to_PG(
    input [15:0] p,
    input [15:0] g,
    output [3:0] bp,
    output [3:0] bg
    );

assign bg[0]=g[3 ]|p[3 ]&g[2 ]|p[3 ]&p[2 ]&g[1 ]|p[3 ]&p[2 ]&p[1 ]&g[0 ],
       bg[1]=g[7 ]|p[7 ]&g[6 ]|p[7 ]&p[6 ]&g[5 ]|p[7 ]&p[6 ]&p[5 ]&g[4 ], 
       bg[2]=g[11]|p[11]&g[10]|p[11]&p[10]&g[9 ]|p[11]&p[10]&p[9 ]&g[8 ], 
       bg[3]=g[15]|p[15]&g[14]|p[15]&p[14]&g[13]|p[15]&p[14]&p[13]&g[12];
		 
assign	bp[0]=p[3]&p[2]&p[1]&p[0],
			bp[1]=p[7]&p[6]&p[5]&p[4],
			bp[2]=p[11]&p[10]&p[9]&p[8],
			bp[3]=p[15]&p[14]&p[13]&p[12];

endmodule

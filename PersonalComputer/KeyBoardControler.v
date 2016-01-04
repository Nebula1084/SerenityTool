`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:07:09 11/04/2015 
// Design Name: 
// Module Name:    KeyBoardControler 
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
module KeyBoardControler(
input wire clk,
input wire PS2C,
input wire PS2D,
input wire InteAccept,
output reg[15:0] scanCode,
output reg KeyBoardInte
    );
/*
reg clk;
reg PS2C;
reg PS2D;
wire InteAccept;
reg en;
assign InteAccept = KeyBoardInte & en;
initial begin
	clk = 0;
	PS2C = 0;
	PS2D = 0;
	en = 0;
	#12000
	en = 1;
end
//always@(posedge clk) begin
//	InteAccept <= KeyBoardInte;
//end
always #10 begin
	 clk = ~clk;
end
always #200 begin
	PS2C = ~PS2C;
end
always #330 begin
	PS2D = ~PS2D;
end
*/
	reg[7:0] buffer[15:0];
	reg[3:0] put,get;
	reg[3:0] state;
	reg[22:0] count;
	reg PS2Cf,PS2Df;
	reg [7:0] ps2c_filter,ps2d_filter;
	reg [10:0] shift1,shift2;
	reg new;
	
	initial begin
		put<=0; 
		get<=0;
		ps2c_filter<=0;
		ps2d_filter<=0;
		PS2Cf<=1;
		PS2Df<=1;
		shift1 <= 0;
		shift2 <= 0;
		state <= 0;
		count <= 0;
		new <= 0;
		KeyBoardInte <= 0;
	end
	
	always @(posedge clk) begin
		//对时钟线和数据线进行过滤
		ps2c_filter[7]<=PS2C;
		ps2c_filter[6:0]<=ps2c_filter[7:1];
		ps2d_filter[7]<=PS2D;
		ps2d_filter[6:0]<=ps2d_filter[7:1];
		if(ps2c_filter == 8'b11111111)
			PS2Cf<=1;
		else begin
			if(ps2c_filter == 8'b00000000)
				PS2Cf<=0;
		end
		
		if(ps2d_filter == 8'b11111111)
			PS2Df<=1;
		else begin
			if(ps2d_filter == 8'b00000000)
				PS2Df<=0;
		end
		if(PS2Df) begin
			count <= count + 1;
		end
		else begin
			count <= 1;
		end
		if(state == 10) begin
			new <= 1;
		end
		if((state == 0) && new) begin
			if(put != get-1) begin
				buffer[put] <= shift1[8:1];
				put <= put+1;
				new <= 0;
			end
		end
		if((get == put) | InteAccept) begin
			KeyBoardInte <= 0;
		end
		else begin
			if(KeyBoardInte == 0) begin
				scanCode <= buffer[get];
				KeyBoardInte <= 1;
			end
		end
		if(InteAccept & (get != put)) begin
			get <= get + 1;
		end
	end
	wire rst;
	assign rst = (count == 0);
	
	//移位寄存器，用以根据PS2的时钟记录PS2的扫描码
	always @(negedge PS2Cf or posedge rst) begin
		if(rst) begin
			state <= 0;
		end
		else begin
			shift1<={PS2Df,shift1[10:1]};
			shift2<={shift1[0],shift2[10:1]};//先发送高字节，再发送低字节
			
			if(state == 0 && PS2Df == 0) begin
				state <= 1;
			end
			else if(state == 10) begin
				state <= 0;
			end
			else begin
				state <= state + 1;
			end
		end
	end
endmodule

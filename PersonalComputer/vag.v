////////////////////////////////////////////////////////////////////////////////
module vga_dis(
input clk, //50MHz 
input rst_n, //低电平复位
input[15:0] ZBcode,
output valid, //有效显示区标志
output[9:0] xpos,//有效显示区坐标
output[9:0] ypos,
output hsync, //行同步信号
output vsync, //场同步信号
output vga_r,
output vga_g,
output vga_b
);

//--------------------------------------------------
reg[10:0] x_cnt; //行坐标
reg[9:0] y_cnt; //列坐标
initial begin
	x_cnt <= 0;
	y_cnt <= 0;
end
always @ (posedge clk or negedge rst_n)
if(!rst_n) x_cnt <= 11'd0;
else if(x_cnt == 11'd1039) x_cnt <= 11'd0;
else x_cnt <= x_cnt+1'b1;

always @ (posedge clk or negedge rst_n)
if(!rst_n) y_cnt <= 10'd0;
else if(y_cnt == 10'd665) y_cnt <= 10'd0;
else if(x_cnt == 11'd1039) y_cnt <= y_cnt+1'b1;

//--------------------------------------------------

//187-987,31-631
assign valid = (x_cnt >= 11'd267) && (x_cnt < 11'd907) 
&& (y_cnt >= 10'd131) && (y_cnt < 10'd531); 
 

assign xpos = x_cnt-11'd267;//187
assign ypos = y_cnt-10'd131;//31

//--------------------------------------------------
reg hsync_r,vsync_r; //同步信号产生
initial begin
	hsync_r <= 1;
	vsync_r <= 1;
end
always @ (posedge clk or negedge rst_n)
	if(!rst_n) 
		hsync_r <= 1'b1;
	else if(x_cnt == 11'd0) 
		hsync_r <= 1'b0; //产生hsync信号
	else if(x_cnt == 11'd120) 
		hsync_r <= 1'b1;
 
always @ (posedge clk or negedge rst_n)
if(!rst_n) vsync_r <= 1'b1;
else if(y_cnt == 10'd0) vsync_r <= 1'b0; //产生vsync信号
else if(y_cnt == 10'd6) vsync_r <= 1'b1;

assign hsync = hsync_r;
assign vsync = vsync_r;

//--------------------------------------------------
/*
wire[63:0] line[15:0];
assign  line[0] = 64'h0208000001002208,
        line[1] = 64'h421C200001001108,
        line[2] = 64'h226017FC01001110,
        line[3] = 64'h2240104001000020,
        line[4] = 64'h0FC0804001007FFE,
        line[5] = 64'h82404040FFFE4002,
        line[6] = 64'h427E484001008004,
        line[7] = 64'h52C8084001001FE0,
        line[8] = 64'h1348104002800040,
        line[9] = 64'h2E48104002800180,
        line[10] = 64'hE248E0400440FFFE,
        line[11] = 64'h2248204004400100,
        line[12] = 64'h2248204008200100,
        line[13] = 64'h22482FFE10100100,
        line[14] = 64'h2A88200020080500,
        line[15] = 64'h05080000C0060200;
wire[63:0] chosen;
wire white;
assign  chosen =  line[ypos[3:0]],
        white = chosen[5'd63-xpos[5:0]];
		  
assign vga_r = valid ? white : 1'b0;
assign vga_g = valid ? white : 1'b0;
assign vga_b = valid ? white : 1'b0;
*/
wire white;
wire[15:0] bit16;
wire[7:0] adr;
assign adr = (|ZBcode[15:7])? 8'h1 : ZBcode[7:0];
ZBziku ziku(
	.addra({adr[7:0],ypos[3:0]}),//[11:0]
	.douta(bit16)//[15:0]
);
/*
wire grid;
assign grid = (xpos[3:0] == 0) | (ypos[3:0] == 0);
*/
assign white = bit16[4'd15-xpos[3:0]];
assign vga_r = valid ? white : 1'b0;
assign vga_g = valid ? white : 1'b0;
assign vga_b = valid ? white : 1'b0;
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:49:38 10/13/2015 
// Design Name: 
// Module Name:    LCD_dis 
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
module LCD_dis(
	input clk,
	input[127:0] num,
	input reset,
	output reg lcd_rs,
	output lcd_rw,
	output reg lcd_e,
	output reg[3:0] lcd_d,
	output flash_ce
	);

assign flash_ce = 1;
assign lcd_rw = 0;

reg [19:0] delay_count;
reg [19:0] num_count;
wire[7:0] ascii;
wire[3:0] hex;
reg[4:0] dis_count;

reg [5:0] state;
reg state_change;
/*
initial begin
	dis_count<=5'h0;
	num_count<=20'd750000;
	state<=6'b1;
	state_change <=0;
	delay_count <= 1;
end
*/
assign ascii = (hex[3] & (hex[2] | hex[1]))? (8'h37 + {4'h0,hex}) : {4'h3 ,hex};
assign hex = 
    {4{(dis_count == 5'h1F)}} & num[3:0] |
    {4{(dis_count == 5'h1E)}} & num[7:4] |
    {4{(dis_count == 5'h1D)}} & num[11:8] |
    {4{(dis_count == 5'h1C)}} & num[15:12] |
    {4{(dis_count == 5'h1B)}} & num[19:16] |
    {4{(dis_count == 5'h1A)}} & num[23:20] |
    {4{(dis_count == 5'h19)}} & num[27:24] |
    {4{(dis_count == 5'h18)}} & num[31:28] |
    {4{(dis_count == 5'h17)}} & num[35:32] |
    {4{(dis_count == 5'h16)}} & num[39:36] |
    {4{(dis_count == 5'h15)}} & num[43:40] |
    {4{(dis_count == 5'h14)}} & num[47:44] |
    {4{(dis_count == 5'h13)}} & num[51:48] |
    {4{(dis_count == 5'h12)}} & num[55:52] |
    {4{(dis_count == 5'h11)}} & num[59:56] |
    {4{(dis_count == 5'h10)}} & num[63:60] |
    {4{(dis_count == 5'hF)}} & num[67:64] |
    {4{(dis_count == 5'hE)}} & num[71:68] |
    {4{(dis_count == 5'hD)}} & num[75:72] |
    {4{(dis_count == 5'hC)}} & num[79:76] |
    {4{(dis_count == 5'hB)}} & num[83:80] |
    {4{(dis_count == 5'hA)}} & num[87:84] |
    {4{(dis_count == 5'h9)}} & num[91:88] |
    {4{(dis_count == 5'h8)}} & num[95:92] |
    {4{(dis_count == 5'h7)}} & num[99:96] |
    {4{(dis_count == 5'h6)}} & num[103:100] |
    {4{(dis_count == 5'h5)}} & num[107:104] |
    {4{(dis_count == 5'h4)}} & num[111:108] |
    {4{(dis_count == 5'h3)}} & num[115:112] |
    {4{(dis_count == 5'h2)}} & num[119:116] |
    {4{(dis_count == 5'h1)}} & num[123:120] |
    {4{(dis_count == 5'h0)}} & num[127:124] ;

parameter state1 = 6'b000001;
parameter state2 = 6'b000010; 
parameter state3 = 6'b000011;
parameter state4 = 6'b000100;
parameter state5 = 6'b000101;
parameter state6 = 6'b000110;
parameter state7 = 6'b000111;
parameter state8 = 6'b001000;
parameter state9 = 6'b001001;
parameter state10 = 6'b001010;
parameter state11 = 6'b001011;
parameter state12 = 6'b001100;
parameter state13 = 6'b001101;
parameter state14 = 6'b001110;
parameter state15 = 6'b001111;
parameter state16 = 6'b010000;
parameter state17 = 6'b010001;
parameter state18 = 6'b010010;
parameter state19 = 6'b010011;
parameter state20 = 6'b010100;
parameter state21 = 6'b010101;
parameter state22 = 6'b010110;
parameter state23 = 6'b010111;
parameter state24 = 6'b011000;
parameter state25 = 6'b011001;
parameter state26 = 6'b011010;
parameter state27 = 6'b011011;
parameter state28 = 6'b011100;
parameter state29 = 6'b011101;
parameter state30 = 6'b011110;
parameter state31 = 6'b011111;
parameter state32 = 6'b100000;
parameter state33 = 6'b100001;
parameter state34 = 6'b100010;
parameter state35 = 6'b100011;
parameter state36 = 6'b100100;
parameter state37 = 6'b100101;
parameter state38 = 6'b100110;
parameter state39 = 6'b100111;
parameter state40 = 6'b101000;
parameter state41 = 6'b101001;
parameter state42 = 6'b101010;
parameter state43 = 6'b101011;
parameter state44 = 6'b101100;
parameter state45 = 6'b101101;
parameter state46 = 6'b101110;
parameter state47 = 6'b101111;
parameter state48 = 6'b110000;
parameter state49 = 6'b110001;
parameter state50 = 6'b110010;
parameter state51 = 6'b110011;
parameter state52 = 6'b110100;
parameter state53 = 6'b110101;
parameter state54 = 6'b110110;
parameter state55 = 6'b110111;
parameter state56 = 6'b111000;
parameter state57 = 6'b111001;
parameter state58 = 6'b111010;
parameter state59 = 6'b111011;





always @(posedge clk or posedge reset)
    if(reset) begin
        state_change <= 1'b0;
        delay_count <= 1'b1;
    end
    else
    if(delay_count == num_count - 1) begin
        state_change <= 1'b1;
        delay_count <= 1'b1;
    end
    else
    begin
        state_change <= 1'b0;
        delay_count <= delay_count + 1'b1;
    end

always @(posedge state_change or posedge reset)
    if(reset) begin
        state <= state1;
        num_count <= 20'd750000;
    end
    else begin
        case(state)

        state1:begin
            state <= state2;
            num_count <= 20'd4;
            lcd_rs <= 1'b0;
            lcd_e <= 1'b0;
            lcd_d <= 4'h3;
        end

        state2:begin
            state <= state3;
            num_count <= 20'd12;
            lcd_e <= 1'b1;
        end

        state3:begin
            state <= state4;
            num_count <= 20'd205000;
            lcd_e <= 1'b0;
        end

        state4:begin
            state <= state5;
            num_count <= 20'd4;
            lcd_d <= 4'h3;
        end

        state5:begin
            state <= state6;
            num_count <= 20'd12;
            lcd_e <= 1'b1;
        end

        state6:begin
            state <= state7;
            num_count <= 20'd5000;
            lcd_e <= 1'b0;
        end

        state7:begin
        state <= state8; 
        num_count <= 20'd4;
        lcd_d <= 4'h2;
        end

        state8:begin
        state <= state9;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end

        state9:begin
        state <= state10;
        num_count <= 20'd4000;
        lcd_e <= 1'b0;
        end
        //set funtion mode
        state10:begin
        state <= state11;
        num_count <= 20'd4;
        lcd_rs <= 0;
        lcd_d <= 4'h2;
        end

        state11:begin
        state <= state12;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end

        state12:begin
        state <= state13;
        num_count <= 20'd80;
        lcd_e <= 1'b0;
        end

        state13:begin
        state <= state14;
        num_count <= 20'd4;
        lcd_d <= 4'h8;
        end

        state14:begin
        state <= state15;
        num_count <= 20'd12; 
        lcd_e <= 1'b1;
        end

        state15:begin
        state <= state16;
        num_count <= 20'd4000;
        lcd_e <= 1'b0;
        end
        //set entry mode
        state16:begin
        state <= state17;
        num_count <= 20'd4;
        lcd_d <= 4'h0;
        end

        state17:begin
        state <= state18;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end

        state18:begin
        state <= state19;
        num_count <= 20'd80;
        lcd_e <= 1'b0;
        end

        state19:begin
        state <= state20;
        num_count <= 20'd4;
        lcd_d <= 4'h6;
        end
        state20:begin
        state <= state21;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end
        state21:begin
        state <= state22;
        num_count <= 20'd4000;
        lcd_e <= 1'b0;
        end
        //set display on/off
        state22:begin 
        state <= state23;
        num_count <= 20'd4;
        lcd_d <= 4'h0;
        end
        state23:begin
        state <= state24;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end
        state24:begin
        state <= state25;
        num_count <= 20'd80;
        lcd_e <= 1'b0;
        end
        state25:begin
        state <= state26;
        num_count <= 20'd4;
        lcd_d <= 4'hc;
        end
        state26:begin
        state <= state27;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end
        state27:begin
        state <= state28;
        num_count <= 20'd4000;
        lcd_e <= 1'b0;
        end
        //clear display
        state28:begin
        state <= state29;
        num_count <= 20'd4;
        lcd_d <= 4'h0;
        end
        state29:begin
        state <= state30;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end
        state30:begin
        state <= state31;
        num_count <= 20'd80;
        lcd_e <= 1'b0; 
        end
        state31:begin
        state <= state32;
        num_count <= 20'd4;
        lcd_d <= 4'h1;
        end
        state32:begin
        state <= state33;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end
        state33:begin
        state <= state34;
        num_count <= 20'd2000;
        lcd_e <= 1'b0;
        end
        state34:begin
        state <= state35;
        num_count <= 20'd82000;
        end
        //set DD RAM address
        state35:begin
            state <= state36;
            num_count <= 20'd4;
            lcd_rs <= 1'b0;
            lcd_e <= 1'b0;
            if (dis_count[4]) begin
                lcd_d <= 4'hC; 
            end
            else begin
                lcd_d <= 4'h8;
            end
        end

        state36:begin
        state <= state37;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end

        state37:begin
        state <= state38;
        num_count <= 20'd80;
        lcd_e <= 1'b0;
        end

        state38:begin
        state <= state39;
        num_count <= 20'd4; 
        lcd_d <= dis_count[3:0];
        end

        state39:begin
        state <= state40;
        num_count <= 20'd12;
        lcd_e <= 1'b1;
        end

        state40:begin
        state <= state41;
        num_count <= 20'd4000;
        lcd_e <= 1'b0;
        end
        // now starts writing data to DD RAM
        state41:begin
            state <= state42;
            num_count <= 20'd4;
            lcd_rs <= 1'b1;
            lcd_d <= ascii[7:4];
        end

        state42:begin
            state <= state43;
            num_count <= 20'd12;
            lcd_e <= 1'b1;
        end

        state43:begin
            state <= state44;
            num_count <= 20'd80;
            lcd_e <= 1'b0;
        end

        state44:begin
            state <= state45;
            num_count <= 20'd4;
            lcd_d <= ascii[3:0];
        end
        state45:begin
            state <= state46;
            num_count <= 20'd12;
            lcd_e <= 1'b1;
        end
        state46:begin
            state <= state35;
            num_count <= 20'd2000;
            lcd_e <= 1'b0;
            dis_count <= dis_count+1;
        end

        default:begin
            state <= state1;
            num_count <= 20'd800;
        end
        endcase
    end
endmodule

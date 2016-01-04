`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:47:07 11/27/2015 
// Design Name: 
// Module Name:    VHDControler 
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
module VHDControler(
    input clk,
	 
	 //RS-232
    input RxD,
    output TxD,
	 
	 //mem
	 input[8:0] VHDControlerAddr,
	 input ChipSelect,
	 input[31:0] WriteData,
	 input SaveHalf,
	 input LoadHalf,
	 input MemWrite,
	 input MemRead,
	 output reg [31:0] MemReadData,
	 output reg MemOK
	 );

reg[31:0] VHDoffset;
reg[15:0] Command;
reg[15:0] ready;
reg[5:0] state;

initial begin
	VHDoffset <= 32'h12345678;
	Command <= 0;
	ready <= 1;
	state <= 0;
end

//Mem
reg [7:0] VHDControlerAddrSave;
reg [31:0] WriteDataSave;
reg SaveHalfSave;
reg LoadHalfSave;
reg[2:0] IOcount;
initial begin
	IOcount <= 0;
	VHDControlerAddrSave <= 0;
	WriteDataSave <= 0;
	SaveHalfSave <= 0;
	LoadHalfSave <= 0;
	MemReadData <= 0;
	MemOK <= 0;
end
//reader
wire RxD_data_ready;
wire [7:0] RxD_data;
wire RxD_idle;
wire RxD_endofpacket;
reg RxD_data_ready_old;
async_receiver RX(
	.clk(clk), 
	.RxD(RxD), 
	.RxD_data_ready(RxD_data_ready), 
	.RxD_data(RxD_data),  // data received, valid only (for one clock cycle) when RxD_data_ready is asserted

	// We also detect if a gap occurs in the received stream of characters
	// That can be useful if multiple characters are sent in burst
	//  so that multiple characters can be treated as a "packet"
	.RxD_idle(RxD_idle),  // asserted when no data has been received for a while
	.RxD_endofpacket(RxD_endofpacket)  // asserted for one clock cycle when a packet has been detected (i.e. RxD_idle is going high)
	);
initial begin
	RxD_data_ready_old <= 0;
end
always@(posedge clk )begin
	RxD_data_ready_old <= RxD_data_ready;
end

/*
always @(posedge clk) 
	if(RxD_data_ready) begin
		GPout[7:0] <= RxD_data;
	end
*/




//writer
reg[7:0] TxD_data;
reg TxD_busy_old;
reg TxD_start;
wire TxD_busy;
async_transmitter TX(
	.clk(clk), 
	.TxD(TxD), 
	.TxD_start(TxD_start), 
	.TxD_data(TxD_data),
	.TxD_busy(TxD_busy)
	);
initial begin
	TxD_data <= 0;
	TxD_busy_old <= 0;
	TxD_start <= 0;
end
always @(posedge clk) begin
	TxD_busy_old <= TxD_busy;
end








	
//buffer
parameter BufferSize = 512;
reg[9:0] BufferAddr;
reg BufferWe;
reg[7:0] BufferWriteData;
wire[7:0] BufferOutData;
RAM_8bit_512 buffer (
  .a(BufferAddr[8:0]), // input [8 : 0] a
  .d(BufferWriteData), // input [7 : 0] d
  .clk(clk), // input clk
  .we(BufferWe), // input we
  .spo(BufferOutData) // output [7 : 0] spo
);
/*
assign BufferAddr = 
			{8{(state==20 || state==30)}} & ReadAddr[AddrWidth-1:0] |
			{8{(state==40)}} & WriteAddr[AddrWidth-1:0];*/
initial begin
	BufferAddr <= 0;
	BufferWe <= 0;
	BufferWriteData <= 0;
end


/*
reg [7:0] VHDControlerAddrSave;
reg [31:0] WriteDataSave;
reg SaveHalfSave;
reg LoadHalfSave;
	 output reg [31:0] MemReadData,
	 output reg MemOK,
*/
always @(posedge clk) begin
	case(state)
	00: begin
		MemOK <= 0;
		IOcount <= 0;
		if((Command == 2 || Command == 1)&& TxD_busy_old==0 && TxD_busy == 0) begin
			state <= 10;
			TxD_start <= 1;
			TxD_data <= VHDoffset[31:24];
			ready <= 0;
		end
		else if(ChipSelect) begin
			VHDControlerAddrSave <= VHDControlerAddr[7:0];
			SaveHalfSave <= SaveHalf;
			WriteDataSave <= WriteData;
			LoadHalfSave <= LoadHalf;
			MemReadData <= 0;
			if(MemWrite) begin
				if(VHDControlerAddr[8]) begin
					state <= 03;
				end
				else begin
					state <= 01;
				end
			end
			else if(MemRead) begin
				if(VHDControlerAddr[8]) begin
					state <= 04;
				end
				else begin
					state <= 02;
				end
			end
		end
	end
	
	01: begin	//write
		if(VHDControlerAddrSave[7:0] == 8'hFC) begin
			VHDoffset <= WriteDataSave;
		end
		else if(VHDControlerAddrSave[7:0] == 8'hFE) begin
			Command <= WriteDataSave[15:0];
		end
		MemOK <= 1;
		state <= 5;
	end
	02: begin	//read
		if(VHDControlerAddrSave[7:0] == 8'hFC) begin
			MemReadData <= VHDoffset;
		end
		else if(VHDControlerAddrSave[7:0] == 8'hFE) begin
			MemReadData <= {16'h0,Command};
		end
		else if(VHDControlerAddrSave[7:0] == 8'hFF) begin
			MemReadData <= {16'h0,ready};
		end
		MemOK <= 1;
		state <= 5;
	end
	
	/*
reg[9:0] BufferAddr;
reg BufferWe;
reg[7:0] BufferWriteData;*/

	03: begin //write
		if(SaveHalfSave) begin
			case(IOcount)
			0: begin
				BufferWe <= 1;
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0};
				BufferWriteData <= WriteDataSave[15:8];
				IOcount <= 1;
			end
			1: begin
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b1};
				BufferWriteData <= WriteDataSave[7:0];
				IOcount <= 2;
			end
			2: begin
				BufferAddr <= 0;
				BufferWriteData <= 0;
				BufferWe <= 0;
				IOcount <= 0;
				state <= 5;
				MemOK <= 1;
			end
			endcase
		end
		else begin
			case(IOcount)
			0: begin
				BufferWe <= 1;
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0};
				BufferWriteData <= WriteDataSave[31:24];
				IOcount <= 1;
			end
			1: begin
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b1};
				BufferWriteData <= WriteDataSave[23:16];
				IOcount <= 2;
			end
			2: begin
				if(({VHDControlerAddrSave,1'b0} + 2'b10) == 0) begin
					BufferAddr <= 0;
					BufferWriteData <= 0;
					BufferWe <= 0;
					IOcount <= 0;
					state <= 5;
					MemOK <= 1;
				end
				else begin
					BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0} + 2'b10;
					BufferWriteData <= WriteDataSave[15:8];
					IOcount <= 3;
				end
			end
			3: begin
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0} + 2'b11;
				BufferWriteData <= WriteDataSave[7:0];
				IOcount <= 4;
			end
			4: begin
				BufferAddr <= 0;
				BufferWriteData <= 0;
				BufferWe <= 0;
				IOcount <= 0;
				state <= 5;
				MemOK <= 1;
			end
			endcase
		end
	end
	
	04: begin
		if(LoadHalfSave) begin
			case(IOcount)
			0: begin
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0};
				IOcount <= 1;
			end
			1: begin
				MemReadData[31:16] <= {16{BufferOutData[7]}};
				MemReadData[15:8] <= BufferOutData;
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b1};
				IOcount <= 2;
			end
			2: begin
				MemReadData[7:0] <= BufferOutData;
				BufferAddr <= 0;
				IOcount <= 0;
				state <= 5;
				MemOK <= 1;
			end
			endcase
		end
		else begin
			case(IOcount)
			0: begin
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0};
				IOcount <= 1;
			end
			1: begin
				MemReadData[31:24] <= BufferOutData;
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b1};
				IOcount <= 2;
			end
			2: begin
				MemReadData[23:16] <= BufferOutData;
				if(({VHDControlerAddrSave,1'b0} + 2'b10) == 0) begin
					BufferAddr <= 0;
					IOcount <= 0;
					state <= 5;
					MemOK <= 1;
				end
				else begin
					BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0} + 2'b10;
					IOcount <= 3;
				end
			end
			3: begin
				MemReadData[15:8] <= BufferOutData;
				BufferAddr[8:0] <= {VHDControlerAddrSave,1'b0} + 2'b11;
				IOcount <= 4;
			end
			4: begin
				MemReadData[7:0] <= BufferOutData;
				BufferAddr <= 0;
				IOcount <= 0;
				state <= 5;
				MemOK <= 1;
			end
			endcase
		end
	end
	
	5: begin
		MemOK <= 0;
		state <= 0;
		if(Command != 0) begin
			ready <= 0;
		end
	end
	
	10: begin
		if(TxD_busy == 1 && TxD_busy_old==0) begin
			TxD_start <= 0;
		end
		if(TxD_busy == 0 && TxD_busy_old==1) begin
			state <= 11;
			TxD_start <= 1;
			TxD_data <= VHDoffset[23:16];
		end
	end
	11: begin
		if(TxD_busy == 1 && TxD_busy_old==0) begin
			TxD_start <= 0;
		end
		if(TxD_busy == 0 && TxD_busy_old==1) begin
			state <= 12;
			TxD_start <= 1;
			TxD_data <= VHDoffset[15:8];
		end
	end
	12: begin
		if(TxD_busy == 1 && TxD_busy_old==0) begin
			TxD_start <= 0;
		end
		if(TxD_busy == 0 && TxD_busy_old==1) begin
			state <= 13;
			TxD_start <= 1;
			TxD_data <= VHDoffset[7:0];
		end
	end
	13: begin
		if(TxD_busy == 1 && TxD_busy_old==0) begin
			TxD_start <= 0;
		end
		if(TxD_busy == 0 && TxD_busy_old==1) begin
			state <= 20;
			TxD_start <= 1;
			TxD_data <= Command[7:0];
		end
	end
	
	20: begin
		if(TxD_busy == 1 && TxD_busy_old==0) begin
			TxD_start <= 0;
			Command <= 0;
			BufferAddr <= 0;
			if (Command == 1)begin
				state <= 40;
			end
		end
		if(TxD_busy == 0 && TxD_busy_old==1) begin
			state <= 30;
			TxD_start <= 1;
			TxD_data <= BufferOutData;
		end
	end
	
	30: begin
		if(BufferAddr == BufferSize) begin
			BufferAddr <= 0;
			ready <= 1;
			state <= 0;
		end
		else begin
			if(TxD_busy == 1 && TxD_busy_old==0) begin
				TxD_start <= 0;
				BufferAddr <= BufferAddr + 1;
			end
			if(TxD_busy == 0 && TxD_busy_old==1) begin
				TxD_start <= 1;
				TxD_data <= BufferOutData;
			end
		end
	end
	40: begin
		if(BufferAddr == BufferSize) begin
			state <= 0;
			ready <= 1;
			BufferAddr <= 0;
		end
		else begin
			if(RxD_data_ready == 1 && RxD_data_ready_old == 0) begin
				BufferWriteData <= RxD_data;
				BufferWe <= 1;
			end
			if(RxD_data_ready == 0 && RxD_data_ready_old == 1) begin
				BufferAddr <= BufferAddr + 1;
				BufferWe <= 0;
			end
		end
	end
	endcase
end

endmodule


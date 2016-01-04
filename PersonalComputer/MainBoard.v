`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:25:19 11/02/2015 
// Design Name: 
// Module Name:    MainBoard 
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
module MainBoard(
`ifndef SIMULATION
//clk
input clk,
input clk_50M,

//for debug
input[4:0] RegNum,
`endif
output[31:0] RegData,
output[31:0] ProgramCounter,
output[31:0] ExtraOut,
output[31:0] IR,

//vga
output hsync, //行同步信号
output vsync, //场同步信号
output vga_r,
output vga_g,
output vga_b,

`ifndef SIMULATION
//KeyBoard
input PS2C,
input PS2D,
`endif

//Serial Port
input RxD,
output TxD
    );
`ifdef SIMULATION
reg clk;
reg clk_50M;
reg[4:0] RegNum;
reg PS2C;
reg PS2D;
initial begin
    // Initialize Inputs
    RegNum = 8;
    clk = 0;
    clk_50M = 0;
    clk = 0;
    PS2C = 0;
    PS2D = 0;
    // Wait 100 ns for global reset to finish
    #100;
       
    // Add stimulus here
end

always#10 begin
    clk_50M = ~clk_50M;
end
always#10 begin
    clk = ~clk;
end
always #200 begin
	PS2C = ~PS2C;
end
always #330 begin
	PS2D = ~PS2D;
end
`endif

//Multicycle CPU
reg[31:0] MemReadData;
reg MemOK;
initial begin
    MemReadData <= 0;
    MemOK <= 0;
end
wire OutINTE;
wire[31:0] OutCause;
wire[31:0] RequestedMemAddr;
wire RequestedMemWrite;
wire RequestedMemRead;
wire RequestedSaveHalf;
wire RequestedLoadHalf;
wire[31:0] RequestedMemWriteData;
wire InteAccept;
wire[31:0] MMCExtraOut;
MultiCycleCpu MCC (
     //input
    .clk(clk), 
    // .clk_50M(clk_50M), 
    .MemOK(MemOK),
    .MemReadData(MemReadData),
    .OutINTE(OutINTE),
    .OutCause(OutCause),
     
     //output
    .MemAddr(RequestedMemAddr), 
    .MemWrite(RequestedMemWrite), 
    .MemRead(RequestedMemRead), 
    .SaveHalf(RequestedSaveHalf), 
    .LoadHalf(RequestedLoadHalf), 
    .MemWriteData(RequestedMemWriteData), 
    .InteAccept(InteAccept),
     
     //for debug
    .RegNum(RegNum), 
    .ProgramCounter(ProgramCounter), 
    .IR(IR),
    .ExtraOut(MMCExtraOut),
	 .RegData(RegData)
    );
reg[31:0] MemAddr;
reg MemWrite;
reg MemRead;
reg SaveHalf;
reg LoadHalf;
reg[31:0] MemWriteData;
reg[1:0] state;
initial begin
    MemAddr <= 0;
    MemWrite <= 0;
    MemRead <= 0;
    SaveHalf <= 0;
    LoadHalf <= 0;
    MemWriteData <= 0;
    state <= 0;
end

assign ExtraOut = MemAddr;

//Block Memory
wire BlockMemChipSelect;
wire BlockMemOK;
wire[31:0] BlockMemReadData;
assign BlockMemChipSelect = (0 <= MemAddr) && (MemAddr < 16'h5000);
MemoryManagerUnit MMU (
    .clk(clk), 
    .clk_50M(clk_50M), 
    .MemAddr(MemAddr[14:0]), 
    .MemWrite(MemWrite), 
    .MemRead(MemRead), 
    .SaveHalf(SaveHalf), 
    .LoadHalf(LoadHalf), 
    .MemWriteData(MemWriteData), 
    .ChipSelect(BlockMemChipSelect),
    .MemReadData(BlockMemReadData), 
    .MemOK(BlockMemOK), 
     
    .hsync(hsync), 
    .vsync(vsync), 
    .vga_r(vga_r), 
    .vga_g(vga_g), 
    .vga_b(vga_b)
    );

//VHDControler 
wire VHDChipSelect;
wire VHDMemOK;
wire[31:0] VHDMemReadData;
assign VHDChipSelect = (MemAddr >= 32'h50FC) && (MemAddr < 32'h5200);
VHDControler vhd (
    .clk(clk), 
    .RxD(RxD), 
    .TxD(TxD), 
    .VHDControlerAddr(MemAddr[8:0]), //[8:0]
    .ChipSelect(VHDChipSelect), 
    .WriteData(MemWriteData), //[31:0]
    .SaveHalf(SaveHalf), 
    .LoadHalf(LoadHalf), 
    .MemWrite(MemWrite), 
    .MemRead(MemRead), 
    .MemReadData(VHDMemReadData), //[31:0]
    .MemOK(VHDMemOK)
    );

wire TimerInte;
Timer t(
	.clk(clk_50M),
	.TimerInte(TimerInte)
    );
	 
//KeyBoard
wire[15:0] KeyBoardReadData;
wire KeyBoardChipSelect;
wire KeyBoardOK;
wire KeyBoardInte;
wire KeyBoardInteAccept;
assign KeyBoardChipSelect = (MemAddr==32'h5000);
assign KeyBoardOK = 1;
KeyBoardControler kbc (
    .clk(clk_50M), 
    .PS2C(PS2C), 
    .PS2D(PS2D), 
    .InteAccept(KeyBoardInteAccept),
    .scanCode(KeyBoardReadData), 
    .KeyBoardInte(KeyBoardInte)
    );
	 
assign OutINTE = KeyBoardInte | TimerInte;
assign OutCause= {32{KeyBoardInte}} & 32'h1 | {32{(~KeyBoardInte) & TimerInte}} & 32'h2;
assign KeyBoardInteAccept = InteAccept & KeyBoardInte;

always@(posedge clk) begin
    case(state)
    0: begin
        if(RequestedMemRead | RequestedMemWrite) begin
            state <= 1;
            MemAddr <= RequestedMemAddr;
            MemWrite <= RequestedMemWrite;
            MemRead <= RequestedMemRead;
            SaveHalf <= RequestedSaveHalf;
            LoadHalf <= RequestedLoadHalf;
            MemWriteData <= RequestedMemWriteData;
        end
        else begin
            state <= 0;
        end
    end
    
    1: begin
        if( BlockMemOK & BlockMemChipSelect | 
            KeyBoardChipSelect & KeyBoardOK |
				VHDChipSelect & VHDMemOK
            ) begin
            state <= 2;
            MemOK <= 1;
            MemReadData <= 
					{32{BlockMemChipSelect}} & BlockMemReadData |
					{32{KeyBoardChipSelect}} & {16'h0,KeyBoardReadData} |
					{32{VHDChipSelect}} & VHDMemReadData ;
            MemWrite <= 0;
            MemRead <= 0;
        end
    end
    
    2: begin
        state <= 0;
        MemOK <= 0;
    end
	 
    default: begin
        MemAddr <= 0;
        MemWrite <= 0;
        MemRead <= 0;
        SaveHalf <= 0;
        LoadHalf <= 0;
        MemWriteData <= 0;
        state <= 0;
    end
    endcase
end
endmodule

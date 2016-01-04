`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:24:00 06/09/2015 
// Design Name: 
// Module Name:    MultiCycleCpu 
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
module MultiCycleCpu(
input clk,
//input clk_50M,
input MemOK,
input[31:0] MemReadData,
input OutINTE,
input[31:0] OutCause,
     //output
output[31:0] MemAddr,
output MemWrite,
output MemRead,
output SaveHalf,
output LoadHalf, 
output[31:0] MemWriteData,
output InteAccept,
     //for debug
input[4:0] RegNum,
output[31:0] RegData,
output[31:0] ProgramCounter,
output[31:0] ExtraOut,
output reg[31:0] IR/*,

input rst_n,
output hsync, //行同步信号
output vsync, //场同步信号
output vga_r,
output vga_g,
output vga_b*/
    );

/*
reg clk;
reg clk_50M;
reg rst_n;
reg[4:0] RegNum;
initial begin
    // Initialize Inputs
    RegNum = 8;
    rst_n = 1;
    clk = 0;
    clk_50M = 0;
    clk = 0;
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
*/
//PC
reg[31:0] PC;
initial begin
    PC <=0;
    IR <=0;
end

//IR
//reg[31:0] IR;
wire[5:0] opcode,func;
wire[4:0] RS,RT,RD,SFT;
assign opcode = IR[31:26];
assign func = IR[5:0];
assign RS=IR[25:21];
assign RT=IR[20:16];
assign RD=IR[15:11];
assign SFT=IR[10:6];


//Countrol Unit
wire 
PCWrite,
PCWriteCond,
IorD,
IRWrite,
RegWrite,
C0Write,
CauseSource,
SyscallInte,
IsBne;
/*
MemRead,
MemWrite,
SaveHalf,
LoadHalf,
MemOK;
*/
wire[1:0]
RegDst, 
ALUop,  
ALUSrcA,
C0Dst;
wire[2:0] 
ALUSrcB, 
WriteData, 
PCSource,
C0Src;

wire[5:0] CUState;
wire[5:0] ALUFunc;
wire[31:0] C0State;
ControlUnit cu (
    .clk(clk), 
    .opcode(opcode), 
    .func(func), 
    .RS(RS), 
    .MemOK(MemOK), 
     .C0State(C0State),
    .MemRead(MemRead), 
    .PCWrite(PCWrite), 
    .PCWriteCond(PCWriteCond), 
    .IorD(IorD), 
    .MemWrite(MemWrite), 
    .WriteData(WriteData), 
    .IRWrite(IRWrite), 
    .ALUSrcA(ALUSrcA), 
    .RegWrite(RegWrite), 
    .RegDst(RegDst), 
    .SaveHalf(SaveHalf), 
    .LoadHalf(LoadHalf), 
    .ALUop(ALUop), 
    .ALUSrcB(ALUSrcB), 
    .PCSource(PCSource), 
    .CUState(CUState), 
    .ALUFunc(ALUFunc), 
    .C0Write(C0Write), 
    .C0Dst(C0Dst), 
    .C0Src(C0Src), 
    .CauseSource(CauseSource), 
    .SyscallInte(SyscallInte),
	 .IsBne(IsBne)
    );
assign ProgramCounter = {10'b0,CUState[5:0],PC[15:0]};

//MMU
/*
wire[31:0] MemReadData; 
wire[31:0] MemWriteData;
wire[31:0] MemAddr;
MemoryManagerUnit MMU (
    .clk(clk), 
    .clk_50M(clk_50M), 
    .MemAddr(MemAddr[14:0]), 
    .MemWrite(MemWrite), 
    .MemRead(MemRead), 
    .SaveHalf(SaveHalf), 
    .LoadHalf(LoadHalf), 
    .MemWriteData(MemWriteData), 
    .rst_n(rst_n), 
    .MemReadData(MemReadData), 
    .MemOK(MemOK), 
     
    .hsync(hsync), 
    .vsync(vsync), 
    .vga_r(vga_r), 
    .vga_g(vga_g), 
    .vga_b(vga_b)
    );
always @(posedge clk) begin
    if(IRWrite) begin
        IR <= MemReadData;
    end
end*/

//Register File
wire[4:0] WR;
wire[31:0] regWdata,Rdata1,Rdata2;
assign WR = {5{(RegDst==2'b00)}} & RT |
            {5{(RegDst==2'b01)}} & RD |
                {5{(RegDst==2'b10)}} & 5'd31;
RegisterFile regFile (
    .clk(clk), 
    .R1(RS), 
    .R2(RT), 
    .WR(WR), 
    .Wdata(regWdata), 
    .we(RegWrite), 
    .Rdata1(Rdata1), 
    .Rdata2(Rdata2), 
    .R3(RegNum), 
    .Rdata3(RegData)
    );
reg[31:0] A;
reg[31:0] B;
always @(posedge clk) begin
    A <= Rdata1;
    B <= Rdata2;
end

//ALU
wire[31:0] srcA, srcB, SignExtendedImmediateNum, offset32, result, UnsignExtendedImmediateNum;
wire ZF;
assign SignExtendedImmediateNum = { {16{IR[15]}}, IR[15:0]};
assign UnsignExtendedImmediateNum = { 16'h0, IR[15:0]};
assign offset32 = {SignExtendedImmediateNum[30:0],1'b0};
assign srcA =  {32{(ALUSrcA == 2'b00)}} & PC    |
                    {32{(ALUSrcA == 2'b01)}} & A     |
                    {32{(ALUSrcA == 2'b10)}} & {27'h0,SFT};
                    
assign srcB =  {32{(ALUSrcB == 3'b000)}} & B     |
                    {32{(ALUSrcB == 3'b001)}} & 32'h2 |
                    {32{(ALUSrcB == 3'b010)}} & SignExtendedImmediateNum     |
                    {32{(ALUSrcB == 3'b011)}} & offset32 |
                    {32{(ALUSrcB == 3'b100)}} & UnsignExtendedImmediateNum;
ALU alu (
    .A(srcA), 
    .B(srcB), 
    .ALUop(ALUop), 
    .Func(ALUFunc), 
    .result(result), 
    .ZF(ZF)
    );
reg[31:0] ALUout;
always @(posedge clk) begin 
    ALUout <= result;
end

//Coprocessor0
wire[4:0] C0adr;
wire[31:0] C0Wdata;
wire[31:0] InteCause;
wire Interrupt;
wire[31:0] C0Data;
Coprocessor0 c0 (
    .clk(clk), 
    .C0adr(C0adr), 
    .C0Wdata(C0Wdata), 
    .C0Write(C0Write), 
    .InteCause(InteCause), 
    .Interrupt(Interrupt), 
    .InteAccept(InteAccept), 
    .C0State(C0State), 
    .C0Data(C0Data)
    );
assign InteCause = CauseSource ? 32'h0 : OutCause;
assign C0adr = 
                {5{C0Dst == 2'b00}} & RD |
                {5{C0Dst == 2'b01}} & 5'b00000 |
                {5{C0Dst == 2'b10}} & 5'b00010 |
                {5{C0Dst == 2'b11}} & 5'b00001 ;
assign C0Wdata = 
                {32{C0Src == 3'b000}} & B |
                {32{C0Src == 3'b001}} & PC |
                {32{C0Src == 3'b010}} & (C0Data & 32'hFFFFFFFE) |
                {32{C0Src == 3'b011}} & 32'h0 |
                {32{C0Src == 3'b100}} & (C0Data | 32'h1) ;
assign Interrupt = SyscallInte | OutINTE;
assign ExtraOut = {16'h0,3'b0,MemWrite,3'b0,MemRead,3'b0,SaveHalf,3'b0,LoadHalf};
     
//circuit
always @(posedge clk) begin
    if(IRWrite) begin
        IR <= MemReadData;
    end
end
assign MemAddr = IorD? ALUout : PC;
assign MemWriteData = B;
assign regWdata = {32{(WriteData == 3'b000)}} & ALUout |
                       {32{(WriteData == 3'b001)}} & MemReadData |
                       {32{(WriteData == 3'b010)}} & PC |
                        {32{(WriteData == 3'b011)}} & C0Data |
                        {32{(WriteData == 3'b100)}} & {IR[15:0], 16'h0};
always @(posedge clk) begin 
    if( PCWrite | (PCWriteCond & (ZF^IsBne))) begin
        case(PCSource)
            3'b000: begin
                PC <= result;
            end
            3'b001: begin
                PC <= ALUout;
            end
            3'b010: begin
                PC <= {PC[31:27],IR[25:0],1'b0};
            end
            3'b011: begin
                PC <= A;
            end
            3'b100: begin
                PC <= C0Data;
            end
            3'b101: begin
                PC <= 32'h4F00;
            end
        endcase
    end
end

/*
initial begin
    clk = 0;
    clk_50M = 0;
    rst_n = 1;
    RegNum = 19;
end
always#10
    clk_50M = ~clk_50M;
always#50
    clk = ~clk;
    */
endmodule

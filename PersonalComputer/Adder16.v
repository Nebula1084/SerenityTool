`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:48:59 03/24/2015 
// Design Name: 
// Module Name:    Adder32 
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
module Adder16(
    input [15:0] A,
    input [15:0] B,
    input C0,
    output [3:0] P,
    output [3:0] G,
    output [15:0] sum,
    output SF,
    output CF,
    output OF,
    output PF,
    output ZF
    );
wire[15:0] _p_,_g_;
wire[4:0] C;
wire[3:0] _sf_,_cf_,_of_,_pf_,_zf_;

pg_to_PG pgtoPG(_p_,_g_,P,G);
ParallelCarry4 PC(P,G,C0,C);

Adder4 
    a1(A[ 3: 0],B[ 3: 0],C[0],_p_[ 3: 0],_g_[ 3: 0],sum[ 3: 0],_sf_[0],_cf_[0],_of_[0],_pf_[0],_zf_[0]),
    a2(A[ 7: 4],B[ 7: 4],C[1],_p_[ 7: 4],_g_[ 7: 4],sum[ 7: 4],_sf_[1],_cf_[1],_of_[1],_pf_[1],_zf_[1]),
    a3(A[11: 8],B[11: 8],C[2],_p_[11: 8],_g_[11: 8],sum[11: 8],_sf_[2],_cf_[2],_of_[2],_pf_[2],_zf_[2]),
    a4(A[15:12],B[15:12],C[3],_p_[15:12],_g_[15:12],sum[15:12],_sf_[3],_cf_[3],_of_[3],_pf_[3],_zf_[3]);

assign  SF=_sf_[3],
    CF=C[4],
    OF=_of_[3],
    PF=^_pf_[3:0],
    ZF= ~|(~_zf_[3:0]);

endmodule

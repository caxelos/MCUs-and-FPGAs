
`timescale 1ns / 1ps
`define  PERIOD 20//ns
/*
thelw na ipologisw to 10x9
*/
module testB;

parameter N = 4;


wire [N-1:0] parallel;
wire [N-1:0] serial;
wire [2*N-1:0] product;

reg clk, MULT_EN;
//assign parallel = 8'b0001_0000;//16
//assign serial = 8'b001_0000;//16

assign parallel = 4'b1010;//10
assign serial = 4'b1001;//9

shift_add_mult shift_add_mult0(clk, parallel, serial, MULT_EN, product );
initial begin
  clk = 1;
  MULT_EN = 0;
  #40 MULT_EN = 1;
  
  #(`PERIOD*N-10) MULT_EN = 0;
end

always
  #(`PERIOD/2) clk = ~clk;



endmodule

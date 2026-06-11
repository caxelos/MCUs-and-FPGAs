`timescale 1ns/1ps

module LEDdecoderTb;

reg clk;
reg reset;
//input wire an3, an2, an1, an0, a, b, c, d, e, f, g, dp, CLKIN_IBUFG_OUT;


FourDigitLEDdriver FourDigitLEDdriver_instance(clk, reset);

//initializations

  initial begin
	clk = 1'b0;
	reset = 1'b1;
	#100 reset = 1'b0;
  end
  
  always
	#50 clk = ~clk;
	
  
endmodule 

/*

WARNING:HDLCompiler:1142 - "C:/Xilinx/12.2/ISE_DS/lab1/LEDdecoderTb.v" 
Line 3: Compiler directive `timescale is not allowed here

WARNING:HDLCompiler:1016 - "C:/Xilinx/12.2/ISE_DS/lab1/LEDdecoderTb.v" 
Line 10: Port an3 is not connected to this instance
*/
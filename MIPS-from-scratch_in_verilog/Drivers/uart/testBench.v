`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:38:27 11/08/2016
// Design Name:   ReceiversSystemData
// Module Name:   C:/Xilinx/12.2/ISE_DS/ERG2MER3/myTestBench.v
// Project Name:  ERG2MER3
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: ReceiversSystemData
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////



module TestBench;

	// Inputs
	reg clk;
	reg reset;

   uart_transmitter uart_Tx (
	  .clk(clk),
	  .reset(reset)
	);

initial begin
  clk = 0;
  reset = 0;
  
  #100 reset = 1;
end


always 
  #20 clk = ~clk;  
      
endmodule


`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:33:05 12/15/2016
// Design Name:   instruction_fsm
// Module Name:   C:/Users/Paraskevi/Desktop/ise_backup/LcdLAB/myTest.v
// Project Name:  LcdLAB
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: instruction_fsm
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module myTest;

	// Inputs
   reg clk;
	reg reset;
	wire [11:8] SF_D;
	wire [7:0] data;
	// Instantiate the Unit Under Test (UUT)
	instruction_fsm uut (
		.clk(clk),
		.reset(reset),
		//get
		.SF_D(SF_D),
		.LCD_RS(LCD_RS),
		.LCD_EN(LCD_EN),
		.LCD_RW(LCD_RW),
		.data(data)
	);

	initial begin
		// Initialize Inputs
      clk = 0;
		reset = 1;
		// Wait 100 ns for global reset to finish
		#100 reset = 0; 
		// Add stimulus here

	end
   
  always
    #10 clk = ~clk;  
endmodule


`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:42:21 10/10/2016 
// Design Name: 
// Module Name:    clockDiv16 
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


module clockDiv16( 
  input GLBclk,
  input reset,
  output clkD16,
  output clk50M
 );

/*
dcm_25 instance_name (
    .CLKIN_IN(GLBclk), //50MHz input
    .RST_IN(reset), 
    .CLKDV_OUT(clkD16), 	
    .CLKIN_IBUFG_OUT(),//open
	 .CLK0_OUT(clk50M), 
    .LOCKED_OUT()
);
*/
endmodule




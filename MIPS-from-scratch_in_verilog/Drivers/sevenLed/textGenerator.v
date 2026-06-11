`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:23:14 10/14/2016 
// Design Name: 
// Module Name:    textGenerator 
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
module textGenerator(
    input clk,
    input reset,
    output reg [3:0] char
);

always @(posedge clk)  begin
  if (reset == 0)  begin
    char <= 4'b0000;
  end
  
  if (reset == 1) begin
    if (char == 4'b1111) begin
      char <= 4'b0000;
    end
    else begin
      char <= char + 4'b0001;
    end
  end	 
	 
end


endmodule

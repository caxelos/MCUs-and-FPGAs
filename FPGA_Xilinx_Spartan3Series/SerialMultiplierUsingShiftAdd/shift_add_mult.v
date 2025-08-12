`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:34:59 12/12/2016 
// Design Name: 
// Module Name:    shift_add_mult 
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
//do the 1101x1011

module shift_add_mult(clk,	parallel_IN, serial_IN,	MULT_EN,	product);

parameter N = 4;

input wire clk;
input wire [N-1:0] parallel_IN;
input wire [N-1:0] serial_IN;
input wire MULT_EN;
output reg [2*N-1:0] product;

reg [N-1:0] serial_IN_SAVED;
reg [2*N-1:0] parallel_IN_SAVED;

wire [2*N-1:0] add_to_product;

/*
memory part
*/

always @(posedge clk) begin
    if (MULT_EN == 0) begin
		parallel_IN_SAVED = {8'b0, parallel_IN};
		serial_IN_SAVED = serial_IN;
    end
    else begin
	   parallel_IN_SAVED =   {parallel_IN_SAVED[2*N-2:0], 1'b0};
		serial_IN_SAVED = {1'b0, serial_IN_SAVED[ N-1:1]};
	 end	 
end

/*
choose input
*/
//assign multiplexer_out = (SHIFT_EN==1 )? {parallel_IN_SAVED[6:0], 1'b0} : parallel_IN;

/*
check if the next serial input is zero
 - if it is zero, do only the shift op
 - else do shift and add
*///
/*
assign add_to_product =  {  
		parallel_IN_SAVED[7] & serial_IN_SAVED[0],
		parallel_IN_SAVED[6] & serial_IN_SAVED[0],
		parallel_IN_SAVED[5] & serial_IN_SAVED[0],
		parallel_IN_SAVED[4] & serial_IN_SAVED[0],
		parallel_IN_SAVED[3] & serial_IN_SAVED[0],
		parallel_IN_SAVED[2] & serial_IN_SAVED[0],
		parallel_IN_SAVED[1] & serial_IN_SAVED[0],
		parallel_IN_SAVED[0] & serial_IN_SAVED[0]
  };
*/

genvar i;
generate
  for (i = 0; i < 2*N; i = i + 1) begin:m
    assign  add_to_product[i] = parallel_IN_SAVED[i] & serial_IN_SAVED[0];
  end
endgenerate 

 
/*
save each add until adding
only zeros
*/
always @(posedge clk) begin
  if (MULT_EN == 0) 
    product = 0;
  else 
    product = product + add_to_product;
	 
 	 
end


endmodule


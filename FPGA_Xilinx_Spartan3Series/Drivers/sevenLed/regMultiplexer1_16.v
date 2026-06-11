`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:51:42 10/18/2016 
// Design Name: 
// Module Name:    segMultiplexer1_16 
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
module regMultiplexer1_16(
	input reset,
	input [3:0] rot_counter,
   input [1:0] regSel,	
	output [3:0] char
);
    
	reg [3:0] regs[0:15];
	
always @(negedge reset) begin // arxikopoihsh tis mnhmhs asugxrona me to roloi molis patithei to  reset
	if (reset == 0)	begin
		regs[0] <= 4'b0000;
		regs[1] <= 4'b0001;
		regs[2] <= 4'b0010;
		regs[3] <= 4'b0011;
		regs[4] <= 4'b0100;
		regs[5] <= 4'b0101;
		regs[6] <= 4'b0110;
		regs[7] <= 4'b0111;
		regs[8] <= 4'b1000;
		regs[9] <= 4'b1001;
		regs[10] <= 4'b1010;
		regs[11] <= 4'b1011;
		regs[12] <= 4'b1100;
		regs[13] <= 4'b1101;
		regs[14] <= 4'b1110;
		regs[15] <= 4'b1111;
	end
end
	

assign char = (reset == 0)?regs[regSel+rot_counter]:4'b0000  ;

endmodule

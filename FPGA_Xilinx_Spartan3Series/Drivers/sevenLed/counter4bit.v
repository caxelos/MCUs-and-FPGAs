`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:46:15 10/06/2016 
// Design Name: 
// Module Name:    counter4bit 
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
module counter6bit( 
  input clk16, 
  input reset,   
  output reg an3,
  output reg an2,
  output reg an1,
  output reg an0,
 
  output [1:0] regSel // output to 16:1 multiplexer
);

reg [3:0] counter;


assign regSel[1] = counter[3];
assign regSel[0] = counter[2];

/*
assign regSel[2] = counter[2];
assign regSel[3] = counter[3];
*/

always @(posedge clk16)  begin
  if (reset == 0) begin
    case (counter)

		4'b11_11: begin//15
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 0;
		  
		  counter = 0;
		end
		4'b11_10: begin//14
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b11_01: begin//13
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b11_00: begin//12, anavei to 4o(an0=0)
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b10_11: begin//11
		  an0 = 0;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b10_10: begin//10
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b10_01: begin//9
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b10_00: begin//8, anavei to 3o(an1=0)
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b01_11: begin//7
		  an0 = 1;
		  an1 = 0;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b01_10: begin//6
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b01_01: begin//5
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b01_00: begin//4, anavei to 2o(an2=0)
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		
		  counter = counter + 1;
		end
		4'b00_11: begin//3, ola svista
		  an0 = 1;
		  an1 = 1;
		  an2 = 0;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		4'b00_10: begin//2, ola svista
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		  
		  counter = counter + 1;
		end
		'b00_01: begin//1, ola svista
		  an0 = 1;
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		
		  counter = counter + 1;
		end
		4'b00_00: begin//0, anavei to 1o lampaki(an3=0)
		  an0 = 1; 
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		 
		  counter = counter + 1;
		end
		
		default: begin
		  counter = 4'b11_11;
		  
		  an0 = 1; 
		  an1 = 1;
		  an2 = 1;
		  an3 = 1;
		end
    endcase
  end
end  

endmodule

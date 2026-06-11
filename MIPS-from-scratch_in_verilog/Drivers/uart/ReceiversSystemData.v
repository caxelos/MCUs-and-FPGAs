`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    12:10:18 11/03/2016 
// Design Name: 
// Module Name:    ReceiversSystemData 
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
module ReceiversSystemData(
  input clk,
  input reset, 
  input startBitFound,  
  input [7:0] Rx_DATA,//sto sustma
  input  Rx_FERROR, // Framing Error //
  input  Rx_PERROR, // Parity Error //
  input Rx_VALID,// Rx_DATA is Valid //
  output reg Rx_EN,
  output wire [2:0] Rx_baud_select,
  output reg [0:7] outToLEDs
);


reg [1:0] dataCounter;
assign Rx_baud_select = 3'b111;

/*
metafora stin eksodo gia to 7-led-segment
*/
always @( posedge clk ) begin
  if (reset == 1) begin 
    if (Rx_VALID == 1 && Rx_FERROR == 0 && Rx_PERROR == 0) begin//an einai etoimo to packeto apo ton receiver sto susthma
      Rx_EN = 0;//stamata tin lipsi
		case (dataCounter)
	     2'b00: begin
			 outToLEDs = Rx_DATA;	
			 dataCounter =dataCounter+1;
		  end
		  2'b01: begin
          outToLEDs = Rx_DATA;			 
		    dataCounter =dataCounter+1;
		  end
		  2'b10: begin  
			 outToLEDs = Rx_DATA;
		    dataCounter =dataCounter+1;
		  end
		  default: begin
			 outToLEDs = Rx_DATA;
		    dataCounter = 0;
		  end  
		
      endcase		
    end
    else begin//an ginetai akoma i apostoli bit, to Read na paramenei 0
	   if (startBitFound == 1) begin
	     Rx_EN = 1;
	   end
	   else begin
	     Rx_EN = 0;
	   end
	 end
	 
  end
  else begin//reset=0
    dataCounter = 2'b11;
	 Rx_EN = 0;
	 outToLEDs = Rx_DATA;
  end
  
end


endmodule

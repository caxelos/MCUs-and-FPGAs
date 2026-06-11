`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:26:20 10/29/2016 
// Design Name: 
// Module Name:    systemData 
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

/*
- ta info messages xst 2261 einai fisiologika giati dimiourgountai sts
anatheseis mikra latch se orismena bit
- afto tha mporousa na to diorthwsw alla dn to ekana gia na fainontai
kalutera oi anatheseis sto Tx_DATA
*/
module TransmittersSystemData(
   input clk,
	input reset,
	input Tx_BUSY,
	output Tx_EN,
	output Tx_WR,
	output reg [0:7] Tx_DATA,
   output wire [2:0] Tx_baud_select
);


reg [3:0] dataCounter;   


assign Tx_baud_select = 3'b111;
assign Tx_EN = (Tx_BUSY)? 1:0;
assign Tx_WR = (Tx_BUSY)? 0:1;


always @( posedge clk ) begin
  if (reset == 1) begin
    if (Tx_BUSY == 0) begin//AN DN stelnei o transmitter ston receiver
	    
		case (dataCounter)
	     4'b0001: begin
		    Tx_DATA <= 8'b1000_1001;//regs[0];
		    dataCounter <= dataCounter<<1;
		  end
		  4'b0010: begin
		    Tx_DATA <= 8'b0101_0101;//regs[1];
		    dataCounter <= dataCounter<<1;
		  end
		  4'b0100: begin
		    Tx_DATA <= 8'b1100_1100;//regs[2];
		    dataCounter <= dataCounter<<1;
		  end
		  default: begin
		    Tx_DATA <= 8'b1010_1010;//regs[3];
		    dataCounter <= 4'b0001;
		  end  
		
		
      endcase		
    end
  end
  else begin//reset=0
    dataCounter <= 4'b0000;
    Tx_DATA <= 8'b1111_1111;
  end
end
	
endmodule 
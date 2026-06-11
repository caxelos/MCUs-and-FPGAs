`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: Christos Axelos
// 
// Create Date:    21:24:36 10/28/2016 
// Design Name: 
// Module Name:    uart_transmitter 
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
module uart_transmitter( 
  input clk,//from receiver
  input reset,//from receiver  
  output reg TxD
); 

reg Tx_BUSY_REG;
reg [3:0] sampleCounter;
reg [0:7] Tx_DATA_REG;
reg parity;
wire Tx_BUSY;
wire Tx_sample_ENABLE;
wire [2:0] Tx_baud_select;
wire [0:7] Tx_DATA;



TransmittersSystemData TSD(
   //inputs
	.clk(clk),
	.reset(reset),
	.Tx_BUSY(Tx_BUSY),
	//outputs
	.Tx_baud_select(Tx_baud_select),
	.Tx_DATA(Tx_DATA),
	.Tx_EN(Tx_EN),
	.Tx_WR(Tx_WR)	
);


baud_controller_transmitter baud_controller_tx_instance(
   //inputs
	.clk(clk),
   .reset(Tx_EN),
	.Tx_baud_select(Tx_baud_select), 
	
	//output
	.Tx_sample_ENABLE(Tx_sample_ENABLE)
);

uart_receiver uart_receiver0(
  //input
  .RxD(TxD), 
  
  //output
  .clk(clk),
  .reset(reset) 
);

/*
byte transmission from SystemToTranssmiter
*/
always @(posedge clk) begin
  if (reset == 1) begin
    if (Tx_WR == 1)  begin
      Tx_DATA_REG <= Tx_DATA;
    end
	 
  end 
  else begin//reset=0
    Tx_DATA_REG <= 8'b0000_0000;
  end
end  


assign Tx_BUSY = Tx_BUSY_REG;
/*
bit transmission from TransmitterToReceiver
*/  
always @(posedge clk)  begin
  if (reset == 1)  begin//an to kuklwma ein ON
    if (Tx_EN == 1) begin//an dn teleiwse to transmission
      if (Tx_sample_ENABLE == 1)  begin// irthe i stigmi na steileis to bit...
	     case (sampleCounter)
		    4'b1010: begin //start bit
		      sampleCounter = 4'b0000;
			   TxD = 0;
				Tx_BUSY_REG = 1;
		    end
		    4'b1001: begin // stop bit
		      TxD = 1;
				Tx_BUSY_REG = 0;//gia 2 kuklous afto tha einai miden
				  
				//sampleCounter=sampleCounter+1;
			 end
		    4'b1000: begin //parity bit, 0 ARTIOS 1 PERRITOS
		      TxD = parity;
			   Tx_BUSY_REG = 1;
			   sampleCounter=sampleCounter+1;
		    end
		    default: begin //databits
		      TxD = Tx_DATA_REG[sampleCounter];//reg[0] mexri reg[7]
				Tx_BUSY_REG = 1;
			   if (Tx_DATA_REG[sampleCounter] == 1) begin
			     parity = ~parity;
			   end
			 
			   sampleCounter=sampleCounter+1;
		    end
		
		  endcase
	   end

    end		
	 else begin//Tx_EN=0
	   sampleCounter = 4'b1010;
		Tx_BUSY_REG = 1;
		parity = 0;
	 end
  end
  else begin//reset=0
    sampleCounter = 4'b1010;
	 TxD = 1;//to kanali paramenei adranes
	 Tx_BUSY_REG = 0;
	 parity = 0;
  end

end 




endmodule 
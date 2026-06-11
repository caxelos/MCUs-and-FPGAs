`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    01:07:43 11/02/2016 
// Design Name: 
// Module Name:    uart_receiver 
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
module uart_receiver(
  input clk,
  input reset,  
  input RxD//apton transmitter
);


reg ff1Out, ff2Out;//startBitDebounce
reg Rx_FERROR_REG; //stop bit error
reg Rx_PERROR_REG; // Parity Error //
reg Rx_VALID_REG;// Rx_DATA is Valid //
reg RxDOnePeriodBack, RxDTwoPeriodsBack;
reg parity;
reg [3:0] sampleCounter;
reg [0:7] Rx_DATA_REG;//sto sustma


wire startBitFound;
wire calculatedBit;
wire Rx_sample_ENABLE;
wire Rx_FERROR; // Framing Error //
wire Rx_PERROR;
wire [2:0] Rx_baud_select;
wire [0:7] Rx_DATA; 

assign Rx_VALID = Rx_VALID_REG;
assign Rx_DATA = Rx_DATA_REG;
assign Rx_PERROR = Rx_PERROR_REG;
assign Rx_FERROR = Rx_FERROR_REG;

ReceiversSystemData RSD(
  //inputs
  .clk(clk), 
  .reset(reset),
  .startBitFound(startBitFound),
  .Rx_EN(Rx_EN), 
  .Rx_PERROR(Rx_PERROR),
  .Rx_FERROR(Rx_FERROR),    
  .Rx_DATA(Rx_DATA),
  //outputs
  .Rx_baud_select(Rx_baud_select),  
  .Rx_VALID(Rx_VALID)
);

baud_controller_receiver baud_controller_rx_instance(
  clk, 
  Rx_EN, 
  Rx_baud_select, 
  Rx_sample_ENABLE
);

/*
Start Bit detection-debounce
*/
always @(posedge clk) begin//enas poluplektis
  if (reset == 1) begin
    ff2Out = ff1Out;
  end
  else begin
    ff2Out = 1;
  end
end//afto prepei na nai 0 
 
always @(posedge clk) begin//kiallos poluplektis
  if (reset == 1) begin
    ff1Out = RxD;
  end
  else  begin
	  ff1Out = 1;
  end	
end//afto prepei na nai 1

assign startBitFound = ( (~ff1Out)&ff2Out ) | Rx_EN ;


/*
bit 7/16 in sampling saved
*/
always @(posedge clk) begin
  if (reset == 1  /*&& Rx_sample_ENABLE == 1*/) begin
    RxDTwoPeriodsBack = RxD;
  end
  else begin
    RxDTwoPeriodsBack = 1;
  end

end

/*
bit 8/16 in sampling saved
*/
always @(posedge clk) begin
  if (reset == 1 /*&& Rx_sample_ENABLE == 1*/) begin
    RxDOnePeriodBack = RxDTwoPeriodsBack;  
  end
  else begin
    RxDOnePeriodBack = 1;
  end
end


assign calculatedBit = RxD*RxDTwoPeriodsBack  + RxD*RxDOnePeriodBack +  RxDTwoPeriodsBack*RxDOnePeriodBack;
/*
one-hot counter for sampRepeats 
*/

/*
i want to sample the 9th of 16 SamplesPerBit
and find the final bit after sampling
from the equation calculatedBit = 9th + (7th*8th) 
*/



always @(posedge clk )  begin
  if (reset == 1)  begin//an to kuklwma ein ON
    if (Rx_EN == 1) begin//an dn teleiwse to transmission
      if (Rx_sample_ENABLE == 1)  begin// irthe i stigmi na deigmatoliptiseis...
	     case (sampleCounter)
		    4'b1010: begin //start bit
				sampleCounter = 4'b0000;
				Rx_VALID_REG = 0;
		    end
		    4'b1001: begin // stop bit
		      sampleCounter=sampleCounter+1;
			   if (calculatedBit == 1) begin
				  Rx_VALID_REG = 1;
				  
				end
				else begin
				  Rx_FERROR_REG = 1;
				end
			 end
		    4'b1000: begin //parity bit, 0 ARTIOS 1 PERRITOS
				if (parity == RxD) begin
				  Rx_PERROR_REG = 0;//everything's ok
				end
				else begin
				  Rx_PERROR_REG = 1;//show parity error
				end
				
			   sampleCounter=sampleCounter+1;
		    end
		    default: begin //databits
			   Rx_VALID_REG = 0;
		      Rx_DATA_REG[sampleCounter] = calculatedBit;
			   if (calculatedBit == 1) begin
			     parity = ~parity;
			   end
			 
			   sampleCounter=sampleCounter+1;
		    end
		
		  endcase
	   end
    end		
	 else begin//Rx_EN=0 
	   sampleCounter = 4'b1010;
		
		Rx_PERROR_REG = 0;
		Rx_FERROR_REG = 0;
		Rx_VALID_REG = 0;
		parity = 0;
		Rx_DATA_REG = 8'b1111_1111;
	 end
  end
  else begin//reset=0
    sampleCounter = 4'b1010;
	 parity = 0;
	 Rx_PERROR_REG = 0;
	 Rx_VALID_REG = 0;
	 Rx_FERROR_REG = 0;
	 Rx_DATA_REG = 8'b1111_1111;
  end

end 




endmodule 

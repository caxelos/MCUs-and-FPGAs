`timescale 1ns / 1ps
/*
T_sc = 1/(16 x Baud Rate)
ara
 Baud Rate
*********************
TRANSSMITER BAUD_CONTROL
*********************
FREQUENCIES

- afta einai suxnotites, enw to roloi exei suxnotita 50,000,000Hz
- diairw to roloi me tis parapanw suxnotites ksexwrista kai pairnw tous metrites
- kanw stroggulopoihsi ston poio kontino akeraio
**************************
COUNTERS(tou transsmiter): F/BAUD
000 | 50,000,000/300 =166666.66(counter = 101000101100001011)-18bits
001 | 50,000,000/1200 =41666.66(counter = 001010001011000011)
010 | 50,000,000/4800 =10416.66(counter = 000010100010110001)
011 | 50,000,000/9600 =5208.33 (counter = 000001010001011000)
100 | 50,000,000/19200 =2604.16(counter = 000000101000101100)
101 | 50,000,000/38400 =1302.08(counter = 000000010100010110)
110 | 50,000,000/57600 =868.05 (counter = 000000001101100100)
111 | 50,000,000/115200 =434.02(counter = 000000000110110010=434)...(antistoixosReceiver:11011=27)

SXETIKO SFALMA:

111|  434mod27=0.7
SFALMATA(ston poio kontino akeraio, diaforaPragmatikisTimisApoTousCounters)
000 | 0.36 KATW apto kanoniko 
001 | 0.36 katw 
010 | 0.36 katw
011 | 0.33 panw
100 | 0.16 panw
101 | 0.08 panw
110 | 0.05 panw
111 | 0.02 panw 

/*
kai gia transmmiter
*/

module baud_controller_transmitter(
  input clk,
  input reset,
  input [2:0] Tx_baud_select,
  output reg Tx_sample_ENABLE
);

reg [17:0] counter;

always @(posedge clk)  begin
  if (reset == 1) begin
    if (counter == 18'b1111111111_1111) begin
	   counter = 0;
	 end
	 else begin
      case(Tx_baud_select)
	     3'b000: begin//suxnotita 4,8KHz
		    if (counter == 18'b101000101100001011) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      Tx_sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		  end
	     3'b001: begin//suxnotita 19,2KHz
		    if (counter == 18'b00101000101100) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      Tx_sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		    //baud_rate = 1200;
		  end
		  3'b010: begin//syxnotita 76,8KHz
		    if (counter == 18'b000010100010110001) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      Tx_sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		    //baud_rate = 4800;
		  end
	     3'b011: begin//syxnotita 153,6KHz
		    if (counter == 18'b000001010001011000) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      Tx_sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		    //baud_rate = 9600;
		  end
		  3'b100: begin//syxnotita 307,2KHz
		    if (counter == 18'b000000101000101100) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      counter = counter + 1;
		      Tx_sample_ENABLE = 0;
		    end
		    //baud_rate = 19200;
		  end
		  3'b101: begin//suxnotita 614,4KHz
		    if (counter == 18'b000000010100010110) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      counter = counter + 1;
		      Tx_sample_ENABLE = 0;
		    end
		  
		    //baud_rate = 38400;
		  end
		  3'b110: begin//syxnotita 921,6 KHz
		    if (counter == 18'b000000001101100100) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      counter = counter + 1;
		      Tx_sample_ENABLE = 0;
		    end
		    //baud_rate = 57600;
		  
		  end
		
		
		  3'b111: begin//syxnotita 1,843MHz
		    if (counter == 18'b 000000000110110010) begin
		      counter = 0;
			   Tx_sample_ENABLE = 1;
		    end
		    else begin
		      counter = counter + 1;
		      Tx_sample_ENABLE = 0;
		    end
		    //baud_rate = 115200;
		  end
	   endcase	
    end
  end	 
  else begin
    counter = 0;
	 Tx_sample_ENABLE = 0;
  end  
end

endmodule

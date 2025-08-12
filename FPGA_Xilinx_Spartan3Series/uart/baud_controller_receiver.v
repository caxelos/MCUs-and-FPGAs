`timescale 1ns / 1ps
/*
T_sc = 1/(16 x Baud Rate)
ara
f_sc = 16 x Baud Rate
*********************
RECEIVER BAUD_CONTROL
**********************
COUNTERS(tou receiver)
000 | 50,000,000/4,800 = 10416.66(counter = 10100010110001)
001 | 50,000,000/19,200 = 2604.16(counter = 00101000101100)
010 | 50,000,000/76,800 = 651.04(counter =  00001010001011)
011 | 50,000,000/153,600 = 325.520(Counter =00000101000110)
100 | 50,000,000/307,200 = 162.76(counter = 00000010100011)
101 | 50,000,000/614,400 = 81.38(counter =  00000001010001)
110 | 50,000,000/921,600 = 54.2(counter =   00000000110110)
111 | 50,000,000/1,843,200 = 27.1(counter = 00000000011011)
Epomenws theloume metriti twn 14 bit(2^14=16384)

SFALMATA(ston poio kontino akeraio, diaforaPragmatikisTimisApoTousCounters)
000 | 0.36 panw apto kanoniko 
001 | 0.16 katw 
010 | 0.04 katw
011 | 0.48 panw
100 | 0.24 panw
101 | 0.38 katw
110 | 0.2 katw
111 | 0.1 panw 
*/





module baud_controller_receiver(
  input clk,
  input reset,
  input [2:0] Rx_baud_select,
  output reg sample_ENABLE
);

reg [13:0] counter;

/*
 -one-hot counter for number of sample. I use 3 sampling  bits,
 the 7th, 8th, 9th and when i reach the 9th bit i set enable sampling.
 -7th, 8th are saved in module the module(ReceiversSystemData)
*/
parameter SAMPLING_BIT = 4'b1000;
reg [15:0] numOfSample;


always @(posedge clk)  begin
  if (reset == 1) begin	 
	 if (numOfSample == 16'b0000_0000_0000_0000) begin//16samplesPerBit done 		
	     numOfSample = 16'b0000_0000_0000_0001;
	 end
    else begin    		
		
		case(Rx_baud_select)
	     3'b000: begin//suxnotita 4,8KHz
		    if (counter == 14'b1010001011) begin//max value
				numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		  end
	     3'b001: begin//suxnotita 19,2KHz
		    if (counter == 14'b00101000101100) begin
			   numOfSample = numOfSample << 1;
		      counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		    //baud_rate = 1200;
		  end
		  3'b010: begin//syxnotita 76,8KHz
		    if (counter == 14'b00001010001011) begin
		      numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		    //baud_rate = 4800;
		  end
	     3'b011: begin//syxnotita 153,6KHz
		    if (counter == 14'b00000101000110) begin
		      numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      sample_ENABLE = 0;
			   counter = counter + 1;
		    end
		    //baud_rate = 9600;
		  end
		  3'b100: begin//syxnotita 307,2KHz
		    if (counter == 14'b00000010100011) begin
		      numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      counter = counter + 1;
		      sample_ENABLE = 0;
		    end
		    //baud_rate = 19200;
		  end
		  3'b101: begin//suxnotita 614,4KHz
		    if (counter == 14'b00000001010001) begin
		      numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      counter = counter + 1;
		      sample_ENABLE = 0;
		    end
		  
		    //baud_rate = 38400;
		  end
		  3'b110: begin//syxnotita 921,6 KHz
		    if (counter == 14'b00000000110110) begin
		      numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      counter = counter + 1;
		      sample_ENABLE = 0;
		    end
		    //baud_rate = 57600;
		  
		  end
		
		
		  3'b111: begin//syxnotita 1,843MHz
		    if (counter == 14'b00000000011011) begin
		      numOfSample = numOfSample << 1;
				counter = 0;
			   sample_ENABLE = numOfSample[SAMPLING_BIT];
		    end
		    else begin
		      counter = counter + 1;
		      sample_ENABLE = 0;
		    end
		    //baud_rate = 115200;
		  end
	   endcase		
    end 
  end	 
  else begin//reset = 0
    numOfSample = 16'b0000_0000_0000_0001;
    counter = 0;
	 sample_ENABLE = 0;
  end  
end

endmodule

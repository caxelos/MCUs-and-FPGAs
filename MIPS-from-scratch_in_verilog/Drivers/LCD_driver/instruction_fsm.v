`timescale 1ns / 1ps

module instruction_fsm(
	input clk,
	input reset,
   output reg [11:8] SF_D,
   output reg LCD_RS, LCD_EN,
	output wire LCD_RW,
	output [7:0] data
);

wire CONFIG_EN;
wire [11:0] wcounter;
assign LCD_RW = 0;

init init_instance(
  //give
  .clk(clk),
  .reset(reset),
  //get
  .CONFIG_EN(CONFIG_EN),
  .data(data),
  .wcounter(wcounter)
);

always @(posedge clk) begin
  if (reset == 1) begin
	 LCD_EN = 0;
	 LCD_RS = 0;
	 SF_D = data[7:4];
  end
  else  begin
	 if (CONFIG_EN)
	   LCD_RS = 1;
	 else
	   LCD_RS = 0;
		
    case (wcounter)
	   12'd1999: begin		 
		  SF_D = data[7:4];//4 MSB
		end
		
		/*data1*/
		12'd2001: begin
		  LCD_EN = 1;
		end
		/*
		LCD enabled for 12 cycles
		*/
      12'd2013: begin
		   LCD_EN = 0;
		end
      /*pause and then 
		data2
		*/
		12'd2063: begin
		  SF_D = data[3:0];//4 LSB
		end
      12'd2066: begin
		  LCD_EN = 1;
		end
	   12'd2078: begin
		  LCD_EN = 0;
      end
		/*
		wait 40microSecs
		*/		  
    endcase	 
  end
end  
	 
endmodule




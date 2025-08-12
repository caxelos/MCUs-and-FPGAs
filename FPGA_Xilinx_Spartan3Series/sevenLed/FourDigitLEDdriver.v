`timescale 1ns/1ps

module FourDigitLEDdriver(clk, reset, an3, an2, an1, an0, a, b, c, d, e, f, g, dp, CLKIN_IBUFG_OUT);

input clk, reset;
output an3, an2, an1, an0;
output a, b, c, d, e, f, g, dp;
output CLKIN_IBUFG_OUT;

wire clkD16;//, clk50;
wire [3:0] char;

wire CLKDV_BUF;
wire CLKFB_IN;
wire CLKIN_IBUFG;
wire CLK0_BUF;


/*
dcm_25 instance_name (
    .CLKIN_IN(clk), //50MHz input
    .RST_IN(reset), 
    .CLKDV_OUT(clkD16),	
    //.CLKIN_IBUFG_OUT(),//open
    //.CLKIN_IBUFG_OUT(),//open
    //.CLKIN_IBUFG_OUT(),//open
	 //.CLK0_OUT(clk50) 
    .LOCKED_OUT()
);
*/

 IBUFG  CLKIN_IBUFG_INST (.I(clk), .O(CLKIN_IBUFG) );
 BUFG  CLKDV_BUFG_INST (.I(CLKDV_BUF), .O(clkD16) ); 
 BUFG  CLK0_BUFG_INST (.I(CLK0_BUF), .O(CLKFB_IN) );
 
 assign CLKIN_IBUFG_OUT = CLKIN_IBUFG;
 //assign CLK0_OUT = CLKFB_IN;
 
  DCM #( 
		.CLK_FEEDBACK("1X"), 
		.CLKDV_DIVIDE(16.0),
		.DLL_FREQUENCY_MODE("LOW")
  )
  DCM_INST (
		.CLKFB(CLKFB_IN),
		.CLKIN(CLKIN_IBUFG),
		.RST(reset),	
      .CLKDV(CLKDV_BUF),
		.CLK0(CLK0_BUF)
  );
	

//clockDiv16 clockDiv16_instance( clk, reset, clkD16, clk50);//diairesi rologiou apo 50MHz se 3.2MHz(320ns)



textGenerator textGenerator_instance(clkD16, reset, char);//char is the output



LEDdecoder LEDdecoder_instance(clkD16, reset, char, a, b, c, d, e, f, g, dp);//a-dp are outputs
//LEDdecoder_meLEDS  LEDdecoder_meLEDS_instance(clk50, reset, char, a ,b ,c ,d ,e ,f ,g ,dp);//a-dp are outputs
// poio apta 4 segment anavei
counter4bit counter4bit0(clkD16, reset, an3, an2, an1, an0);


endmodule
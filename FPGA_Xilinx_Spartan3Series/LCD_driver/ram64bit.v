`timescale 1ns / 1ps

// RAM16X1S_1  : In order to incorporate this function into the design,
//   Verilog   : the forllowing instance declaration needs to be placed
//  instance   : in the body of the design code.  The instance name
// declaration : (RAM16X1S_1_inst) and/or the port declarations within the
//    code     : parenthesis may be changed to properly reference and
//             : connect this function to the design.  All inputs
//             : and outputs must be connected.

//  <-----Cut code below this line---->

   // RAM16X1S_1: 16 x 1 negedge write distributed (LUT) RAM
   //             Spartan-3E
   // Xilinx HDL Language Template, version 14.7

module ram64bit(
  input WCLK, 
  input A0, 
  input A1, 
  input A2, 
  input A3, 
  input A4, 
  input A5,
  input D,
  input WE,
  
  output O
);


   // RAM64X1S: 64 x 1 positive edge write, asynchronous read single-port distributed RAM
   //           Spartan-3E
   // Xilinx HDL Language Template, version 14.7

   RAM64X1S #(
		.INIT(64'hF_E_D_C_B_A_9_8_7_6_5_4_3_2_1_0) // Initial contents of RAM
   ) RAM64X1S_inst (
      .O(O),        // 1-bit data output
      .A0(A0),      // Address[0] input bit
      .A1(A1),      // Address[1] input bit
      .A2(A2),      // Address[2] input bit
      .A3(A3),      // Address[3] input bit
      .A4(A4),      // Address[4] input bit
      .A5(A5),      // Address[5] input bit
      .D(D),        // 1-bit data input
      .WCLK(WCLK),  // Write clock input
      .WE(WE)       // Write enable input
   );

   // End of RAM64X1S_inst instantiation
endmodule							
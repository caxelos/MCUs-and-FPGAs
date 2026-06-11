// Define top-level testbench
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Top level has no inputs or outputs
// It only needs to instantiate CPU, Drive the inputs to CPU (clock, reset)
// and monitor the outputs. This is what all testbenches do

`include "constants.h"
`timescale 1ns/1ps

module cpu_tb;
integer   i;
reg       clock, reset;    // Clock and reset signals

// Instantiate CPU
cpu cpu0(clock, reset);

// Initialization and signal generation
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

initial  
  begin 
   clock = 1'b0;       
   reset = 1'b0;  // Apply reset for a few cycles
   #10 reset = 1'b1;
  end
     
always 
   #10 clock = ~clock;  // Clock generation 


initial begin  // The statements from this begin up to the "end" are executed sequentially.

  // Initialize Register File with "random" numbers
  for (i = 0; i < 32; i = i+1)
    cpu0.cpu_regs.data[i] = i;   // Note that R0 = 0 in MIPS 

  // Initialize Data Memory 
  $readmemh("program_hex.txt", cpu0.cpu_IMem.data);

// Here, "memory.hex" is a file that must be located in the
// directory from which you run Verilog, and it must have the following format:
  
  // @0    00000000
  // @4    20100009
  // @8    00000000
  // @C    00000000
  // ...
  
// The left column, after the @, contains the memory address (in hex),
// and the right column contains the data stored at that address
// (also in hex).
// In the example above, the words at addresses 0, 8, and 12
// are 0, and at address 4 there is the value 32'h20100009.
// If the PC reads address 4, the value there contains the instruction:
//
//   addi $16 <- $0 + 9

// The second argument of $readmemh specifies exactly where the memory
// to be initialized is located.
// In the example, "dat0" is the name we gave to the datapath instance.
// "mem" is the name of the memory instance INSIDE the datapath,
// and "data" is the name of the actual memory array inside its module.
// If you have used different names, modify the $readmemh call accordingly.

// Alternatively, if it is more convenient, there is the $readmemb command
// with exactly the same syntax.
// The left column of the file has the same format (addresses in hex),
// but the right column contains the data in binary.
// Thus, the add instruction could be written as:

  // @4    00100000000100000000000000001001

  // ...or even better:
  
  // @4    001000_00000_10000_0000000000001001

  // (h Verilog epitrepei diaxwristika underscores).


  // Termatismos ekteleshs:
  // $finish;

end 

endmodule

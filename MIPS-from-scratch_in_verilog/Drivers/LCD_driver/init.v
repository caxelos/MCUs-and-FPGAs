/*
1o always block: o wcounter tou instruction_fsm
2o always block: i diadikasia init,configure kai apostoli dedomenwn
sto telos tou arxeio vrisketai i mnimi
*/


`timescale 1ns / 1ps

module init(
  input clk,
  input reset,
  output reg CONFIG_EN,
  output reg [11:0] wcounter,
  //output reg WRITE_EN,
  output wire [7:0] data
);
wire memOut;

reg [25:0] gcounter;
reg [3:0] threeToZero;
reg fifth, sixth, seventh;
reg WRITE_EN, DATA_SEND;
reg STOPPRINT;//sima pou stamataei tin apostoli epomenwn dedomenwn

reg [3:0] mem_data;
reg [1:0] indexCounter;

/*
- edw spaw to data[7:0] se { seventh, sixth, fifth, 0, threeToZero } epeidi 
mono ta bits 7, 6, 5, [3:0] allazoun kai na apofigw ta munhmata latches
*/ 
assign data[7] = (CONFIG_EN)?0:seventh;
assign data[6] = (CONFIG_EN)?1:sixth;
assign data[5] = (CONFIG_EN)?0:fifth;
assign data[4] = 0;//assign data[4] = (CONFIG_EN)?0:0;


/*
- epeidi ta grammata A mexri O stin mnimi tis plaketas exoun idia timi
gia ta upper bits "0100", apothikevw mono ta lower bits stin mnimi, ara
thelw 4 bits ana gramma 
*/
assign data[0] = (CONFIG_EN)?mem_data[0]:threeToZero[0];
assign data[1] = (CONFIG_EN)?mem_data[1]:threeToZero[1];
assign data[2] = (CONFIG_EN)?mem_data[2]:threeToZero[2];
assign data[3] = (CONFIG_EN)?mem_data[3]:threeToZero[3];

/*
- wait until command is send..wcounter: waitCounter
- o counter aftos trexei oso diarkei i epeksergasia dedomenwn
apo to instruction_fsm module. Sto diastima afto o gcounter exei
statheri timi
- Ksekina na afksanetai afou ginei to WRITE_EN = 1
*/
always @(posedge clk) begin
  if (reset) begin
    wcounter = 0;
	 DATA_SEND = 1;
	 
  end
  else if (WRITE_EN) begin
    case (wcounter)
      13'd4079: begin
	     wcounter = 0;
		  DATA_SEND = 1;
	   end
	   default: begin
		  DATA_SEND = 0;
		  wcounter = wcounter + 1;
      end
    endcase
  end	  
end

/*
To kurio fsm tou module
- Stelnei tis entoles sto instruction_fsm kai molis oloklirwthei to configuration (CONFIG_EN = 1) arxizei
- Oso to CONFIG_EN einai miden, i mnimi den metatopizetai 
- o gcounter einai o metritis tou module(21bits) 
- ta 6 LSB bits tou gcounter ta xrisimopoiw ws diefthinsi tis mnimis
- xrisimopoiw arketa states, dioti to kommati tis arxikopoihshs to exw sumperilavei sto hardware
kai oxi ws entoles se mia mnimi(tha mporouse na tan poly oikonomikotero)
*/
always @(posedge clk or posedge reset) begin
  if (reset == 1) begin
    gcounter = 0;//global counter
	 indexCounter = 0;//for memory load
	 
	 fifth = 0;//reg for command data-transfer
	 sixth = 0;//
	 seventh = 0;
	 threeToZero = 0;//the same
	 CONFIG_EN = 0;//enable LCD initial configuratin
	 WRITE_EN = 0;//send 8-bit data to lcd or not
	 
	 mem_data = 0;
	 STOPPRINT = 1;
  end
  else if (DATA_SEND) begin //an dn egine setup kai dn trexei kapoia alli diadikasia sto instruction
	 
	 if ((~CONFIG_EN)) begin	 
	 /*
	 Initialization and configuration part fsm
	 */
    case (gcounter[20:0])
	   /*
		wait 15ms
		*/
      21'd749999: begin//0x03
		  WRITE_EN = 1;
        threeToZero = 4'h3;
		  gcounter = gcounter + 1;
		end
		/*
		wait 4.1ms
		*/
		21'd955000: begin//0x03 //20'd 909999
		  WRITE_EN = 1;
        gcounter = gcounter + 1;		  
		end
		/*
		wait 100microSecs
		*/
		21'd960001: begin//0x03..//20'd914999
		  WRITE_EN = 1;
        gcounter = gcounter + 1;		  
		end
		/*
		wait 40microSecs
		*/
		21'd962002: begin//0x02 // 20'd 961999 //21'd1121999
		  WRITE_EN = 1;
		  fifth = 0;
		  sixth = 0;
        threeToZero = 4'h2;
        gcounter = gcounter + 1;		  
		end
		/*
		wait 40microSecs
		start modulation
		*/
		21'd964003: begin//0x28...0010_1000
		  WRITE_EN = 1;
		  fifth = 1;
		  threeToZero = 4'h8;
		  gcounter = gcounter + 1;
		end
		21'd964004 : begin//0x06 //20'd919000
		  WRITE_EN = 1;
		  fifth = 0;
		  threeToZero = 4'h6;
		  gcounter = gcounter + 1;
		end
		21'd964005 : begin//0x0C 
		  WRITE_EN = 1;
		  fifth = 0;
		  threeToZero = 4'hC;
		  gcounter = gcounter + 1;
		end
		21'd964006: begin//Clear-0x01
		  WRITE_EN = 1;
		  fifth = 0;
		  threeToZero = 4'h1;
		  gcounter = gcounter + 1;
		end
		//1.64ms
		21'd1046007: begin//0x80: assign dram address = zero
		  WRITE_EN = 1;
		  fifth = 0;
		  sixth = 0;
		  seventh = 1;
		  threeToZero = 0;
		  gcounter = gcounter + 1;
      end
		21'd1046008: begin//start writing data to LCD    //20'd1001003
		  CONFIG_EN = 1;
		  gcounter = 0;
		  gcounter = 4;
      end
       		
		
		default: begin
		  if (DATA_SEND) 
		    WRITE_EN = 0;
		  else
          WRITE_EN = 1;		  
		  
		  
		  gcounter = gcounter + 1;
		end
    endcase
  end
  else  begin //config_en=1
    if (gcounter == 65)  // pagwse to kuklwma edw(epeidi den ilopoihthike i leitourgia tou refresh)   
      STOPPRINT = 1;	 
    else begin
	   if ( (gcounter[1:0] ==2'b00) & (~STOPPRINT) ) begin// exei simplirwthei i omada bit gia apostoli 
        WRITE_EN = 1;
		  indexCounter = 0;
		  STOPPRINT = 1;
      end
      else begin// edw travame dedomena apti mnimi 
		  WRITE_EN = 0;
		  STOPPRINT = 0;
		  
		  mem_data[indexCounter] = memOut;
        gcounter = gcounter + 1;	
		  indexCounter = indexCounter + 1; 
		end
    end   	  
  end
end
end  
 
/*
- edw kalw tin distributed mnimi 64bits
- oso to CONFIG_EN einai miden, den allazei i diefthinsi
pou diavazei
- gia tin epilogi tis diefthinsis, xrisimopoiw ta 6 LSB tis mnimis
*/
ram64bit ram64bit_inst(
  .WCLK(clk), // Write clock input
  
  .A0(gcounter[0] & CONFIG_EN),     // RAM address[0] input
  .A1(gcounter[1] & CONFIG_EN),     // RAM address[1] input
  .A2(gcounter[2] & CONFIG_EN),     // RAM address[2] input
  .A3(gcounter[3] & CONFIG_EN),     // RAM address[3] input
  .A4(gcounter[4] & CONFIG_EN),     // RAM address[4] input
  .A5(gcounter[5] & CONFIG_EN),     // RAM address[5] input
  .D(1'b0),       // RAM data input
  .WE(1'b0),      // Write enable input 

  .O(memOut)       // RAM output  
);	 
   
endmodule


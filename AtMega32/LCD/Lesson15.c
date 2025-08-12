//CHAPTER 15 - seven segments display
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define scanPORT PORTB //P'
#define scanDDR DDRB
#define segPORT PORTA //P
#define segDDR DDRA

//PORTC cannot be used with JTAG
volatile uint8_t fontBuffer[4]={255,255,255,255};

//Font table
const uint8_t fonts[16]={
0b01010000,0b01011111,0b00110010,0b00010110,
0b00011101,0b10010100,0b10010000,0b01011110,
0b00010000,0b00010100,0b00011000,0b10010001,
0b11110000,0b00010011,0b10110000,0b10111000
};

void dispHex1(uint8_t);
void dispHex2(uint16_t);
void dispDec1(uint8_t);
void dispDec2(uint16_t); //max 9999 (6)5535

//===============================================
int main() {
	//init Ports
	scanDDR |= 0b11110000; //4 high bits->outputs
	segDDR = 0xFF;
	//init T0 for interrupt every 4096 us
	TIMSK |= (1<<TOIE0); //overflow interrupt
	TCCR0 |= (1<<CS02); //P = 256, normal mode
	sei();
	while (1) {
		//Display demo (check display functions)
		fontBuffer[0] = 0xF8; //G
		fontBuffer[1] = 0xB0; //?
		fontBuffer[2] = 0x5F; //?
		fontBuffer[3] = 0x18; //?
		_delay_ms(2000);
		dispDec1(123);
		_delay_ms(2000);
		dispDec2(4567);
		_delay_ms(2000);
		dispHex1(0x89);
		_delay_ms(2000);
		dispHex1(0xAB);
		_delay_ms(2000);
		dispHex2(0xCDEF);
		_delay_ms(2000);
		dispDec2(65530);
		_delay_ms(2000);
	}
}

//===============================================
ISR(TIMER0_OVF_vect) {
	static uint8_t scanCol = 0b11101111;
	static uint8_t fontPointer = 0;
	scanPORT = 0xFF; //all disp off
	segPORT = fontBuffer[fontPointer++];
	scanPORT = scanCol;
	scanCol = 2*scanCol+1;
	if (scanCol == 255) { //end of scan?
		scanCol = 0b11101111;
		fontPointer = 0;
	}
}

//============== DISPLAY ROUTINES ===============
void dispHex1(uint8_t num) {
	fontBuffer[0]=255;
	fontBuffer[1]=255;
	uint8_t highNibble = num/16;
	fontBuffer[2]=fonts[highNibble];
	uint8_t lowNibble = num & 0x0F;
	fontBuffer[3]=fonts[lowNibble];
}

//===============================================
void dispHex2(uint16_t num) {
	uint8_t highByte = num/256;
	uint8_t highNibble = highByte/16;
	fontBuffer[0]=fonts[highNibble];
	uint8_t lowNibble = highByte & 0x0F;
	fontBuffer[1]=fonts[lowNibble];
	uint8_t lowByte = num - 256*highByte;
	highNibble = lowByte/16;
	fontBuffer[2]=fonts[highNibble];
	lowNibble = lowByte & 0x0F;
	fontBuffer[3]=fonts[lowNibble];
}

//===============================================
void dispDec1(uint8_t num) {
	uint8_t monades = num % 10;
	num = (num - monades) / 10;
	uint8_t dekades = num % 10;

	num = (num - dekades) / 10;
	fontBuffer[0] = 255;
	fontBuffer[1] = fonts[num];
	fontBuffer[2] = fonts[dekades];
	fontBuffer[3] = fonts[monades];
}

//===============================================
void dispDec2(uint16_t num) { //num = 65432
	uint8_t monades = num % 10; //monad = (2)
	num = (num - monades) / 10; //num = 6543
	uint8_t dekades = num % 10; //dekad = (3)
	num = (num - dekades) / 10; //num = 654
	uint8_t ekatontades = num % 10; //ekato = (4)
	num = (num - ekatontades) / 10; //num = 65
	uint8_t xiliades = num % 10; //xilia = (5)
	//num = (num - xiliades) / 10; //num=6,no room for this
	fontBuffer[0] = fonts[xiliades];
	fontBuffer[1] = fonts[ekatontades];
	fontBuffer[2] = fonts[dekades];
	fontBuffer[3] = fonts[monades];
}

//CHAPTER 16 - HEX kbd (and part of 7seg display)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Select port where hex kbd is connected (PORTB recommended)
#define scanPORT PORTB
#define scanDDR DDRB //whole port
#define scanPIN PINB

#include "LCDinclude.h"

//Wiring info
//-----------
//PB7->220O->pin1, PB6->220O->pin2, PB5->220O->pin3,
//PB4->220O->pin4, PB3->pin5, PB2->pin6
//PB1->pin7, PB0->kbd pin8

#define segPORT PORTA

#define segDDR DDRA

volatile uint8_t fontBuffer[4]={255,255,255,255};

//Font table

const uint8_t fonts[16]={
0b01010000,0b01011111,0b00110010,0b00010110,
0b00011101,0b10010100,0b10010000,0b01011110,
0b00010000,0b00010100,0b00011000,0b10010001,
0b11110000,0b00010011,0b10110000,0b10111000};

//ISR return variables
volatile uint8_t keyCurr=0xFF; //Code of currently pressed key
volatile uint8_t keyPrev=0xFF; //Value of last pressed key
void KBDsetup();

const uint8_t keyValues[16]={
13,12,11,10,
15, 9, 6, 3, //<#> = 15
 0, 8, 5, 2,
14, 7, 4, 1}; //<*> = 14

void init7segPorts();
void init7segVars(); 
void dispHex1 (uint8_t);
void dispDec1(uint8_t);
void dispDec2(uint16_t); //max 9999 (6)5535
uint8_t inputDec1();
uint16_t inputDec2();
void initKbdPorts();
void initKbdVars();
void clearDisp();
uint8_t inputHex1();



void KBDsetup() {

	#if defined(debugPin)
		debugDDR |= (1<<debugPin); //Output for oscilloscope
	#endif

	scanDDR |= 0b11110000; //4 high bits->outputs, 4 low inputs
	// scanDDR = DDRB
	scanPORT |= 0b11111111; // with pullup

	//init T0 for interrupt every 1ms
	//1000?s=16000c= (P=64)x(OCR0+1=250)??;

	TIFR |= (1<<OCF0);

	TIMSK |= (1<<OCIE0);

	TCCR0 = (1<<WGM01)|(1<<CS01)|(1<<CS00); //P = 64, CTC mode

	OCR0 = 250-1;
	sei();
	
}





//DISPLAY ROUTINES
void dispHex1 (uint8_t number) {
	fontBuffer[0]=255;
	fontBuffer[1]=255;
	fontBuffer[2]=fonts[(number>>4) & 0x0F];
	fontBuffer[3]=fonts[number & 0x0F];
}

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


void dispDec2(uint16_t num) { //num = 65432
	uint8_t monades = num % 10; //monad = (2)
	num = (num - monades) / 10; //num = 6543
	uint8_t dekades = num % 10; //dekad = (3)
	num = (num - dekades) / 10; //num = 654
	uint8_t ekatontades = num % 10; //ekato = (4)
	num = (num - ekatontades) / 10; //num = 65
	uint8_t xiliades = num % 10; //xilia = (5)
	//num = (num - xiliades) / 10; //num=6,no display for this
	fontBuffer[0] = fonts[xiliades];
	fontBuffer[1] = fonts[ekatontades];
	fontBuffer[2] = fonts[dekades];
	fontBuffer[3] = fonts[monades];
}

void clearDisp() {
	fontBuffer[0]=255;
	fontBuffer[1]=255;
	fontBuffer[2]=255;
	fontBuffer[3]=255;
}

uint8_t inputHex1() {
	keyPrev = 0xFF;
	dispHex1(0);
	while (keyPrev == 0xFF); //wait for a key press
	uint8_t result = keyPrev*16;
	dispHex1(result);
	keyPrev = 0xFF;
	while (keyPrev == 0xFF);
	result += keyPrev;
	keyPrev = 0xFF;
	dispHex1(result);
	return result;
}

uint8_t inputDec1() {
	keyPrev = 0xFF;
	uint8_t result = 0;
	dispDec1(result);
	for (int i=0; i<3; i++) {
		while (keyPrev == 0xFF); //wait for a key press
		if (keyPrev > 9) 
			return result;
		result = result*10+keyPrev;
		dispDec1(result);
		keyPrev = 0xFF;
	}

	return result;
}



uint16_t inputDec2() {
	keyPrev = 0xFF;
	uint16_t result = 0;
	dispDec2(result);
	uint8_t i = 0;
	while (i<4) {
		while (keyPrev == 0xFF); //wait for a key press
		if (keyPrev == 13) {
			return result;
		} else if (keyPrev == 10) {
			result = result / 10;
			dispDec2(result);
			if (i>0) 
				i--;
		} else if (keyPrev <= 9) {
			result = result*10+keyPrev;
			dispDec2(result);
			i++;
		}
		keyPrev = 0xFF;
	}

	return result;
}


/*
ISR(TIMER1_COMPA_vect) {
	//OCR1A = 80;
	//OCR1B = 20;
	static uint8_t scanCol = 0b11101111;
	static uint8_t pPressed = 0;
	static uint8_t fontPointer = 0;

	//7 seg disp
	scanPORT = 0xFF; //all disp off
	segPORT = fontBuffer[fontPointer++];
	scanPORT = scanCol;
	asm("NOP");
	//hex kbd

	static uint8_t col;
	static uint8_t pressed = 0;
	uint8_t scanLine = scanPIN;
	uint8_t line = 0;
	while (line < 4) {
		if ((scanLine % 2) == 0) {
			keyCurr = line + col;
			pressed = 1;
			break;
		}
		scanLine = scanLine / 2;
		line++;
	}
	col = col + 4;
	scanCol = 2*scanCol+1;
	if (scanCol == 255) {
		scanCol = 0b11101111;
		fontPointer = 0;
	
		//KbdNewScan
		//----------
		// if (pressed != 1) {
		//		keyCurr = 0xFF;
		//}
		//col = 0;
		//pressed = 0;
		//
		col = 0;
		if (pressed != 1) {
			pPressed = 0;
		} else {
			if (pPressed != 1) {
				pPressed = 1;
				keyPrev = keyValues[keyCurr];
			}
		}
		if (pressed == 1) {
			pressed = 0;
		} else {
			keyCurr = 0xFF;
		}
	}
}



ISR(TIMER1_COMPB_vect) {
	static uint8_t scanCol = 0b11101111;
	static uint8_t pPressed = 0;
	static uint8_t fontPointer = 0;

	//7 seg disp
	scanPORT = 0xFF; //all disp off
	segPORT = fontBuffer[fontPointer++];
	scanPORT = scanCol;
	asm("NOP");
	//hex kbd

	static uint8_t col;
	static uint8_t pressed = 0;
	uint8_t scanLine = scanPIN;
	uint8_t line = 0;
	while (line < 4) {
		if ((scanLine % 2) == 0) {
			keyCurr = line + col;
			pressed = 1;
			break;
		}
		scanLine = scanLine / 2;
		line++;
	}
	col = col + 4;
	scanCol = 2*scanCol+1;
	if (scanCol == 255) {
		scanCol = 0b11101111;
		fontPointer = 0;
	
		//KbdNewScan
		//----------
		// if (pressed != 1) {
		//		keyCurr = 0xFF;
		//}
		//col = 0;
		//pressed = 0;
		//
		col = 0;
		if (pressed != 1) {
			pPressed = 0;
		} else {
			if (pPressed != 1) {
				pPressed = 1;
				keyPrev = keyValues[keyCurr];
			}
		}
		if (pressed == 1) {
			pressed = 0;
		} else {
			keyCurr = 0xFF;
		}
	}

	//scanPORT = scanCol;
} 
*/



ISR(TIMER0_COMP_vect) {
	static uint8_t scanCol = 0b11101111;
	static uint8_t pPressed = 0;
	static uint8_t fontPointer = 0;

	//7 seg disp
	scanPORT = 0xFF; //all disp off
	segPORT = fontBuffer[fontPointer++];
	scanPORT = scanCol;
	asm("NOP");
	//hex kbd

	static uint8_t col;
	static uint8_t pressed = 0;
	uint8_t scanLine = scanPIN; // PINB


	uint8_t line = 0;
	while (line < 4) {
		if ((scanLine % 2) == 0) {
			keyCurr = line + col;
			pressed = 1;
			break;
		}
		scanLine = scanLine / 2;
		line++;
	}
	col = col + 4;
	scanCol = 2*scanCol+1;
	if (scanCol == 255) {
		scanCol = 0b11101111;
		fontPointer = 0;
	
		//KbdNewScan
		//----------
		// if (pressed != 1) {
		//		keyCurr = 0xFF;
		//}
		//col = 0;
		//pressed = 0;
		//
		col = 0;
		if (pressed != 1) {
			pPressed = 0;
		} else {
			if (pPressed != 1) {
				pPressed = 1;
				keyPrev = keyValues[keyCurr];
			}
		}
		if (pressed == 1) {
			pressed = 0;
		} else {
			keyCurr = 0xFF;
		}
	}

	//scanPORT = scanCol;
}


//**********************************************



/*
int main() {
	KBDsetup();
	segDDR = 0xFF;
	while (1) {
		clearDisp();
		fontBuffer[0]=fonts[12]; //'C'
		fontBuffer[1]=fonts[0]; //'O'
		fontBuffer[2]=fonts[13]; //'d'
		fontBuffer[3]=fonts[14]; //'E'
		_delay_ms(1000);
		dispDec1(keyCurr);
		_delay_ms(1000);
		fontBuffer[0]=0b01010001; //'V'
		fontBuffer[1]=fonts[10]; //'A'
		fontBuffer[2]=0b11110001; //'L'
		fontBuffer[3]=255;//' '
		_delay_ms(1000);
		dispDec1(keyVal);
		_delay_ms(1000);
	}
}
*/

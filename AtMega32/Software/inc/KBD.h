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

extern volatile uint8_t fontBuffer[4];

//Font table

extern const uint8_t fonts[16];

//ISR return variables
extern volatile uint8_t keyCurr; //Code of currently pressed key
extern volatile uint8_t keyPrev; //Value of last pressed key
extern const uint8_t keyValues[16];

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

void KBDsetup(); 


//Interrupts, uncommenting is obsolete
//ISR(TIMER1_COMPA_vect);
//ISR(TIMER0_COMP_vect);

//DISPLAY ROUTINES
void dispHex1 (uint8_t number);

void dispDec1(uint8_t num);


void dispDec2(uint16_t num);

void clearDisp();

uint8_t inputHex1();

uint8_t inputDec1();

uint16_t inputDec2();




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
		dispDec1(keyCode);
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

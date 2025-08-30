//chapter 20 - experiment 1
//aplo programma hxous (echo)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <limits.h>
//USART functions
void Utx(uint8_t);
uint8_t Urx();
uint8_t UrxE(uint8_t *);
void Usetup();
void UsendCT(char*);
void UreceiveCT(char*);
//*********************************************

#include "LCDinclude.h"
#include "KBDinclude.h" 


/*
 * Peirama 1o
*/ 
int main() { 
	Usetup();
	while (1) {
		uint8_t receivedChar = Urx();
		Utx(receivedChar);
	}
}
 


/*
 * Peirama 2o
 
void ADCsetup() {
	DDRA &= ~(1<<2); //Analog 2 = input
	PORTA &= ~(1<<2); //no pullup
	ADMUX = 0b01100010; //?p????? a?a??? ?a?a???? 2,a??st.
	//st????s? ?a? es?te???? Vref=5V
	ADCSRA = 0b11101101; //Enable, Start, Auto Trigger, IRQ
	//prescaler,?e???a t?? 畫tat??p??
	SFIOR = (1<<ADTS1)|(1<<ADTS0); //s?a?d: ?0 s?????s?
}

int main() { 
	LCDsetup4();
	KBDsetup();
	Usetup();
	ADCsetup();
	sei();
	LCDsendCmd(LCDclear); 
	uint8_t LCDline = 0, LCDcol = 0;
	while (1) {
		LCDcursor(LCDline, LCDcol);
		if (keyPrev != 0xFF) {
			Utx(0x30+keyPrev);
			LCDprintChar(0x30+keyPrev);
			keyPrev = 0xFF;
			LCDcol++;
		}

		uint8_t SerialStatus;
		if ((UCSRA & (1<<RXC)) != 0) {
			uint8_t SerialChar = UrxE(&SerialStatus);
			if (SerialStatus == 0) {
				//LCDprintGrkChar(SerialChar);
			} else {
				LCDprintChar('?');
			}
			Utx(SerialChar);
			LCDcol++;
		}

		if (LCDcol == 16) {
			LCDcol = 0;	
			LCDline = 1 - LCDline;
		}
	}


}
*/



//G
//R
//T

//===========================================================
void Usetup() {
	#define BAUD 57600
	#define BAUD_RATE_CALC ((F_CPU/16/BAUD) - 1)
	#define maxStringLength 20
	DDRD |= (1<<1); //PD1 = TXD, PD0 = RXD
	PORTD &= ~(1<<0); //no pullup in RXD
	UBRRH = (BAUD_RATE_CALC >> 8);
	UBRRL = BAUD_RATE_CALC;
	//1 stop, 8 data-->p??.20.9, even parity
	UCSRB = (1<<RXEN)|(1<<TXEN); 
	UCSRC = (1<<URSEL)|(3<<UCSZ0)|(1<<UPM1);
}

//===========================================================

void Utx(uint8_t TXdata) {
	while ((UCSRA & (1<<UDRE)) == 0) {}; //?e??畫?e ?a te?e??se? 
	 //? p??????畫?? e?p?皖?
	UDR = TXdata;
}

//===========================================================
uint8_t Urx() {
	while ((UCSRA & (1<<RXC)) == 0) {}; //?e??畫?e ???? ?a?a?t.
	uint8_t RXdata = UDR;
	return RXdata;
}

//===========================================================
uint8_t UrxE(uint8_t * RXstatus) {
	while ((UCSRA & (1<<RXC)) == 0) {};
	uint8_t statusM = UCSRA;
	uint8_t RXdata = UDR;
	//Isolate status bits
	*RXstatus = (statusM & ((1<<FE)|(1<<DOR)|(1<<PE)));

	return RXdata;
}

//===========================================================
void UsendCT(char* ct2send) {
	for (uint16_t i=0; i<maxStringLength; i++) {
		char d = ct2send[i];
		if (d == 0) {
			break;
		}
		if ((d == 13) || (d == 10)) {
			Utx(13); Utx(10);
			break;	
		}

		Utx(d);
	}
	return;
}

//===========================================================
void UreceiveCT(char* ctReceived) {
	for (uint16_t i=0; i<maxStringLength; i++) {
		char d = Urx();
		ctReceived[i] = d;
		if ((d==0) || (d==13) || (d==10)) {
			return; //return if end of string
		}
	}
	return; //return if max length exceeded
}

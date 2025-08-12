//Chapter 19 - ADC

////////Experiment 1 - Single ended transfer characteristic //////

// Sindeste stin eisodo enos akrou ADC0 ena potensiometro, epilekste 10-bit metatropi kai
// eswteriki tasi anaforas AVcc. Grapste ena programma pou na emfanizei stin othoni LCD
// to psifiako isodunamo tis analogikis tasis. Parte toulaxiston 10 analogikes isapexouses
// taseis apo 0 mexri AREF kai vreite to psifiako tous isodunamo. Peraste tis times sto Excel
// kai sxediaste ti xaraktiristiki metaforas tou metatropea
//
/*
#include <avr/io.h>
#include <util/delay.h>
#include "LCDinclude.h"

int main() { 
	LCD4init();
	DDRA &= ~(1<<0); //Analog 0 = input
	PORTA &= ~(1<<0); //no pullup
	ADMUX = 0b01000000; //Analog channel 0, right align,
	//internal Vref=5V
	ADCSRA = 0b11100101; //Enable, Start, Auto Trigger Enable,
	//?=16, start conversions
	SFIOR &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0)); //free run

	while(1) {
		int16_t result = ADC; //ADC read
		LCDcursor(0,0); LCDprintCT("               "); LCDcursor(0,0);
		LCDprintCT("ADC val: ");
		LCDprintInt(result);
		_delay_ms(250);
	
	}

}
*/


/////////// Experiment 2 - Differential transfer characteristic //////
// Epivallete ston akrodekti ADC1 kai ston akrodekti AREF tasi 2.5V mesw enos diaireti
// tasis me duo idies mikres statheres antistaseis akriveias 1%. To potensiometro 
// eksakolouthei na einai sundedemeno sto kanali ADC0. Epilekste diaforiki eisodo ADC0-ADC1
// me monadiaia enisxusi kai sxediaste ti xaraktiristiki metaforas. Prosoxi: i allagi sto
// programma na ginei prin apo tis allages sto kuklwma, wste na min epivalloume tasi ston
// akrodekti AREF enw einai epilegmeni i eswteriki tasi anaforas.

//Chapter 19 - ADC
//Experiment 2 - Differential transfer characteristic
/*
#include <avr/io.h>
#include <util/delay.h>
#include "LCDinclude.c"

int main() { 
	LCD4init(); 
	DDRA &= ~((1<<0)|(1<<1)); //Analog 0 & 1 = inputs
	PORTA &= ~((1<<0)|(1<<1)); //no pullups

	ADMUX = 0b00010000; //Differnetial channel 0-1, right
	//align, AREF pin
	ADCSRA = 0b11100100; //Enable, start, auto trigger enable,
	//?=16, start conersions
	SFIOR &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0)); //free run
	while(1) {
		uint16_t result = ADC; //ADC read
		LCDcursor(0,0); LCDprintCT("               "); 
		LCDcursor(0,0); LCDprintCT("ADC val: ");
		LCDprintInt(result);
		_delay_ms(250);
	}
}
*/


/////////// Experiment 3 - Analog echo //////
#include <avr/io.h>
#include <avr/interrupt.h>
int main() { 
	DDRA &= ~(1<<2); //Analog 2 = input
	PORTA &= ~(1<<2); //no pullup
	DDRD = 0xFF; //Outputs to R-2R ladder
	//ADC init
	ADMUX = 0b01100010; //Analog channel 2,left align
	//internal Vref=5V
	ADCSRA = 0b11101101; //Enable, start, auto trigger, IRQ
	//prescaler, start conversions
	SFIOR = (1<<ADTS1)|(1<<ADTS0); //Trigger from ?0 compare


	//T = (0.0625us) * (62 + 1) * 64 *1 = 252 microsecs 
	//T0 init
	TCCR0 = (1<<WGM01)|(1<<CS01)|(1<<CS00); //CTC T0, P=64
	OCR0 = 249;//62; //Desired sampling period minus one
	sei();
	while(1) {
		asm("NOP");
	}
}



// T = 16MHz * 1000us = 16000
// 16000/40 = 400

ISR(ADC_vect) {
	
	uint16_t result = ADCL;
	result = ADCH; //8 high bits
	PORTD = result;
	TIFR = (1<<OCF0); //Clear T0 compare interrupt flag
}

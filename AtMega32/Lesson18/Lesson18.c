//Chapter 18 - PWM
//Experiments 18-1 (fPWM) and 18-2 (pcPWM)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <limits.h>
#include "LCDinclude.h"
#include "KBDinclude.h"



//======================
void printParams() {
	LCDsendCmd(LCDclear);
	LCDprintCT("ICR1=");
	LCDprintULong(ICR1);

	LCDprintCT(",D=");
	//LCDprintFloat((100*OCR1A)/ICR1, 4, 1);
	LCDprintULong( (100*OCR1A)/ICR1 );
	LCDprintCT("%");

	LCDcursor(1,0);
	LCDprintCT("A=");

	LCDprintULong(OCR1A);
	LCDcursor(1,8);

	LCDprintCT("B=");
	LCDprintULong(OCR1B);
}



//= 16MHz / [ P * (TOP+1) ] = 158416Hz



//*********************************************
int main() { 
	LCD4init();
	KBDsetup();//KBDinit();
	DDRD |= (1<<5)|(1<<4); // PD4, PD5 as Outputs (tsekare askisi kefal.13)
	TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS10); //exp 18-1
	// TCCR1B = (1<<WGM13)|(1<<CS10); //exp 18-2

	ICR1 = 100;
	OCR1A = 80;
	OCR1B = 20;

	printParams();
	sei();
	while(1) {
		keyPrev = 0xFF;
		while (keyPrev == 0xFF) {
		
		} ; //Wait for a key press
		if (keyPrev == 1) {
			ICR1=100; 
			OCR1A=25;  // f(Hz)=158200, D(%)=25.8 ---> PB5, selides 222, 223 sto vivlio
			OCR1B=75;  // f(Hz)=158200, D(%)=75.3 ---> PB4
		}
		if (keyPrev == 2) {
			ICR1=10000; 
			OCR1A=2500; // f(Hz)=1600, D(%)=25
			OCR1B=7500; // f(Hz)=1600, D(%)=74.9
		}
		if (keyPrev == 3) {
			ICR1=65000; 
			OCR1A=0;  // f(Hz)=246.3, 62.5ns-4.06ms ´0.0015%
			OCR1B=65000; // DC, 100
		}
		if (keyPrev == 4) {
			ICR1=65000; 
			OCR1A=1;   // f(Hz)=246.3, 62.5ns/4.06ms = 0.003%
			OCR1B=64999;   // f(Hz)= 246.3,    (4.06ms-62.5ns)/4.06ms = 99.998%
		}
		//if (keyVal == '#') { //diko mou afto
		//	ICR1=444; 
		//	OCR1A=1; 
		//	OCR1B=444;
		//}
		_delay_ms(175);
		printParams();
	}
}

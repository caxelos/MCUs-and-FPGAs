//Include routines for LCD (Chapter 17)
//========================

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

//LCD pinout: 1:GND, 2:+5V, 3:Contrast, 4:RS,
//5:R/!W, 6:E, 7:D0, ..., 14:D7, 15:BL+, 16:BL- 
//Check your LCD for power supply and back light pins

#define lcd_DDR DDRA
#define lcd_PORT PORTA	//4 high bits
#define lcd_PIN PINA
#define lcdBUSYline 7

#define lcdRS_DDR DDRC
#define lcdRS_PORT PORTC
#define lcdRSline 1

#define lcdRW_DDR DDRC
#define lcdRW_PORT PORTC
#define lcdRWline 6

#define lcdEN_DDR DDRC
#define lcdEN_PORT PORTC
#define lcdENline 7

#define LCDclear 0b00000001
#define LCDhome 0b00000010
#define LCDon 0b00001100
#define LCDoff 0b00001000
#define LCDcursorOn 0b00001111
#define LCDcursorOff 0b00001100

char* itoa(int, char* , int);
char* ultoa (unsigned long, char*, int);
char* dtostrf (double, signed char, unsigned char, char*);

extern const uint8_t PROGMEM LCDgreekFontTbl[];

extern const uint8_t translationTbl[];


//======================================
void LCDsendEnPulse();

//===========================================
void LCDtransmit4(uint8_t LCDcharacter);

//======================================
void LCDwait4();

//======================================
void LCDsendCmd(uint8_t LCDdata);

//=======================================
void LCDprintChar(uint8_t LCDdata);

//============================================
void LCDdefineGrk();

//======================================
void LCD4init();

//============================================
void LCDcursor(uint8_t line, uint8_t column);

//============================================
void LCDprintInt(int number);

//============================================
void LCDprintByte(uint8_t number);

//============================================
void LCDprintULong(unsigned long number);

//============================================
void LCDprintFloat(float number,uint8_t width,uint8_t prec);

//============================================
void LCDprintCT(char s[]);

#include "reg51.h"
#include "intrins.h"

#define FOSC    11059200L
#define BAUD    9600

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define     URMD    0           //0:??SG¶¨?±??2????²¨????·?????
                                //1:??SG¶¨?±??1µ??£?½0(16?»??¶?F????£?½)????²¨????·?????
                                //2:??SG¶¨?±??1µ??£?½2(8?»??¶?F????£?½)????²¨????·?????

sfr T2H   = 0xd6;               //¶¨?±??2??8?»
sfr T2L   = 0xd7;               //¶¨?±??2µ?8?»


sbit P10 = P1^0;	// adc input 1
sbit P11 = P1^1;	// adc input 2

sbit P32 = P3^2;	// LED 1
sbit P33 = P3^3;	// LED 2

sfr P5   = 0xC8;
sbit P54 = P5^4; // out 1
sbit P55 = P5^5; // out 2

sfr  AUXR       =   0x8e;       //?¨F???????                              

sfr ADC_CONTR = 0xbc;
sfr ADC_RES = 0xbd;
sfr ADC_RESL = 0xbE;
sfr P1ASF = 0x9D;

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00             //540 clocks
#define ADC_SPEEDL  0x20            //360 clocks
#define ADC_SPEEDH  0x40            //180 clocks
#define ADC_SPEEDHH 0x60            //90 clocks

/*----------------------------
Initial ADC sfr
----------------------------*/
void InitADC(BYTE chan)
{
	P1ASF |= 1 << chan;             //enable channel ADC function
	ADC_RES = 0;                    //Clear previous result
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
	_nop_(); _nop_(); _nop_(); //waiting for power stability;    
	//Delay(2);                       //ADC power-on and delay
}

int getADCResult(char chan)
{
	// todo: was ADC_SPEEDHH
	
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | chan;
	_nop_();       //Must wait before inquiry
	_nop_(); _nop_(); _nop_();
	while (!(ADC_CONTR & ADC_FLAG));  //Wait complete flag
	ADC_CONTR &= ~ADC_FLAG;           //Close ADC

	return  ADC_RES; // << 8 | ADC_RESL;  //Return ADC result, 10 bits
}


char input;

void input1() {
	input = 1;
	P32 = 1; // LED2 off
	P33 = 0; // LED1 on
	P54 = 1; // Set P Channel MOSFET 
	P55 = 0;
}

void input2() {
	input = 2;
	P32 = 0; // LED2 on
	P33 = 1; // LED1 off
	P54 = 0; // Set P Channel MOSFET 
	P55 = 1; 
}	

void delay(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

void main() {
	int in1;
	//	InitUart();                     //³???»??®??
	InitADC(0);
	input1();
	//InitADC(1);
	while (1) {
		in1 = getADCResult(0); // get input 1 
		if ( (input == 1) && ( in1 < 58) ) {
			input2();
		}
		if ( (input == 2) && (in1 > 61)) {
			input1();
			delay(10);
		}	
	}
}



/*
stcgal -p COM8 C:\ups.hex
*/
/************************************/
/*									*/
/*	file:       ADC080x.h			*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Venice 26/12/2018 	*/
/*									*/
/************************************/

#include <xc.h>
#include "delay.h"

#define ADC080x_DATA7   _RB9
#define ADC080x_DATA6   _RB8
#define ADC080x_DATA5   _RB7
#define ADC080x_DATA4   _RB5
#define ADC080x_DATA3   _RC5
#define ADC080x_DATA2   _RC4
#define ADC080x_DATA1   _RA9
#define ADC080x_DATA0   _RA8
#define ADC080x_START   _LATC2
#define ADC080x_READY   _RC6

#define ADC080x_DATA7_DIRECTION   _TRISB9
#define ADC080x_DATA6_DIRECTION   _TRISB8
#define ADC080x_DATA5_DIRECTION   _TRISB7
#define ADC080x_DATA4_DIRECTION   _TRISB5
#define ADC080x_DATA3_DIRECTION   _TRISC5
#define ADC080x_DATA2_DIRECTION   _TRISC4
#define ADC080x_DATA1_DIRECTION   _TRISA9
#define ADC080x_DATA0_DIRECTION   _TRISA8
#define ADC080x_START_DIRECTION   _TRISC2
#define ADC080x_READY_DIRECTION   _TRISC6

typedef union {
	unsigned char byte;
	struct {
		unsigned bit0:1;
		unsigned bit1:1;
		unsigned bit2:1;
		unsigned bit3:1;
		unsigned bit4:1;
		unsigned bit5:1;
		unsigned bit6:1;
		unsigned bit7:1;
	};
} ADC080x_byte;

void ADC080x_start_conversion();

void ADC080x_init();

unsigned char ADC080x_read();

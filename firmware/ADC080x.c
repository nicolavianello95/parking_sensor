/************************************/
/*									*/
/*	file:       ADC080x.c			*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Venice 26/12/2018 	*/
/*									*/
/************************************/

#include "ADC080x.h"

void ADC080x_init(){
ADC080x_DATA7_DIRECTION=1;
ADC080x_DATA6_DIRECTION=1;
ADC080x_DATA5_DIRECTION=1;
ADC080x_DATA4_DIRECTION=1;
ADC080x_DATA3_DIRECTION=1;
ADC080x_DATA2_DIRECTION=1;
ADC080x_DATA1_DIRECTION=1;
ADC080x_DATA0_DIRECTION=1;
ADC080x_READY_DIRECTION=1;
ADC080x_START_DIRECTION=0;

ADC080x_START=1;
}

void ADC080x_start_conversion(){
    ADC080x_START=0;
    __delay_us(1);  //at least 100ns
    ADC080x_START=1;
}

unsigned char ADC080x_read(){
    ADC080x_byte data;
    data.byte=0;
    if(ADC080x_DATA0)
        data.bit0=1;
    if(ADC080x_DATA1)
        data.bit1=1;
    if(ADC080x_DATA2)
        data.bit2=1;
    if(ADC080x_DATA3)
        data.bit3=1;
    if(ADC080x_DATA4)
        data.bit4=1;
    if(ADC080x_DATA5)
        data.bit5=1;
    if(ADC080x_DATA6)
        data.bit6=1;
    if(ADC080x_DATA7)
        data.bit7=1;
    return data.byte;
}

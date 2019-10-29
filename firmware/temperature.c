/************************************/
/*									*/
/*	file:   	temperature.c		*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Venice 26/12/2018 	*/
/*									*/
/************************************/

#include "temperature.h"
	
//return temperature in degree Celsius having a value of ADC conversion
float temperature_get_celsius(unsigned conversion){
	float sensor_out=((float)conversion)*ADC_GAP;	//out in volts
	float celsius=(sensor_out-TEMPERATURE_OFFSET)*(1.0F/TEMPERATURE_GAIN);
	return celsius;
}

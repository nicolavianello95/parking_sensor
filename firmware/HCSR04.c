/************************************/
/*									*/
/*	file:       hcsr04.c			*/
/*	author: 	Nicola Vianello		*/
/*	last edit:	Venice 27/12/2018 	*/
/*									*/
/************************************/

#include "hcsr04.h"

float hcsr04_sound_speed; //sound speed in centimeters per seconds

void HCSR04_init(){
	//data direction
	HCSR04_TRIGGER1_DIRECTION=0;
	HCSR04_TRIGGER2_DIRECTION=0;
	HCSR04_TRIGGER3_DIRECTION=0;
	HCSR04_TRIGGER4_DIRECTION=0;
	HCSR04_ECHO1_DIRECTION=1;
	HCSR04_ECHO2_DIRECTION=1;
	HCSR04_ECHO3_DIRECTION=1;
	HCSR04_ECHO4_DIRECTION=1;

	//clear output
	HCSR04_TRIGGER1=0;
	HCSR04_TRIGGER2=0;
	HCSR04_TRIGGER3=0;
	HCSR04_TRIGGER4=0;

	//init sound speed at 20 Celsius degree
	HCSR04_set_temperature(20);
}

//strobe on trigger of channel "channel"
void HCSR04_trigger_strobe(unsigned char channel){
	switch(channel){
		case 1:
			HCSR04_TRIGGER1=1;
			__delay_us(HCSR04_T_TRIGGER);
			HCSR04_TRIGGER1=0;
			break;
		case 2:
			HCSR04_TRIGGER2=1;
			__delay_us(HCSR04_T_TRIGGER);
			HCSR04_TRIGGER2=0;
			break;
		case 3:
			HCSR04_TRIGGER3=1;
			__delay_us(HCSR04_T_TRIGGER);
			HCSR04_TRIGGER3=0;
			break;
		default:	//channel 4
			HCSR04_TRIGGER4=1;
			__delay_us(HCSR04_T_TRIGGER);
			HCSR04_TRIGGER4=0;	
			break;
	}
}

//return the distance in centimeters having a rising and falling edge times of a 16 bit timer
//only one wrapping of timer is allawed
//if target is out of range return -1
float HCSR04_get_distance(unsigned T_start, unsigned T_stop){
	float time, distance;	//time in seconds and distance in centimeters
	unsigned T_delta;	//number of timer ticks
	T_delta=T_stop-T_start;
	time=((float)T_delta)*HCSR04_TIMER_PERIOD;
	if(time>=0.038F)	//out of range
		distance=HCSR04_OUT_OF_RANGE;	//distance=0
	else
		distance=((time*hcsr04_sound_speed)/2);
	return distance;
}

//set the sound speed at a given temperature
void HCSR04_set_temperature(float temperature){
	hcsr04_sound_speed=33145.0F+(62.0F*temperature);
}


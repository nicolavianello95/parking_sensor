/************************************/
/*									*/
/*	file:       hcsr04.h			*/
/*	author: 	Nicola Vianello		*/
/*	last edit:	Venice 27/12/2018 	*/
/*									*/
/************************************/

#include <xc.h>
#include "delay.h"

#define HCSR04_TRIGGER1	_LATC3
#define HCSR04_TRIGGER2	_LATB3
#define HCSR04_TRIGGER3	_LATB2
#define HCSR04_TRIGGER4	_LATC8
#define HCSR04_ECHO1	_RC7
#define HCSR04_ECHO2	_RB10
#define HCSR04_ECHO3	_RB11
#define HCSR04_ECHO4	_RC9

#define HCSR04_TRIGGER1_DIRECTION	_TRISC3
#define HCSR04_TRIGGER2_DIRECTION	_TRISB3
#define HCSR04_TRIGGER3_DIRECTION	_TRISB2
#define HCSR04_TRIGGER4_DIRECTION	_TRISC8
#define HCSR04_ECHO1_DIRECTION	_TRISC7
#define HCSR04_ECHO2_DIRECTION	_TRISB10
#define HCSR04_ECHO3_DIRECTION	_TRISB11
#define HCSR04_ECHO4_DIRECTION	_TRISC9

#define HCSR04_TIMER_PRESCALER 64	//prescaler 1:64

#define HCSR04_TIMER_PERIOD ((float)HCSR04_TIMER_PRESCALER/((float)FCY))	//timer period in seconds

#define HCSR04_T_TRIGGER 10	//us

#define HCSR04_OUT_OF_RANGE -1.0F

//initialize pins
void HCSR04_init();

//pulse on trigger
void HCSR04_trigger_strobe(unsigned char channel);

//return the distance having a timer start and stop time; only one wrapping of timer; if distance is out of range then return 0.0F
float HCSR04_get_distance(unsigned T_start, unsigned T_stop);	

//set the speed of sound at a given temperature
void HCSR04_set_temperature(float temperature);

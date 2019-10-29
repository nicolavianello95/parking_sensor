/************************************/
/*									*/
/*	file:       buzzer.h			*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Venice 27/12/2018 	*/
/*									*/
/************************************/

#include <xc.h>

#define BUZZER _LATB13
#define BUZZER_DIRECTION _TRISB13

//buzzer stay enabled until this function is called T_on times, and after buzzer stay disabled for T_off times
void buzzer_blink(unsigned T_on, unsigned T_off);

//initialize pin
void buzzer_init();

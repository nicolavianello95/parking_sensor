/************************************/
/*									*/
/*	file:       buzzer.c			*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Venice 27/12/2018 	*/
/*									*/
/************************************/

#include "buzzer.h"

//enable buzzer for "T_on" times and after disable buzzer for "T_off" times; T_on and T_off are the number of times the function is called up
void buzzer_blink(unsigned T_on, unsigned T_off){
	static unsigned count=0;
    static unsigned char blink_state=0;
    BUZZER=blink_state;
    count++;
    if(blink_state==0 && count>=T_off){
        blink_state=1;
        count=0;
    }
    else if(blink_state==1 && count>=T_on){
        blink_state=0;
        count=0;
    }
}

//initialize pin
void buzzer_init(){
	BUZZER_DIRECTION=0;
	BUZZER=0;
}


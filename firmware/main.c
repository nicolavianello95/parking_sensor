/************************************/
/*									*/
/*	file:       main.c				*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Venice 04/03/2019 	*/
/*									*/
/************************************/

#include "main.h"

volatile float distance[NUMBER_OF_SENSORS];
volatile float celsius;

void port_mapping(){
	__builtin_write_OSCCONL(OSCCON & ~(1<<6)); //unlock PPS registers
	_INT1R=22;  //connect external interrupt 1 to pin RP22
    _IC1R=23;	//connect channel 1 of input capture to pin RP9
	_IC2R=10;	//connect channel 2 of input capture to pin RP8
	_IC3R=11;	//connect channel 3 of input capture to pin RP7
	_IC4R=25;	//connect channel 4 of input capture to pin RP10
	__builtin_write_OSCCONL(OSCCON | (1<<6)); //lock PPS registers
}

void external_interrupt_init(){
    _INT1EP=1;  //interrupt on negative edge;
    _INT1IP=5;  //interrupt priority is 5
    _INT1IF=0;  //clear flag
    _INT1IE=1;  //enable interrupt 
}

void timer_init(){
    //timer1
	//used for input capture and output compare, period 5.333us, overflow every 349.5ms
	T1CONbits.TSIDL=0;		//continue count in idle mode
	T1CONbits.TGATE=0;		//gated time accumulation is disabled
	T1CONbits.TCKPS=0b10; 	//prescaler 1:64
	T1CONbits.TCS=0;		//clock source from internal clock
	T1CONbits.TON=1;		//start count
}

void input_capture_init(){
	unsigned garbage;
	
	//channel 1, use for echo1
	IC1CON1bits.ICSIDL=0;           //continues to operate in CPU idle mode
	IC1CON2bits.IC32=0;             //16bit
	while(IC1CON1bits.ICBNE==1)     //remove garbage from the FIFO
		garbage=IC1BUF;
	IC1CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	IC1CON1bits.ICTSEL=0b100;       //clock source from timer1
	IC1CON1bits.ICI=0b00;           //interrupt on every capture event
	IC1CON2bits.ICTRIG=0;           //synchronous mode
	IC1CON1bits.ICM=0b001;          //capture on every edge
	_IC1IP=7;                       //interrupt priority is 7
	_IC1IF=0;                       //clear flag
	_IC1IE=1;                       //enable interrupt

	//channel 2, use for echo2
	IC1CON1bits.ICSIDL=0;           //continues to operate in CPU idle mode
	IC2CON2bits.IC32=0;             //16bit
	while(IC2CON1bits.ICBNE==1)     //remove garbage from the FIFO
		garbage=IC2BUF;
	IC2CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	IC2CON1bits.ICTSEL=0b100;       //clock source from timer1
	IC2CON1bits.ICI=0b00;           //interrupt on every capture event
	IC2CON2bits.ICTRIG=0;           //synchronous mode
	IC2CON1bits.ICM=0b001;          //capture on every edge
	_IC2IP=7;                       //interrupt priority is 7
	_IC2IF=0;                       //clear flag
	_IC2IE=1;                       //enable interrupt

	//channel 3, use for echo3
	IC1CON1bits.ICSIDL=0;           //continues to operate in CPU idle mode
	IC3CON2bits.IC32=0;             //16bit
	while(IC3CON1bits.ICBNE==1)     //remove garbage from the FIFO
		garbage=IC3BUF;
	IC3CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	IC3CON1bits.ICTSEL=0b100;       //clock source from timer1
	IC3CON1bits.ICI=0b00;           //interrupt on every capture event
	IC3CON2bits.ICTRIG=0;           //synchronous mode
	IC3CON1bits.ICM=0b001;          //capture on every edge
	_IC3IP=7;                       //interrupt priority is 7
	_IC3IF=0;                       //clear flag
	_IC3IE=1;                       //enable interrupt				

	//channel 4, use for echo4
	IC1CON1bits.ICSIDL=0;           //continues to operate in CPU idle mode
	IC4CON2bits.IC32=0;             //16bit
	while(IC4CON1bits.ICBNE==1)     //remove garbage from the FIFO
		garbage=IC4BUF;
	IC4CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	IC4CON1bits.ICTSEL=0b100;       //clock source from timer1
	IC4CON1bits.ICI=0b00;           //interrupt on every capture event
	IC4CON2bits.ICTRIG=0;           //synchronous mode
	IC4CON1bits.ICM=0b001;          //capture on every edge
	_IC4IP=7;                       //interrupt priority is 7
	_IC4IF=0;                       //clear flag
	_IC4IE=1;                       //enable interrupt
}

void output_compare_init(){

	//channel 1, used for trigger1
	OC1CON1bits.OCSIDL=0;       	//continues to operate in CPU idle mode
	OC1CON1bits.ENFLT2=0;           //comparator fault input is disabled
	OC1CON1bits.ENFLT1=0;           //OCFB fault is disabled
	OC1CON1bits.ENFLT0=0;           //OCFA fault is disabled
	OC1CON2bits.OCINV=0;            //output is not inverted
	OC1CON2bits.DCB=0b00;           //capture event occurs at start of the instruction cycle
	OC1CON2bits.OC32=0;             //16bit
	OC1CON2bits.OCTRIS=1;           //tri-stated output
	OC1R=0;                         //init value
	OC1CON2bits.OCTRIG=0;           //synchronous mode
	OC1CON2bits.SYNCSEL=0b01011;    //synchronization source from timer1
	OC1CON1bits.OCTSEL=0b100;       //based on timer1
	OC1CON1bits.OCM=0b011;          //single compare continuous pulse mode
	_OC1IP=6;                       //interrupt priority is 6
	_OC1IF=0;                       //clear flag
	_OC1IE=1;                       //enable interrupt

	//channel 2, used for trigger2
	OC2CON1bits.OCSIDL=0;           //continues to operate in CPU idle mode
	OC2CON1bits.ENFLT2=0;           //comparator fault input is disabled
	OC2CON1bits.ENFLT1=0;           //OCFB fault is disabled
	OC2CON1bits.ENFLT0=0;           //OCFA fault is disabled
	OC2CON2bits.OCINV=0;            //output is not inverted
	OC2CON2bits.DCB=0b00;           //capture event occurs at start of the instruction cycle
	OC2CON2bits.OC32=0;             //16bit
	OC2CON2bits.OCTRIS=1;           //tri-stated output
	OC2R=DELTA_TRIGGER+DELTA_ECHO;	//init value
	OC2CON2bits.OCTRIG=0;           //synchronous mode
	OC2CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	OC2CON1bits.OCTSEL=0b100;       //based on timer1
	OC2CON1bits.OCM=0b011;          //single compare continuous pulse mode
	_OC2IP=6;                       //interrupt priority is 6
	_OC2IF=0;                       //clear flag
	_OC2IE=1;                       //enable interrupt

	//channel 3, used for trigger3
	OC3CON1bits.OCSIDL=0;           //continues to operate in CPU idle mode
	OC3CON1bits.ENFLT2=0;           //comparator fault input is disabled
	OC3CON1bits.ENFLT1=0;           //OCFB fault is disabled
	OC3CON1bits.ENFLT0=0;           //OCFA fault is disabled
	OC3CON2bits.OCINV=0;            //output is not inverted
	OC3CON2bits.DCB=0b00;           //capture event occurs at start of the instruction cycle
	OC3CON2bits.OC32=0;             //16bit
	OC3CON2bits.OCTRIS=1;           //tri-stated output
	OC3R=(DELTA_TRIGGER+DELTA_ECHO)*2;//init value
	OC3CON2bits.OCTRIG=0;           //synchronous mode
	OC3CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	OC3CON1bits.OCTSEL=0b100;       //based on timer1
	OC3CON1bits.OCM=0b011;          //single compare continuous pulse mode
	_OC3IP=6;                       //interrupt priority is 6
	_OC3IF=0;                       //clear flag
	_OC3IE=1;                       //enable interrupt

	//channel 4, used for trigger4
	OC4CON1bits.OCSIDL=0;           //continues to operate in CPU idle mode
	OC4CON1bits.ENFLT2=0;           //comparator fault input is disabled
	OC4CON1bits.ENFLT1=0;           //OCFB fault is disabled
	OC4CON1bits.ENFLT0=0;           //OCFA fault is disabled
	OC4CON2bits.OCINV=0;            //output is not inverted
	OC4CON2bits.DCB=0b00;           //capture event occurs at start of the instruction cycle
	OC4CON2bits.OC32=0;             //16bit
	OC4CON2bits.OCTRIS=1;           //tri-stated output
	OC4R=(DELTA_TRIGGER+DELTA_ECHO)*3;//init value
	OC4CON2bits.OCTRIG=0;           //synchronous mode
	OC4CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	OC4CON1bits.OCTSEL=0b100;       //based on timer1
	OC4CON1bits.OCM=0b011;          //single compare continuous pulse mode
	_OC4IP=6;                       //interrupt priority is 6
	_OC4IF=0;                       //clear flag
	_OC4IE=1;                       //enable interrupt

	//channel 5, used to start conversion every second
	OC5CON1bits.OCSIDL=0;           //continues to operate in CPU idle mode
	OC5CON1bits.OCTSEL=0b100;       //based on timer1
	OC5CON1bits.ENFLT2=0;           //comparator fault input is disabled
	OC5CON1bits.ENFLT1=0;           //OCFB fault is disabled
	OC5CON1bits.ENFLT0=0;           //OCFA fault is disabled
	OC5CON2bits.OCINV=0;            //output is not inverted
	OC5CON2bits.DCB=0b00;           //capture event occurs at start of the instruction cycle
	OC5CON2bits.OC32=0;             //16bit
	OC5CON2bits.OCTRIS=1;           //tri-stated output
	OC5R=DELTA_PERIODIC_POLLING;	//init value
	OC5CON2bits.OCTRIG=0;           //synchronous mode
	OC5CON2bits.SYNCSEL=0b01011;	//synchronization source from timer1
	OC5CON1bits.OCM=0b011;          //single compare continuous pulse mode
	_OC5IP=4;                       //interrupt priority is 4
	_OC5IF=0;                       //clear flag
	_OC5IE=1;                       //enable interrupt
}

unsigned get_min_array_index(float array[], unsigned size){
	unsigned i, min_index=0;
	for(i=0; i<size; i++)
		if(array[i]<array[min_index])
			min_index=i;
	return min_index;
}

void periodic_polling(){
    static unsigned char conversion_count=0, display_count=0;
    unsigned char n, min_distance_number;
    char buffer[8];
    min_distance_number=get_min_array_index((float*)distance, NUMBER_OF_SENSORS);
    
    //conversion
    conversion_count++;
    if(conversion_count>=20){   //every second
        conversion_count=0;
        ADC080x_start_conversion();
    }
    
    //display
    display_count++;
    if(display_count>=4){   //every 200ms
        display_count=0;
        for(n=0;n<NUMBER_OF_SENSORS;n++){
            switch(n){  //format string in the lcd
                case 0:
                    lcd_goto(7,0);
					break;
				case 1:
					lcd_goto(14,0);
					break;
				case 2:
					lcd_goto(14,3);
					break;
				default:    //case 3
					lcd_goto(7,3);
					break;
			}
            if(distance[n]==HCSR04_OUT_OF_RANGE || (unsigned)distance[n]>=THRESHOLD_HIGH)
                lcd_print_string(" Out ");
            else{
                sprintf(buffer,"%2.0fcm  ",(double)distance[n]-0.5);
                if(min_distance_number==n){
                    if((unsigned)distance[n]<=THRESHOLD_LOW)
                        lcd_blink_print_string(buffer, 2, 1);
                    else
                        lcd_blink_print_string(buffer, 4, 1);
                }
                else
                    lcd_print_string(buffer);
            }
        }
        sprintf(buffer,"%3.0f%cC ", (double)celsius, LCD_DEGREE_SYMBOL);	//'ß' is '°' in the HD44780
        lcd_goto(0,1);
        lcd_print_string(buffer);
    }
    
    //buzzer
    if((unsigned)distance[min_distance_number]<=THRESHOLD_LOW)
		BUZZER=1;
	else if((unsigned)distance[min_distance_number]<THRESHOLD_HIGH)
		buzzer_blink(2,((unsigned)distance[min_distance_number]-THRESHOLD_LOW)/2);
	else
		BUZZER=0;
}

void main(){
    port_mapping();
    lcd_init();
    HCSR04_init();
    ADC080x_init();
    external_interrupt_init();
    input_capture_init();
	output_compare_init();
    buzzer_init();
	timer_init();
    _TRISC0=0;
    _TRISC1=0;
    while(1){      
		Idle();     	//set micro in idle mode
		ClrWdt();       //feed the dog
    }
}

/*********interrupt*********/

//output compare channel 1
void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(){
	_OC1IF=0;	//clear flag
	HCSR04_TRIGGER1=!HCSR04_TRIGGER1;
	if(HCSR04_TRIGGER1==1)	//rising edge
		OC1R+=DELTA_TRIGGER;
	else	//falling edge
		OC1R+=DELTA_ECHO*4+DELTA_TRIGGER*3;
}

//output compare channel 2
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(){
	_OC2IF=0;	//clear flag
	HCSR04_TRIGGER2=!HCSR04_TRIGGER2;
	if(HCSR04_TRIGGER2==1)	//rising edge
		OC2R+=DELTA_TRIGGER;
	else	//falling edge
		OC2R+=DELTA_ECHO*4+DELTA_TRIGGER*3;
}

//output compare channel 3
void __attribute__((interrupt, no_auto_psv)) _OC3Interrupt(){
	_OC3IF=0;	//clear flag
	HCSR04_TRIGGER3=!HCSR04_TRIGGER3;
	if(HCSR04_TRIGGER3==1)	//rising edge
		OC3R+=DELTA_TRIGGER;
	else	//falling edge
		OC3R+=DELTA_ECHO*4+DELTA_TRIGGER*3;
}

//output compare channel 4
void __attribute__((interrupt, no_auto_psv)) _OC4Interrupt(){
	_OC4IF=0;	//clear flag
	HCSR04_TRIGGER4=!HCSR04_TRIGGER4;
	if(HCSR04_TRIGGER4==1)	//rising edge
		OC4R+=DELTA_TRIGGER;
	else	//falling edge
		OC4R+=DELTA_ECHO*4+DELTA_TRIGGER*3;
}

//output compare channel 5
void __attribute__((interrupt, no_auto_psv)) _OC5Interrupt(){
	_OC5IF=0;	//clear flag
	OC5R+=DELTA_PERIODIC_POLLING;
	periodic_polling();
}

//input capture channel 1
void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(){
	static unsigned T_rise, T_fall;
	_IC1IF=0;	//clear flag
	if(HCSR04_ECHO1==1)	//rising edge
		T_rise=IC1BUF;
	else{	//falling edge
		T_fall=IC1BUF;
		distance[0]=HCSR04_get_distance(T_rise, T_fall);
	}
}

//input capture channel 2
void __attribute__((interrupt, no_auto_psv)) _IC2Interrupt(){
	static unsigned T_rise, T_fall;
	_IC2IF=0;	//clear flag
	if(HCSR04_ECHO2==1)	//rising edge
		T_rise=IC2BUF;
	else{	//falling edge
		T_fall=IC2BUF;
		distance[1]=HCSR04_get_distance(T_rise, T_fall);
	}
}

//input capture channel 3
void __attribute__((interrupt, no_auto_psv)) _IC3Interrupt(){
	static unsigned T_rise, T_fall;
	_IC3IF=0;	//clear flag
	if(HCSR04_ECHO3==1)	//rising edge
		T_rise=IC3BUF;
	else{	//falling edge
		T_fall=IC3BUF;
		distance[2]=HCSR04_get_distance(T_rise, T_fall);
	}
}

//input capture channel 4
void __attribute__((interrupt, no_auto_psv)) _IC4Interrupt(){
	static unsigned T_rise, T_fall;
	_IC4IF=0;	//clear flag
	if(HCSR04_ECHO4==1)	//rising edge
		T_rise=IC4BUF;
	else{	//falling edge
		T_fall=IC4BUF;
		distance[3]=HCSR04_get_distance(T_rise, T_fall);
	}
}

//external interrupt 1
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(){
	_INT1IF=0;	//clear flag
    __delay_us(1);  //at least 200ns
	celsius=temperature_get_celsius(ADC080x_read());
	HCSR04_set_temperature(celsius);
}

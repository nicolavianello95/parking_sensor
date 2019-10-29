/************************************/
/*									*/
/*	file:       main.h				*/
/*	author: 	Nicola Vianello		*/
/*	last edit:	Venice 27/12/2018 	*/
/*									*/
/************************************/


// CONFIG4
#pragma config DSWDTPS = DSWDTPSF       // DSWDT Postscale Select (1:2,147,483,648 (25.7 days))
#pragma config DSWDTOSC = LPRC          // Deep Sleep Watchdog Timer Oscillator Select (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config RTCOSC = LPRC            // RTCC Reference Oscillator Select (RTCC uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = ON             // Deep Sleep BOR Enable bit (BOR enabled in Deep Sleep)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer (DSWDT disabled)

// CONFIG3
#pragma config WPFP = WPFP63            // Write Protection Flash Page Segment Boundary (Highest Page (same as page 42))
#pragma config SOSCSEL = IO             // Secondary Oscillator Pin Mode Select (SOSC pins have digital I/O functions (RA4, RB4))
#pragma config WUTSEL = LEG             // Voltage Regulator Wake-up Time Select (Default regulator start-up time used)
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable (Segmented code protection disabled)
#pragma config WPCFG = WPCFGDIS         // Write Protect Configuration Page Select (Last page and Flash Configuration words are unprotected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMOD = HS             // Primary Oscillator Select (HS Oscillator mode selected)
#pragma config I2C1SEL = PRI            // I2C1 Pin Select bit (Use default SCL1/SDA1 pins for I2C1 )
#pragma config IOL1WAY = ON             // IOLOCK One-Way Set Enable (Once set, the IOLOCK bit cannot be cleared)
#pragma config OSCIOFNC = OFF           // OSCO Pin Configuration (OSCO pin functions as clock output (CLKO))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor (Sw Disabled, Mon Disabled)
#pragma config FNOSC = PRI              // Initial Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config PLL96MHZ = OFF           // 96MHz PLL Startup Select (96 MHz PLL Startup is enabled by user in software( controlled with the PLLEN bit))
#pragma config PLLDIV = DIV12           // USB 96 MHz PLL Prescaler Select (Oscillator input divided by 12 (48 MHz input))
#pragma config IESO = ON                // Internal External Switchover (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer (Watchdog Timer is enabled)
#pragma config ICS = PGx3               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC2/PGED2)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

#include <stdio.h>
#include <xc.h>
#include "lcd.h"
#include "ADC080x.h"
#include "temperature.h"
#include "HCSR04.h"
#include "delay.h"
#include "buzzer.h"

#define NUMBER_OF_SENSORS       4           //number of HC-SR04

#define THRESHOLD_HIGH          100U        //cm
#define THRESHOLD_LOW           10U         //cm

#define T_PERIODIC_POLLING      0.05        //second
#define T_ECHO                  0.04        //second
#define T_TRIGGER               0.000016    //second

#define TIMER1_PRESCALER        64

#define T_TIMER1                ((float)TIMER1_PRESCALER/((float)FCY))              //second
#define DELTA_ECHO              ((unsigned)((float)T_ECHO/T_TIMER1))
#define DELTA_TRIGGER           ((unsigned)((float)T_TRIGGER/T_TIMER1))
#define DELTA_PERIODIC_POLLING	((unsigned)((float)T_PERIODIC_POLLING/T_TIMER1))

void port_mapping();

void external_interrupt_init();

void timer_init();

void input_capture_init();

void output_compare_init();

unsigned get_min_array_index(float array[], unsigned size);

void periodic_polling();

void main();

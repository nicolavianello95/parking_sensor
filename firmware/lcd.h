/************************************/
/*									*/
/*	file:       lcd.h				*/
/*	author:     Nicola Vianello		*/
/*	last edit:	Turin 04/03/2019 	*/
/*									*/
/************************************/

#include <xc.h>
#include "delay.h"

#define LCD_LINES 4		//must be 1,2 or 4
#define LCD_BUS_WIDTH_MODE 4	//must be 4 or 8

//pin
#define LCD_EN          _LATB15	//enable
#define LCD_RS          _LATB14	//register select
#define LCD_DATA0               //only for 8bit mode
#define LCD_DATA1               //only for 8bit mode
#define LCD_DATA2               //only for 8bit mode
#define LCD_DATA3               //only for 8bit mode
#define LCD_DATA4       _LATA0	
#define LCD_DATA5       _LATA1
#define LCD_DATA6       _LATB0
#define LCD_DATA7       _LATB1 

//direction
#define LCD_EN_DIRECTION _TRISB15		//data direction register
#define LCD_RS_DIRECTION _TRISB14
#define LCD_DATA0_DIRECTION	//only for 8bit mode
#define LCD_DATA1_DIRECTION //only for 8bit mode
#define LCD_DATA2_DIRECTION //only for 8bit mode
#define LCD_DATA3_DIRECTION //only for 8bit mode
#define LCD_DATA4_DIRECTION _TRISA0
#define LCD_DATA5_DIRECTION _TRISA1
#define LCD_DATA6_DIRECTION _TRISB0
#define LCD_DATA7_DIRECTION _TRISB1

//time
#define LCD_T_POWERUP 10 //ms
#define LCD_T_NIBBLE 6 //us
#define LCD_T_STROBE 25 //us
#define LCD_T_SLOW_CMD 2 //ms
#define LCD_T_CMD 40 //us	

//command
#define CLEAR_DISPLAY			0b00000001
#define RETURN_HOME 			0b00000010
#define ENTRY_MODE_SET 		0b00000100
	#define INCREMENT				0b00000010
	#define DECREMENT				0b00000000
	#define SHIFT_ON 				0b00000001
	#define SHIFT_OFF 				0b00000000
#define DISPLAY_ON_OFF_CONTROL 0b00001000
	#define DISPLAY_ON 			0b00000100
	#define DISPLAY_OFF 			0b00000000
	#define CURSOR_ON 				0b00000010
	#define CURSOR_OFF 			0b00000000
	#define BLINK_ON				0b00000001
	#define BLINK_OFF 				0b00000000
#define CURSOR_OR_DISPLAY_SHIFT 0b00010000
	#define DISPLAY_SHIFT 			0b00001000
	#define CURSOR_SHIFT 			0b00000000
	#define SHIFT_RIGHT			0b00000100
	#define SHIFT_LEFT				0b00000000
#define FUNCTION_SET			0b00100000
	#define DATA_LENGTH_8BIT		0b00010000
	#define DATA_LENGTH_4BIT		0b00000000
	#define DISPLAY_LINE_2			0b00001000
	#define DISPLAY_LINE_1			0b00000000
	#define FONT_5x11				0b00000100
	#define FONT_5x8				0b00000000
#define SET_CGRAM_ADDRESS 		0b01000000
#define SET_DDRAM_ADDRESS 		0b10000000

//special character
#define LCD_DEGREE_SYMBOL	0b11011111

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
} lcd_byte;

//pulse on enable
void lcd_strobe();

//send a byte to lcd controller 
void lcd_write(unsigned char c);

//initialise the lcd 
void lcd_init();

//writes a character in the lcd
void lcd_print_char(char c);

//writes a string in the lcd
void lcd_print_string(const char* string);

//write a command in the lcd controller
void lcd_command(unsigned char command);

//clear lcd
void lcd_clear(void);

//set position to write in the lcd
void lcd_goto(unsigned char x, unsigned char y);

//print "string" for "T_on" times that this function is called and after print a blank string for "T_off" times this function is called
void lcd_blink_print_string(const char* string, unsigned T_on, unsigned T_off);

//print "c" for "T_on" times that this function is called and after print a blank string for "T_off" times this function is called
void lcd_blink_print_char(char c, unsigned T_on, unsigned T_off);

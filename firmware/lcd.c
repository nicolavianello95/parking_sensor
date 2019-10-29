/************************************/
/*									*/
/*	file:       lcd.c				*/
/*	author: 	Nicola Vianello		*/
/*	last edit:	Venice 26/12/2018 	*/
/*									*/
/************************************/

#include "lcd.h"

//send a pulse on enable
void lcd_strobe(){
	LCD_EN=1;
	__delay_us(LCD_T_STROBE);
	LCD_EN=0;
}

//write a byte to the LCD
void lcd_write(unsigned char c){
	lcd_byte input;
	input.byte=c;
	#if LCD_BUS_WIDTH_MODE==4
		LCD_DATA7=input.bit7;
		LCD_DATA6=input.bit6;
		LCD_DATA5=input.bit5;
		LCD_DATA4=input.bit4;
		lcd_strobe();
		__delay_us(LCD_T_NIBBLE);
		LCD_DATA7=input.bit3;
		LCD_DATA6=input.bit2;
		LCD_DATA5=input.bit1;
		LCD_DATA4=input.bit0;
		lcd_strobe();
		__delay_us(LCD_T_NIBBLE);
	#elif LCD_BUS_WIDTH_MODE==8
		LCD_DATA7=input.bit7;
		LCD_DATA6=input.bit6;
		LCD_DATA5=input.bit5;
		LCD_DATA4=input.bit4;
		LCD_DATA3=input.bit3;
		LCD_DATA2=input.bit2;
		LCD_DATA1=input.bit1;
		LCD_DATA0=input.bit0;
		lcd_strobe();
		__delay_us(LCD_T_NIBBLE);
	#else
		#error lcd.h: LCD_BUS_WIDTH_MODE must be 4 or 8
	#endif
}
 
//initialise the LCD 
void lcd_init(){
	//init sequence

	//set port as output
    LCD_RS_DIRECTION=0;		
	LCD_EN_DIRECTION=0;
	LCD_DATA7_DIRECTION=0;
	LCD_DATA6_DIRECTION=0;
	LCD_DATA5_DIRECTION=0;
    LCD_DATA4_DIRECTION=0;

	//clear control and data
    LCD_RS = 0;		
    LCD_EN = 0;
	LCD_DATA7=0;
	LCD_DATA6=0;
	LCD_DATA5=0;
	LCD_DATA4=0;

	//wait after power applied,
    __delay_ms(LCD_T_POWERUP);

	//init data 0011
	LCD_DATA5=1;	
	LCD_DATA4=1;

	//wait
    __delay_ms(LCD_T_NIBBLE);

	//strobe
    lcd_strobe();

	//wait
    __delay_ms(LCD_T_SLOW_CMD);	

	//strobe
    lcd_strobe();

	//wait
    __delay_ms(LCD_T_NIBBLE);

	//strobe
    lcd_strobe();

	//wait
    __delay_ms(LCD_T_CMD);

	//now display is started and it is in 8bit mode

	#if LCD_BUS_WIDTH_MODE==4
		LCD_DATA7=0;	
		LCD_DATA6=0;
		LCD_DATA5=1;
		LCD_DATA4=0;
		__delay_us(LCD_T_NIBBLE);
		lcd_strobe();
		__delay_us(LCD_T_CMD);
		//now display is in a 4bit mode
	#endif

	//init settings
	#if LCD_BUS_WIDTH_MODE==4
		#if LCD_LINES==1
			lcd_command(FUNCTION_SET | DATA_LENGTH_4BIT | DISPLAY_LINE_1 | FONT_5x8);
		#elif (LCD_LINES==2 || LCD_LINES==4)
			lcd_command(FUNCTION_SET | DATA_LENGTH_4BIT | DISPLAY_LINE_2 | FONT_5x8);
		#else
			#error lcd.h: LCD_LINES must be 1, 2 or 4
		#endif
	#elif LCD_BUS_WIDTH_MODE==8
		#if LCD_LINES==1
			lcd_command(FUNCTION_SET | DATA_LENGTH_8BIT | DISPLAY_LINE_1 | FONT_5x8);
		#elif (LCD_LINES==2 || LCD_LINES==4)
			lcd_command(FUNCTION_SET | DATA_LENGTH_8BIT | DISPLAY_LINE_2 | FONT_5x8);
		#else
			#error lcd.h: LCD_LINES must be 1, 2 or 4
		#endif
	#else
		#error lcd.h: LCD_BUS_WIDTH_MODE must be 4 or 8
	#endif

	//hide powerup garbage
    lcd_command(DISPLAY_ON_OFF_CONTROL | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF);
	__delay_us(LCD_T_CMD);

	//clear powerup garbage
    lcd_clear();

	//entry mode set
    lcd_command(ENTRY_MODE_SET | INCREMENT | SHIFT_OFF);
	__delay_us(LCD_T_CMD);

	//display on
    lcd_command(DISPLAY_ON_OFF_CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF);
	__delay_us(LCD_T_CMD);
}

//write one character to the LCD
void lcd_print_char(char c){
    LCD_RS=1; 
    lcd_write(c);
}

//write a string of chars to the LCD
void lcd_print_string(const char *string){
    LCD_RS=1; 
    while((*string)!=0){
        lcd_write(*string);
		string++;
	}
}

//send a command to the LCD
void lcd_command(unsigned char command){
	LCD_RS=0;
	lcd_write(command);
}

//clear and home the LCD
void lcd_clear(void){
    lcd_command(CLEAR_DISPLAY);
	__delay_ms(LCD_T_SLOW_CMD);
}
 
//  Go to the specified position
void lcd_goto(unsigned char x, unsigned char y){
	unsigned char lcd_ddram_address;
	#if LCD_LINES==1
		lcd_ddram_address=x;
	#elif LCD_LINES==2
		switch(y){
			case 0:
				lcd_ddram_address=x;
				break;
			default:	//y=1
				lcd_ddram_address=0x40+x;
				break;
		}
	#elif LCD_LINES==4
		switch(y){
			case 0:
				lcd_ddram_address=x;
				break;
			case 1:
				lcd_ddram_address=0x40+x;
				break;
			case 2:
				lcd_ddram_address=0x14+x;
				break;
			default:	//y=3
				lcd_ddram_address=0x54+x;
				break;
		}
	#else
		#error lcd.h: LCD_LINES must be 1, 2 or 4
	#endif
	lcd_command(SET_DDRAM_ADDRESS | lcd_ddram_address);
	__delay_us(LCD_T_CMD);
}

//print "string" for "T_on" times that this function is called and after print a blank string for "T_off" times this function is called
void lcd_blink_print_string(const char* string, unsigned T_on, unsigned T_off){
	static unsigned count=0;
	static unsigned char blink_state=1;
    if(blink_state){
        lcd_print_string(string);
    }
    else{
       	while((*string)!=0){
            lcd_print_char(' ');
            string++;
        }
    }
    count++;
    if(blink_state==1 && count>=T_on){
        blink_state=0;
        count=0;
    }
    else if(blink_state==0 && count>T_off){
        blink_state=1;
        count=0;
    }
}

//print "c" for "T_on" times that this function is called and after print a blank string for "T_off" times this function is called
void lcd_blink_print_char(char c, unsigned T_on, unsigned T_off){
	static unsigned count=0;
	static unsigned char blink_state=1;
    if(blink_state){
        lcd_print_char(c);
    }
    else{
		lcd_print_char(' ');
    }
    count++;
    if(blink_state==1 && count>=T_on){
        blink_state=0;
        count=0;
    }
    else if(blink_state==0 && count>T_off){
        blink_state=1;
        count=0;
    }
}

/******************************************************************************
*
* EXPRESS RECEIVER BY AD5GH
* (http://www.expressreceiver.com)
* RIGHT DISPLAY & CONTROL BOARD SOFTWARE
* LCD ROUTINES
*
* Copyright 2015 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 4.0
* DECEMBER 2015
*
******************************************************************************/

#include <xc.h>
#include "system.h"
#include <stdio.h>
#include <stdlib.h>

char DisplayInitFlag = 0;                           //Initialize to force display clear on start up


void Disp_Update (void)
{
    if(DisplayInitFlag == 0)                        //Clear display on first time through
    {
        LCD_PUTC(0x01); 
        __delay_ms(2);
        DisplayInitFlag = 1;
    }

    LCD_PUTC(0x80);                                 //Display Line 1 start address
    LCD_PUTD('I');
    LCD_PUTD('F');
    LCD_PUTD('B');
    LCD_PUTD('W');
    LCD_PUTD(':');
    LCD_PUTD(IFBW_Dsply[0]);
    LCD_PUTD(IFBW_Dsply[1]);
    LCD_PUTD(IFBW_Dsply[2]);
    LCD_PUTD(IFBW_Dsply[3]);
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD('M');
    LCD_PUTD('U');
    LCD_PUTD('T');
    LCD_PUTD('E');
    LCD_PUTD(':');
    LCD_PUTD(MUTE_Dsply[0]);
    LCD_PUTD(MUTE_Dsply[1]);
    LCD_PUTD(MUTE_Dsply[2]);

    LCD_PUTC(0xC0);                                 //Display Line 2 start address
    LCD_PUTD('R');
    LCD_PUTD('A');
    LCD_PUTD('T');
    LCD_PUTD('E');
    LCD_PUTD(':');
    LCD_PUTD(RATE_Dsply[0]);
    LCD_PUTD(RATE_Dsply[1]);
    LCD_PUTD(RATE_Dsply[2]);
    LCD_PUTD(RATE_Dsply[3]);
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD('T');
    LCD_PUTD('X');
    LCD_PUTD('L');
    LCD_PUTD('K');
    LCD_PUTD(':');
    LCD_PUTD(TXLK_Dsply[0]);
    LCD_PUTD(TXLK_Dsply[1]);
    LCD_PUTD(TXLK_Dsply[2]);

    LCD_PUTC(0x94);                                 //Display Line 3 start address
    LCD_PUTD('A');
    LCD_PUTD('T');
    LCD_PUTD('T');
    LCD_PUTD(' ');
    LCD_PUTD('A');
    LCD_PUTD('M');
    LCD_PUTD('P');
    LCD_PUTD(' ');
    LCD_PUTD('B');
    LCD_PUTD('N');
    LCD_PUTD('D');
    LCD_PUTD(' ');
    LCD_PUTD('M');
    LCD_PUTD('O');
    LCD_PUTD('D');
    LCD_PUTD(' ');
    LCD_PUTD('A');
    LCD_PUTD('G');
    LCD_PUTD('C');
    LCD_PUTD(' ');

    LCD_PUTC(0xD4);                                 //Display Line 4 start address
    LCD_PUTD(' ');
    LCD_PUTD(ATTN_Dsply[0]);
    LCD_PUTD(ATTN_Dsply[1]);
    LCD_PUTD(' ');
    LCD_PUTD(AMP_Dsply[0]);
    LCD_PUTD(AMP_Dsply[1]);
    LCD_PUTD(AMP_Dsply[2]);
    LCD_PUTD(' ');
    LCD_PUTD(Band_Dsply[0]);
    LCD_PUTD(Band_Dsply[1]);
    LCD_PUTD(Band_Dsply[2]);
    LCD_PUTD(' ');
    LCD_PUTD(Mode_Dsply[0]);
    LCD_PUTD(Mode_Dsply[1]);
    LCD_PUTD(Mode_Dsply[2]);
    LCD_PUTD(' ');
    LCD_PUTD(AGC_Dsply[0]);
    LCD_PUTD(AGC_Dsply[1]);
    LCD_PUTD(AGC_Dsply[2]);
    LCD_PUTD(' ');
}

/*********************LCD INITIALIZATION ROUTINE******************************/

void LCD_INIT(void)
{
    __delay_ms(20);
    __delay_ms(20);
    LCD_PUTC(0x30);
    __delay_us(100);
    LCD_PUTC(0x30);
    __delay_us(100);
    LCD_PUTC(0x38);                                 //2-Line display, 5x7 dots
    __delay_us(100);
    LCD_PUTC(0x0C);                                 //Display on, cursor off, blink off
    __delay_us(100);
    LCD_PUTC(0x01);                                 //Clear display
    __delay_ms(2);
    LCD_PUTC(0x06);                                 //Increment mode, entire shift off
    __delay_ms(2);
    
    Disp_Update();
}

/*
    LCD_PUTC(0x80);                                 //Display Line 1 start address
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD('A');
    LCD_PUTD('D');
    LCD_PUTD('5');
    LCD_PUTD('G');
    LCD_PUTD('H');

    LCD_PUTC(0xC0);                                 //Display Line 2 start address
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD('H');
    LCD_PUTD('F');
    LCD_PUTD(0x20);
    LCD_PUTD('A');
    LCD_PUTD('L');
    LCD_PUTD('L');
    LCD_PUTD(0x20);
    LCD_PUTD('B');
    LCD_PUTD('A');
    LCD_PUTD('N');
    LCD_PUTD('D');

    LCD_PUTC(0x94);                                 //Display Line 3 start address
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD(0x20);
    LCD_PUTD('U');
    LCD_PUTD('P');
    LCD_PUTD(0x20);
    LCD_PUTD('C');
    LCD_PUTD('O');
    LCD_PUTD('N');
    LCD_PUTD('V');
    LCD_PUTD('E');
    LCD_PUTD('R');
    LCD_PUTD('T');
    LCD_PUTD('I');
    LCD_PUTD('N');
    LCD_PUTD('G');

    LCD_PUTC(0xD4);                                 //Display Line 4 start address
    LCD_PUTD(0x20);
    LCD_PUTD('S');
    LCD_PUTD('U');
    LCD_PUTD('P');
    LCD_PUTD('E');
    LCD_PUTD('R');
    LCD_PUTD(0x20);
    LCD_PUTD('H');
    LCD_PUTD('E');
    LCD_PUTD('T');
    LCD_PUTD(0x20);
    LCD_PUTD('R');
    LCD_PUTD('E');
    LCD_PUTD('C');
    LCD_PUTD('E');
    LCD_PUTD('I');
    LCD_PUTD('V');
    LCD_PUTD('E');
    LCD_PUTD('R');
}
*/

/*********************SEND CONTROL CHARACTER TO LCD***************************/

void LCD_PUTC (unsigned char C)
{
    PORTD = C;
    __delay_us(20);
    PORTE = 0x01;
    __delay_us(20);
    PORTE = 0x00;
    __delay_us(10);
}


/*********************SEND DATA CHARACTER TO LCD******************************/

void LCD_PUTD(unsigned char C)
{
    PORTD = C;
    PORTE = 0x04;
    __delay_us(20);
    PORTE = 0x05;
    __delay_us(20);
    PORTE = 0x00;
    __delay_us(10);
}



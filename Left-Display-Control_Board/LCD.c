/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* LEFT DISPLAY & CONTROL BOARD SOFTWARE
* LCD ROUTINES
*
* Copyright 2015 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 2.0
* DECEMBER 2015
*
******************************************************************************/

#include <xc.h>
#include "system.h"
#include <stdio.h>
#include <stdlib.h>

extern char AGC_Mode;

char S_Meter_Hist[10];
char count = 0;

char Man_IF_Gain_dB[19] = {45, 40, 35, 30, 25, 20, 15, 10, 5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45};
    
void LCD_AGC_Update (char temp)
{
    char Level_Disp[2];
    char sign, level;
    
    if(AGC_Mode == AGC_MAN)                     //IF AGC OFF, activate manual IF gain control
    {
        if(temp < 9) sign = '-';                //minus sign
        else sign = ' ';
    
        level = Man_IF_Gain_dB[temp];
    
        ultoa(Level_Disp, level, 10);
    
        if(level < 10)
        {
            Level_Disp[1] = Level_Disp[0];
            Level_Disp[0] = 0x20;
        }

        LCD_PUTC(0xD4);                         //Display Line 4 start address
        LCD_PUTD('I');
        LCD_PUTD('F');
        LCD_PUTD(' ');
        LCD_PUTD('G');
        LCD_PUTD('A');
        LCD_PUTD('I');
        LCD_PUTD('N');
        LCD_PUTD(' ');
        LCD_PUTD('F');
        LCD_PUTD('I');
        LCD_PUTD('X');
        LCD_PUTD('E');
        LCD_PUTD('D');
        LCD_PUTD(':');
        LCD_PUTD(' ');
        LCD_PUTD(sign);
        LCD_PUTD(Level_Disp[0]);
        LCD_PUTD(Level_Disp[1]);
        LCD_PUTD('d');
        LCD_PUTD('B');
    }

    else
    {
        LCD_PUTC(0xD4);                         //Display Line 4 start address
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
        LCD_PUTD(' ');
    }
}

void S_Meter_Update(unsigned char S_Level)
{
    unsigned char peak, c;

    S_Meter_Hist[count] = S_Level;
    count++;
    if(count == 10) count = 0;
    
    peak = S_Meter_Hist[0];
 
    for (c = 1; c < 10; c++)
    {
        if (S_Meter_Hist[c] > peak) peak  = S_Meter_Hist[c];
    }

    switch (S_Level)
    {
        case 0:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 1:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 2:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 3:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 4:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 5:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 6:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 7:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 8:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 9:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 10:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 11:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 12:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 13:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 14:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 15:

            LCD_PUTC(0x94);                     //Display Line 3 Start Address
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(0xFF);
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;
    }


    switch (peak)
    {
        case 0:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 1:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD('1');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 2:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD('2');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 3:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('3');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 4:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('4');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 5:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('5');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 6:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('6');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 7:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('7');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 8:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('8');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 9:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('9');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 10:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('+');
            LCD_PUTD('1');
            LCD_PUTD('0');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 11:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('+');
            LCD_PUTD('2');
            LCD_PUTD('0');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 12:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('+');
            LCD_PUTD('3');
            LCD_PUTD('0');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 13:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('+');
            LCD_PUTD('4');
            LCD_PUTD('0');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 14:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('+');
            LCD_PUTD('5');
            LCD_PUTD('0');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;

        case 15:

            LCD_PUTC(0xC0);                     //Display Line 2 Start Address
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD('+');
            LCD_PUTD('6');
            LCD_PUTD('0');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            LCD_PUTD(' ');
            break;
    }
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
    LCD_PUTC(0x38);                         //4-Line display, 5x7 dots
    __delay_us(100);
    LCD_PUTC(0x0C);                         //Display on, cursor off, blink off
    __delay_us(100);
    LCD_PUTC(0x01);                         //Clear display
    __delay_ms(2);
    LCD_PUTC(0x06);                         //Increment mode, entire shift off
    __delay_ms(2);
    
    LCD_PUTC(0x80);                         //Display Line 1 Start Address
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD('S');
    LCD_PUTD('-');
    LCD_PUTD('M');
    LCD_PUTD('e');
    LCD_PUTD('t');
    LCD_PUTD('e');
    LCD_PUTD('r');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
    LCD_PUTD(' ');
}

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



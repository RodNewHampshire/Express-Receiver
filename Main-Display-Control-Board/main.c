/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* MAIN ROUTINES
*
* Copyright 2016 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 16.0
* FEBRUARY 2016
*
******************************************************************************/

#include <xc.h>
#include "system.h"
#include <stdio.h>
#include <stdlib.h>


/**********PIC18F CONFIGURATION BITS**********/
#pragma config FOSC = ECHPIO6           //external clock, high power
#pragma config PLLCFG = OFF             //4 x PLL under software control
#pragma config PRICLKEN = ON            //Primary clock always enabled
#pragma config WDTEN = OFF              //Watchdog timer disabled
#pragma config PWRTEN = ON              //Power up timer enabled
#pragma config BOREN = NOSLP            //Brown-out reset
#pragma config BORV = 220               //Brown-out reset voltage bits
#pragma config IESO = OFF               //Oscillator switch over mode disable
#pragma config FCMEN = OFF              //Fail-safe clock monitor disabled
#pragma config MCLRE = INTMCLR          //MCLR enabled, RE3 input disabled
#pragma config PBADEN = OFF             //PORTB<4:0> conf for digital I/O on Reset
#pragma config DEBUG = OFF              //background debug enabled
#pragma config STVREN = OFF             //stack full/underflow will not cause reset
#pragma config CP0 = OFF                //Block 0 code-protected
#pragma config CP1 = OFF                //Block 1 code-protected
#pragma config CPB = OFF                //Boot block code-protected
#pragma config CPD = OFF                //Data EEPROM code-protected
#pragma config WRT0 = OFF               //Block 0 write-protected
#pragma config WRT1 = OFF               //Block 1 write-ptotected
#pragma config WRTC = OFF               //Config registers write-protected
#pragma config WRTB = OFF               //Boot block write-protected
#pragma config WRTD = OFF               //Data EEPROM write-protected


#define SPLASH_DELAY    3000            //3 second splash using Utilities_Long_Delay
#define HEART_BEAT      4000            //heart beat period
#define S_METER_PERIOD  4               //S-Meter update period, multiple of heart beat period
#define DEBOUNCE_PERIOD 20000           //Keyboard button press debounce period
#define MEM_HIDE_PERIOD 1000000         //Time of memory display after last memory button press

char Old_Encdr;
unsigned long FreqUpdateAccum = 0;
char FreqUpdateFlag = 0;
int TuningRate;
long Keyboard_Debounce = DEBOUNCE_PERIOD;
unsigned long Memory_Display_Hide = MEM_HIDE_PERIOD;

extern char MEM_Disp_Flag;


void main(void)
{
    long count = 0;
    char S_Meter_Update = S_METER_PERIOD;

    Initialization_Start();
    LCD_INIT();
    Utilities_Long_Delay(SPLASH_DELAY);
    VFO_INIT();
    Initialization_Right_Display();
  
    while (1)
    {
        if(++count == HEART_BEAT)
        {
            count = 0;

            //Flash heart beat LED
            PORTC ^= LED_Green;

            //get RF attenuator & RF pre-amp settings from Right Display & Control Board and set Relay Board accordingly
            RelayBoard_ATTN_Select(Read_I2C_Byte(Right_Display));

            //get S Meter reading from IF Board and send to Left Display & Control Board
            if(--S_Meter_Update == 0)
            {
                S_Meter_Update = S_METER_PERIOD;
                S_Meter_Get_Reading();
            }
        }

        //If main tuning dial has changed, update VFO frequency
        if(FreqUpdateFlag) VFO_Freq_Update();
        
        //scan keyboard for button press
        if(Keyboard_Debounce != 0) Keyboard_Debounce--;                                         //Decrement debounce timer
        if((Keyboard_Debounce == 0) && (Keypad_Update())) Keyboard_Debounce = DEBOUNCE_PERIOD;  //Set debounce timer if button pressed

        //Timer for hiding memory display on Main LCD
        if(MEM_Disp_Flag)
        {
            if(Memory_Display_Hide == 0)
            {
                LCD_Hide_Memory_Display();
                Memory_Display_Hide = MEM_HIDE_PERIOD;
                MEM_Disp_Flag = 0;
            }
            else Memory_Display_Hide--;
        }
    }
}

/**********High Priority Interrupt Routine**********/

//Main tuning optical shaft encoder interrupt routine

void interrupt high_isr (void)
{
    unsigned char New_PB, Diff_PB, A, B, C;

    if (INTCONbits.RBIF == 1)
    {
        New_PB = PORTB & 0x30;
        Diff_PB = New_PB ^ Old_Encdr;
        A = New_PB & 0x30;

        switch (Diff_PB)
        {
            case 0x20:
                if (A == 0 | A == 0x30) FreqUpdateAccum -=TuningRate;
                else FreqUpdateAccum +=TuningRate;
                Old_Encdr = New_PB;
                break;

            case 0x10:
                if (A == 0 | A == 0x30) FreqUpdateAccum +=TuningRate;
                else FreqUpdateAccum -=TuningRate;
                Old_Encdr = New_PB;
                break;
        }
        FreqUpdateFlag = 1;
        INTCON = 0x88;                              //Re-enable interrupts after update complete
    }
}

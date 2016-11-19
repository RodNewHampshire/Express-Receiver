/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* LEFT DISPLAY & CONTROL BOARD SOFTWARE
* MAIN
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


#define Splash_Delay    3000                        //3 second splash
#define toggle          20000                       //heart beat period

#define COUNT           0x04                        //dial count between indents

char Old_Encdr;
char IF_Gain;
char Dial = 0x00;
char AGC_Mode = AGC_SLOW;


void main(void)
{
    unsigned char temp;

    Long_Delay(Splash_Delay);
    Init();
    LCD_INIT();

    temp = SSP1BUF;                                 //Clear I2C buffer to avoid NACK

    while (1)
    {
        while(PIR1bits.SSP1IF != 1){}               //Wait for address byte from master
        temp = SSP1BUF;                             //Read I2C buffer
        PIR1bits.SSP1IF = 0;

        if(SSP1STAT & 0x04)                         //check if master is waiting to receive byte
        {
            SSP1BUF = IF_Gain;                      //load I2C buffer with data to be sent
            SSP1CON1bits.CKP = 1;
        }

        else if(temp != 0x2C) Execute(temp);        //else check received data
    }
}

    void Execute(unsigned char command)
    {
        switch(command & 0x07)
        {
            case S_MTR_UPDATE:
                S_Meter_Update(command >> 3);
                PORTCbits.RC2 = !PORTCbits.RC2;
                break;

            case AGC_SLOW:
                AGC_Mode = AGC_SLOW;
                PORTAbits.RA6 = 1;                                  //Green LED off
                LCD_AGC_Update(0);
                break;
                
            case AGC_FAST:
                AGC_Mode = AGC_FAST;
                PORTAbits.RA6 = 1;                                  //Green LED off
                LCD_AGC_Update(0);
                break;
                
            case AGC_MAN:
                AGC_Mode = AGC_MAN;
                PORTAbits.RA6 = 0;                                  //Green LED off
                LCD_AGC_Update(command >> 3);
                break;
        }
    }

    /**********High Priority Interrupt Routine**********/

void interrupt high_isr (void)
{
    unsigned char New_PB, Diff_PB, A, B, C;

    if (INTCONbits.RBIF == 1)
    {
        INTCON = 0x08;
        __delay_ms(2);
            
        New_PB = PORTB & 0x30;
        Diff_PB = New_PB ^ Old_Encdr;
        A = New_PB & 0x30;

        switch (Diff_PB)
        {
            case 0x20:
                Dial++;
                if (A == 0 | A == 0x30)
                {
                    if(Dial == COUNT)
                    {
                        Dial = 0x00;
                        if(IF_Gain == 0x00) break;
                        else IF_Gain = IF_Gain - 0x0A;
                    }
                }
                else
                {
                    if(Dial == COUNT)
                    {
                        Dial = 0x00;
                        if(IF_Gain == 0xC8) break;
                        else IF_Gain = IF_Gain + 0x0A;
                    }
                }
                Old_Encdr = New_PB;
                break;

            case 0x10:
                Dial++;
                if (A == 0 | A == 0x30)
                {
                    if(Dial == COUNT)
                    {
                        Dial = 0x00;
                        if(IF_Gain == 0xC8) break;
                        else IF_Gain = IF_Gain + 0x0A;
                    }
                }
                else
                {
                    if(Dial == COUNT)
                    {
                        Dial = 0x00;
                        if(IF_Gain == 0x00) break;
                        else IF_Gain = IF_Gain - 0x0A;
                    }
                }
                Old_Encdr = New_PB;
                break;
            }
        //Disp_Update();
    }
    INTCON = 0x88;                                  //Re-enable interrupts after update complete
}
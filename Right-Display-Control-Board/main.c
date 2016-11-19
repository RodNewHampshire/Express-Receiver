/******************************************************************************
*
* EXPRESS RECEIVER BY AD5GH
* (http://www.expressreceiver.com)
* RIGHT DISPLAY & CONTROL BOARD SOFTWARE
* MAIN
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

#define COUNT           0x04                        //RF attenuation dial count between indents

char Old_Encdr;
char RF_Dial = 0x30;
char Dial = 0x00;
char RF_Attn = 0x00;
char RF_Preamp = 0x00;


void main(void)
{
    char temp;
    long count = 0;

    Long_Delay(Splash_Delay);
    Init();
    LCD_INIT();

    temp = SSP1BUF;                                 //Clear I2C buffer to avoid NACK

    while (1)
    {
        while(PIR1bits.SSP1IF != 1){}               //Wait for address byte from master
        PORTCbits.RC2 = !PORTCbits.RC2;             //Toggle Green LED
        temp = SSP1BUF;                             //Read I2C buffer
        PIR1bits.SSP1IF = 0;

        if(SSP1STAT & 0x04)                         //check if master is waiting to receive byte
        {
            SSP1BUF = RF_Attn + RF_Preamp;          //load I2C buffer with data to be sent
            SSP1CON1bits.CKP = 1;
        }

        else if(temp != 0x2A) Execute(temp);        //else check received data
    }
}

    void Execute(char command)
    {
        switch(command >> 4)
        {
            case 1:                                 //IF FILTER
                command = command & 0x0F;
                IFBW_Dsply[0] = IFBW_1[command];
                IFBW_Dsply[1] = IFBW_2[command];
                IFBW_Dsply[2] = IFBW_3[command];
                IFBW_Dsply[3] = IFBW_4[command];
                LCD_PUTC(0x85);
                LCD_PUTD(IFBW_Dsply[0]);
                LCD_PUTD(IFBW_Dsply[1]);
                LCD_PUTD(IFBW_Dsply[2]);
                LCD_PUTD(IFBW_Dsply[3]);
                break;

            case 2:                                 //NOT USED

                break;

            case 3:                                 //RF ATTENUATOR
                command = command & 0x0F;
                ATTN_Dsply[0] = ATTN_1[command];
                ATTN_Dsply[1] = ATTN_2[command];
                LCD_PUTC(0xD5);
                LCD_PUTD(ATTN_Dsply[0]);
                LCD_PUTD(ATTN_Dsply[1]);
                RF_Attn = RF_ATT[command];
                break;

            case 4:                                 //RF PRE-AMP
                command = command & 0x0F;
                AMP_Dsply[0] = AMP_1[command];
                AMP_Dsply[1] = AMP_2[command];
                AMP_Dsply[2] = AMP_3[command];
                LCD_PUTC(0xD8);
                LCD_PUTD(AMP_Dsply[0]);
                LCD_PUTD(AMP_Dsply[1]);
                LCD_PUTD(AMP_Dsply[2]);
                RF_Preamp = RF_Amp[command];
                break;

            case 5:                                 //BAND
                command = command & 0x0F;
                Band_Dsply[0] = Band_1[command];
                Band_Dsply[1] = Band_2[command];
                Band_Dsply[2] = Band_3[command];
                LCD_PUTC(0xDC);
                LCD_PUTD(Band_Dsply[0]);
                LCD_PUTD(Band_Dsply[1]);
                LCD_PUTD(Band_Dsply[2]);
                break;

            case 6:                                 //MODE
                command = command & 0x0F;
                Mode_Dsply[0] = Mode_1[command];
                Mode_Dsply[1] = Mode_2[command];
                Mode_Dsply[2] = Mode_3[command];
                LCD_PUTC(0xE0);
                LCD_PUTD(Mode_Dsply[0]);
                LCD_PUTD(Mode_Dsply[1]);
                LCD_PUTD(Mode_Dsply[2]);
                if(command == 0) BFO_DDS_USB();
                else if (command == 1) BFO_DDS_LSB();
                else if (command == 2) BFO_DDS_CW();
                break;

            case 7:                                 //AGC
                command = command & 0x0F;
                AGC_Dsply[0] = AGC_1[command];
                AGC_Dsply[1] = AGC_2[command];
                AGC_Dsply[2] = AGC_3[command];
                LCD_PUTC(0xE4);
                LCD_PUTD(AGC_Dsply[0]);
                LCD_PUTD(AGC_Dsply[1]);
                LCD_PUTD(AGC_Dsply[2]);
                break;

            case 8:                                 //MUTE
                command = command & 0x0F;
                if(command) PORTA |= MUTE_LINE;
                else PORTA &= ~MUTE_LINE;
                MUTE_Dsply[0] = MUTE_1[command];
                MUTE_Dsply[1] = MUTE_2[command];
                MUTE_Dsply[2] = MUTE_3[command];
                LCD_PUTC(0x91);
                LCD_PUTD(MUTE_Dsply[0]);
                LCD_PUTD(MUTE_Dsply[1]);
                LCD_PUTD(MUTE_Dsply[2]);
                break;

            case 9:                                 //TXLK
                command = command & 0x0F;
                TXLK_Dsply[0] = TXLK_1[command];
                TXLK_Dsply[1] = TXLK_2[command];
                TXLK_Dsply[2] = TXLK_3[command];
                LCD_PUTC(0xD1);
                LCD_PUTD(TXLK_Dsply[0]);
                LCD_PUTD(TXLK_Dsply[1]);
                LCD_PUTD(TXLK_Dsply[2]);
                break;

            case 10:                                //RATE
                command = command & 0x0F;
                RATE_Dsply[0] = RATE_1[command];
                RATE_Dsply[1] = RATE_2[command];
                RATE_Dsply[2] = RATE_3[command];
                RATE_Dsply[3] = RATE_4[command];
                LCD_PUTC(0xC5);
                LCD_PUTD(RATE_Dsply[0]);
                LCD_PUTD(RATE_Dsply[1]);
                LCD_PUTD(RATE_Dsply[2]);
                LCD_PUTD(RATE_Dsply[3]);
                break;
        }
    }

    /**********High Priority Interrupt Routine**********/

void interrupt high_isr (void)
{
    unsigned char New_PB, Diff_PB, A, B, C;

    INTCON = 0x08;

    if (INTCONbits.RBIF == 1)
    {
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
                        if(RF_Dial == 0x41) RF_Dial = 0x40;     //turn off RF pre-amp
                        else if (RF_Dial == 0x40) RF_Dial = 0x30;
                        else if (RF_Dial == 0x3F);
                        else RF_Dial++;
                    }
                }
                else
                {
                    if(Dial == COUNT)
                    {
                        Dial = 0x00;
                        if (RF_Dial == 0x2F) RF_Dial = 0x41;    //turn on RF pre-amp
                        else if (RF_Dial == 0x40)RF_Dial = 0x41;
                        else if (RF_Dial == 0x41);
                        else RF_Dial--;
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
                        if (RF_Dial == 0x2F) RF_Dial = 0x41;    //turn on RF pre-amp
                        else if (RF_Dial == 0x40)RF_Dial = 0x41;
                        else if (RF_Dial == 0x41);
                        else RF_Dial--;
                    }
                }
                else
                {
                    if(Dial == COUNT)
                    {
                        Dial = 0x00;
                        if(RF_Dial == 0x41) RF_Dial = 0x40;     //turn off RF pre-amp
                        else if (RF_Dial == 0x40) RF_Dial = 0x30;
                        else if (RF_Dial == 0x3F);
                        else RF_Dial++;
                    }
                }
                Old_Encdr = New_PB;
                break;
            }
        
            Execute(RF_Dial);
        }
        
        INTCON = 0x88;                              //Re-enable interrupts after update complete
    }
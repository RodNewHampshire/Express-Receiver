/******************************************************************************
*
* EXPRESS RECEIVER BY AD5GH
* (http://www.expressreceiver.com)
* RIGHT DISPLAY & CONTROL BOARD SOFTWARE
* INITIALIZATION ROUTINES
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

extern char Old_Encdr;

void Init (void)
{

/*********************PORT PIN DEFINITION & INITIALIZATION********************/

    /********************PORTA*************************************************
    *RA0 - 0, Output
    *RA1 - 0, Output, Mute
    *RA2 - 0, Output, 
    *RA3 - 0, Output, DDS_CLK
    *RA4 - 0, Output, DDS_DATA
    *RA5 - 0, Output, DDS_FSYNCH
    *RA6 - 0, Output,Red LED
    *RA7 - 1, Input, ECHP clock oscillator
     *************************************************************************/
    TRISA = 0x80;
    ANSELA = 0x00;                                  //ADC channel 0 enabled
    PORTA |= LED_Red;                               //LED_Red Off
    PORTA &= ~MUTE_LINE;                            //Un-mute receiver


    /********************PORTB*************************************************
    *RB0 - 0, Output
    *RB1 - 0, Output
    *RB2 - 0, Output
    *RB3 - 0, Output
    *RB4 - 1, Input, ENCDR_A
    *RB5 - 1, Input, ENCDR_B
    *RB6 - 0, Output
    *RB7 - 0, Output
     *************************************************************************/
    TRISB = 0x30;
    ANSELB = 0x00;


    /********************PORTC*************************************************
    *RC0 - 0, Output
    *RC1 - 0, Output
    *RC2 - 0, Output,Green LED
    *RC3 - 1, Input, SCL
    *RC4 - 1, Input, SDA
    *RC5 - 0, Output
    *RC6 - 0, Output
    *RC7 - 0, Output
     *************************************************************************/
    TRISC = 0x18;
    ANSELC = 0x00;
    PORTC |= LED_Green;                             //LED_Green Off

    /********************I2C INITIALIZATION***********************************/
    unsigned char sync_mode = 0b00000110;           //Slave mode, 7-bit address, SSP1CON1 REGISTER
    unsigned char slew = 0b10000000;                //Slew rate disabled for 100KHz mode, SSP1STAT REGISTER

    SSPADD = I2C_ADDRESS;                           //Slave address

    SSP1STAT &= 0x3F;                               // power on state
    SSP1CON1 = 0x00;                                // power on state
    SSP1CON2 = 0x00;                                // power on state
    SSP1CON1 |= sync_mode;                          // select serial mode
    SSP1STAT |= slew;                               // slew rate on/off

    SSP1CON1bits.SSPEN = 1;                         //Enable synchronous serial port

    /********************PORTD*************************************************
    *RD0 - 0, Output, LCD_DB0
    *RD1 - 0, Output, LCD_DB1
    *RD2 - 0, Output, LCD_DB2
    *RD3 - 0, Output, LCD_DB3
    *RD4 - 0, Output, LCD_DB4
    *RD5 - 0, Output, LCD_DB5
    *RD6 - 0, Output, LCD_DB6
    *RD7 - 0, Output, LCD_DB7
     *************************************************************************/
    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;


    /********************PORTE*************************************************
    *RE0 - 0, Output, LCD_ENB
    *RE1 - 0, Output, LCD_RW
    *RE2 - 0, Output, LCD_RS
     *************************************************************************/
    TRISE = 0x00;
    ANSELE = 0x00;
    PORTE = 0x00;

/*********************INTERRUPT INITIALIZATION********************************/

    /********************Always place at end of initialization routine********/

    RCONbits.IPEN = 0;              //Disable priority levels on interrupts
    
    INTCON = 0x08;
                        //Bit 7 GIE/GIEH    clear - disable all unmasked interrupts
                        //Bit 6 PEIE/GIEL   clear - disable all peripheral interrupts
                        //Bit 5 TMR0IE      clear - disable TMR0 overflow interrupt
                        //Bit 4 INT0IE      clear - disable INT0 external interrupt
                        //Bit 3 RBIE        set - enable IOCB port change interrupt
                        //Bit 2 TMR0IF      clear - TMR0 register overflow flag
                        //Bit 1 INT0IF      clear - INT0 external interrupt flag
                        //Bit 0 RBIF        clear - Port B IOC interrupt flag bit

    INTCON2 = 0x80;
                        //Bit 7 RBPU        set - PORTB pull-ups are disabled
                        //Bit 6 INTEDG0     clear - interrupt on falling edge
                        //Bit 5 INTEDG1     clear - interrupt on falling edge
                        //Bit 4 INTEDG2     clear - interrupt on falling edge
                        //Bit 3 Unused      clear
                        //Bit 2 TMR0IP      clear - TMR0 Overflow Interrupt Priority bit
                        //Bit 1 Unused      clear
                        //Bit 0 RBIP        clear - RB Port Change Interrupt Priority bit

    INTCON3 = 0x00;
                        //Bit 7 INT2IP      clear - INT2 External Interrupt Priority bit
                        //Bit 6 INT1IP      clear - INT1 External Interrupt Priority bit
                        //Bit 5 Unused      clear
                        //Bit 4 INT2IE      clear - INT2 External Interrupt Enable bit
                        //Bit 3 INT1IE      clear - INT1 External Interrupt Enable bit
                        //Bit 2 Unused      clear
                        //Bit 1 INT2IF      clear - INT2 External Interrupt Flag bit
                        //Bit 0 INT1IF      clear - INT1 External Interrupt Flag bit

    IOCB = 0x30;
                        //Bit 7 IOCB7       clear - Interrupt on change enable bit
                        //Bit 6 IOCB6       clear - Interrupt on change enable bit
                        //Bit 5 IOCB5       set - Interrupt on change enable bit
                        //Bit 4 IOCB4       set - Interrupt on change enable bit
                        //Bit 3 Unused      clear
                        //Bit 2 Unused      clear
                        //Bit 1 Unused      clear
                        //Bit 0 Unused      clear

    Old_Encdr = PORTB & 0x30;                       //Read encoder status & store
    INTCON = 0x88;
}



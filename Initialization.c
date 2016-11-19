/******************************************************************************
*
*EXPRESS RECEIVER BY AD5GH
*IF BOARD SOFTWARE
*INITIALIZATION ROUTINES
*
* Copyright 2015 - 2016 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
*VERSION 4.0
*MARCH 2016
*
******************************************************************************/
#define FOSC            40000000ULL
#define FCY             (FOSC/4)

#include <xc.h>
#include <libpic30.h>
#include "system.h"

extern int AGC_Level_Set;

void Init (void)
{

/*********************PORT PIN DEFINITION & INITIALIZATION********************/

    AD1PCFGL = 0xFFFC;                              //AN0 & AN1 Enabled, all other pins digital I/O
    
    TRISAbits.TRISA3 = 0;                           //RA3, output, CS MCP4822 #2, Post IF Amp
    TRISAbits.TRISA4 = 0;                           //RA4, output, SCK MCP4822 #1 & #2

    TRISBbits.TRISB4 = 0;                           //RB4, output, SDI MCP4822 #1 & #2
    TRISBbits.TRISB7 = 0;                           //RB7, output, CS MCP4822 #1, IF Amp
    ODCBbits.ODCB8 = 1;                             //RB8, SCL1
    ODCBbits.ODCB9 = 1;                             //RB9, SDA1
    TRISBbits.TRISB14 = 0;                          //RB14, output, Green LED
    
    PORTAbits.RA4 = 0;                              //MCP4822 SCK Low


    PORTBbits.RB7 = 1;                              //MCP4822 #1 CS High
    PORTBbits.RB14 = 1;                             //Green LED off

    OSCCONbits.IOLOCK = 0;                          //Map U1TX to RP15
    RPOR7 = 0x0300;
    OSCCONbits.IOLOCK = 1;
    
    //*********************ADC CONFIG******************************************
    
    AD1CON1 = 0x84E4;           //Bit 15 ADON = 1 to enable ADC
                                //Bit 10 AD12B = 1 12-bit, 1-channel ADC operation
                                //Bits 7-5 SSRC = 111, Internal counter ends sampling and starts conversion (auto-convert)
                                //Bit 2 ASAM = 1 Sampling begins immediately after last conversion. SAMP bit is auto-set
                                //Bit 0 DONE = 1 when conversion cycle complete, clear when read and wait for next conversion

    AD1CON2 = 0x0000;           //Bit 15-13 VCFG = 000 ADREF+ = AVdd, ADREF- = AVss
                                //Bit 12-11 unimplemented
                                //Bit 10 CSCNA = 0 Do not scan inputs
                                //Bit 9-8 CHPS = 00 Converts CH0
                                //Bit 7 BUFS Read only
                                //Bit 6 unimplemented
                                //Bit 5-2 SMPI = 0000 Interrupts at the completion of conversion for each sample/convert sequence
                                //Bit 1 BUFM = 0 Always starts filling buffer from the beginning
                                //Bit 0 ALTS = 0 Always uses channel input selects for Sample A

    AD1CON3 = 0x1F3F;           //Bit 15 ADRC = 0 Clock derived from system clock
                                //Bit 14-13 unimplemented
                                //Bit 12-8 SAM = 11111, 31 auto sample time bits
                                //Bit 7-0 ADCS = 00111111, = 64 · TCY = TAD


    //*********************TIMER 1 CONFIG**************************************

    // Tcy = 4/Fcy = 4/40x10-6 = 100nS
    //Period = PR1 * prescaler * Tcy = 39 * 256 * 100ns = 1ms
    //I used 78 for PRI after measuring the interrupt period with a scope, somehwere a factor of 2 is missing in the calculation

    T1CON = 0;                                      // Clear Timer 1 configuration
    T1CONbits.TCKPS = 3;                            // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR1 = 78;                                       // Set Timer 1 period (max value is 65535)
    _T1IP = 1;                                      // Set Timer 1 interrupt priority
    _T1IF = 0;                                      // Clear Timer 1 interrupt flag
    _T1IE = 1;                                      // Enable Timer 1 interrupt
    T1CONbits.TON = 1;                              // Turn on Timer 1

    //*********************UART CONFIG****************************************
    /* Not currently used in Express Receiver*/

    U1BRG = 0x000A;                                 //Baud Rate = 115,200, U1BRG = ((Fcy/115,200)/16) - 1 Fcy = 40-MHz/2 = 20,000,000
    U1MODE = 0x0000;                                //UART Mode register
    IEC0bits.U1TXIE = 0;                            //Disable UART TX Interrupt
    IFS0bits.U1TXIF = 0;                            //Clear the Transmit Interrupt Flag
    U1MODEbits.UARTEN = 0;                          //Disable UART
    U1STAbits.UTXEN = 0;                            //Transmit disabled

    /********************AGC INITIALIZATION***********************************/

    Write_MCP4822 (IF_AMP, DAC_A, AGC_Level_Set);   //Initialize AGC Line level
    Write_MCP4822 (IF_AMP, DAC_B, 0x0000);          //Not used

    /********************I2C INITIALIZATION***********************************/
    
    I2C1CONbits.I2CEN = 1;                          //Enable I2C module
    I2C1ADD = 0x08;                                 //7-bit address, master should use 0x10 to communicate with the device
    IFS1bits.SI2C1IF = 0;                           //Clear I2C interrupt flag
    IEC1bits.SI2C1IE = 1;                           //Enable I2C interrupt
}





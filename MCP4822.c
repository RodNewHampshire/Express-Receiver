/******************************************************************************
*
*EXPRESS RECEIVER BY AD5GH
*IF BOARD SOFTWARE
*MCP4822 DAC ROUTINES
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

/* There are two MCP4822 DACs in the Express Receiver referred to as the "IF DAC" and "POST IF DAC."
 The POST IF DAC is not currently used. Within each MCP4822 there are two DACs referred to as "DAC A"
 and "DAC B." All writes to the MCP4822 are 16 bits as follows:
 
 Bit 15:    DAC Select Bit: 0 = Write DACA / 1 = Write DACB
 Bit 14:    Don't care
 Bit 13:    DAC Gain Set:   0 = 2x / 1 = 1x
 Bit 12:    Output Power Down Control Bit: 0 = Output disabled / 1 = Output enabled
 Bits 11-0  DAC Data bits
 
 Refer to MCP4822 data sheet for further details on configuring and programming the MCP4822.


*/
void Write_MCP4822 (char IF_DAC, char DAC, int value )
{
    
    int config_bits, DAC_bits, test_byte;
    char i;

    if(IF_DAC) PORTB &= ~CS_MCP4822_1;                      //Assert CS low for IF DAC
    else PORTA &= ~CS_MCP4822_2;                            //Else assert CS low for Post IF DAC

    if(DAC) config_bits = 0x1000;                           //Set up configuration bits for DAC A
    else config_bits = 0x9000;                              //Else DAC B
                                                            //Gain = 2x, Output Enabled
    DAC_bits = config_bits + value;                         //Add DAC data bits to get 16 Bits to be written to DAC
    
    for (i=0; i<16; i++)                                    //All writes to the MCP4822 are 16 bits
    {
        test_byte = DAC_bits & 0x8000;                      //Test bit for SDI write of 1 or 0
        if (test_byte == 0) PORTB &= ~SDI_MCP4822;          //Write 0
        else PORTB |= SDI_MCP4822;                          //Write 1
        __delay_us(1);
        PORTA |= SCK_MCP4822;                               //SCK low to high
        __delay_us(1);
        PORTA &= ~SCK_MCP4822;                              //SCK high to low
        __delay_us(1);
        DAC_bits <<= 1;                                     //Shift left for next bit
    }

    if(IF_DAC) PORTB |= CS_MCP4822_1;                       //Assert CS high for IF DAC
    else PORTA |= CS_MCP4822_2;                             //Else assert CS high for Post IF DAC
}


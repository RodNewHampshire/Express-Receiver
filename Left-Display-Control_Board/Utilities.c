/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* LEFT DISPLAY & CONTROL BOARD SOFTWARE
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

void Long_Delay(unsigned int multiple)              // Multiples of 1ms
{
    unsigned int x;

    INTCONbits.GIE = 0;                             //Disable interrupts
    for (x=0; x < multiple; x++) __delay_ms(1);
    INTCON = 0x88;                                  //Re-enable interrupts
}

unsigned char ReadADC_8Bits(unsigned char ADC_Channel)
{
    ADCON0 = (ADC_Channel << 2);                    //Set analog channel select bits
    ADCON0bits.ADON = 1;                            //enable ADC
    ADCON0bits.GO_DONE = 1;                         //Start conversion
    while(ADCON0bits.GO_DONE){}                     //wait for conversion to complete
    ADCON0bits.ADON = 0;                            //disable ADC
    return  ADRESH;                                 //return result
}

long EEPROM_Read_Long(unsigned char addr)
{
    long result;
    char *ptr = (char*)&result;
    char i;

    for (i=0; i<4; i++) *(ptr++) = EEPROM_Read(addr++);

    return result;
}

void EEPROM_Write_Long (unsigned char addr, long data)
{
    char *ptr = (char*)&data;
    char i;

    for( i=0; i<4; i++) EEPROM_Write(addr++, *(ptr++));
}

void EEPROM_Write(unsigned char address, unsigned char data)
{
    unsigned char INTCON_SAVE;

    EEADR  = address;
    EEDATA = data;

    EECON1bits.EEPGD= 0;                            // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0;                            // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.WREN = 1;                            // enable writes to internal EEPROM

    INTCON_SAVE=INTCON;                             // Save INTCON register contants
    INTCON=0;                                       // Disable interrupts, Next two lines SHOULD run without interrupts

    EECON2=0x55;                                    // Required sequence for write to internal EEPROM
    EECON2=0xaa;                                    // Required sequence for write to internal EEPROM

    EECON1bits.WR=1;                                // begin write to internal EEPROM
    INTCON=INTCON_SAVE;                             //Now we can safely enable interrupts if previously used

    Nop();

    while (PIR2bits.EEIF==0)                        //Wait till write operation complete
    {
        Nop();
    }

    EECON1bits.WREN=0;                              // Disable writes to EEPROM on write complete (EEIF flag on set PIR2 )
    PIR2bits.EEIF=0;                                //Clear EEPROM write complete flag. (must be cleared in software. So we do it here)

}


unsigned char EEPROM_Read (unsigned char address)   // This function reads data from address given in internal EEPROM of PIC
{
    EEADR=address;
    EECON1bits.EEPGD= 0;                            // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0;                            // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.RD   = 1;                            // EEPROM Read
    return EEDATA;                                  // return data
}
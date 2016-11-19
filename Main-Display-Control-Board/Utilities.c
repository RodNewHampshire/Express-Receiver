/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* UTILITIES ROUTINES
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

void Utilities_Long_Delay(unsigned int multiple)              // Multiples of 1ms
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

    if( data == EEPROM_Read(address)) return;       //Do not overwrite if data already stored
    
    PORTA &= ~LED_Red;
    
    EEADR  = address;
    EEDATA = data;

    EECON1bits.EEPGD= 0;                            // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0;                            // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.WREN = 1;                            // enable writes to internal EEPROM

    INTCON_SAVE=INTCON;                             // Save INTCON register contents
    INTCON=0;                                       // Disable interrupts, Next two lines SHOULD run without interrupts

    EECON2=0x55;                                    // Required sequence for write to internal EEPROM
    EECON2=0xAA;                                    // Required sequence for write to internal EEPROM

    EECON1bits.WR=1;                                // begin write to internal EEPROM
    INTCON=INTCON_SAVE;                             // Now we can safely enable interrupts if previously used

    Nop();

    while (PIR2bits.EEIF==0)                        // Wait till write operation complete
    {
        Nop();
    }

    EECON1bits.WREN=0;                              // Disable writes to EEPROM on write complete (EEIF flag on set PIR2 )
    PIR2bits.EEIF=0;                                // Clear EEPROM write complete flag. (must be cleared in software. So we do it here)
    PORTA |= LED_Red;

}

unsigned char EEPROM_Read (unsigned char address)   // This function reads data from address given in internal EEPROM of PIC
{
    EEADR=address;
    EECON1bits.EEPGD= 0;                            // 0 = Access data EEPROM memory
    EECON1bits.CFGS = 0;                            // 0 = Access Flash program or DATA EEPROM memory
    EECON1bits.RD   = 1;                            // EEPROM Read
    return EEDATA;                                  // return data
}

void Write_I2C_Byte(char address, char data)
{
    int status;

    myIdleI2C1();
    myStartI2C1();
    myWriteI2C1(address);
    myWriteI2C1(data);
    myStopI2C1();
}


void Write_Two_I2C_Byte(char address, char Byte_1, char Byte_2)
{
    int status;
    char data;

    myIdleI2C1();
    myStartI2C1();

    do                                              //write Relay Board address with R/W bit set to "0" for write
    {
        status = myWriteI2C1(address);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    //clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
        PORTA &= ~LED_Red;                          //turn on error LED
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

    do                                              //write Port 0 data
    {
        status = myWriteI2C1(Byte_1);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    //clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

   do                                               //write Port_1 data
    {
        status = myWriteI2C1(Byte_2);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    //clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

    myStopI2C1();
}

char Read_I2C_Byte(char address)
{
    char data;

    address = (address | 0x01);                 //set I2C R/W bit to "1" for read
    myIdleI2C1();
    myStartI2C1();
    data = SSP1BUF;                              //Clear SSPBUF
    myWriteI2C1(address);
    data = myReadI2C1();
    myNotAckI2C1();
    myStopI2C1();
    return data;
}

void myIdleI2C1(void)
{
    while ( ( SSP1CON2 & 0x1F ) | ( SSP1STATbits.R_W ) )
    continue;
}

void myStopI2C1(void)
{
    SSP1CON2bits.PEN = 1;                       //initiate bus stop condition
}

void myStartI2C1(void)
{
    SSP1CON2bits.SEN = 1;                       //initiate bus start condition
    __delay_ms(2);
}

void myNotAckI2C1(void)
{
    SSP1CON2bits.ACKDT = 1;                     //set acknowledge bit for not ACK
    SSP1CON2bits.ACKEN = 1;                     //initiate bus acknowledge sequence
}

unsigned char myReadI2C1(void)                  //MASTER MODE ONLY
{
    SSP1CON2bits.RCEN = 1;                      //enable master for 1 byte reception
    while ( !SSP1STATbits.BF );                 //wait until byte received
    return ( SSP1BUF );                         //return with read byte
}

signed char myWriteI2C1(unsigned char data_out)
{
  SSP1BUF = data_out;                           //write single byte to SSP1BUF
  if ( SSP1CON1bits.WCOL ) return ( -1 );       //test if write collision occurred, if WCOL bit is set return negative #

  else
    {
    while( SSP1STATbits.BF );                   //wait until write cycle is complete
    myIdleI2C1();                               //ensure module is idle
    if(SSP1CON2bits.ACKSTAT)                    //test for ACK condition received
        return ( -2 );				//Return NACK
    else return ( 0 );                          //Return ACK
    }
}
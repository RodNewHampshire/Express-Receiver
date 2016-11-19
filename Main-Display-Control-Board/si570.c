/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* Si570 ROUTINES
*
* Copyright 2016 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 16.0
* FEBRUARY 2016
*
******************************************************************************/

/* Si570 model no. CCC000107G
 *
 * C - 3.3V CMOS, Output Enable Polarity High
 * C - Temperature stability +/-7ppm, total stability +/-20ppm
 * C - 10- to 160-MHz
 * 0001070 - 10.000-MHz start up frequency
 * G - Operating temperature range -40 to +85 deg C
 */

#include <xc.h>
#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Index to arrays:
//0=160M, 1=80M, 2=40M, 3=30M, 4=20M, 5=17M, 6=15M, 7=12M, 8=10Ma, 9=10Mb, 10=10Mc

//VFO SI570 BAND DCO CENTER FREQUENCY CONSTANTS
const char Si_Reg_13[11] = {0xA1, 0xA1, 0xE1, 0xE1, 0xE1, 0x61, 0xA1, 0xA1, 0xA1, 0xA1, 0xA1};
const char Si_Reg_14[11] = {0xC2, 0xC2, 0x42, 0x42, 0x43, 0xC2, 0x42, 0x42, 0x42, 0x42, 0x42};
const char Si_Reg_15[11] = {0xD9, 0xEC, 0xCD, 0xE8, 0x0D, 0xB6, 0xB5, 0xD1, 0xEA, 0xEE, 0xF2};
const char Si_Reg_16[11] = {0x80, 0x27, 0x3B, 0xBA, 0xED, 0x91, 0x42, 0x59, 0x60, 0x37, 0xC1};
const char Si_Reg_17[11] = {0xF0, 0xE8, 0x91, 0x58, 0x66, 0xD6, 0x02, 0x80, 0xFF, 0xAE, 0x29};
const char Si_Reg_18[11] = {0x65, 0xA7, 0x73, 0xC7, 0x75, 0x25, 0xE5, 0xC0, 0x03, 0x98, 0x41};

const long RFREQ_Center[11] = {0xD980F058, 0xEC27E89A, 0xCD3B9167, 0xE8BA58BA, 0x0E288CA7, 0xB691D619, 0xB54202D9,
                                0xD15980B4, 0xEA60FEF6, 0xEE37AE8C, 0xF2C12934};

const unsigned long Band_Center[11] = {1900000, 3750000, 7150000, 10125000, 14175000, 18118000, 21225000, 24940000, 28250000,
                                        28757640, 29357640};

//TUNING OFFSETS FOR 1000-, 100-, AND 10-Hz
const unsigned long Inc_1000Hz[11] = {0x0002949B, 0x00025D8E, 0x00025D8E, 0x00025D8E, 0x00025D8E, 0x000201CE, 0x0001EF74, 0x0001EF74, 0x0001EF74, 0x0001EF74, 0x0001EF74};
const unsigned long Inc_100Hz[11] = {0x00004213, 0x00004213, 0x00003C91, 0x00003C91, 0x00003C91, 0x00003364, 0x0000318E, 0x0000318E, 0x0000318E, 0x0000318E, 0x0000318E};
const unsigned long Inc_10Hz[11] = {0x0000069B, 0x0000069B, 0x0000060E, 0x0000060E, 0x0000060E, 0x00000523, 0x000004F4, 0x000004F4, 0x000004F4, 0x000004F4, 0x000004F4};

//LO SI570 FREQUENCY CONSTANTS
const char Si570_LO[6] = {0x62, 0x43, 0x0A, 0xD7, 0xB3, 0x9D};

char RFREQ[6];
char LO_CAL[12], VFO_CAL[12];

long Cal_Offset[11] = {0, 0, 0, 20, -70, 0, 0, 0, 0, 0, 0};

char Current_Band = 0xFF;

void Si570_VFO_Update(unsigned long Freq, char Band)
{
    unsigned long Freq_Offset, New_RFREQ;
    char NEG_OFFSET_FLAG = 0;
    char thousands, hundreds, tens, n;

    extern int TuningRate;

    if(Band != Current_Band)                        //UPDATE DCO CENTER FREQUENCY IF BAND CHANGED
    {
        //put audio mute code here to mute receiver during band change
        Current_Band = Band;
        Band_Change(Band);
    }

    Freq = Freq + Cal_Offset[Band];
                                                    //DETERMINE IF NEW FREQUENCY IS ABOVE OR BELOW CENTER FREQUENCY
    if(Freq >= Band_Center[Band]) 
    {
        Freq_Offset = Freq - Band_Center[Band];
        NEG_OFFSET_FLAG = 0;
    }
    else if(Freq < Band_Center[Band])
    {
        Freq_Offset = Band_Center[Band] - Freq;
        NEG_OFFSET_FLAG = 1;
    }
                                                    //CALCULATE REQUIRED SI570 RFREQ OFFSET
    thousands = 0;
    while(Freq_Offset >= 1000)
    {
        thousands++;
        Freq_Offset = Freq_Offset - 1000;
    }

    hundreds = 0;
    while(Freq_Offset >= 100)
    {
        hundreds++;
        Freq_Offset = Freq_Offset - 100;
    }

    tens = 0;
    while(Freq_Offset != 0)
    {
        tens++;
        Freq_Offset = Freq_Offset - 10;
    }

    Freq_Offset =   (Inc_1000Hz[Band] * thousands) +
                    (Inc_100Hz[Band] * hundreds) +
                    (Inc_10Hz[Band] * tens);

    if(NEG_OFFSET_FLAG) New_RFREQ = RFREQ_Center[Band] - Freq_Offset;
    else New_RFREQ = RFREQ_Center[Band] + Freq_Offset;

    for(n=0; n<4; n++) RFREQ[n] = (char)(New_RFREQ >> 8*n);
                                                    //explicitly truncating long New_RFREQ to char
                                                    //RFREQ[0] = Reg 18
                                                    //RFREQ[1] = Reg 17
                                                    //RFREQ[2] = Reg 16
                                                    //RFREQ[3] = Reg 15
    Write_Si570_Byte(I2C_Si570, 135, 0x20);
    for(n=3; n>0; n--) Write_Si570_Byte(I2C_Si570, 18-n, RFREQ[n]);
    Write_Si570_Byte(I2C_Si570, 135, 0x00);
}

void Band_Change(char New_Band)                     //SET DCO CENTER FREQUENCY FOR NEW BAND
{
    char Reg137, n;
    Reg137 = Read_Si570_Byte(I2C_Si570, 137);
    Write_Si570_Byte(I2C_Si570, 137, Reg137 | 0x10);//Set DCO Freeze for reg 13 - 18 update
    Write_Si570_Byte(I2C_Si570, 13, Si_Reg_13[New_Band]);
    Write_Si570_Byte(I2C_Si570, 14, Si_Reg_14[New_Band]);
    RFREQ[3] = Si_Reg_15[New_Band];
    RFREQ[2] = Si_Reg_16[New_Band];
    RFREQ[1] = Si_Reg_17[New_Band];
    RFREQ[0] = Si_Reg_18[New_Band];
    for(n=3; n>0; n--) Write_Si570_Byte(I2C_Si570, 18-n, RFREQ[n]);
    Reg137 = Read_Si570_Byte(I2C_Si570, 137);
    Write_Si570_Byte(I2C_Si570, 137, Reg137 & 0xEF);//Clear DCO Freeze
    Write_Si570_Byte(I2C_Si570, 135, 0x40);         //Set the NewFreq bit to update frequency
}

void Si570_LO_Set (void)                            //SET LO FREQUENCY
{
    char Reg137, n;
    PORTA |= Si570_SEL;                             //select Si570 LO
    Reg137 = Read_Si570_Byte(I2C_Si570, 137);
    Write_Si570_Byte(I2C_Si570, 137, Reg137 | 0x10);//Set DCO Freeze for reg 13 - 18 update
    for(n=0; n<6; n++) Write_Si570_Byte(I2C_Si570, 13+n, Si570_LO[n]);
    Reg137 = Read_Si570_Byte(I2C_Si570, 137);
    Write_Si570_Byte(I2C_Si570, 137, Reg137 & 0xEF);//Clear DCO Freeze
    Write_Si570_Byte(I2C_Si570, 135, 0x40);         //Set the NewFreq bit to update frequency
}

char Read_Si570_Byte(char Si570_address, char Si570_reg)
{
    int status;
    char data;

    myIdleI2C1();
    myStartI2C1();

    do                                              //write Si570 address with R/W bit set to "0" for write
    {
        status = myWriteI2C1(Si570_address);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    //clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

    do                                              //write Si570 register address
    {
        status = myWriteI2C1(Si570_reg);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    //clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

    myStartI2C1();

    do                                              //write Si570 address with R/W bit set to "1" for read
    {
        status = myWriteI2C1(Si570_address | 0x01);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    // clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED
    
    data = myReadI2C1();
    myNotAckI2C1();                                 //send NACK to complete read
    myStopI2C1();
    return data;
}


void Write_Si570_Byte(unsigned char Si570_address, unsigned char Si570_reg, unsigned char data)
{
    int status;

    myIdleI2C1();
    myStartI2C1();

    do                                              //write Si570 address with R/W bit set to "0" for write
    {
        status = myWriteI2C1(Si570_address);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    //clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

    do                                              //write Si570 register address
    {
        status = myWriteI2C1(Si570_reg);
        if(status == -1)                            //check if bus collision happened
        {
            data = SSP1BUF;                         //upon bus collision detection clear the buffer,
            SSP1CON1bits.WCOL=0;                    // clear the bus collision status bit
            PORTA &= ~LED_Red;                      //turn on error LED
        }
    }
    while(status!=0);
    PORTA |= LED_Red;                               //extinguish error LED

   do                                               //write data to the Si570
    {
        status = myWriteI2C1( data );
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

void Get_10MHz_RFREQ(void)
{
    char n;
    
    PORTA |= Si570_SEL;                             //Select LO
    for(n=13; n<19; n++)RFREQ[n-13] = Read_Si570_Byte(I2C_Si570, n);
    LO_CAL[11] = (RFREQ[0] & 0xF0)>>4;
    LO_CAL[10] = (RFREQ[0] & 0x0F);
    LO_CAL[9] = (RFREQ[1] & 0xF0)>>4;
    LO_CAL[8] = (RFREQ[1] & 0x0F);
    LO_CAL[7] = (RFREQ[2] & 0xF0)>>4;
    LO_CAL[6] = (RFREQ[2] & 0x0F);
    LO_CAL[5] = (RFREQ[3] & 0xF0)>>4;
    LO_CAL[4] = (RFREQ[3] & 0x0F);
    LO_CAL[3] = (RFREQ[4] & 0xF0)>>4;
    LO_CAL[2] = (RFREQ[4] & 0x0F);
    LO_CAL[1] = (RFREQ[5] & 0xF0)>>4;
    LO_CAL[0] = (RFREQ[5] & 0x0F);

    for(n=0; n<12; n++)
    {
        char temp;

        temp = LO_CAL[n];
        if(temp <= 0x09) LO_CAL[n] = temp + 0x30;
        else LO_CAL[n] = temp + 0x37;
    }

    PORTA &= ~Si570_SEL;                             //Select VFO
    for(n=13; n<19; n++)RFREQ[n-13] = Read_Si570_Byte(I2C_Si570, n);
    VFO_CAL[11] = (RFREQ[0] & 0xF0)>>4;
    VFO_CAL[10] = (RFREQ[0] & 0x0F);
    VFO_CAL[9] = (RFREQ[1] & 0xF0)>>4;
    VFO_CAL[8] = (RFREQ[1] & 0x0F);
    VFO_CAL[7] = (RFREQ[2] & 0xF0)>>4;
    VFO_CAL[6] = (RFREQ[2] & 0x0F);
    VFO_CAL[5] = (RFREQ[3] & 0xF0)>>4;
    VFO_CAL[4] = (RFREQ[3] & 0x0F);
    VFO_CAL[3] = (RFREQ[4] & 0xF0)>>4;
    VFO_CAL[2] = (RFREQ[4] & 0x0F);
    VFO_CAL[1] = (RFREQ[5] & 0xF0)>>4;
    VFO_CAL[0] = (RFREQ[5] & 0x0F);

    for(n=0; n<12; n++)
    {
        if(VFO_CAL[n] <= 0x09) VFO_CAL[n] = VFO_CAL[n] + 0x30;
        else VFO_CAL[n] = VFO_CAL[n] + 0x37;
    }
}
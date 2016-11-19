/******************************************************************************
*
*EXPRESS RECEIVER BY AD5GH
*IF BOARD SOFTWARE
*
* Copyright 2015 - 2016 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
*VERSION 4.0
*MARCH 2016
*
******************************************************************************/

#include <xc.h>
#include <p33FJ12GP201.h>
#include <libpic30.h>
#include "system.h"
#include <math.h>

#pragma config GWRP = OFF                                               //General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF                                                //General Segment Code-Protect bit (General Segment Code protect is disabled)

#pragma config FNOSC = PRI                                              //Primary oscillator, no PLL
#pragma config IESO = OFF                                               //Start up device with primary oscillator

// FOSC
#pragma config POSCMD = EC                                              //Primary oscillator mode = External Clock (EC)
#pragma config OSCIOFNC = ON                                            //OSC2 Pin 2 function general purpose I/O
#pragma config IOL1WAY = OFF                                            //Peripheral pin select, allow multiple reconfigurations
#pragma config FCKSM = CSDCMD                                           //Clock switching disabled, fail-safe clock monitor disabled

#pragma config FWDTEN = OFF                                             //WDT enabled/disabled by user software; default state is disabled

#pragma config ICS = PGD1                                               //ICD communication channel select bits

#define AGC_Recovery_Rate    1
#define AGC_Attack_Rate     10

char S_Meter_Reading;

char Manual_AGC = 0;

int AGC_Set_Point = 1800;                                               //Desired signal level for driving Diode Ring Mixer / Product Detector; target AGC line level = 1.8V (1800 = (1.7 x 4069)/(2*2.048))
int AGC_Level_Set = 1600;                                               //Starting level set; starting AGC line level = 1.6V (1600 = (1.6 x 4069)/(2*2.048))
int AGC_Error;

int Hang_Time = 2000;
int Slow_AGC = 2000;
int Fast_AGC = 100;
int Decay = 0;

int Man_AGC_ADC_Level[19] = {1958, 1972, 1988, 2010, 2056, 2115, 2172,  //IF Gain ADC values corresponding to -45dB to +45dB in 5dB steps
        2229, 2286, 2344, 2413, 2489, 2566, 2649, 2742, 2837,
        2937, 3047, 3166};

int main(void)
{
    Init();

    while (1)
    {
    }
}

void Execute(unsigned char command)
{
    switch(command & 0x03)                                              //Extract AGC command
    {
        case SLOW:
            Manual_AGC = 0;
            Hang_Time = Slow_AGC;
            break;
            
        case FAST:
            Manual_AGC = 0;
            Hang_Time = Fast_AGC;
            break;
            
        case MAN:
            Manual_AGC = 1;
            AGC_Level_Set = Man_AGC_ADC_Level[(command >> 3)];          //Extract MAN AGC index and lookup AGC level
            Write_MCP4822 (IF_AMP, DAC_A, AGC_Level_Set);               //Set manual AGC level
            break;
        
        case AGC_SLW_REC:                                               //Set AGC slow recovery rate
            Slow_AGC = (command >> 3);
            break;

        case AGC_FST_REC:                                               //Set AGC fast recovery rate
            Fast_AGC = (command >> 3);                        
            break;

        default:
            break;
    }
}


/********************Interrupt Routines********************************/

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;                   

    if(Manual_AGC) return;                                              //If manual AGC, we're all done

    AGC_Error = AGC_Set_Point - ADC1BUF0;                               //Determine error versus set point
                                                                        //Range of AD8307 output in terms of ADC1 12 bit values is 620 - 3100
    
    if(Decay == 0)                                                      //If decay time not equal to zero, do nothing, wait for decay time to get to zero
    {
        if(AGC_Error > 2)                                               //Signal level below Set Point
        {
            AGC_Level_Set = AGC_Level_Set + AGC_Recovery_Rate;          //Increase AGC gain at the recovery rate
            if(AGC_Level_Set > 3200) AGC_Level_Set = 3200;              //Max AGC level is 3.2V
            Write_MCP4822 (IF_AMP, DAC_A, AGC_Level_Set);
        }
    }
    else Decay--;
    
    if(AGC_Error < -100)                                                //Signal level well above Set Point
    {
        AGC_Level_Set = AGC_Level_Set - (6*AGC_Attack_Rate);            //Decrease AGC gain at 6x AGC Attack Rate
        if(AGC_Level_Set < 1600) AGC_Level_Set = 1600;                  //Min AGC level is 1.6V
        Write_MCP4822 (IF_AMP, DAC_A, AGC_Level_Set);
        Decay = Hang_Time;
    }

    if(AGC_Error < -20)                                                 //Signal level above Set Point
    {
        AGC_Level_Set = AGC_Level_Set - AGC_Attack_Rate;                //Decrease AGC gain at AGC Attack Rate
        if(AGC_Level_Set < 1600) AGC_Level_Set = 1600;                  //Min AGC level is 1.6V
        Write_MCP4822 (IF_AMP, DAC_A, AGC_Level_Set);
        Decay = Hang_Time;
    }
    
    S_Meter_Reading = (AGC_Level_Set - 1600) >> 3;                      //Set zero reference and divide by 8 to get to single byte S-meter reading
}


void __attribute__((interrupt,no_auto_psv)) _SI2C1Interrupt(void)
{
    unsigned char temp;
    
    if((I2C1STATbits.R_W == 0)&&(I2C1STATbits.D_A == 0))                //Address matched
    {
        temp = I2C1RCV;                                                 //dummy read to clear register
    }
    else if((I2C1STATbits.R_W == 0)&&(I2C1STATbits.D_A == 1))           //Main Display & Control Board sending commands
    {
        temp = I2C1RCV;
        Execute(temp);
    }

    else if((I2C1STATbits.R_W == 1)&&(I2C1STATbits.D_A == 0))           //Main Display & Control Board requesting S Meter reading
    {
        PORTBbits.RB14 = !PORTBbits.RB14;                               //Toggle Green LED on S Meter read
        temp = I2C1RCV;
        I2C1TRN = S_Meter_Reading;
        I2C1CONbits.SCLREL = 1;
        while(I2C1STATbits.TBF){}
    }

    IFS1bits.SI2C1IF = 0;
}


/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* VFO ROUTINES
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
#include <math.h>

extern unsigned long FreqUpdateAccum;
extern char FreqUpdateFlag;

char VFO_Flag;                                      //Flag = 0 VFO A, Flag = 1 VFO B
char DH_Flag = 0;                                   //Flag = 1 Dial Hold
char Band_Change_Flag = 0;

unsigned long VFOA_Band_Limits[2], VFOB_Band_Limits[2];


/*********************VFO FREQUENCY UPDATE ROUTINE****************************/
/*0=160M, 1=80M, 2=40M, 3=30M, 4=20M, 5=17M, 6=15M, 7=12M, 8=10aM, 9=10bM, 10=10cM*/


void VFO_Freq_Update (void)
{
    long long FreqData;

    if(DH_Flag) return;
    
    if(!VFO_Flag)                                   //Update VFO A
    {
        if(Band_Change_Flag)
        {
            Last_Band_Data_A[VFOA_Last_Band].Freq = VFO_A[0].Freq;          //Save current band frequency & mode
            Last_Band_Data_A[VFOA_Last_Band].Mode = VFO_A[0].Mode;

            VFO_A[0].Freq = Last_Band_Data_A[VFO_A[0].Band].Freq;           //Load new band frequency & mode
            VFO_A[0].Mode = Last_Band_Data_A[VFO_A[0].Band].Mode;

            VFO_Set_Band_Limits((long*)&VFOA_Band_Limits, VFO_A[0].Band);
            VFOA_Last_Band = VFO_A[0].Band;
            RelayBoard_BPF_Select(VFO_A[0].Band);
            Write_I2C_Byte(Right_Display, (Band_Cmnd+VFO_A[0].Band));
            __delay_ms(2);
            Write_I2C_Byte(Right_Display, (Mode_Cmnd+VFO_A[0].Mode));
            __delay_ms(20);
            Band_Change_Flag = 0;
        }

        FreqData = VFO_A[0].Freq + FreqUpdateAccum;
        if(FreqData < VFOA_Band_Limits[LOWER]) FreqData = VFOA_Band_Limits[LOWER];
        if(FreqData > VFOA_Band_Limits[UPPER]) FreqData = VFOA_Band_Limits[UPPER];
        VFO_A[0].Freq = FreqData;
        Si570_VFO_Update(VFO_A[0].Freq, VFO_A[0].Band);
    }

    else if(VFO_Flag)                               //Update VFO B
    {
        if(Band_Change_Flag)
        {
            Last_Band_Data_B[VFOB_Last_Band].Freq = VFO_B[0].Freq;
            Last_Band_Data_B[VFOB_Last_Band].Mode = VFO_B[0].Mode;

            VFO_B[0].Freq = Last_Band_Data_B[VFO_B[0].Band].Freq;
            VFO_B[0].Mode = Last_Band_Data_B[VFO_B[0].Band].Mode;

            VFO_Set_Band_Limits((long*)&VFOB_Band_Limits, VFO_B[0].Band);
            VFOB_Last_Band = VFO_B[0].Band;
            RelayBoard_BPF_Select(VFO_B[0].Band);
            Write_I2C_Byte(Right_Display, (Band_Cmnd+VFO_B[0].Band));
            __delay_ms(2);
            Write_I2C_Byte(Right_Display, (Mode_Cmnd+VFO_B[0].Mode));
            __delay_ms(20);
            Band_Change_Flag = 0;
        }

        FreqData = VFO_B[0].Freq + FreqUpdateAccum;
        if(FreqData < VFOB_Band_Limits[LOWER]) FreqData = VFOB_Band_Limits[LOWER];
        if(FreqData > VFOB_Band_Limits[UPPER]) FreqData = VFOB_Band_Limits[UPPER];
        VFO_B[0].Freq = FreqData;
        Si570_VFO_Update(VFO_B[0].Freq, VFO_B[0].Band);
    }

    FreqUpdateAccum = 0;
    FreqUpdateFlag = 0;
    LCD_Freq_Disp_Update();
}


void VFO_Set_Band_Limits(long *Band_Limit_Array, char Band_Number)
{
    switch(Band_Number)
    {
        case 0:
            *(Band_Limit_Array) = Bottom_160M;
            *(Band_Limit_Array + 1) = Top_160M;
            break;

        case 1:
            *(Band_Limit_Array) = Bottom_80M;
            *(Band_Limit_Array + 1) = Top_80M;
            break;

        case 2:
            *(Band_Limit_Array) = Bottom_40M;
            *(Band_Limit_Array + 1) = Top_40M;
            break;

        case 3:
            *(Band_Limit_Array) = Bottom_30M;
            *(Band_Limit_Array + 1) = Top_30M;
            break;

        case 4:
            *(Band_Limit_Array) = Bottom_20M;
            *(Band_Limit_Array + 1) = Top_20M;
            break;

        case 5:
            *(Band_Limit_Array) = Bottom_17M;
            *(Band_Limit_Array + 1) = Top_17M;
            break;

        case 6:
            *(Band_Limit_Array) = Bottom_15M;
            *(Band_Limit_Array + 1) = Top_15M;
            break;

        case 7:
            *(Band_Limit_Array) = Bottom_12M;
            *(Band_Limit_Array + 1) = Top_12M;
            break;

        case 8:
            *(Band_Limit_Array) = Bottom_10aM;
            *(Band_Limit_Array + 1) = Top_10aM;
            break;
            
        case 9:
            *(Band_Limit_Array) = Bottom_10bM;
            *(Band_Limit_Array + 1) = Top_10bM;
            break;
            
        case 10:
            *(Band_Limit_Array) = Bottom_10cM;
            *(Band_Limit_Array + 1) = Top_10cM;
            break;
    }
}

void VFO_INIT(void)
{
    Si570_LO_Set();                                 //Set Si570 LO frequency
    PORTA &= ~Si570_SEL;                            //select Si570 VFO
    
    VFO_Flag = 1;                                   //initialize VFO B
    Band_Change_Flag = 1;
    VFO_Freq_Update();
    
    VFO_Flag = 0;                                   //start on VFO A
    Band_Change_Flag = 1;                           //force band limits to initialize
    VFO_Freq_Update();                              //Initialize VFO
}

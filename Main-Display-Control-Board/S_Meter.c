/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* S_METER ROUTINES
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

void S_Meter_Get_Reading(void)
{
    int Sig_Strength;

    unsigned char S_Mtr_Cmnd = 0x06;

    __delay_ms(2);
    Sig_Strength = Read_I2C_Byte(IF_Board);

    if(Sig_Strength > 199) S_Mtr_Cmnd = S_Mtr_Cmnd+0x00;                                //S0
    else if(Sig_Strength <= 199 && Sig_Strength > 194) S_Mtr_Cmnd = S_Mtr_Cmnd+0x08;    //S1
    else if(Sig_Strength <= 194 && Sig_Strength > 188) S_Mtr_Cmnd = S_Mtr_Cmnd+0x10;    //S2
    else if(Sig_Strength <= 188 && Sig_Strength > 172) S_Mtr_Cmnd = S_Mtr_Cmnd+0x18;    //S3
    else if(Sig_Strength <= 172 && Sig_Strength > 158) S_Mtr_Cmnd = S_Mtr_Cmnd+0x20;    //S4
    else if(Sig_Strength <= 158 && Sig_Strength > 143) S_Mtr_Cmnd = S_Mtr_Cmnd+0x28;    //S5
    else if(Sig_Strength <= 143 && Sig_Strength > 130) S_Mtr_Cmnd = S_Mtr_Cmnd+0x30;    //S6
    else if(Sig_Strength <= 130 && Sig_Strength > 117) S_Mtr_Cmnd = S_Mtr_Cmnd+0x38;    //S7
    else if(Sig_Strength <= 117 && Sig_Strength > 106) S_Mtr_Cmnd = S_Mtr_Cmnd+0x40;     //S8
    else if(Sig_Strength <= 106 && Sig_Strength > 88) S_Mtr_Cmnd = S_Mtr_Cmnd+0x48;      //S9
    else if(Sig_Strength <= 88 && Sig_Strength > 73) S_Mtr_Cmnd = S_Mtr_Cmnd+0x50;      //+10
    else if(Sig_Strength <= 73 && Sig_Strength > 58) S_Mtr_Cmnd = S_Mtr_Cmnd+0x58;      //+20
    else if(Sig_Strength <= 58 && Sig_Strength > 45) S_Mtr_Cmnd = S_Mtr_Cmnd+0x60;      //+30
    else if(Sig_Strength <= 45 && Sig_Strength > 41) S_Mtr_Cmnd = S_Mtr_Cmnd+0x68;      //+40
    else if(Sig_Strength <= 41 && Sig_Strength > 40) S_Mtr_Cmnd = S_Mtr_Cmnd+0x70;      //+50
    else if(Sig_Strength <= 40) S_Mtr_Cmnd = S_Mtr_Cmnd+0x78;                           //+60

    __delay_ms(2);
    Write_I2C_Byte(Left_Display, S_Mtr_Cmnd);
}

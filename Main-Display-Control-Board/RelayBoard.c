/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* RELAY BOARD ROUTINES
*
* Copyright 2016 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 16.0
* FEBRUARY 2016
*
******************************************************************************/

/* PCF8575C I2C I/O EXPANDER
 *
 *                                              BITS
 *                      7       6       5       4       3       2       1       0
 *
 * I2C Slave Address    L       H       L       L       A2      A1      A0      R/W
 *
 * POx I/O Data Bus     P07     P06     PO5     PO4     PO3     PO2     PO1     PO0
 *
 * p1X I/O Data Bus     P17     P16     P15     P14     P13     P12     P11     P10
 *
 * 7-Bit 12C address plus R/W bit
 */


/********************RELAY BOARD 1 PORT ADDRESSING***************/

/*
 *  Relay Board 1
 *
 * Band Pass Filter select
 *  P0_0    15M                 0x0001
 *  PO_1    30M                 0x0002
 *  PO_2    160M                0x0004
 *  PO_3    17M                 0x0008
 *  PO_4    20M                 0x0010
 *  PO_5    80M                 0x0020
 *  PO_6    12M                 0x0040
 *  PO_7    10M                 0x0080
 *  P1_7    40M                 0x8000
 *
 * RF Attenuator select
 *  P1_3    2dB                 0x0800
 *  P1_4    4dB                 0x1000
 *  P1_5    8dB                 0x2000
 *  P1_6    16dB                0x4000
 *
 * RF Preamp select
 *  P1_2    ON                  0x0400
 *
 *  P1_1    Spare               0x0200
 *  P1_0    Spare               0x0100
 *
*/


/********************RELAY BOARD 2 PORT ADDRESSING***************/

/*
 *  Relay Board 2
 *
 * IF Filter select, IF Filter Board #1
 *  P0_0    1800-Hz             0x0001
 *  PO_1    2100-Hz             0x0002
 *  PO_2    Not used            0x0004
 *
 * IF Filter select, IF Filter Board #2
 *  PO_3    250-Hz              0x0008
 *  PO_4    500-Hz              0x0010
 *  PO_5    Not used            0x0020
 *
 *  PO_6    Spare               0x0040
 *  PO_7    Spare               0x0080
 *  P1_7    Spare               0x8000
 *
 *  P1_3    Spare               0x0800
 *  P1_4    Spare               0x1000
 *  P1_5    Spare               0x2000
 *  P1_6    Spare               0x4000
 *
 *  P1_2    Spare               0x0400
 *
 *  P1_1    Spare               0x0200
 *  P1_0    Spare               0x0100
 *
*/

#include <xc.h>
#include "system.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int BPF[11] = {0x0004, 0x0020, 0x8000, 0x0002, 0x0010, 0x0008, 0x0001, 0x0040, 0x0080, 0x0080, 0x0080};
const int IF_BW[4] = {0x0002, 0x0001, 0x0010, 0x0008};

char Rly_Brd1_P0, Rly_Brd1_P1;

char Saved_Band, Saved_Attn = 0, Saved_BW = 5;

void RelayBoard_BPF_Select(char band)
{
    char P0, P1;

    Saved_Band = band;
    P0 = BPF[band];
    P1 = (BPF[band]>>8) + Saved_Attn;
    Write_Two_I2C_Byte(Relay_Driver_1, P0, P1);
}

void RelayBoard_ATTN_Select(char attn)
{
    char P0, P1;

    if(attn == Saved_Attn)return;
    Saved_Attn = attn;
    P0 = BPF[Saved_Band];
    P1 = (BPF[Saved_Band]>>8) + attn;
    Write_Two_I2C_Byte(Relay_Driver_1, P0, P1);
}

void RelayBoard_IF_BW_Select(char BW)
{
    char P0, P1;

    if(BW == Saved_BW)return;
    Saved_BW = BW;
    P0 = IF_BW[Saved_BW];
    P1 = (IF_BW[Saved_BW]>>8);
    Write_Two_I2C_Byte(Relay_Driver_2, P0, P1);
}
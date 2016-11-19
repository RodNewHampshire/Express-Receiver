/******************************************************************************
*
*EXPRESS RECEIVER BY AD5GH
*IF BOARD SOFTWARE
*HEADER FILE
*
* Copyright 2015 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
*VERSION 3.0
*DECEMBER 2015
*
******************************************************************************/


/**********FUNCTION PROTOTYPES**********/

void Init(void);

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void __attribute__((interrupt,no_auto_psv)) _SI2C1Interrupt(void);

void Write_MCP4822 (char, char, int);
void Execute(unsigned char);

/**********DEFINESS*********************/

#define _XTAL_FREQ      40000000

#define UPPER           1
#define LOWER           0

#define IF_AMP          1
#define POST_IF_AMP     0

#define DAC_A           1
#define DAC_B           0

#define SLOW            0
#define FAST            1
#define MAN             2
#define AGC_SLW_REC     3
#define AGC_FST_REC     4
#define AGC_ATCK_RATE   5

/********************PORTA*************************************************

    *RA0 -  Analog Input, IF Sense
    *RA1 -  Analog Input, external Post IF Sense
    *RA3 -  0, Output, CS MCP4822 #2, Post IF Amp
    *RA4 -  0, Output, SCK MCP4822 #1 & #2

    *****************PORTB*************************************************

    *RB4 -  0, Output, SDI MCP4822 #1 & #2
    *RB7 -  0, Output, CS MCP4822 #1, IF Amp
    *RB8 -  SCL1
    *RB9 -  SDA1
    *RB14 - 0, Output, LED_Green
    *RB15 - U1TX

     *************************************************************************/

#define IF_Sense            0x0001
#define Post_IF_Sense       0x0002
#define CS_MCP4822_2        0x0008
#define SCK_MCP4822         0x0010

#define SDI_MCP4822         0x0010
#define CS_MCP4822_1        0x0080
#define SCL                 0x0100
#define SDA                 0x0200
#define LED_Green           0x4000
#define S_Meter             0x8000


#define I2C_ADDRESS         0x2A


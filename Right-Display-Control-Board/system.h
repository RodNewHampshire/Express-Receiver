/******************************************************************************
*
* EXPRESS RECEIVER BY AD5GH
* (http://www.expressreceiver.com)
* RIGHT DISPLAY & CONTROL BOARD SOFTWARE
* HEADER FILE
*
* Copyright 2015 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 4.0
* DECEMBER 2015
*
******************************************************************************/


/**********FUNCTION PROTOTYPES**********/
void InterruptHandlerHigh (void);

void Init (void);

void Execute (char);

void Disp_Update (void);
void LCD_INIT(void);
void LCD_PUTC (unsigned char C);
void LCD_PUTD(unsigned char C);

void Long_Delay(unsigned int);
unsigned char ReadADC_8Bits(unsigned char);
long EEPROM_Read_Long(unsigned char);
void EEPROM_Write_Long (unsigned char, long);
unsigned char EEPROM_Read(unsigned char);
void EEPROM_Write(unsigned char, unsigned char);

void BFO_DDS_Write (unsigned int);
void BFO_DDS_CW(void);
void BFO_DDS_LSB(void);
void BFO_DDS_USB(void);


#define _XTAL_FREQ      25000000

#define UPPER           1
#define LOWER           0

/*PIC I/O DEFINITIONS*/
#define LED_Red             0x40
#define LED_Green           0x04
#define Keypad_Cols         0x3E
#define KP_Col_1            0x02
#define KP_Col_2            0x04
#define KP_Col_3            0x08
#define KP_Col_4            0x10
#define KP_Col_5            0x20

#define KP_Row_1            0x01
#define KP_Row_2            0x02
#define KP_Row_3            0x04
#define EncoderA            0x10
#define EncoderB            0x20

#define RS485_DE            0x01
#define RS485_RE            0x02
#define DDS_MRESET          0x04
#define RC5                 0x10

#define LCD_DB0             0x01
#define LCD_DB1             0x02
#define LCD_DB2             0x04
#define LCD_DB3             0x08
#define LCD_DB4             0x10
#define LCD_DB5             0x20
#define LCD_DB6             0x40
#define LCD_DB7             0x80

#define PB_1                0x20
#define PB_2                0x10
#define PB_3                0x08
#define PB_4                0x04
#define PB_5                0x01

#define LCD_ENB             0x01
#define LCD_RW              0x02
#define LCD_RS              0x04

#define MUTE_LINE           0x04
#define DDS_CLK             0x08
#define DDS_DATA            0x10
#define DDS_FSYNC           0x20

#define I2C_ADDRESS         0x2A

const char IFBW_1[4] = {'2', '1', ' ', ' '};
const char IFBW_2[4] = {'1', '8', '5', '2'};
const char IFBW_3[4] = {'0', '0', '0', '5'};
const char IFBW_4[4] = {'0', '0', '0', '0'};

const char Band_1[11] = {'1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '1', '1', '1'};
const char Band_2[11] = {'6', '8', '4', '3', '2', '1', '1', '1', '0', '0', '0'};
const char Band_3[11] = {'0', '0', '0', '0', '0', '7', '5', '2', 'a', 'b', 'c'};

const char Mode_1[3] = {'U', 'L', ' '};
const char Mode_2[3] = {'S', 'S', 'C'};
const char Mode_3[3] = {'B', 'B', 'W'};

const char AGC_1[3] = {'S', 'F', 'O'};
const char AGC_2[3] = {'L', 'S', 'F'};
const char AGC_3[3] = {'W', 'T', 'F'};

const char RATE_1[3] = {'1', ' ', ' '};
const char RATE_2[3] = {'0', '1', ' '};
const char RATE_3[3] = {'0', '0', '1'};
const char RATE_4[3] = {'0', '0', '0'};

const char ATTN_1[16] = {'0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '2', '2', '2', '2', '2', '3'};
const char ATTN_2[16] = {'0', '2', '4', '6', '8', '0', '2', '4', '6', '8', '0', '2', '4', '6', '8', '0'};

const char AMP_1[2] = {'O', ' '};
const char AMP_2[2] = {'F', 'O'};
const char AMP_3[2] = {'F', 'N'};

const char MUTE_1[2] = {'O', ' '};
const char MUTE_2[2] = {'F', 'O'};
const char MUTE_3[2] = {'F', 'N'};

const char TXLK_1[2] = {'O', ' '};
const char TXLK_2[2] = {'F', 'O'};
const char TXLK_3[2] = {'F', 'N'};

char IFBW_Dsply[4];
char Band_Dsply[3];
char Mode_Dsply[3];
char AGC_Dsply[3];
char RATE_Dsply[4];
char ATTN_Dsply[2];
char AMP_Dsply[3];
char MUTE_Dsply[3];
char TXLK_Dsply[3];

const char RF_ATT[16] = {0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
                        0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78};

const char RF_Amp[2] = {0x00, 0x04};
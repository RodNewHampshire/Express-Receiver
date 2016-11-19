/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* LEFT DISPLAY & CONTROL BOARD SOFTWARE
* HEADER FILE
*
* Copyright 2015 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 2.0
* DECEMBER 2015
*
******************************************************************************/


/**********FUNCTION PROTOTYPES**********/
void InterruptHandlerHigh (void);

void Init (void);

void Execute (char);
void IF_Gain_Display_Update(char);
void S_Meter_Update(unsigned char);

void LCD_AGC_Update (char);
void LCD_INIT (void);
void LCD_PUTC (unsigned char C);
void LCD_PUTD(unsigned char C);

void Long_Delay (unsigned int);
unsigned char ReadADC_8Bits (unsigned char);
long EEPROM_Read_Long (unsigned char);
void EEPROM_Write_Long (unsigned char, long);
unsigned char EEPROM_Read (unsigned char);
void EEPROM_Write (unsigned char, unsigned char);

#define _XTAL_FREQ          25000000

#define UPPER               1
#define LOWER               0

#define AGC_SLOW            0x00
#define AGC_FAST            0x01
#define AGC_MAN             0x02

#define S_MTR_UPDATE        0x06

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

#define MUTE_LINE           0x02
#define DDS_CLK             0x04
#define DDS_DATA            0x08
#define DDS_FSYNC           0x10

#define I2C_ADDRESS         0x2C

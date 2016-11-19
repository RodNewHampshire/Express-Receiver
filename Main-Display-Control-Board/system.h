/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* HEADER FILE
*
* Copyright 2016 Rod Gatehouse AD5GH
* Distributed under the terms of the MIT License:
* http://www.opensource.org/licenses/mit-license
*
* VERSION 16.0
* FEBRUARY 2016
*
******************************************************************************/


/**********FUNCTION PROTOTYPES**********/

//VFO Routines
void VFO_Freq_Update (void);
void VFO_Set_Band_Limits (long*, char);
void VFO_INIT(void);

//LCD Routines
void LCD_Freq_Disp_Update (void);
void LCD_Display_Update(void);
void LCD_INIT(void);
void LCD_Function_Routine(void);
void LCD_Clear(void);
void LCD_PUTC (char);
void LCD_PUTD(char);
void LCD_Line_Update(char*);
void LCD_Show_Memory_Display (void);
void LCD_Hide_Memory_Display (void);
void LCD_Si570_Calibration_Data_Display (void);
void LCD_S_Meter_Calibration_Data_Display(int);
void LCD_S_Meter_Display(char);
void LCD_Si570_Calibration(void);
void LCD_S_Meter_Calibration(void);
void LCD_Aux_Audio_Level(void);
void LCD_Main_Audio_Level(void);
void LCD_Man_IF_Gain_Level();
void LCD_Read_EEPROM_Location(void);
void LCD_Display_EEPROM(unsigned char);
void LCD_Display_Aux_Audio_Level(long);
void LCD_Display_Main_Audio_Level(long);
void LCD_Display_Man_IF_Gain_Level(char);
void LCD_Band_Cal_Offset(void);

//Si570 Routines
void Si570_VFO_Update (unsigned long, char);
void Band_Change(char);
void Si570_LO_Set(void);
void Write_Si570_Byte(char, char, char);
unsigned char Read_Si570_Byte(char, char);
void Get_10MHz_RFREQ(void);

//RelayBoard Routines
void RelayBoard_BPF_Select(char);
void RelayBoard_ATTN_Select(char);
void RelayBoard_IF_BW_Select(char);
void Write_Relay_Brd(char, char, char);

//Initialization Routines
void Initialization_Start(void);
void Initialization_Right_Display(void);
void Initialization_EEPROM (void);

//Keyboard Routines
char Keypad_Update(void);
int Keypad_Scan(void);

//Utilities Routines
void Utilities_Long_Delay(unsigned int);
unsigned char ReadADC_8Bits(char);
long EEPROM_Read_Long(char);
void EEPROM_Write_Long (char, long);
unsigned char EEPROM_Read(char);
void EEPROM_Write(char, char);
void Write_I2C_Byte(char, char);
void Write_Two_I2C_Byte(char, char, char);
char Read_I2C_Byte(char);
void myIdleI2C1(void);
void myStopI2C1(void);
void myStartI2C1(void);
void myNotAckI2C1(void);
unsigned char myReadI2C1(void);
signed char myWriteI2C1(unsigned char);

//S-Meter Routines
void S_Meter_Calibration(void);
void S_Meter_Get_Reading(void);
void S_Meter_Keypad_Update(void);

//Function Routines
void Function(void);
char Function_Keypad_Update(void);
void Execute(int);
void Function_Display(int);
void Function_Read_EEPROM(void);
void Function_Si570_Cal_Data(void);
void Function_Aux_Audio_Set(void);
void Function_Main_Audio_Set(void);
void Function_Man_IF_Gain_Set(void);
void Function_Band_Cal_Offset(void);
void Function_S_Meter_Cal(void);

#define _XTAL_FREQ      25000000

#define UPPER           1
#define LOWER           0

#define SLOW            0                   //AGC commands
#define FAST            1
#define MAN             2
#define AGC_SLW_REC     3
#define AGC_FST_REC     4
#define AGC_ATCK_RATE   5

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

#define LCD_LINE_1          0x80
#define LCD_LINE_2          0xC0
#define LCD_LINE_3          0x94
#define LCD_LINE_4          0xD4

#define Si570_SEL           0x01


/********************EEPROM DATA MEMORY USAGE********************/
#define MEM0_Freq           0x00        //Ten memories, 4 byte blocks each
#define MEM1_Freq           0x04
#define MEM2_Freq           0x08
#define MEM3_Freq           0x0C
#define MEM4_Freq           0x10
#define MEM5_Freq           0x14
#define MEM6_Freq           0x18
#define MEM7_Freq           0x1C
#define MEM8_Freq           0x20
#define MEM9_Freq           0x24

#define VFOA_Freq           0x28        //4 byte blocks each
#define VFOB_Freq           0x2C

#define Last_A_160M_Freq    0x30        //Offset band index, 4 byte blocks each
#define Last_A_80M_Freq     0x34
#define Last_A_40M_Freq     0x38
#define Last_A_30M_Freq     0x3C
#define Last_A_20M_Freq     0x40
#define Last_A_17M_Freq     0x44
#define Last_A_15M_Freq     0x48
#define Last_A_12M_Freq     0x4C
#define Last_A_10aM_Freq    0x50
#define Last_A_10bM_Freq    0x54
#define Last_A_10cM_Freq    0x58

#define EEPROM_INIT         0x59        //1 byte
#define A_Band              0x5A        //1 byte
#define B_Band              0x5B        //1 byte
#define Aux_Audio_Level     0x5C        //1 byte
#define Main_Audio_Level    0x5D        //1 byte
#define Man_IF_Gain_Level   0x5E        //1 byte
#define AGC_Slow_Rec_Rate   0x5F        //1 byte
#define AGC_Fast_Rec_Rate   0x60        //1 byte
#define AGC_Attack_Rate     0x61        //1 byte
#define AGC_Mode            0x62        //1 byte


#define Last_B_160M_Freq    0x66        //Offset band index, 4 byte blocks each
#define Last_B_80M_Freq     0x6A
#define Last_B_40M_Freq     0x6E
#define Last_B_30M_Freq     0x72
#define Last_B_20M_Freq     0x76
#define Last_B_17M_Freq     0x7A
#define Last_B_15M_Freq     0x7E
#define Last_B_12M_Freq     0x82
#define Last_B_10aM_Freq    0x86
#define Last_B_10bM_Freq    0x8A
#define Last_B_10cM_Freq    0x8E

#define MEM0_Band           0x8F
#define MEM1_Band           0x90
#define MEM2_Band           0x91
#define MEM3_Band           0x92
#define MEM4_Band           0x93
#define MEM5_Band           0x94
#define MEM6_Band           0x95
#define MEM7_Band           0x96
#define MEM8_Band           0x97
#define MEM9_Band           0x98

#define MEM0_Mode           0x99
#define MEM1_Mode           0x9A
#define MEM2_Mode           0x9B
#define MEM3_Mode           0x9C
#define MEM4_Mode           0x9D
#define MEM5_Mode           0x9E
#define MEM6_Mode           0x9F
#define MEM7_Mode           0xA0
#define MEM8_Mode           0xA1
#define MEM9_Mode           0xA2

#define VFOA_Mode           0xA3
#define VFOB_Mode           0xA4

#define Last_A_160M_Mode    0xA5
#define Last_A_80M_Mode     0xA6
#define Last_A_40M_Mode     0xA7
#define Last_A_30M_Mode     0xA8
#define Last_A_20M_Mode     0xA9
#define Last_A_17M_Mode     0xAA
#define Last_A_15M_Mode     0xAB
#define Last_A_12M_Mode     0xAC
#define Last_A_10aM_Mode    0xAD
#define Last_A_10bM_Mode    0xAE
#define Last_A_10cM_Mode    0xAF

#define Last_B_160M_Mode    0xB0
#define Last_B_80M_Mode     0xB1
#define Last_B_40M_Mode     0xB2
#define Last_B_30M_Mode     0xB3
#define Last_B_20M_Mode     0xB4
#define Last_B_17M_Mode     0xB5
#define Last_B_15M_Mode     0xB6
#define Last_B_12M_Mode     0xB7
#define Last_B_10aM_Mode    0xB8
#define Last_B_10bM_Mode    0xB9
#define Last_B_10cM_Mode    0xBA

/****************************************************************/

/********************BANDS EDGE LIMITS AND CHANNELS**************/

#define Bottom_160M         1800000
#define Top_160M            2000000

#define Bottom_80M          3500000
#define Top_80M             4000000

#define Bottom_40M          7000000
#define Top_40M             7300000

#define Bottom_30M          10100000
#define Top_30M             10150000

#define Bottom_20M          14000000
#define Top_20M             14350000

#define Bottom_17M          18068000
#define Top_17M             18168000

#define Bottom_15M          21000000
#define Top_15M             21450000

#define Bottom_12M          24890000
#define Top_12M             24990000

#define Bottom_10aM          28000000
#define Top_10aM             28500000

#define Bottom_10bM          28500000
#define Top_10bM             29000000

#define Bottom_10cM          29000000
#define Top_10cM             29700000

/****************************************************************/

#define IFBW_Cmnd           0x10
#define ATTN_Cmnd           0x30
#define AMP_Cmnd            0x40
#define Band_Cmnd           0x50
#define Mode_Cmnd           0x60
#define AGC_Cmnd            0x70
#define MUTE_Cmnd           0x80
#define TXLK_Cmnd           0x90
#define RATE_Cmnd           0xA0

#define READ_RFAttn         0xB0

/****************************************************************/

char VFOA_Last_Band, VFOB_Last_Band;

typedef struct Memory
{
    long Freq;
    char Band;
    char Mode;
};

struct Memory Memory_Array[10];


typedef struct VFO
{
    long Freq;
    char Band;
    char Mode;
};

struct VFO VFO_A[1];
struct VFO VFO_A_Last[1];
struct VFO VFO_B[1];
struct VFO VFO_B_Last[1];

struct VFO Last_Band_Data_A[11];
struct VFO Last_Band_Data_B[11];

/********************DS1807 Addressable Dual Potentiometers******/

#define Write_Pot_One       0xA9
#define Write_Pot_Two       0xAA
#define Write_Pot_Both      0xAF

/********************I2C ADDRESSING******************************/

#define I2C_Si570           0xAA
#define Right_Display       0x2A
#define Left_Display        0x2C
#define Relay_Driver_1      0x4E
#define Relay_Driver_2      0x4C
#define IF_Board            0x10
#define Audio_Attn          0x50

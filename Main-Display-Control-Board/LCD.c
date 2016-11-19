/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* LCD ROUTINES
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

extern char VFO_Flag;
extern unsigned long MEM;
extern char MEM_NUM;
extern char LO_CAL[], VFO_CAL[];

char DisplayInitFlag = 0;                           //Initialize to force display clear on start up

long VFO_A_Adjusted, VFO_B_Adjusted;

void LCD_Freq_Disp_Update (void)
{
    char VFOA_FreqDisp[10];
    char VFOB_FreqDisp[10];
    char VFOA_Select, VFOB_Select;

    if(VFO_A[0].Mode == 0) VFO_A_Adjusted = VFO_A[0].Freq - 1500;
    else if (VFO_A[0].Mode == 1) VFO_A_Adjusted = VFO_A[0].Freq + 1500;
    else VFO_A_Adjusted = VFO_A[0].Freq;

    if(VFO_B[0].Mode == 0) VFO_B_Adjusted = VFO_B[0].Freq - 1500;
    else if (VFO_B[0].Mode == 1) VFO_B_Adjusted = VFO_B[0].Freq + 1500;
    else VFO_B_Adjusted = VFO_B[0].Freq;

    ultoa(VFOA_FreqDisp, VFO_A_Adjusted, 10);
    ultoa(VFOB_FreqDisp, VFO_B_Adjusted, 10);

    if(!VFO_Flag)
    {
        VFOA_Select = '*';
        VFOB_Select = ' ';
    }

    else
    {
        VFOA_Select = ' ';
        VFOB_Select = '*';
    }

    if(DisplayInitFlag == 0)
    {
        LCD_Clear();                                //Clear display on first time through
        DisplayInitFlag = 1;
    }

    if(VFO_A[0].Freq < 10000000)
    {
        char i;
        for(i=7; i>0; i--) VFOA_FreqDisp[i] = VFOA_FreqDisp[i-1];
        VFOA_FreqDisp[0] = 0x20;
    }

    if(VFO_B[0].Freq < 10000000)
    {
        char i;
        for(i=7; i>0; i--) VFOB_FreqDisp[i] = VFOB_FreqDisp[i-1];
        VFOB_FreqDisp[0] = 0x20;
    }

    char Disp_VFOA[21] = {LCD_LINE_1,' ','V','F','O',' ','A',':',' ',' ',',',' ',' ',' ','.',' ',' ',' ','K','H','z'};
    Disp_VFOA[1] = VFOA_Select;
    Disp_VFOA[8] = VFOA_FreqDisp[0];
    Disp_VFOA[9] = VFOA_FreqDisp[1];
    Disp_VFOA[11] = VFOA_FreqDisp[2];
    Disp_VFOA[12] = VFOA_FreqDisp[3];
    Disp_VFOA[13] = VFOA_FreqDisp[4];
    Disp_VFOA[15] = VFOA_FreqDisp[5];
    Disp_VFOA[16] = VFOA_FreqDisp[6];
    Disp_VFOA[17] = VFOA_FreqDisp[7];
    LCD_Line_Update(&Disp_VFOA);
    
    char Disp_VFOB[21] = {LCD_LINE_3,' ','V','F','O',' ','A',':',' ',' ',',',' ',' ',' ','.',' ',' ',' ','K','H','z'};
    Disp_VFOB[1] = VFOB_Select;
    Disp_VFOB[8] = VFOB_FreqDisp[0];
    Disp_VFOB[9] = VFOB_FreqDisp[1];
    Disp_VFOB[11] = VFOB_FreqDisp[2];
    Disp_VFOB[12] = VFOB_FreqDisp[3];
    Disp_VFOB[13] = VFOB_FreqDisp[4];
    Disp_VFOB[15] = VFOB_FreqDisp[5];
    Disp_VFOB[16] = VFOB_FreqDisp[6];
    Disp_VFOB[17] = VFOB_FreqDisp[7];
    LCD_Line_Update(&Disp_VFOB);
}

void LCD_Show_Memory_Display (void)
{
    char MEM_NUM_Disp[1];
    char MEMX_FreqDisp[10];
    unsigned long MEM_Adjusted;

    if(Memory_Array[MEM_NUM].Mode == 0) MEM_Adjusted = MEM - 1500;
    else if (Memory_Array[MEM_NUM].Mode == 1) MEM_Adjusted = MEM + 1500;

    MEM_NUM_Disp[0] = MEM_NUM + 0x30;               //convert char to ASCII
    ultoa(MEMX_FreqDisp, MEM_Adjusted, 10);

    if(MEM_Adjusted < 10000000)
    {
        char i;
        for(i=7; i>0; i--) MEMX_FreqDisp[i] = MEMX_FreqDisp[i-1];
        MEMX_FreqDisp[0] = 0x20;
    }

    char Show_Memory[21] = {LCD_LINE_4,' ','M','E','M',' ',' ',':',' ',' ',',',' ',' ',' ','.',' ',' ',' ','K','H','z'};
    
    Show_Memory[6] = MEM_NUM_Disp[0];
    Show_Memory[8] = MEMX_FreqDisp[0];
    Show_Memory[9] = MEMX_FreqDisp[1];
    Show_Memory[11] = MEMX_FreqDisp[2];
    Show_Memory[12] = MEMX_FreqDisp[3];
    Show_Memory[13] = MEMX_FreqDisp[4];
    Show_Memory[15] = MEMX_FreqDisp[5];
    Show_Memory[16] = MEMX_FreqDisp[6];
    Show_Memory[17] = MEMX_FreqDisp[7];
    LCD_Line_Update(&Show_Memory);
}

void LCD_Hide_Memory_Display (void)
{
    const char Hide_Memory[21] = {LCD_LINE_4,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    LCD_Line_Update(&Hide_Memory);
}

/*********************LCD INITIALIZATION ROUTINE******************************/

void LCD_INIT(void)
{
    __delay_ms(20);
    __delay_ms(20);
    LCD_PUTC(0x30);
    __delay_us(100);
    LCD_PUTC(0x30);
    __delay_us(100);
    LCD_PUTC(0x38);                                 //2-Line display, 5x7 dots
    __delay_us(100);
    LCD_PUTC(0x0C);                                 //Display on, cursor off, blink off
    __delay_us(100);
    LCD_Clear();                                    //Clear display
    LCD_PUTC(0x06);                                 //Increment mode, entire shift off
    __delay_ms(2);

    const char Splash_Line_1[21] = {LCD_LINE_1,' ',' ',' ',' ',' ',' ',' ','A','D','5','G','H',' ',' ',' ',' ',' ',' ',' ',' '};
    LCD_Line_Update(&Splash_Line_1);
    
    const char Splash_Line_2[21] = {LCD_LINE_2,' ',' ','E','X','P','R','E','S','S',' ','R','E','C','E','I','V','E','R',' ',' '};
    LCD_Line_Update(&Splash_Line_2);
    
    const char Splash_Line_3[21] = {LCD_LINE_3,' ',' ','H','F',' ','U','P',' ','C','O','N','V','E','R','T','I','N','G',' ',' '};
    LCD_Line_Update(&Splash_Line_3);

    const char Splash_Line_4[21] = {LCD_LINE_4,' ','S','U','P','E','R',' ','H','E','T',' ','R','E','C','E','I','V','E','R',' '};
    LCD_Line_Update(&Splash_Line_4);
}

/*********************LCD FUNCTION ROUTINES***********************************/

void LCD_Function_Routine(void)
{
    const char Menu_Function[21] = {LCD_LINE_1,'F','U','N','C','T','I','O','N',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    LCD_Line_Update(&Menu_Function);
}

void LCD_Si570_Calibration(void)
{
    const char Menu_Si570_Cal[21] = {LCD_LINE_2,'S','i','5','7','0',' ','C','a','l','i','b','.',' ','d','a','t','a',' ',' ',' '};
    LCD_Line_Update(&Menu_Si570_Cal);
}

void LCD_S_Meter_Calibration(void)
{
    const char Menu_S_Meter_Cal[21] = {LCD_LINE_2,'S','-','M','e','t','e','r',' ','C','a','l','i','b','r','a','t','i','o','n',' '};
    LCD_Line_Update(&Menu_S_Meter_Cal);
}

void LCD_Aux_Audio_Level(void)
{
    const char Menu_Aux_Audio_Level[21] = {LCD_LINE_2,'A','u','x',' ','A','u','d','i','o',' ','L','e','v','e','l',' ','A','d','j',' '};
    LCD_Line_Update(&Menu_Aux_Audio_Level);
}

void LCD_Main_Audio_Level(void)
{
    const char Menu_Main_Audio_Level[21] = {LCD_LINE_2,'M','a','i','n',' ','A','u','d','i','o',' ','L','e','v','e','l',' ','A','d','j'};
    LCD_Line_Update(&Menu_Main_Audio_Level);
}

void LCD_Man_IF_Gain_Level(void)
{
    const char Menu_Man_IF_Gain[21] = {LCD_LINE_2,'M','a','n','u','a','l',' ','I','F',' ','G','a','i','n',' ','S','e','t',' ',' '};
    LCD_Line_Update(&Menu_Man_IF_Gain);
}

void LCD_Band_Cal_Offset(void)
{
    const char Menu_Band_Cal_Offset[21] = {LCD_LINE_2,'S','e','t',' ','B','a','n','d',' ','C','a','l',' ','O','f','f','s','e','t','s'};
    LCD_Line_Update(&Menu_Band_Cal_Offset);
}
    
void LCD_Read_EEPROM_Location(void)
{
    const char Menu_Read_EEPROM[21] = {LCD_LINE_2,'R','e','a','d',' ','E','E','P','R','O','M',' ','L','o','c','a','t','i','o','n'};
    LCD_Line_Update(&Menu_Read_EEPROM);
}

void LCD_Display_EEPROM(char address)
{
    char EEPROM_Data, Data_Disp[4];
    char Disp_EEPROM_Line_3[21] = {LCD_LINE_3,'E','E','P','R','O','M',' ','A','D','D','R','E','S','S',':',' ',' ',' ',' ',' '};
    char Disp_EEPROM_Line_4[21] = {LCD_LINE_4,'E','E','P','R','O','M',' ','D','A','T','A',':',' ',' ',':',' ',' ',' ',' ',' '};
    
    char n;
    
    EEPROM_Data = EEPROM_Read(address);
    
    Data_Disp[3] = (EEPROM_Data & 0xF0)>>4;
    Data_Disp[2] = (EEPROM_Data & 0x0F);
    
    Data_Disp[1] = (address & 0xF0)>>4;
    Data_Disp[0] = (address & 0x0F);
    
    for(n=0; n<4; n++)
    {
        char temp;
        temp = Data_Disp[n];
        if(temp <= 0x09) Data_Disp[n] = temp + 0x30;
        else Data_Disp[n] = temp + 0x37;
    }
    
    Disp_EEPROM_Line_3[17] = Data_Disp[1];
    Disp_EEPROM_Line_3[18] = Data_Disp[0];
    LCD_Line_Update(&Disp_EEPROM_Line_3);
    
    Disp_EEPROM_Line_4[17] = Data_Disp[3];
    Disp_EEPROM_Line_4[18] = Data_Disp[2];
    LCD_Line_Update(&Disp_EEPROM_Line_4);
}

void LCD_Si570_Calibration_Data_Display (void)
{
    char Disp_LO_Data[21] = {LCD_LINE_3,'L','O',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
    char Disp_VFO_Data[21] = {LCD_LINE_4,'V','F','O',':',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

    Disp_LO_Data[6] = LO_CAL[11];
    Disp_LO_Data[7] = LO_CAL[10];
    Disp_LO_Data[9] = LO_CAL[9];
    Disp_LO_Data[10] = LO_CAL[8];
    Disp_LO_Data[12] = LO_CAL[7];
    Disp_LO_Data[13] = LO_CAL[6];
    Disp_LO_Data[14] = LO_CAL[5];
    Disp_LO_Data[15] = LO_CAL[4];
    Disp_LO_Data[16] = LO_CAL[3];
    Disp_LO_Data[17] = LO_CAL[2];
    Disp_LO_Data[18] = LO_CAL[1];
    Disp_LO_Data[19] = LO_CAL[0];
    LCD_Line_Update(&Disp_LO_Data);
    
    Disp_VFO_Data[6] = VFO_CAL[11];
    Disp_VFO_Data[7] = VFO_CAL[10];
    Disp_VFO_Data[9] = VFO_CAL[9];
    Disp_VFO_Data[10] = VFO_CAL[8];
    Disp_VFO_Data[12] = VFO_CAL[7];
    Disp_VFO_Data[13] = VFO_CAL[6];
    Disp_VFO_Data[14] = VFO_CAL[5];
    Disp_VFO_Data[15] = VFO_CAL[4];
    Disp_VFO_Data[16] = VFO_CAL[3];
    Disp_VFO_Data[17] = VFO_CAL[2];
    Disp_VFO_Data[18] = VFO_CAL[1];
    Disp_VFO_Data[19] = VFO_CAL[0];
    LCD_Line_Update(&Disp_VFO_Data);
}

void LCD_Display_Aux_Audio_Level(long level)
{
    char Level_Disp[2];
    char Disp_Aux_Audio_Lvl[21] = {LCD_LINE_3,'A','t','t','e','n','u','a','t','i','o','n',':',' ',' ',' ',' ','d','B',' ',' '};
    
    ultoa(Level_Disp, level, 10);
    
    if(level < 10)
    {
        Level_Disp[1] = Level_Disp[0];
        Level_Disp[0] = 0x20;
    }
    
    Disp_Aux_Audio_Lvl[14] = Level_Disp[0];
    Disp_Aux_Audio_Lvl[15] = Level_Disp[1];
    LCD_Line_Update(&Disp_Aux_Audio_Lvl);
}

void LCD_Display_Main_Audio_Level(long level)
{
    char Level_Disp[2];
    char Disp_Main_Audio_Lvl[21] = {LCD_LINE_3,'A','t','t','e','n','u','a','t','i','o','n',':',' ',' ',' ',' ','d','B',' ',' '};
    
    ultoa(Level_Disp, level, 10);
    
    if(level < 10)
    {
        Level_Disp[1] = Level_Disp[0];
        Level_Disp[0] = 0x20;
    }
    
    Disp_Main_Audio_Lvl[14] = Level_Disp[0];
    Disp_Main_Audio_Lvl[15] = Level_Disp[1];
    LCD_Line_Update(&Disp_Main_Audio_Lvl);
}

void LCD_Display_Man_IF_Gain_Level(char temp)
{
    char Level_Disp[2];
    char sign, level;
    const char Man_IF_Gain_dB[19] = {45, 40, 35, 30, 25, 20, 15, 10, 5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45};
    char Disp_Man_IF_Gain[21] = {LCD_LINE_3,'M','a','n','.',' ','I','F',' ','G','a','i','n',':',' ',' ',' ',' ',' ','d','B'};
    
    if(temp < 9) sign = '-';                        //minus sign
    else sign = ' ';
    
    level = Man_IF_Gain_dB[temp];
    
    ultoa(Level_Disp, level, 10);
    
    if(level < 10)
    {
        Level_Disp[1] = Level_Disp[0];
        Level_Disp[0] = 0x20;
    }
    
    Disp_Man_IF_Gain[16] = sign;
    Disp_Man_IF_Gain[17] = Level_Disp[0];
    Disp_Man_IF_Gain[18] = Level_Disp[1];
    LCD_Line_Update(&Disp_Man_IF_Gain);
}

void LCD_S_Meter_Calibration_Data_Display(int Sig_Strength)
{
    char S_Meter_Reading[4];
    char Disp_S_Meter_Cal[21] = {LCD_LINE_4,'S','-','M','e','t','e','r',' ','R','e','a','d','i','n','g',':',' ',' ',' ',' '};

    ultoa(S_Meter_Reading, Sig_Strength, 10);

    if(Sig_Strength < 0x0A)                         //"10" decimal
    {
        S_Meter_Reading[2] = S_Meter_Reading[0];
        S_Meter_Reading[1] = 0x20;
        S_Meter_Reading[0] = 0x20;
    }

    else if(Sig_Strength < 0x64)                    //"100" decimal
    {
        char i;
        for(i=2; i>0; i--) S_Meter_Reading[i] = S_Meter_Reading[i-1];
        S_Meter_Reading[0] = 0x20;
    }
    
    Disp_S_Meter_Cal[18] = S_Meter_Reading[0];
    Disp_S_Meter_Cal[19] = S_Meter_Reading[1];
    Disp_S_Meter_Cal[20] = S_Meter_Reading[2];
    LCD_Line_Update(&Disp_S_Meter_Cal);
}

/*********************SEND CONTROL CHARACTER TO LCD***************************/

void LCD_PUTC (unsigned char C)
{
    PORTD = C;
    __delay_us(20);
    PORTE = 0x01;
    __delay_us(20);
    PORTE = 0x00;
    __delay_us(10);
}

/*********************SEND DATA CHARACTER TO LCD******************************/

void LCD_PUTD(unsigned char C)
{
    PORTD = C;
    PORTE = 0x04;
    __delay_us(20);
    PORTE = 0x05;
    __delay_us(20);
    PORTE = 0x00;
    __delay_us(10);
}

/********************UPDATE ONE 20 CHARACTER LCD LINE*************************/
void LCD_Line_Update(char *Line_To_Display)
{
    char count;
    
    LCD_PUTC(Line_To_Display[0]);
    for(count = 1; count < 21; count++) LCD_PUTD(Line_To_Display[count]);
}

/*********************LCD CLEAR DISPLAY***************************************/

void LCD_Clear(void)
{
    LCD_PUTC(0x01);                                 //Clear display
    __delay_ms(2);
}
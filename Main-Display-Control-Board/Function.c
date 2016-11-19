/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* FUNCTION ROUTINES
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

#define DEBOUNCE 250

int function;
extern char AGC;

void Function(void)
{
    char done = 0;
    
    LCD_Clear();
    LCD_Function_Routine();
    Utilities_Long_Delay(450);                          //because I like the effect of the delay
    function = 1;
    Function_Display(function);
    
    while(1)
    {
        Utilities_Long_Delay(DEBOUNCE);                 //needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit function routines
                done = 1;
                break;

            case 12:                                    //ENTER, execute function
                Execute(function);
                LCD_Clear();
                LCD_Function_Routine();
                Function_Display(function);
                break;

            case 13:                                    //UP
                function++;
                if(function == 8) function = 1;
                Function_Display(function);
                break;

            case 14:                                    //DOWN
                function--;
                if(function == 0) function = 7;
                Function_Display(function);
                break;
            
            default:                                    //Ignore other button presses
                break;
        }
        
        if(done) return;                                //Needed to exit the while loop
    }
}


void Function_Display(int func)
{
    switch(func)
    {
        case 0:
            break;
        
        case 1:                                     //S1570 Calibration Data Retrieval
            LCD_Si570_Calibration();
            break;

        case 2:                                     //S Meter Calibration
            LCD_S_Meter_Calibration();
            break;

        case 3:                                     //Aux audio level set
            LCD_Aux_Audio_Level();
            break;
            
        case 4:                                     //Aux audio level set
            LCD_Main_Audio_Level();
            break;
            
        case 5:                                     //Manual IF gain level set
            LCD_Man_IF_Gain_Level();                
            break;
        
        case 6:                                     //Enter Band frequency calibration offsets
            LCD_Band_Cal_Offset();                  
            break;
        
        case 7:                                     //Read EEPROM location
            LCD_Read_EEPROM_Location();
            break;
            
        default:                                    //Ignore other button presses
            break;
    }
}

void Execute (int func)
{
    switch(func)
    {
        case 0:
            break;
        
        case 1:                                     //S1570 Calibration Data Retrieval
            Function_Si570_Cal_Data();
            break;

        case 2:                                     //S Meter Calibration
            Function_S_Meter_Cal();
            break;

        case 3:                                     //Aux audio level adjustment 
            Function_Aux_Audio_Set();
            break;
            
        case 4:                                     //Main audio level adjustment 
            Function_Main_Audio_Set();
            break;
            
        case 5:                                     
            Function_Man_IF_Gain_Set();             //Manual IF gain adjustment
            break;
        
        case 6:
            //Function_Band_Cal_Offset();
            break;
        
        case 7:                                     //Read EEPROM location 
            Function_Read_EEPROM();
            break;
            
        default:                                    //Ignore other button presses
            break;
    }
}

void Function_S_Meter_Cal()
{
    int Sig_Strength;
    char done = 0;
    
    while(1)
    {
        Utilities_Long_Delay(DEBOUNCE);                 //Needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit from routine
                done = 1;
                break;

            case 12:
                
                break;
                
            case 13:                                    //UP
                
                break;

            case 14:                                    //DOWN
                
                break;
            
            default:                                    //Ignore other button presses
                break;
        }
        Sig_Strength = Read_I2C_Byte(IF_Board);
        LCD_S_Meter_Calibration_Data_Display(Sig_Strength);
        
        if(done) return;                                //Needed to exit the while loop
    }
}

void Function_Read_EEPROM()
{
    char address = 0;
    char done = 0;
    
    LCD_Display_EEPROM(address);
    
    while(1)
    {
        Utilities_Long_Delay(DEBOUNCE);                 //Needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit from routine
                done = 1;
                break;

            case 13:                                    //UP
                address++;
                LCD_Display_EEPROM(address);
                break;

            case 14:                                    //DOWN
                address--;
                LCD_Display_EEPROM(address);
                break;
            
            default:                                    //Ignore other button presses
                break;
        }
        
        if(done) return;                                //Needed to exit the while loop
    }
}

void Function_Si570_Cal_Data(void)
{
    char done = 0;
    
    Get_10MHz_RFREQ();
    LCD_Si570_Calibration_Data_Display();
    while(1)
    {   
        Utilities_Long_Delay(DEBOUNCE);                 //Needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit function routines
                done = 1;
                return;
            
            default:                                    //Ignore other button presses
                break;
        }
        if(done) return;                                //Needed to exit the while loop
    }
}

void Function_Aux_Audio_Set(void)
{
    char level = EEPROM_Read(Aux_Audio_Level);
    char done = 0;
    
    LCD_Display_Aux_Audio_Level((long) level);
    while(1)
    {
        Utilities_Long_Delay(DEBOUNCE);                 //Needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit function without changing value
                done = 1;
                return;                                    
            
            case 12:                                    //ENTER, save new value and exit function
                EEPROM_Write(Aux_Audio_Level, level);
                done = 1;
                return;

            case 13:                                    //UP
                if(level == 64);                        //Max value, do nothing
                else
                {
                    level++;
                    LCD_Display_Aux_Audio_Level((long)level);
                    Write_Two_I2C_Byte(Audio_Attn, Write_Pot_Two, level);
                }
                break;

            case 14:                                    //DOWN
                if(level == 0);                         //Min value, do nothing
                else
                {
                    level--;
                    LCD_Display_Aux_Audio_Level((long)level);
                    Write_Two_I2C_Byte(Audio_Attn, Write_Pot_Two, level);
                }
                break;
            
            default:                                    //Ignore any other button presses
                break;
        }
        if(done) return;
    }
}

void Function_Main_Audio_Set(void)
{
    char level = EEPROM_Read(Main_Audio_Level);
    char done = 0;
    
    LCD_Display_Main_Audio_Level((long) level);
    while(1)
    {   
        Utilities_Long_Delay(DEBOUNCE);                 //Needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit function without changing value
                done = 1;
                return;                                    
                
            case 12:                                    //ENTER, save new value and exit function
                EEPROM_Write(Main_Audio_Level, level);
                done = 1;
                return;

            case 13:                                    //UP
                if(level == 64);                        //Max value, do nothing
                else
                {
                    level++;
                    LCD_Display_Main_Audio_Level((long)level);
                    Write_Two_I2C_Byte(Audio_Attn, Write_Pot_One, level);
                }
                break;

            case 14:                                    //DOWN
                if(level == 0);                         //Min value, do nothing
                else
                {
                    level--;
                    LCD_Display_Main_Audio_Level((long)level);
                    Write_Two_I2C_Byte(Audio_Attn, Write_Pot_One, level);
                }
                break;
        
            default:                                    //Ignore any other button presses
                break;
        }
        if(done) return;
    }
}

void Function_Man_IF_Gain_Set(void)
{
    char level = EEPROM_Read(Man_IF_Gain_Level);
    char done = 0;
    int Sig_Strength;
    
    LCD_Display_Man_IF_Gain_Level(level);
    Write_I2C_Byte(IF_Board, MAN + (level << 3) );
    Write_I2C_Byte(Left_Display, MAN + (level << 3) );
    Write_I2C_Byte(Right_Display, AGC_Cmnd+MAN);
    AGC = MAN;                                          //AGC mode to manual
    
    while(1)
    {   
        Utilities_Long_Delay(DEBOUNCE);                 //Needed to debounce keypad within the while loop
        switch(Keypad_Scan())
        {
            case 11:                                    //FUNC, exit function without changing value
                done = 1;
                return;                                    
                
            case 12:                                    //ENTER, save new value and exit function
                EEPROM_Write(Man_IF_Gain_Level, level);
                done = 1;
                return;

            case 13:                                    //UP
                if(level == 18);                        //Max value, do nothing
                else
                {
                    level++;
                    LCD_Display_Man_IF_Gain_Level(level);
                    Write_I2C_Byte(Left_Display, MAN + (level << 3) );
                    Write_I2C_Byte(IF_Board, MAN + (level << 3) );
                }
                break;

            case 14:                                    //DOWN
                if(level == 0);                         //Min value, do nothing
                else
                {
                    level--;
                    LCD_Display_Man_IF_Gain_Level(level);
                    Write_I2C_Byte(Left_Display, MAN + (level << 3) );
                    Write_I2C_Byte(IF_Board, MAN + (level << 3) );
                }
                break;
        
            default:
                break;
        }
        if(done) return;
        
        Sig_Strength = Read_I2C_Byte(IF_Board);
        LCD_S_Meter_Calibration_Data_Display(Sig_Strength);
    }
}
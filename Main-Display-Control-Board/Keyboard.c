/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* KEYBOARD ROUTINES
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

extern char FreqUpdateFlag;
extern unsigned long Memory_Display_Hide;
extern char VFO_Flag, DH_Flag;
extern char Band_Change_Flag;
extern char BAND_NUM_A, BAND_NUM_B;
extern int TuningRate;
extern unsigned long FreqUpdateAccum;

unsigned long MEM;

char MEM_NUM;
char MEM_Disp_Flag = 0;
char DH_Mode_A_Flag = 0;
char DH_Mode_B_Flag = 0;
char AGC, IFBW, RATE, ATTN, MUTE, AMP, TXLK;

/*Keyboard Key Mapping

 FUNC       ENTER   UP      DOWN    TX LOCK
 11         12      13      14      15

 A/B        A->B    MEM     A->MEM  MEM->A
 1          2       3       4       5

 IF FLTR    AGC     MODE    RATE    MUTE
 6          7       8       9       10
 */

char Keypad_Update(void)
{
    char button, temp;

    button = Keypad_Scan();
    if(button == 0) return 0;
    
    switch(button)
    {
        case 1:                                     //VFO A/B
            if(VFO_Flag) VFO_Flag = 0;
            else VFO_Flag = 1;
            DH_Flag = 0;
            FreqUpdateAccum = 0;
            Band_Change_Flag = 1;
            VFO_Freq_Update();
            break;

        case 2:                                     //VFO A -> B

            Last_Band_Data_B[VFO_A[0].Band].Freq = VFO_A[0].Freq;
            Last_Band_Data_B[VFO_A[0].Band].Mode = VFO_A[0].Mode;

            if(VFO_Flag)                            //VFO B active
            {
                if(VFO_A[0].Band != VFO_B[0].Band)
                {
                    VFO_B[0].Band = VFO_A[0].Band;
                    Band_Change_Flag = 1;
                }
                
                else
                {
                    VFO_B[0].Freq = VFO_A[0].Freq;
                    VFO_B[0].Mode = VFO_A[0].Mode;
                }

                VFO_Freq_Update();
            }
            
            else                                    //VFO A active
            {
                Last_Band_Data_B[VFOB_Last_Band].Freq = VFO_B[0].Freq;
                Last_Band_Data_B[VFOB_Last_Band].Mode = VFO_B[0].Mode;
                VFO_B[0].Freq = VFO_A[0].Freq;
                VFO_B[0].Mode = VFO_A[0].Mode;
                VFO_B[0].Band = VFO_A[0].Band;
                LCD_Freq_Disp_Update();
            }
            break;

        case 3:                                     //MEMORY SELECT
            if(!MEM_Disp_Flag)
            {
                MEM_Disp_Flag = 1;
                LCD_Show_Memory_Display();
                break;
            }
            else
            {
                Memory_Display_Hide = 1000000;
                MEM_NUM++;
                if(MEM_NUM == 10)MEM_NUM = 0;
                MEM = Memory_Array[MEM_NUM].Freq;
                LCD_Show_Memory_Display();
                break;
            }

        case 4:                                     //VFO A -> MEMORY
            if(!MEM_Disp_Flag) break;
            else
            {
                Memory_Display_Hide = 1000000;
                Memory_Array[MEM_NUM].Freq = VFO_A[0].Freq;
                MEM = Memory_Array[MEM_NUM].Freq;
                Memory_Array[MEM_NUM].Band = VFO_A[0].Band;
                Memory_Array[MEM_NUM].Mode = VFO_A[0].Mode;
                LCD_Show_Memory_Display();
                break;
            }

        case 5:                                     //MEMORY -> VFO A
            if(!MEM_Disp_Flag) break;
            else
            {
                Memory_Display_Hide = 1000000;

                if(VFO_A[0].Band != Memory_Array[MEM_NUM].Band)
                {
                    VFO_A[0].Band = Memory_Array[MEM_NUM].Band;
                    Last_Band_Data_A[VFO_A[0].Band].Freq = Memory_Array[MEM_NUM].Freq;
                    Last_Band_Data_A[VFO_A[0].Band].Mode = Memory_Array[MEM_NUM].Mode;
                    Band_Change_Flag = 1;
                }
                else
                {
                    VFO_A[0].Freq = Memory_Array[MEM_NUM].Freq;
                    VFO_A[0].Mode = Memory_Array[MEM_NUM].Mode;
                }
                VFO_Freq_Update();
                break;
            }

        case 6:                                     //IF BANDWIDTH
            if(IFBW == 3) IFBW = 0;
            else IFBW++;
            Write_I2C_Byte(Right_Display, IFBW_Cmnd+IFBW);
            RelayBoard_IF_BW_Select(IFBW);
            if(AGC == MAN)
            {
                
            }
            break;

            /* IFBW = 0     2100-Hz
               IFBW = 1     1800-Hz
               IFBW = 2     500-Hz
               IFBW = 3     250-Hz  */

            /*IF Filter Insertion Loss as measured with N2PK Vector Network Analyzer
             2100-Hz:     5.25dB        0dB
             1800-Hz:     5.25dB        0db
             500-Hz:      7.5dB         -2.25dB
             250-Hz:      10.5dB        -5.25dB*/
             
        case 7:                                     //AGC
            if(AGC == MAN) AGC = SLOW;
            else AGC++;
            
            Write_I2C_Byte(Right_Display, AGC_Cmnd+AGC);
            Write_I2C_Byte(Left_Display, AGC + (EEPROM_Read(Man_IF_Gain_Level) << 3) ); //Manual IF gain level is ignored by Left Display Board if AGC command is SLOW or FAST
            Write_I2C_Byte(IF_Board, AGC + (EEPROM_Read(Man_IF_Gain_Level) << 3));      //Manual IF gain level is ignored by IF Board if AGC command is SLOW or FAST
            break;

        case 8:                                     //MODE
            if(!VFO_Flag)                           //VFO A active
            {
                temp = VFO_A[0].Mode;
                if(temp == 2) temp = 0;
                else temp++;
                VFO_A[0].Mode = temp;
                Write_I2C_Byte(Right_Display, (Mode_Cmnd+VFO_A[0].Mode));
            }

            else                                    //VFO B active
            {
                temp = VFO_B[0].Mode;
                if(temp == 2) temp = 0;
                else temp++;
                VFO_B[0].Mode = temp;
                Write_I2C_Byte(Right_Display, (Mode_Cmnd+VFO_B[0].Mode));
            }

            LCD_Freq_Disp_Update();
            break;
                          
        case 9:                                     //RATE
            if(TuningRate == 1000)
            {
                TuningRate = 100;
                RATE = 0x01;
            }
            else if(TuningRate == 100)
            {
                TuningRate = 10;
                RATE = 0x02;
            }
            else if(TuningRate == 10)
            {
                TuningRate = 1000;
                RATE = 0x00;
            }
            Write_I2C_Byte(Right_Display, RATE_Cmnd+RATE);
            break;

        case 10:                                    //MUTE
            if(MUTE == 0) MUTE = 1;
            else MUTE = 0;
            Write_I2C_Byte(Right_Display, MUTE_Cmnd+MUTE);
            if(MUTE) Write_Two_I2C_Byte(Audio_Attn, Write_Pot_One, 0x40);
            else Write_Two_I2C_Byte(Audio_Attn, Write_Pot_One, 0x00);
            break;

        case 11:                                    //FUNCTION
            INTCON = 0x00;                          //Disable interrupts
            Function();
            LCD_Clear();
            LCD_Freq_Disp_Update();                 //Return Main LCD Display to normal operation
            INTCON = 0x88;                          //Enable interrupts
            break;

        case 12:                                    //ENTER
            Write_I2C_Byte(IF_Board, 0xC8);
            break;

        case 13:                                    //UP
            Band_Change_Flag = 1;

            if(!VFO_Flag)
            {
                VFO_A[0].Band++;
                if(VFO_A[0].Band == 11)VFO_A[0].Band = 0;
            }

            if(VFO_Flag)
            {
                VFO_B[0].Band++;
                if(VFO_B[0].Band == 11)VFO_B[0].Band = 0;
            }

            VFO_Freq_Update();

            break;

            /* RF Front End Insertion Loss as measured with N2PK Vector Network Analyzer
             160M:  0.7dB
             80M:   0.5dB
             40M:   1.0dB
             30M:   1.3dB
             20M:   1,2dB
             17M:   1.4dB
             15M:   1.4dB
             12M:   1.3dB
             10M:   1.8dB */

        case 14:                                    //DOWN
            Band_Change_Flag = 1;

            if(!VFO_Flag)
            {
                if(VFO_A[0].Band == 0)VFO_A[0].Band = 10;
                else VFO_A[0].Band--;
            }

            if(VFO_Flag)
            {
                if(VFO_B[0].Band == 0)VFO_B[0].Band = 10;
                else VFO_B[0].Band--;
            }

            VFO_Freq_Update();

            break;
            
        case 15:                                    //DIAL HOLD - Doubling up with TX LOCK for now
            if(DH_Flag == 0) DH_Flag = 1;
            else if(DH_Flag == 1)                   //release dial hold
            {
                DH_Flag = 0;
                FreqUpdateAccum = 0;                //clear accumulator
            }

            if(TXLK == 0) TXLK = 1;                 //XMT LOCK
            else TXLK = 0;
            Write_I2C_Byte(Right_Display, TXLK_Cmnd+TXLK);
            break;
        }
    return 1;
    }


   
int Keypad_Scan(void)
{
    int b = 0;

    if(!(PORTB & KP_Row_1))
    {
        __delay_ms(1);                              //debounce keyboard

        PORTA |= KP_Col_1;
        __delay_us(2);
        if(PORTB & KP_Row_1) 
        {
            PORTA &= ~Keypad_Cols;
            return b=5;
        }

        PORTA |= KP_Col_2;
        __delay_us(2);
        if(PORTB & KP_Row_1)
        {
            PORTA &= ~Keypad_Cols;
            return b=4;
        }

        PORTA |= KP_Col_3;
        __delay_us(2);
        if(PORTB & KP_Row_1)
        {
            PORTA &= ~Keypad_Cols;
            return b=3;
        }

        PORTA |= KP_Col_4;
        __delay_us(2);
        if(PORTB & KP_Row_1)
        {
            PORTA &= ~Keypad_Cols;
            return b=2;
        }

        PORTA |= KP_Col_5;
        __delay_us(2);
        if(PORTB & KP_Row_1)
        {
            PORTA &= ~Keypad_Cols;
            return b=1;
        }
    }

    else if(!(PORTB & KP_Row_2))
    {
        __delay_ms(1);                              //debounce keyboard

        PORTA |= KP_Col_1;
        if(PORTB & KP_Row_2)
        {
            PORTA &= ~Keypad_Cols;
            return b=10;
        }

        PORTA |= KP_Col_2;
        __delay_us(2);
        if(PORTB & KP_Row_2)
        {
            PORTA &= ~Keypad_Cols;
            return b=9;
        }

        PORTA |= KP_Col_3;
        __delay_us(2);
        if(PORTB & KP_Row_2)
        {
            PORTA &= ~Keypad_Cols;
            return b=8;
        }

        PORTA |= KP_Col_4;
        __delay_us(2);
        if(PORTB & KP_Row_2)
        {
            PORTA &= ~Keypad_Cols;
            return b=7;
        }

        PORTA |= KP_Col_5;
        __delay_us(2);
        if(PORTB & KP_Row_2)
        {
            PORTA &= ~Keypad_Cols;
            return b=6;
        }
    }

    else if(!(PORTB & KP_Row_3))
    {
        __delay_ms(1);                              //debounce keyboard

        PORTA |= KP_Col_1;
        if(PORTB & KP_Row_3)
        {
            PORTA &= ~Keypad_Cols;
            return b=15;
        }

        PORTA |= KP_Col_2;
        __delay_us(2);
        if(PORTB & KP_Row_3)
        {
            PORTA &= ~Keypad_Cols;
            return b=14;
        }

        PORTA |= KP_Col_3;
        __delay_us(2);
        if(PORTB & KP_Row_3)
        {
            PORTA &= ~Keypad_Cols;
            return b=13;
        }

        PORTA |= KP_Col_4;
        __delay_us(2);
        if(PORTB & KP_Row_3)
        {
            PORTA &= ~Keypad_Cols;
            return b=12;
        }

        PORTA |= KP_Col_5;
        __delay_us(2);
        if(PORTB & KP_Row_3)
        {
            PORTA &= ~Keypad_Cols;
            return b=11;
        }
    }

    return b;
}


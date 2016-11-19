/******************************************************************************
*
* EXPRESS RECEIVER by AD5GH
* (http://www.expressreceiver.com)
* MAIN DISPLAY & CONTROL BOARD SOFTWARE
* INITIALIZATION ROUTINES
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

extern char Old_Encdr;
extern unsigned long VFOA, VFOB;
extern unsigned long MEM, Last_Band_Freq_A[9], Last_Band_Freq_B[9];
extern char MEM_NUM;
extern char VFO_Flag, BAND_NUM_A, BAND_NUM_B, BAND_NUM_A_LAST, BAND_NUM_B_LAST, Band_Change_Flag;
extern char AGC, IFBW, RATE, ATTN, MUTE, AMP, TXLK;
extern char Man_IF_Gain;
extern int TuningRate;

void Initialization_Start (void)
{

/*********************PORT PIN DEFINITION & INITIALIZATION********************/

    /********************PORTA*************************************************
    *RA0 - 0, Output, digital, Si570 select
    *RA1 - 0, Output, digital, Keypad column #1
    *RA2 - 0, Output, digital, Keypad column #2
    *RA3 - 0, Output, digital, Keypad column #3
    *RA4 - 0, Output, digital, Keypad column #4
    *RA5 - 0, Output, digital, keypad column #5
    *RA6 - 0, Output,digital, Red LED
    *RA7 - 1, Input, ECHP clock oscillator
     *************************************************************************/
    TRISA = 0x80;
    ANSELA = 0x00;                                  //ADC channel 0 enabled
    PORTA |= LED_Red;                               //LED_Red Off
    PORTA &= ~Keypad_Cols;                          //Keypad columns low

                                                    //Initialize ADC
    //ADCON1 = 0b00000000;                          //ADC Vref+ = Avdd
                                                    //ADC Vref- = Avss

    //ADCON2 = 0b00101110;                          //Left justified result, only want the 8 MSBs in ADRESH
                                                    //12 TAD
                                                    //Fosc/64


    /********************PORTB*************************************************
    *RB0 - 1, Input, Keypad Row #1
    *RB1 - 1, Input, Keypad Row #2
    *RB2 - 1, Input, Keypad Row #3
    *RB3 - 1, Input, Digital I/O
    *RB4 - 1, Input, ENCDR_A
    *RB5 - 1, Input, ENCDR_B
    *RB6 - 1, Input, Not used
    *RB7 - 1, Input, Not used
     *************************************************************************/
    TRISB = 0xFF;
    ANSELB = 0x00;


    /********************PORTC*************************************************
    *RC0 - 0, Output, RS485_DE
    *RC1 - 0, Output, RS485_RE
    *RC2 - 0, Output,digital, Green LED
    *RC3 - 1, Input, SCL
    *RC4 - 1, Input, SDA
    *RC5 - 1, Input, Digital I/O
    *RC6 - 1, EUSART TXD1
    *RC7 - 1, EUSART RXD1
     *************************************************************************/
    TRISC = 0xF8;
    ANSELC = 0x00;
    PORTC |= RS485_DE;                              //Driver output enabled
    PORTC &= ~RS485_RE;                             //Receiver output enabled
    PORTC |= LED_Green;                             //LED_Green Off

    /********************USART INITIALIZATION*********************************/

    //To be added when RS485 enabled

    /********************I2C INITIALIZATION***********************************/
    
    unsigned char sync_mode = 0b00001000;           //Master mode, SSP1CON1 REGISTER
    unsigned char slew = 0b10000000;                //Slew rate disabled for 100KHz mode, SSP1STAT REGISTER

    SSPADD = 0x2B;                                  //sets bus speed to 100-KHz

    SSP1STAT &= 0x3F;                               // power on state
    SSP1CON1 = 0x00;                                // power on state
    SSP1CON2 = 0x00;                                // power on state
    SSP1CON1 |= sync_mode;                          // select serial mode
    SSP1STAT |= slew;                               // slew rate on/off

    SSP1CON1bits.SSPEN = 1;                         //Enable synchronous serial port

    /********************PORTD*************************************************
    *RD0 - 0, Output, LCD_DB0
    *RD1 - 0, Output, LCD_DB1
    *RD2 - 0, Output, LCD_DB2
    *RD3 - 0, Output, LCD_DB3
    *RD4 - 0, Output, LCD_DB4
    *RD5 - 0, Output, LCD_DB5
    *RD6 - 0, Output, LCD_DB6
    *RD7 - 0, Output, LCD_DB7
     *************************************************************************/
    TRISD = 0x00;
    ANSELD = 0x00;
    PORTD = 0x00;


    /********************PORTE*************************************************
    *RE0 - 0, Output, LCD_ENB
    *RE1 - 0, Output, LCD_RW
    *RE2 - 0, Output, LCD_RS
     *************************************************************************/
    TRISE = 0x00;
    ANSELE = 0x00;
    PORTE = 0x00;

/********************RECEIVER INITIALIZATION**********************************/

    if(EEPROM_Read(EEPROM_INIT)!= 0xAB) Initialization_EEPROM();             // Initialize EEPROM if new board

    /********************INITIALIZE MEMORY ARRAY FROM EEPROM*****/
    
    Memory_Array[0].Freq = EEPROM_Read_Long(MEM0_Freq);
    Memory_Array[1].Freq = EEPROM_Read_Long(MEM1_Freq);
    Memory_Array[2].Freq = EEPROM_Read_Long(MEM2_Freq);
    Memory_Array[3].Freq = EEPROM_Read_Long(MEM3_Freq);
    Memory_Array[4].Freq = EEPROM_Read_Long(MEM4_Freq);
    Memory_Array[5].Freq = EEPROM_Read_Long(MEM5_Freq);
    Memory_Array[6].Freq = EEPROM_Read_Long(MEM6_Freq);
    Memory_Array[7].Freq = EEPROM_Read_Long(MEM7_Freq);
    Memory_Array[8].Freq = EEPROM_Read_Long(MEM8_Freq);
    Memory_Array[9].Freq = EEPROM_Read_Long(MEM9_Freq);

    Memory_Array[0].Band = EEPROM_Read(MEM0_Band);
    Memory_Array[1].Band = EEPROM_Read(MEM1_Band);
    Memory_Array[2].Band = EEPROM_Read(MEM2_Band);
    Memory_Array[3].Band = EEPROM_Read(MEM3_Band);
    Memory_Array[4].Band = EEPROM_Read(MEM4_Band);
    Memory_Array[5].Band = EEPROM_Read(MEM5_Band);
    Memory_Array[6].Band = EEPROM_Read(MEM6_Band);
    Memory_Array[7].Band = EEPROM_Read(MEM7_Band);
    Memory_Array[8].Band = EEPROM_Read(MEM8_Band);
    Memory_Array[9].Band = EEPROM_Read(MEM9_Band);

    Memory_Array[0].Mode = EEPROM_Read(MEM0_Mode);
    Memory_Array[1].Mode = EEPROM_Read(MEM1_Mode);
    Memory_Array[2].Mode = EEPROM_Read(MEM2_Mode);
    Memory_Array[3].Mode = EEPROM_Read(MEM3_Mode);
    Memory_Array[4].Mode = EEPROM_Read(MEM4_Mode);
    Memory_Array[5].Mode = EEPROM_Read(MEM5_Mode);
    Memory_Array[6].Mode = EEPROM_Read(MEM6_Mode);
    Memory_Array[7].Mode = EEPROM_Read(MEM7_Mode);
    Memory_Array[8].Mode = EEPROM_Read(MEM8_Mode);
    Memory_Array[9].Mode = EEPROM_Read(MEM9_Mode);

    /********************INITIALIZE LAST BAND DATA ARRAYS***/

    Last_Band_Data_A[0].Freq = EEPROM_Read_Long(Last_A_160M_Freq);
    Last_Band_Data_A[1].Freq = EEPROM_Read_Long(Last_A_80M_Freq);
    Last_Band_Data_A[2].Freq = EEPROM_Read_Long(Last_A_40M_Freq);
    Last_Band_Data_A[3].Freq = EEPROM_Read_Long(Last_A_30M_Freq);
    Last_Band_Data_A[4].Freq = EEPROM_Read_Long(Last_A_20M_Freq);
    Last_Band_Data_A[5].Freq = EEPROM_Read_Long(Last_A_17M_Freq);
    Last_Band_Data_A[6].Freq = EEPROM_Read_Long(Last_A_15M_Freq);
    Last_Band_Data_A[7].Freq = EEPROM_Read_Long(Last_A_12M_Freq);
    Last_Band_Data_A[8].Freq = EEPROM_Read_Long(Last_A_10aM_Freq);
    Last_Band_Data_A[9].Freq = EEPROM_Read_Long(Last_A_10bM_Freq);
    Last_Band_Data_A[10].Freq = EEPROM_Read_Long(Last_A_10cM_Freq);

    Last_Band_Data_A[0].Mode = EEPROM_Read(Last_A_160M_Mode);
    Last_Band_Data_A[1].Mode = EEPROM_Read(Last_A_80M_Mode);
    Last_Band_Data_A[2].Mode = EEPROM_Read(Last_A_40M_Mode);
    Last_Band_Data_A[3].Mode = EEPROM_Read(Last_A_30M_Mode);
    Last_Band_Data_A[4].Mode = EEPROM_Read(Last_A_20M_Mode);
    Last_Band_Data_A[5].Mode = EEPROM_Read(Last_A_17M_Mode);
    Last_Band_Data_A[6].Mode = EEPROM_Read(Last_A_15M_Mode);
    Last_Band_Data_A[7].Mode = EEPROM_Read(Last_A_12M_Mode);
    Last_Band_Data_A[8].Mode = EEPROM_Read(Last_A_10aM_Mode);
    Last_Band_Data_A[9].Mode = EEPROM_Read(Last_A_10bM_Mode);
    Last_Band_Data_A[10].Mode = EEPROM_Read(Last_A_10cM_Mode);

    Last_Band_Data_B[0].Freq = EEPROM_Read_Long(Last_B_160M_Freq);
    Last_Band_Data_B[1].Freq = EEPROM_Read_Long(Last_B_80M_Freq);
    Last_Band_Data_B[2].Freq = EEPROM_Read_Long(Last_B_40M_Freq);
    Last_Band_Data_B[3].Freq = EEPROM_Read_Long(Last_B_30M_Freq);
    Last_Band_Data_B[4].Freq = EEPROM_Read_Long(Last_B_20M_Freq);
    Last_Band_Data_B[5].Freq = EEPROM_Read_Long(Last_B_17M_Freq);
    Last_Band_Data_B[6].Freq = EEPROM_Read_Long(Last_B_15M_Freq);
    Last_Band_Data_B[7].Freq = EEPROM_Read_Long(Last_B_12M_Freq);
    Last_Band_Data_B[8].Freq = EEPROM_Read_Long(Last_B_10aM_Freq);
    Last_Band_Data_B[9].Freq = EEPROM_Read_Long(Last_B_10bM_Freq);
    Last_Band_Data_B[10].Freq = EEPROM_Read_Long(Last_B_10cM_Freq);

    Last_Band_Data_B[0].Mode = EEPROM_Read(Last_B_160M_Mode);
    Last_Band_Data_B[1].Mode = EEPROM_Read(Last_B_80M_Mode);
    Last_Band_Data_B[2].Mode = EEPROM_Read(Last_B_40M_Mode);
    Last_Band_Data_B[3].Mode = EEPROM_Read(Last_B_30M_Mode);
    Last_Band_Data_B[4].Mode = EEPROM_Read(Last_B_20M_Mode);
    Last_Band_Data_B[5].Mode = EEPROM_Read(Last_B_17M_Mode);
    Last_Band_Data_B[6].Mode = EEPROM_Read(Last_B_15M_Mode);
    Last_Band_Data_B[7].Mode = EEPROM_Read(Last_B_12M_Mode);
    Last_Band_Data_B[8].Mode = EEPROM_Read(Last_B_10aM_Mode);
    Last_Band_Data_B[9].Mode = EEPROM_Read(Last_B_10bM_Mode);
    Last_Band_Data_B[10].Mode = EEPROM_Read(Last_B_10cM_Mode);

    VFO_A[0].Band = EEPROM_Read(A_Band);
    VFO_B[0].Band = EEPROM_Read(B_Band);

    VFOA_Last_Band = VFO_A[0].Band;
    VFOB_Last_Band = VFO_B[0].Band;

    VFO_A[0].Freq = EEPROM_Read_Long(VFOA_Freq);
    VFO_B[0].Freq = EEPROM_Read_Long(VFOB_Freq);

    VFO_A[0].Mode = EEPROM_Read(VFOA_Mode);
    VFO_B[0].Mode = EEPROM_Read(VFOB_Mode);

    MEM_NUM = 6;                                                                    //start on MEM 6
    MEM = Memory_Array[MEM_NUM].Freq;
    
    Write_Two_I2C_Byte(Relay_Driver_1, 0x00, 0x00);                                 //All relays to reset position
    Write_Two_I2C_Byte(Relay_Driver_2, 0x00, 0x00);

    Write_Two_I2C_Byte(Audio_Attn, Write_Pot_One, EEPROM_Read(Main_Audio_Level));   //Initialize audio pots
    Write_Two_I2C_Byte(Audio_Attn, Write_Pot_Two, EEPROM_Read(Aux_Audio_Level));
    
    
    Write_I2C_Byte(IF_Board, EEPROM_Read(AGC_Mode) + (EEPROM_Read(Man_IF_Gain_Level) << 3)); //Manual IF gain is ignored by IF board if AGC command is SLOW or FAST
    //Write_I2C_Byte(IF_Board, AGC_SLW_REC + (EEPROM_Read(AGC_Slow_Rec_Rate) << 3));
    //Write_I2C_Byte(IF_Board, AGC_FST_REC + (EEPROM_Read(AGC_Fast_Rec_Rate) << 3));
    //Write_I2C_Byte(IF_Board, AGC_ATCK_RATE + (EEPROM_Read(AGC_Attack_Rate) << 3));


/*********************INTERRUPT INITIALIZATION********************************/

    /********************Always place at end of initialization routine********/

    RCONbits.IPEN = 0;              //Disable priority levels on interrupts
    
    INTCON = 0x08;
                        //Bit 7 GIE/GIEH    clear - disable all unmasked interrupts
                        //Bit 6 PEIE/GIEL   clear - disable all peripheral interrupts
                        //Bit 5 TMR0IE      clear - disable TMR0 overflow interrupt
                        //Bit 4 INT0IE      clear - disable INT0 external interrupt
                        //Bit 3 RBIE        set - enable IOCB port change interrupt
                        //Bit 2 TMR0IF      clear - TMR0 register overflow flag
                        //Bit 1 INT0IF      clear - INT0 external interrupt flag
                        //Bit 0 RBIF        clear - Port B IOC interrupt flag bit

    INTCON2 = 0x80;
                        //Bit 7 RBPU        set - PORTB pull-ups are disabled
                        //Bit 6 INTEDG0     clear - interrupt on falling edge
                        //Bit 5 INTEDG1     clear - interrupt on falling edge
                        //Bit 4 INTEDG2     clear - interrupt on falling edge
                        //Bit 3 Unused      clear
                        //Bit 2 TMR0IP      clear - TMR0 Overflow Interrupt Priority bit
                        //Bit 1 Unused      clear
                        //Bit 0 RBIP        clear - RB Port Change Interrupt Priority bit

    INTCON3 = 0x00;
                        //Bit 7 INT2IP      clear - INT2 External Interrupt Priority bit
                        //Bit 6 INT1IP      clear - INT1 External Interrupt Priority bit
                        //Bit 5 Unused      clear
                        //Bit 4 INT2IE      clear - INT2 External Interrupt Enable bit
                        //Bit 3 INT1IE      clear - INT1 External Interrupt Enable bit
                        //Bit 2 Unused      clear
                        //Bit 1 INT2IF      clear - INT2 External Interrupt Flag bit
                        //Bit 0 INT1IF      clear - INT1 External Interrupt Flag bit

    IOCB = 0x30;
                        //Bit 7 IOCB7       clear - Interrupt on change enable bit
                        //Bit 6 IOCB6       clear - Interrupt on change enable bit
                        //Bit 5 IOCB5       set - Interrupt on change enable bit
                        //Bit 4 IOCB4       set - Interrupt on change enable bit
                        //Bit 3 Unused      clear
                        //Bit 2 Unused      clear
                        //Bit 1 Unused      clear
                        //Bit 0 Unused      clear

    Old_Encdr = PORTB & 0x30;                       //Read encoder status & store
}

    
void Initialization_Right_Display(void)                          //RIGHT DISPLAY BOARD INITIALIZATION
{

    AGC = 0;                                        //AGC MODE: SLOW
    IFBW = 0;                                       //IF BW: 2500Hz
    RATE = 1;                                       //TUNING RATE: 100Hz
    TuningRate = 100;
    ATTN = 0;                                       //RF ATTN: 0dB
    MUTE = 0;                                       //MUTE: OFF
    AMP = 0;                                        //RF PREAMP: OFF
    TXLK = 0;                                       //TX LOCK: OFF

    __delay_ms(2);
    Write_I2C_Byte(Right_Display, AGC_Cmnd+AGC);
    __delay_ms(2);
    Write_I2C_Byte(Right_Display, IFBW_Cmnd+IFBW);
    __delay_ms(2);
    RelayBoard_IF_BW_Select(IFBW);
    __delay_ms(2);
    Write_I2C_Byte(Right_Display, RATE_Cmnd+RATE);
    __delay_ms(2);
    Write_I2C_Byte(Right_Display, ATTN_Cmnd+ATTN);
    __delay_ms(2);
    Write_I2C_Byte(Right_Display, TXLK_Cmnd+TXLK);
    __delay_ms(2);
    Write_I2C_Byte(Right_Display, MUTE_Cmnd+MUTE);
    __delay_ms(2);
    Write_I2C_Byte(Right_Display, AMP_Cmnd+AMP);
}


void Initialization_EEPROM (void)                                           //Initialize new board
{
    EEPROM_Write(EEPROM_INIT, 0xAB);                            
        
    //Initialize memories with JT65 frequencies

    EEPROM_Write_Long(MEM0_Freq, 1838000+1500);
    EEPROM_Write_Long(MEM1_Freq, 3576000+1500);
    EEPROM_Write_Long(MEM2_Freq, 7039000+1500);
    EEPROM_Write_Long(MEM3_Freq, 7076000+1500);
    EEPROM_Write_Long(MEM4_Freq, 10139000+1500);
    EEPROM_Write_Long(MEM5_Freq, 14076000+1500);
    EEPROM_Write_Long(MEM6_Freq, 14300000+1500);
    EEPROM_Write_Long(MEM7_Freq, 21076000+1500);
    EEPROM_Write_Long(MEM8_Freq, 24917000+1500);
    EEPROM_Write_Long(MEM9_Freq, 28076000+1500);

    EEPROM_Write(MEM0_Band, 0x00);
    EEPROM_Write(MEM1_Band, 0x01);
    EEPROM_Write(MEM2_Band, 0x02);
    EEPROM_Write(MEM3_Band, 0x02);
    EEPROM_Write(MEM4_Band, 0x03);
    EEPROM_Write(MEM5_Band, 0x04);
    EEPROM_Write(MEM6_Band, 0x04);
    EEPROM_Write(MEM7_Band, 0x06);
    EEPROM_Write(MEM8_Band, 0x07);
    EEPROM_Write(MEM9_Band, 0x08);

    EEPROM_Write(MEM0_Mode, 0x00);
    EEPROM_Write(MEM1_Mode, 0x00);
    EEPROM_Write(MEM2_Mode, 0x00);
    EEPROM_Write(MEM3_Mode, 0x00);
    EEPROM_Write(MEM4_Mode, 0x00);
    EEPROM_Write(MEM5_Mode, 0x00);
    EEPROM_Write(MEM6_Mode, 0x00);
    EEPROM_Write(MEM7_Mode, 0x00);
    EEPROM_Write(MEM8_Mode, 0x00);
    EEPROM_Write(MEM9_Mode, 0x00);

    EEPROM_Write_Long(Last_A_160M_Freq, 1838000+1500);
    EEPROM_Write_Long(Last_A_80M_Freq, 3576000+1500);
    EEPROM_Write_Long(Last_A_40M_Freq, 7039000+1500);
    EEPROM_Write_Long(Last_A_30M_Freq, 10139000+1500);
    EEPROM_Write_Long(Last_A_20M_Freq, 14076000+1500);
    EEPROM_Write_Long(Last_A_17M_Freq, 18102000+1500);
    EEPROM_Write_Long(Last_A_15M_Freq, 21076000+1500);
    EEPROM_Write_Long(Last_A_12M_Freq, 24917000+1500);
    EEPROM_Write_Long(Last_A_10aM_Freq, 28076000+1500);
    EEPROM_Write_Long(Last_A_10bM_Freq, 28500000+1500);
    EEPROM_Write_Long(Last_A_10cM_Freq, 29000000+1500);

    EEPROM_Write(Last_A_160M_Mode, 0x00);
    EEPROM_Write(Last_A_80M_Mode, 0x00);
    EEPROM_Write(Last_A_40M_Mode, 0x00);
    EEPROM_Write(Last_A_30M_Mode, 0x00);
    EEPROM_Write(Last_A_20M_Mode, 0x00);
    EEPROM_Write(Last_A_17M_Mode, 0x00);
    EEPROM_Write(Last_A_15M_Mode, 0x00);
    EEPROM_Write(Last_A_12M_Mode, 0x00);
    EEPROM_Write(Last_A_10aM_Mode, 0x00);
    EEPROM_Write(Last_A_10bM_Mode, 0x00);
    EEPROM_Write(Last_A_10cM_Mode, 0x00);

    //Initialize VFO B with WSPR frequencies and modes

    EEPROM_Write_Long(Last_B_160M_Freq, 1836600+1500);
    EEPROM_Write_Long(Last_B_80M_Freq, 3592600+1500);
    EEPROM_Write_Long(Last_B_40M_Freq, 7038600+1500);
    EEPROM_Write_Long(Last_B_30M_Freq, 10138700+1500);
    EEPROM_Write_Long(Last_B_20M_Freq, 14095600+1500);
    EEPROM_Write_Long(Last_B_17M_Freq, 18104600+1500);
    EEPROM_Write_Long(Last_B_15M_Freq, 21094600+1500);
    EEPROM_Write_Long(Last_B_12M_Freq, 24924600+1500);
    EEPROM_Write_Long(Last_B_10aM_Freq, 28124000+1500);
    EEPROM_Write_Long(Last_B_10bM_Freq, 28500000+1500);
    EEPROM_Write_Long(Last_B_10cM_Freq, 29000000+1500);

    EEPROM_Write(Last_B_160M_Mode, 0x00);
    EEPROM_Write(Last_B_80M_Mode, 0x00);
    EEPROM_Write(Last_B_40M_Mode, 0x00);
    EEPROM_Write(Last_B_30M_Mode, 0x00);
    EEPROM_Write(Last_B_20M_Mode, 0x00);
    EEPROM_Write(Last_B_17M_Mode, 0x00);
    EEPROM_Write(Last_B_15M_Mode, 0x00);
    EEPROM_Write(Last_B_12M_Mode, 0x00);
    EEPROM_Write(Last_B_10aM_Mode, 0x00);
    EEPROM_Write(Last_B_10bM_Mode, 0x00);
    EEPROM_Write(Last_B_10cM_Mode, 0x00);

    //VFO A & B start up frequencies
        
    EEPROM_Write_Long(VFOA_Freq, (14250000+1500));  //need to add USB offset
    EEPROM_Write_Long(VFOB_Freq, (10124000+1500));  //need to add USB offset
    
    EEPROM_Write(A_Band, 0x04);                     //20M Band
    EEPROM_Write(B_Band, 0x03);                     //30M Band

    EEPROM_Write(VFOA_Mode, 0x00);                  //USB
    EEPROM_Write(VFOB_Mode, 0x00);                  //USB
        
    EEPROM_Write(Aux_Audio_Level, 0x00);            //Default aux audio levels
    EEPROM_Write(Main_Audio_Level, 0x00);
    
    EEPROM_Write(AGC_Mode, 0x00);                   //Default AGC mode = SLOW
    EEPROM_Write(Man_IF_Gain_Level, 0x00);          //Default IF gain level = -45dB
    EEPROM_Write(AGC_Slow_Rec_Rate, 0x01);
    EEPROM_Write(AGC_Fast_Rec_Rate, 0x05);
    EEPROM_Write(AGC_Attack_Rate, 0x19);
    
}


/******************************************************************************
 * File Name    : lcd.h
 * Description  : Header file for the 20x4 LCD driver. Provides APIs for LCD
 *                initialization, command transmission, data display, and
 *                custom character generation.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef LCD_H
#define LCD_H

#include "types.h"

/*----------------------------------------------------------------------------
 * LCD Driver APIs
 *--------------------------------------------------------------------------*/

/* Low-Level LCD Functions */
void WriteLCD(u8 byte);
void CmdLCD(u8 cmd);
void BuildCGRAM(void);

/* LCD Initialization */
void InitLCD(void);

/* Display Functions */
void CharLCD(u8 asciival);
void StrLCD(s8 *str);
void U32LCD(u32 num);
void S32LCD(s32 num);
void F32LCD(f32 num, u8 ndp);

#endif

/******************************************************************************
 * File Name    : lcd_defines.h
 * Description  : LCD command definitions and GPIO pin assignments for the
 *                20x4 Character LCD interfaced with LPC2148.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef LCD_DEFINES_H
#define LCD_DEFINES_H

/*----------------------------------------------------------------------------
 * LCD Command Definitions
 *--------------------------------------------------------------------------*/

/* Basic Commands */
#define CLEAR_LCD          0x01
#define RET_CUR_HOME       0x02

/* Cursor Movement */
#define SHIFT_CUR_RIGHT    0x06
#define SHIFT_CUR_LEFT     0x07

/* Display Control */
#define DSP_OFF            0x08
#define DSP_ON_CUR_OFF     0x0C
#define DSP_ON_CUR_ON      0x0E
#define DSP_ON_CUR_BLINK   0x0F

/* Display Shift */
#define SHIFT_DSP_LEFT     0x10
#define SHIFT_DSP_RIGHT    0x14

/* LCD Interface Modes */
#define MODE_8BIT_1LINE    0x30
#define MODE_8BIT_2LINE    0x38
#define MODE_4BIT_1LINE    0x20
#define MODE_4BIT_2LINE    0x28

/* DDRAM Addresses */
#define GOTO_LINE1_POS0    0x80
#define GOTO_LINE2_POS0    0xC0
#define GOTO_LINE3_POS0    0x94
#define GOTO_LINE4_POS0    0xD4

/* CGRAM Starting Address */
#define GOTO_CGRAM_START   0x40


/*----------------------------------------------------------------------------
 * LCD Pin Configuration
 *--------------------------------------------------------------------------*/

/* LCD Data Bus starts from P0.8 */
#define LCD_DATA           8

/* LCD Control Pins */
#define LCD_RS             16
#define LCD_EN             17
#define LCD_RW             18

#endif

/******************************************************************************
 * File Name    : lcd.c
 * Description  : LCD driver for interfacing 16x2 LCD with LPC2148 in 8-bit
 *                mode. This driver provides functions to initialize the LCD,
 *                display characters, strings, numbers, floating point values,
 *                and create custom characters using CGRAM.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include <LPC21xx.h>
#include "delays.h"
#include "lcd_defines.h"
#include "defines.h"
#include "types.h"

/*----------------------------------------------------------------------------
 * Custom Character Lookup Table
 * Stores user-defined characters that are loaded into the LCD CGRAM.
 *--------------------------------------------------------------------------*/
u8 Lut[] =
{
    /* Custom Character 0 : BOAT */
    0x00,0x04,0x04,0x1F,0x11,0x0E,0x04,0x00,

    /* Custom Character 1 : DIAMOND */
    0x00,0x04,0x0A,0x11,0x0A,0x04,0x00,0x00,

    /* Custom Character 2 : FLAG */
    0x04,0x07,0x07,0x07,0x04,0x04,0x04,0x00,

    /* Custom Character 3 : TREE */
    0x04,0x0E,0x1F,0x0E,0x1F,0x04,0x04,0x00,

    /* Custom Character 4 : TICK */
    0x00,0x00,0x01,0x03,0x16,0x1C,0x08,0x00,

    /* Custom Character 5 : CROSS */
    0x00,0x11,0x0A,0x04,0x04,0x0A,0x11,0x00
};

/******************************************************************************
 * Function Name : WriteLCD
 * Description   : Writes a byte (command/data) to the LCD.
 *
 * Parameter     : byte - Data or command byte to be written.
 *
 * Returns       : None
 ******************************************************************************/
void WriteLCD(u8 byte)
{
    /* Select write operation */
    IOCLR0 = 1 << LCD_RW;

    /* Place the byte on LCD data bus */
    WRITEBYTE(IOPIN0, LCD_DATA, byte);

    /* Generate Enable pulse to latch data */
    IOSET0 = 1 << LCD_EN;
    delay_us(1);
    IOCLR0 = 1 << LCD_EN;

    /* Wait for LCD execution */
    delay_ms(2);
}

/******************************************************************************
 * Function Name : CmdLCD
 * Description   : Sends a command to the LCD.
 *
 * Parameter     : Cmd - LCD command.
 *
 * Returns       : None
 ******************************************************************************/
void CmdLCD(u8 Cmd)
{
    /* Select command register */
    IOCLR0 = 1 << LCD_RS;

    /* Send command */
    WriteLCD(Cmd);

    /* Restore RS for data operations */
    IOSET0 = 1 << LCD_RS;
}

/******************************************************************************
 * Function Name : BuildCGRAM
 * Description   : Loads user-defined characters into LCD CGRAM.
 *
 * Returns       : None
 ******************************************************************************/
void BuildCGRAM(void)
{
    u32 i;
    u32 nBytes = sizeof(Lut);

    /* Move cursor to CGRAM */
    CmdLCD(GOTO_CGRAM_START);

    /* Select data register */
    IOCLR0 = 1 << LCD_RW;
    IOSET0 = 1 << LCD_RS;

    /* Load custom characters */
    for(i = 0; i < nBytes; i++)
    {
        WriteLCD(Lut[i]);
    }

    /* Return cursor to first line */
    CmdLCD(GOTO_LINE1_POS0);
}

/******************************************************************************
 * Function Name : InitLCD
 * Description   : Initializes the LCD in 8-bit, 2-line mode and loads
 *                 custom characters.
 *
 * Returns       : None
 ******************************************************************************/
void InitLCD(void)
{
    /* Configure LCD data pins as output */
    WRITEBYTE(IODIR0, LCD_DATA, 0xFF);

    /* Configure LCD control pins as output */
    SETBIT(IODIR0, LCD_RS);
    SETBIT(IODIR0, LCD_RW);
    SETBIT(IODIR0, LCD_EN);

    /* LCD power-on delay */
    delay_ms(15);

    /* Initialization sequence */
    CmdLCD(0x30);
    delay_ms(4);

    delay_us(100);
    CmdLCD(0x30);

    delay_us(100);
    CmdLCD(0x30);

    /* Configure LCD */
    CmdLCD(MODE_8BIT_2LINE);
    CmdLCD(DSP_ON_CUR_OFF);
    CmdLCD(0x01);                /* Clear display */
    CmdLCD(SHIFT_CUR_RIGHT);     /* Entry mode */

    /* Load custom characters */
    BuildCGRAM();
}

/******************************************************************************
 * Function Name : CharLCD
 * Description   : Displays a single character on the LCD.
 *
 * Parameter     : asciival - ASCII character.
 *
 * Returns       : None
 ******************************************************************************/
void CharLCD(u8 asciival)
{
    WriteLCD(asciival);
}

/******************************************************************************
 * Function Name : StrLCD
 * Description   : Displays a null-terminated string on the LCD.
 *
 * Parameter     : str - Pointer to string.
 *
 * Returns       : None
 ******************************************************************************/
void StrLCD(s8 *str)
{
    while(*str)
    {
        CharLCD(*str++);
    }
}

/******************************************************************************
 * Function Name : U32LCD
 * Description   : Displays an unsigned 32-bit integer on the LCD.
 *
 * Parameter     : num - Unsigned integer.
 *
 * Returns       : None
 ******************************************************************************/
void U32LCD(u32 num)
{
    u8 arr[11] = {0};
    s32 count = 0;
    s32 i;

    if(num == 0)
    {
        CharLCD('0');
        return;
    }

    /* Extract digits */
    while(num > 0)
    {
        arr[count++] = num % 10;
        num /= 10;
    }

    /* Display digits in correct order */
    for(i = count - 1; i >= 0; i--)
    {
        CharLCD(arr[i] + '0');
    }
}

/******************************************************************************
 * Function Name : S32LCD
 * Description   : Displays a signed 32-bit integer on the LCD.
 *
 * Parameter     : num - Signed integer.
 *
 * Returns       : None
 ******************************************************************************/
void S32LCD(s32 num)
{
    if(num < 0)
    {
        CharLCD('-');
        num = -num;
    }

    U32LCD(num);
}

/******************************************************************************
 * Function Name : F32LCD
 * Description   : Displays a floating point number with the specified number
 *                 of decimal places.
 *
 * Parameters    :
 *      num - Floating point number.
 *      ndp - Number of digits after decimal point.
 *
 * Returns       : None
 ******************************************************************************/
void F32LCD(f32 num, u8 ndp)
{
    s32 temp = num;
    s32 i;

    /* Display integer part */
    S32LCD(temp);

    CharLCD('.');

    /* Handle negative numbers */
    if(num < 0)
    {
        num = -num;
    }

    /* Extract fractional part */
    num = num - temp;

    for(i = 0; i < ndp; i++)
    {
        num *= 10;
    }

    temp = num;

    /* Display fractional part */
    U32LCD(temp);
}

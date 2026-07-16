/******************************************************************************
 * File Name    : dl.c
 * Description  : Implements the Driving License service. This module provides
 *                RTC configuration, driving license validation based on the
 *                current date, and displays the driving license information
 *                of the authenticated user.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/*----------------------------------------------------------------------------
 * Global Variables
 *--------------------------------------------------------------------------*/

/* Stores current RTC date and time */
u32 year;
u8 hour, min, sec;
u8 date, month;

/* RTC configuration request flag (set by external interrupt) */
u8 RTC_Flag = 0;

/******************************************************************************
 * Function Name : Read_Date
 * Description   : Reads the date from the keypad and validates the entered
 *                 day, month and year before updating the global variables.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Read_Date(void)
{
    u32 temp = 0;
    u8 flag = 0;

L1:
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter DATE:");

    CmdLCD(GOTO_LINE2_POS0);
    U32LCD(date);

    CmdLCD(GOTO_LINE2_POS0);
    temp = Read_Num(date, &flag, GOTO_LINE2_POS0);

    if(flag)
    {
        return;
    }

    if(temp < 1 || temp > 31)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Input");
        goto L1;
    }

    date = temp;

L2:
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter Month:");

    CmdLCD(GOTO_LINE2_POS0);
    U32LCD(month);

    CmdLCD(GOTO_LINE2_POS0);
    temp = Read_Num(month, &flag, GOTO_LINE2_POS0);

    if(flag)
    {
        return;
    }

    if(temp < 1 || temp > 12)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Input");
        goto L2;
    }

    month = temp;

    CmdLCD(CLEAR_LCD);
    StrLCD("Enter YEAR:");

    CmdLCD(GOTO_LINE2_POS0);
    U32LCD(year);

    CmdLCD(GOTO_LINE2_POS0);
    temp = Read_Num(year, &flag, GOTO_LINE2_POS0);

    if(flag)
    {
        return;
    }

    year = temp;
}

/******************************************************************************
 * Function Name : Read_Time
 * Description   : Reads the current time from the keypad and updates the
 *                 global RTC time variables.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Read_Time(void)
{
    u32 temp = 0;
    u8 flag = 0;

L1:
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter HOUR:");

    CmdLCD(GOTO_LINE2_POS0);
    U32LCD(hour);

    CmdLCD(GOTO_LINE2_POS0);
    temp = Read_Num(hour, &flag, GOTO_LINE2_POS0);

    if(flag)
    {
        return;
    }

    if(temp > 23)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Input");
        goto L1;
    }

    hour = temp;

L2:
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter MIN:");

    CmdLCD(GOTO_LINE2_POS0);
    U32LCD(min);

    CmdLCD(GOTO_LINE2_POS0);
    temp = Read_Num(min, &flag, GOTO_LINE2_POS0);

    if(flag)
    {
        return;
    }

    if(temp > 59)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Input");
        goto L2;
    }

    min = temp;

    /* Seconds are initialized to zero */
    sec = 0;
}

/******************************************************************************
 * Function Name : Configure_Rtc
 * Description   : Reads the current RTC values, allows the user to modify
 *                 the date and time, and updates the RTC peripheral.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Configure_Rtc(void)
{
    /* Read current RTC values */
    RTC_Get_Time(&hour, &min, &sec);
    RTC_Get_Date(&date, &month, &year);

    /* Read updated values from user */
    Read_Date();
    Read_Time();

    /* Update RTC */
    RTC_Set_Date(date, month, year);
    RTC_Set_Time(hour, min, sec);

    /* Clear RTC configuration request */
    RTC_Flag = 0;
}

/******************************************************************************
 * Function Name : Get_DL_Details
 * Description   : Extracts the expiry date, month, and year from the driving
 *                 license validity string (DD/MM/YYYY).
 *
 * Parameters    :
 *      dl_date  - Pointer to store expiry date.
 *      dl_month - Pointer to store expiry month.
 *      dl_year  - Pointer to store expiry year.
 *
 * Returns       : None
 ******************************************************************************/
void Get_DL_Details(u8 *dl_date, u8 *dl_month, u16 *dl_year)
{
    u16 temp = 0;
    u8 i = 0;

    /* Extract Date (DD) */
    while(i < 2)
    {
        temp *= 10;
        temp += (rfid[curr_card_index].dl.validity[i] - '0');
        i++;
    }

    *dl_date = temp;

    /* Skip '/' */
    i++;
    temp = 0;

    /* Extract Month (MM) */
    while(i < 5)
    {
        temp *= 10;
        temp += (rfid[curr_card_index].dl.validity[i] - '0');
        i++;
    }

    *dl_month = temp;

    /* Skip '/' */
    i++;
    temp = 0;

    /* Extract Year (YYYY) */
    while(i < 10)
    {
        temp *= 10;
        temp += (rfid[curr_card_index].dl.validity[i] - '0');
        i++;
    }

    *dl_year = temp;
}

/******************************************************************************
 * Function Name : Get_DL_Validity
 * Description   : Compares the current RTC date with the driving license
 *                 expiry date and determines whether the license is valid.
 *
 * Parameters    : None
 *
 * Returns       :
 *      1 -> Driving License is Valid
 *      0 -> Driving License has Expired
 ******************************************************************************/
u8 Get_DL_Validity(void)
{
    u8 dl_date;
    u8 dl_month;
    u16 dl_year;

    /* Extract expiry date from DL record */
    Get_DL_Details(&dl_date, &dl_month, &dl_year);

    /* Compare Year */
    if(year < dl_year)
    {
        return 1;
    }
    else if(year > dl_year)
    {
        return 0;
    }

    /* Compare Month */
    if(month < dl_month)
    {
        return 1;
    }
    else if(month > dl_month)
    {
        return 0;
    }

    /* Compare Date */
    if(date <= dl_date)
    {
        return 1;
    }

    return 0;
}
/******************************************************************************
 * Function Name : Show_DL_Info
 * Description   : Displays the driving license details of the authenticated
 *                 user along with the license validity status. A custom Tick
 *                 symbol indicates a valid license, while a Cross symbol
 *                 indicates an expired license.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Show_DL_Info(void)
{
    u8 flag = 0;

    CmdLCD(CLEAR_LCD);

    /* Display Driving License Number */
    StrLCD("DL:");
    StrLCD(rfid[curr_card_index].dl.no);

    /* Display Vehicle Class */
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Type:");
    StrLCD(rfid[curr_card_index].dl.type);

    /* Display Expiry Date */
    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("Exp:");
    StrLCD(rfid[curr_card_index].dl.validity);

    /* Display validity status inside brackets */
    CmdLCD(GOTO_LINE3_POS0 + 17);
    CharLCD('(');

    CmdLCD(GOTO_LINE3_POS0 + 18);

    if(Get_DL_Validity())
    {
        /* Tick symbol */
        CharLCD(4);
    }
    else
    {
        /* Cross symbol */
        CharLCD(5);
    }

    CmdLCD(GOTO_LINE3_POS0 + 19);
    CharLCD(')');

    /* Display Address Information */
    CmdLCD(GOTO_LINE4_POS0);
    StrLCD("Addr:");
    StrLCD(rfid[curr_card_index].dl.state);
    StrLCD("-");
    U32LCD(rfid[curr_card_index].dl.pincode);

    /* Wait for any key press or timeout */
    KeyScan_t(&flag);
}

/******************************************************************************
 * Function Name : Get_Dl_Info
 * Description   : Reads the current RTC date and time, displays the driving
 *                 license information, and returns control to the dashboard.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Get_Dl_Info(void)
{
    /* Read current RTC date and time */
    RTC_Get_Date(&date, &month, &year);
    RTC_Get_Time(&hour, &min, &sec);

    /* Display driving license details */
    Show_DL_Info();

    /* Return to Dashboard */
    state = DASHBOARD;
}

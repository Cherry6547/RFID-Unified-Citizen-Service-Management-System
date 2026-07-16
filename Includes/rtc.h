/******************************************************************************
 * File Name    : rtc.h
 * Description  : Header file for the Real Time Clock (RTC) driver. Provides
 *                APIs for RTC initialization, date/time configuration, and
 *                retrieval of current date, time, and day information.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef _RTC_H
#define _RTC_H

#include "types.h"

/*----------------------------------------------------------------------------
 * Days of the Week
 *--------------------------------------------------------------------------*/
enum day
{
    SUN,
    MON,
    TUE,
    WED,
    THU,
    FRI,
    SAT
};

/* Day name lookup table */
extern s8 *days[];

/*----------------------------------------------------------------------------
 * RTC Driver APIs
 *--------------------------------------------------------------------------*/

/* Initialize RTC peripheral */
void RTC_Init(void);

/* Read current time */
void RTC_Get_Time(u8 *hour, u8 *min, u8 *sec);

/* Read current date */
void RTC_Get_Date(u8 *date, u8 *month, u32 *year);

/* Read current day of the week */
void RTC_Get_Day(u8 *day);

/* Configure current time */
void RTC_Set_Time(u8 hour, u8 min, u8 sec);

/* Configure current date */
void RTC_Set_Date(u8 date, u8 month, u32 year);

/* Configure current day of the week */
void RTC_Set_Day(u8 day);

#endif

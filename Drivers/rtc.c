/******************************************************************************
 * File Name    : rtc.c
 * Description  : Real Time Clock (RTC) driver for LPC2148.
 *                This driver initializes the RTC using the external 32.768 kHz
 *                crystal oscillator and provides APIs to set and retrieve
 *                date, time and day information.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "types.h"
#include <LPC214x.h>

/* RTC Control Register Bit Definitions */
#define ENABLE     (1 << 0)    /* Enables RTC */
#define RESET      (1 << 1)    /* Resets RTC counters */
#define CLKSRC     (1 << 4)    /* Select external RTC crystal */

/* Lookup table for day names */
s8 *days[] =
{
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
};

/******************************************************************************
 * Function Name : RTC_Init
 * Description   : Initializes the RTC by selecting the external clock source
 *                 and enabling the RTC.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Init(void)
{
    /* Reset RTC to avoid uncertain state */
    CCR = RESET;

    /* Enable RTC and select external clock source */
    CCR = ENABLE | CLKSRC;
}

/******************************************************************************
 * Function Name : RTC_Get_Time
 * Description   : Reads the current time from the RTC.
 *
 * Parameters    :
 *      hour - Pointer to store hour.
 *      min  - Pointer to store minute.
 *      sec  - Pointer to store second.
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Get_Time(u8 *hour, u8 *min, u8 *sec)
{
    *hour = HOUR;
    *min  = MIN;
    *sec  = SEC;
}

/******************************************************************************
 * Function Name : RTC_Get_Date
 * Description   : Reads the current date from the RTC.
 *
 * Parameters    :
 *      date  - Pointer to store date.
 *      month - Pointer to store month.
 *      year  - Pointer to store year.
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Get_Date(u8 *date, u8 *month, u32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}

/******************************************************************************
 * Function Name : RTC_Get_Day
 * Description   : Reads the current day of the week.
 *
 * Parameters    :
 *      day - Pointer to store day number.
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Get_Day(u8 *day)
{
    *day = DOW;
}

/******************************************************************************
 * Function Name : RTC_Set_Time
 * Description   : Updates the RTC time registers.
 *
 * Parameters    :
 *      hour - Hour (0-23)
 *      min  - Minute (0-59)
 *      sec  - Second (0-59)
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Set_Time(u8 hour, u8 min, u8 sec)
{
    HOUR = hour;
    MIN  = min;
    SEC  = sec;
}

/******************************************************************************
 * Function Name : RTC_Set_Date
 * Description   : Updates the RTC date registers.
 *
 * Parameters    :
 *      date  - Day of month
 *      month - Month
 *      year  - Year
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Set_Date(u8 date, u8 month, u32 year)
{
    DOM   = date;
    MONTH = month;
    YEAR  = year;
}

/******************************************************************************
 * Function Name : RTC_Set_Day
 * Description   : Updates the day of the week register.
 *
 * Parameters    :
 *      day - Day number (0 = Sunday, ..., 6 = Saturday)
 *
 * Returns       : None
 ******************************************************************************/
void RTC_Set_Day(u8 day)
{
    DOW = day;
}

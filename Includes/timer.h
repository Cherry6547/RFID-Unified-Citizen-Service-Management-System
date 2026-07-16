/******************************************************************************
 * File Name    : timer.h
 * Description  : Header file for the Timer0 driver. Provides delay routines
 *                in microseconds, milliseconds, and seconds. Also supports
 *                background timer operation without blocking program execution.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef _TIMER_H
#define _TIMER_H

#include "types.h"

/*----------------------------------------------------------------------------
 * Timer0 Driver APIs
 *
 * LPC2148 provides two 32-bit timers (Timer0 and Timer1) which can operate
 * either as timers or event counters.
 *--------------------------------------------------------------------------*/

/* Initialize Timer0 */
void Timer0_Init(void);

/* Blocking delay routines */
void tdelay_us(u8 Mchn, u32 delay);
void tdelay_ms(u8 Mchn, u32 delay);
void tdelay_s(u8 Mchn, u32 delay);

/* Background (Non-Blocking) Delay */
void tdelay_sBG(u8 Mchn, u32 delay);

#endif

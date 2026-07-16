/******************************************************************************
 * File Name    : delays.h
 * Description  : Header file for software delay routines. Provides blocking
 *                delay functions in microseconds, milliseconds, and seconds.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef _DELAYS_H
#define _DELAYS_H

#include "types.h"

/*----------------------------------------------------------------------------
 * Software Delay APIs
 *--------------------------------------------------------------------------*/

/* Delay in Microseconds */
void delay_us(u32 delayUS);

/* Delay in Milliseconds */
void delay_ms(u32 delayMS);

/* Delay in Seconds */
void delay_s(u32 delayS);

#endif

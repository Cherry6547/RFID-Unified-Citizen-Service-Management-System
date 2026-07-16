/******************************************************************************
 * File Name    : led.h
 * Description  : Header file for the LED driver. Provides APIs to initialize
 *                and control the status LEDs connected to LPC2148 GPIO pins.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef LED_H
#define LED_H

#include "types.h"

/*----------------------------------------------------------------------------
 * LED Pin Definitions
 *--------------------------------------------------------------------------*/

/* Status LEDs connected to GPIO Port 0 */
#define LED_GREEN    25
#define LED_RED      28

/*----------------------------------------------------------------------------
 * LED Driver APIs
 *--------------------------------------------------------------------------*/

/* Initialize LED GPIO pins */
void Led_Init(void);

/* Turn ON specified LED */
void Led_On(u8 pinNo);

/* Turn OFF specified LED */
void Led_Off(u8 pinNo);

#endif

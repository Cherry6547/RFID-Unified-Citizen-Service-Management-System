/******************************************************************************
 * File Name    : buzzer.h
 * Description  : Header file for the buzzer driver. Provides APIs to
 *                initialize and control the buzzer connected to the LPC2148.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef BUZZER_H
#define BUZZER_H

#include "types.h"

/*----------------------------------------------------------------------------
 * Buzzer Pin Definition
 *--------------------------------------------------------------------------*/

/* Buzzer connected to GPIO Port 0 */
#define BUZZER    29

/*----------------------------------------------------------------------------
 * Buzzer Driver APIs
 *--------------------------------------------------------------------------*/

/* Initialize buzzer GPIO pin */
void Buzzer_Init(void);

/* Turn ON buzzer */
void Buzzer_On(void);

/* Turn OFF buzzer */
void Buzzer_Off(void);

#endif

/******************************************************************************
 * File Name    : exti_switch.h
 * Description  : Header file for the External Interrupt (EINT3) driver.
 *                Provides APIs to initialize the external interrupt and the
 *                corresponding Interrupt Service Routine (ISR).
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef EXTI_SWITCH_H
#define EXTI_SWITCH_H

#include "types.h"

/*----------------------------------------------------------------------------
 * External Interrupt Driver APIs
 *--------------------------------------------------------------------------*/

/* External Interrupt Service Routine (EINT3) */
void EXTI_ISR(void) __irq;

/* Initialize External Interrupt */
void Exti_switch_Init(void);

#endif

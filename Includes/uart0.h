/******************************************************************************
 * File Name    : uart0.h
 * Description  : Header file for the UART0 driver. Provides APIs for UART
 *                initialization, data transmission, and data reception.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef UART0_H
#define UART0_H

#include "types.h"

/*----------------------------------------------------------------------------
 * UART0 Driver APIs
 *--------------------------------------------------------------------------*/

/* Initialize UART0 peripheral */
void UART0_Init(void);

/* Transmit one byte */
void UART0_Tx(u8 data);

/* Receive one byte */
u8 UART0_Rx(void);

#endif

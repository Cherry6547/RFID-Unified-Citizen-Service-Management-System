/******************************************************************************
 * File Name    : kpm.h
 * Description  : Header file for the 4x4 Matrix Keypad driver. Provides APIs
 *                for keypad initialization, row/column scanning, and key
 *                detection with and without timeout support.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef _KPM_H
#define _KPM_H

#include "types.h"

/*----------------------------------------------------------------------------
 * Keypad Driver APIs
 *--------------------------------------------------------------------------*/

/* Initialize keypad GPIO pins */
void KPM_Init(void);

/* Scan keypad rows */
u32 RowScan(void);

/* Identify the active row */
u32 RowCheck(void);

/* Identify the active column */
u32 ColCheck(void);

/* Read a key (Blocking) */
u8 KeyScan(void);

/* Read a key with timeout support */
u8 KeyScan_t(u8 *flag);

#endif

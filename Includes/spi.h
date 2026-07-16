/******************************************************************************
 * File Name    : spi.h
 * Description  : Header file for the SPI0 driver. Provides APIs for SPI
 *                initialization and full-duplex data transfer with SPI
 *                peripherals such as the external EEPROM.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef SPI_H
#define SPI_H

#include "types.h"

/*----------------------------------------------------------------------------
 * SPI0 Driver APIs
 *--------------------------------------------------------------------------*/

/* Initialize SPI0 peripheral */
void SPI0_Init(void);

/* Transmit one byte and simultaneously receive one byte */
u8 SPI0(u8 data);

#endif

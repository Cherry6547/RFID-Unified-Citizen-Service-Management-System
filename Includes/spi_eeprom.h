/******************************************************************************
 * File Name    : spi_eeprom.h
 * Description  : Header file for the SPI EEPROM (25LC512) driver. Provides
 *                APIs for byte write, page write, sequential read, and byte
 *                read operations.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef SPI_EEPROM_H
#define SPI_EEPROM_H

#include "types.h"

/*----------------------------------------------------------------------------
 * SPI EEPROM Driver APIs
 *--------------------------------------------------------------------------*/

/* Write one byte into EEPROM */
void Byte_Write(u16 addr, u8 data);

/* Read multiple bytes sequentially from EEPROM */
void Sequential_Read(u16 addr, u8 nB, u8 *p);

/* Write multiple bytes into EEPROM */
void Page_Write(u16 addr, u8 nB, u8 *p);

/* Read one byte from EEPROM */
u8 ByteRead_25LC512(u16 rBufAddr);

#endif

/******************************************************************************
 * File Name    : spi_eeprom.c
 * Description  : SPI EEPROM (25LC512) driver for LPC2148. This driver provides
 *                APIs to send EEPROM commands, read/write a single byte,
 *                perform sequential reads, and write an entire page.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "spi.h"
#include "delays.h"
#include <LPC214x.h>
#include "spi_defines.h"

/*----------------------------------------------------------------------------
 * EEPROM Instruction Set
 *--------------------------------------------------------------------------*/
#define WRITE      0x02      /* Write data to memory */
#define READ       0x03      /* Read data from memory */
#define WRDI       0x04      /* Disable Write */
#define WREN       0x06      /* Enable Write */

/******************************************************************************
 * Function Name : Cmd
 * Description   : Sends a single command to the EEPROM.
 *
 * Parameters    :
 *      cmd - EEPROM command opcode.
 *
 * Returns       : None
 ******************************************************************************/
void Cmd(u8 cmd)
{
    /* Select EEPROM */
    IOCLR0 = (1 << CS);

    /* Send command */
    SPI0(cmd);

    /* Deselect EEPROM */
    IOSET0 = (1 << CS);
}

/******************************************************************************
 * Function Name : Byte_Write
 * Description   : Writes a single byte to the specified EEPROM address.
 *
 * Parameters    :
 *      addr - EEPROM memory address.
 *      data - Data byte to be written.
 *
 * Returns       : None
 ******************************************************************************/
void Byte_Write(u16 addr, u8 data)
{
    /* Enable write operation */
    Cmd(WREN);

    /* Begin write transaction */
    IOCLR0 = (1 << CS);

    SPI0(WRITE);          /* Write opcode */
    SPI0(addr >> 8);      /* Address MSB */
    SPI0(addr);           /* Address LSB */
    SPI0(data);           /* Data byte */

    /* End write transaction */
    IOSET0 = (1 << CS);

    /* Wait for internal EEPROM write cycle */
    delay_ms(10);

    /* Disable further write operations */
    Cmd(WRDI);
}

/******************************************************************************
 * Function Name : Sequential_Read
 * Description   : Reads multiple bytes sequentially from EEPROM.
 *
 * Parameters    :
 *      addr - Starting EEPROM address.
 *      nB   - Number of bytes to read.
 *      p    - Pointer to destination buffer.
 *
 * Returns       : None
 ******************************************************************************/
void Sequential_Read(u16 addr, u8 nB, u8 *p)
{
    u16 i = 0;

    /* Begin read transaction */
    IOCLR0 = (1 << CS);

    SPI0(READ);          /* Read opcode */
    SPI0(addr >> 8);     /* Address MSB */
    SPI0(addr);          /* Address LSB */

    /* Read requested number of bytes */
    while(i < nB)
    {
        *(p + i) = SPI0(0x00);    /* Dummy byte generates SPI clock */
        i++;
    }

    /* End read transaction */
    IOSET0 = (1 << CS);
}

/******************************************************************************
 * Function Name : Page_Write
 * Description   : Writes multiple bytes sequentially into one EEPROM page.
 *
 * Parameters    :
 *      addr - Starting EEPROM address.
 *      nB   - Number of bytes to write.
 *      p    - Pointer to source buffer.
 *
 * Returns       : None
 ******************************************************************************/
void Page_Write(u16 addr, u8 nB, u8 *p)
{
    u16 i = 0;

    /* Enable EEPROM write */
    Cmd(WREN);

    /* Begin page write transaction */
    IOCLR0 = (1 << CS);

    SPI0(WRITE);          /* Write opcode */
    SPI0(addr >> 8);      /* Address MSB */
    SPI0(addr);           /* Address LSB */

    /* Write all bytes */
    while(i < nB)
    {
        SPI0(*(p + i));
        i++;
    }

    /* End transaction */
    IOSET0 = (1 << CS);

    /* Disable write operation */
    Cmd(WRDI);
}

/******************************************************************************
 * Function Name : ByteRead_25LC512
 * Description   : Reads a single byte from the specified EEPROM address.
 *
 * Parameters    :
 *      rBufAddr - EEPROM address to read.
 *
 * Returns       :
 *      Byte read from EEPROM.
 ******************************************************************************/
u8 ByteRead_25LC512(u16 rBufAddr)
{
    u8 dat;

    /* Begin read transaction */
    IOCLR0 = (1 << CS);

    SPI0(READ);              /* Read opcode */
    SPI0(rBufAddr >> 8);     /* Address MSB */
    SPI0(rBufAddr);          /* Address LSB */

    /* Read data byte */
    dat = SPI0(0x00);

    /* End transaction */
    IOSET0 = (1 << CS);

    return dat;
}

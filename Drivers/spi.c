/******************************************************************************
 * File Name    : spi.c
 * Description  : SPI0 driver for LPC2148. This driver initializes the SPI
 *                peripheral in Master mode and provides an API to transmit
 *                and receive data over the SPI bus.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "types.h"
#include "spi_defines.h"
#include <LPC214x.h>

/******************************************************************************
 * Function Name : SPI0_Init
 * Description   : Initializes SPI0 in Master mode with a clock frequency of
 *                 1 MHz. Configures SPI pins and Chip Select (CS) pin.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void SPI0_Init(void)
{
    /* Configure SPI pins:
       P0.4  -> SCK
       P0.5  -> MISO
       P0.6  -> MOSI
    */
    PINSEL0 |= MISO | MOSI | SCK;

    /* Set SPI Clock = PCLK / 15 = 1 MHz
       PCLK = 15 MHz
       SPCCR = 15 (Register value = 14 since counting starts from 0)
    */
    S0SPCCR = 15 - 1;

    /* Configure SPI as Master
       CPOL = 1
       CPHA = 1
       Data is transferred MSB first
    */
    S0SPCR |= (1 << MSTR) | (1 << CPOL) | (1 << CPHA);
    // S0SPCR |= (1 << LSBF);     // Uncomment for LSB first transmission

    /* Configure Chip Select pin as GPIO output */
    IODIR0 |= (1 << CS);

    /* Keep slave deselected (CS High) */
    IOSET0 = (1 << CS);
}

/******************************************************************************
 * Function Name : SPI0
 * Description   : Transmits one byte over SPI and simultaneously receives
 *                 one byte from the slave device.
 *
 * Parameters    :
 *      data - Byte to be transmitted.
 *
 * Returns       :
 *      Byte received from the slave.
 ******************************************************************************/
u8 SPI0(u8 data)
{
    /* Load transmit data into SPI Data Register */
    S0SPDR = data;

    /* Wait until SPI transfer is complete */
    while(!((S0SPSR >> SPIF) & 1));

    /* Return received byte */
    return S0SPDR;
}

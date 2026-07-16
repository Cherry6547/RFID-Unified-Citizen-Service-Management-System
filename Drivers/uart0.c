/******************************************************************************
 * File Name    : uart0.c
 * Description  : UART0 driver for LPC2148. This driver provides APIs for
 *                UART initialization, data transmission, data reception, and
 *                interrupt-based UART receive handling.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "types.h"
#include "service.h"
#include <LPC21xx.h>

/*----------------------------------------------------------------------------
 * UART Configuration
 * PCLK     : Peripheral Clock Frequency
 * Baudrate : 9600 bps
 * DIV      : UART Baud Rate Divisor
 *--------------------------------------------------------------------------*/
#define PCLK    15000000
#define DIV     (PCLK / (16 * 9600))

/* UART receive buffer index */
u32 uart_index = 0;

/* UART receive buffer */
s8 uart_data[10];

/* Function Prototype */
void UART0_Tx(u8 data);

/******************************************************************************
 * Function Name : UART0_ISR
 * Description   : UART0 Receive Interrupt Service Routine.
 *                 Stores every received byte into the receive buffer.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
__irq void UART0_ISR(void)
{
    /* Check whether data is available in the receive buffer */
    if(U0LSR & (1 << 0))
    {
        /* Read received byte */
        uart_data[uart_index] = U0RBR;

        /* Increment buffer index */
        uart_index++;
    }

    /* Acknowledge interrupt */
    VICVectAddr = 0;
}

/******************************************************************************
 * Function Name : UART0_Init
 * Description   : Initializes UART0 for 9600 baud, 8-bit data,
 *                 1 stop bit and no parity. Also configures the
 *                 UART interrupt in the VIC.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void UART0_Init(void)
{
    /* Configure P0.0 as TXD0 and P0.1 as RXD0 */
    PINSEL0 &= ~((3 << (2 * 0)) | (3 << (2 * 1)));
    PINSEL0 |=  (1 << (2 * 0)) | (1 << (2 * 1));

    /* Enable access to DLL and DLM registers */
    U0LCR |= (3 << 0) | (1 << 7);

    /* Configure baud rate divisor */
    U0DLL = DIV;
    U0DLM = DIV >> 8;

    /* Configure UART0 interrupt in VIC */
    VICIntEnable |= (1 << 6);
    VICVectCntl0 |= (1 << 5) | 6;
    VICVectAddr0 = (unsigned long)UART0_ISR;

    /* Disable Divisor Latch Access Bit */
    U0LCR &= ~(1 << 7);

    /* Enable FIFO */
    U0FCR |= (1 << 0);

    /* Enable Receive Data Available Interrupt (Enable when required) */
    //U0IER |= (1 << 0);
}

/******************************************************************************
 * Function Name : UART0_Tx
 * Description   : Transmits one byte through UART0.
 *
 * Parameters    :
 *      data - Byte to be transmitted.
 *
 * Returns       : None
 ******************************************************************************/
void UART0_Tx(u8 data)
{
    /* Wait until Transmit Holding Register becomes empty */
    while((U0LSR & (1 << 5)) == 0);

    /* Load data into THR */
    U0THR = data;
}

/******************************************************************************
 * Function Name : UART0_Rx
 * Description   : Receives one byte through UART0 using polling.
 *
 * Parameters    : None
 *
 * Returns       :
 *      Received byte.
 ******************************************************************************/
u8 UART0_Rx(void)
{
    /* Wait until data is received */
    while((U0LSR & (1 << 0)) == 0);

    /* Return received byte */
    return U0RBR;
}

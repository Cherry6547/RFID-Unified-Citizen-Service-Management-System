/******************************************************************************
 * File Name    : delays.c
 * Description  : Software delay routines for LPC2148. These functions generate
 *                approximate delays using instruction execution time. The delay
 *                values are calculated assuming a processor clock of 60 MHz.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "types.h"

/******************************************************************************
 * Function Name : delay_us
 * Description   : Generates an approximate delay in microseconds using a
 *                 software busy-wait loop.
 *
 * Parameters    :
 *      delayUS - Delay duration in microseconds.
 *
 * Returns       : None
 ******************************************************************************/
void delay_us(u32 delayUS)
{
    /* Convert microseconds into loop count */
    for(delayUS *= 12; delayUS; delayUS--);
}

/******************************************************************************
 * Function Name : delay_ms
 * Description   : Generates an approximate delay in milliseconds using a
 *                 software busy-wait loop.
 *
 * Parameters    :
 *      delayMS - Delay duration in milliseconds.
 *
 * Returns       : None
 ******************************************************************************/
void delay_ms(u32 delayMS)
{
    /* Convert milliseconds into loop count */
    for(delayMS *= 12000; delayMS; delayMS--);
}

/******************************************************************************
 * Function Name : delay_s
 * Description   : Generates an approximate delay in seconds using a
 *                 software busy-wait loop.
 *
 * Parameters    :
 *      delayS - Delay duration in seconds.
 *
 * Returns       : None
 ******************************************************************************/
void delay_s(u32 delayS)
{
    /* Convert seconds into loop count */
    for(delayS *= 12000000; delayS; delayS--);
}

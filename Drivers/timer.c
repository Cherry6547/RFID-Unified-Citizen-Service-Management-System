/******************************************************************************
 * File Name    : timer.c
 * Description  : Timer0 driver for LPC2148. This driver provides delay
 *                functions in microseconds, milliseconds, and seconds using
 *                Timer0. It also supports a non-blocking timer for background
 *                timing operations.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "types.h"
#include <LPC214x.h>

/* Timer Match Control Register Bit Definitions */
#define MRI     (1 << 0)    /* Interrupt on Match */
#define MRR     (1 << 1)    /* Reset Timer on Match */
#define MRS     (1 << 2)    /* Stop Timer on Match */

/* Timer Control Register Bit Definitions */
#define RESET   (1 << 1)    /* Reset Timer Counter */
#define ENABLE  (1 << 0)    /* Enable Timer */

/* Prescaler values for different time resolutions (PCLK = 15 MHz) */
#define PR_US   (15 - 1)            /* 1 µs tick */
#define PR_MS   (15000 - 1)         /* 1 ms tick */
#define PR_S    (15000000 - 1)      /* 1 second tick */

/******************************************************************************
 * Function Name : Timer0_Init
 * Description   : Initializes Timer0 by resetting the timer counter.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Timer0_Init(void)
{
    /* Stop Timer */
    T0TCR = 0;

    /* Reset Timer Counter and Prescaler */
    T0TCR = RESET;
}

/******************************************************************************
 * Function Name : tdelay_sBG
 * Description   : Starts a non-blocking timer for the specified number of
 *                 seconds. The timer count can be monitored manually.
 *
 * Parameters    :
 *      Mchn  - Match Register Channel (MR0-MR3)
 *      delay - Delay in seconds
 *
 * Returns       : None
 ******************************************************************************/
void tdelay_sBG(u8 Mchn, u32 delay)
{
    /* Configure timer for 1-second resolution */
    T0PR = PR_S;

    /* Stop timer on match */
    T0MCR |= (MRS << (Mchn * 3));

    /* Load match value */
    T0MR0 = delay;

    /* Start timer */
    T0TCR = ENABLE;
}

/******************************************************************************
 * Function Name : tdelay_us
 * Description   : Generates a blocking delay in microseconds.
 *
 * Parameters    :
 *      Mchn  - Match Register Channel
 *      delay - Delay in microseconds
 *
 * Returns       : None
 ******************************************************************************/
void tdelay_us(u8 Mchn, u32 delay)
{
    /* Configure timer for 1 µs resolution */
    T0PR = PR_US;

    /* Stop timer on match */
    T0MCR |= (MRS << (Mchn * 3));

    /* Load match value */
    T0MR0 = delay;

    /* Start timer */
    T0TCR = ENABLE;

    /* Wait until timer reaches the match value */
    while(T0TC != delay);
}

/******************************************************************************
 * Function Name : tdelay_ms
 * Description   : Generates a blocking delay in milliseconds.
 *
 * Parameters    :
 *      Mchn  - Match Register Channel
 *      delay - Delay in milliseconds
 *
 * Returns       : None
 ******************************************************************************/
void tdelay_ms(u8 Mchn, u32 delay)
{
    /* Configure timer for 1 ms resolution */
    T0PR = PR_MS;

    /* Stop timer on match */
    T0MCR |= (MRS << (Mchn * 3));

    /* Load match value */
    T0MR0 = delay;

    /* Start timer */
    T0TCR = ENABLE;

    /* Wait until timer reaches the match value */
    while(T0TC != delay);
}

/******************************************************************************
 * Function Name : tdelay_s
 * Description   : Generates a blocking delay in seconds.
 *
 * Parameters    :
 *      Mchn  - Match Register Channel
 *      delay - Delay in seconds
 *
 * Returns       : None
 ******************************************************************************/
void tdelay_s(u8 Mchn, u32 delay)
{
    /* Configure timer for 1 second resolution */
    T0PR = PR_S;

    /* Stop timer on match */
    T0MCR |= (MRS << (Mchn * 3));

    /* Load match value */
    T0MR0 = delay;

    /* Start timer */
    T0TCR = ENABLE;

    /* Wait until timer reaches the match value */
    while(T0TC != delay);
}

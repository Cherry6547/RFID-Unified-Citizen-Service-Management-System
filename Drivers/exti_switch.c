/******************************************************************************
 * File Name    : exti_switch.c
 * Description  : External Interrupt (EINT3) driver for LPC2148. This driver
 *                configures an external switch connected to EINT3 and triggers
 *                an interrupt on the falling edge. The interrupt is used to
 *                request RTC reconfiguration.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"
#include <LPC214x.h>

/* External Interrupt Configuration */
#define EXTI_SW    30      /* GPIO Pin P0.30 configured as EINT3 */
#define EINT3      3       /* External Interrupt Channel */

/******************************************************************************
 * Function Name : EXTI_ISR
 * Description   : Interrupt Service Routine for External Interrupt 3.
 *                 Sets the RTC configuration flag whenever the external switch
 *                 is pressed.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
__irq void EXTI_ISR(void)
{
    /* Check whether EINT3 generated the interrupt */
    if(EXTINT & (1 << EINT3))
    {
        /* Set RTC configuration request flag */
        RTC_Flag = 1;
    }

    /* Clear External Interrupt Flag */
    EXTINT |= (1 << EINT3);

    /* Acknowledge interrupt to VIC */
    VICVectAddr = 0;
}

/******************************************************************************
 * Function Name : Exti_switch_Init
 * Description   : Configures P0.30 as External Interrupt 3 (EINT3), enables
 *                 falling-edge triggering, and registers the ISR with the
 *                 Vector Interrupt Controller (VIC).
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Exti_switch_Init(void)
{
    /* Configure P0.30 as EINT3 function */
    PINSEL1 &= ~(3 << (2 * (EXTI_SW - 16)));
    PINSEL1 |=  (2 << (2 * (EXTI_SW - 16)));

    /* Configure EINT3 as Edge Triggered */
    EXTMODE |= (1 << EINT3);

    /* Configure Falling Edge Trigger */
    EXTPOLAR &= ~(1 << EINT3);

    /* Register ISR in VIC Slot 1 */
    VICVectAddr1 = (unsigned)EXTI_ISR;
    VICVectCntl1 = (1 << 5) | 17;

    /* Clear any pending interrupt */
    EXTINT = (1 << EINT3);

    /* Enable EINT3 interrupt in VIC */
    VICIntEnable |= (1 << 17);
}

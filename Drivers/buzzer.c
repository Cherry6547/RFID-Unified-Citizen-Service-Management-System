/******************************************************************************
 * File Name    : buzzer.c
 * Description  : Buzzer driver for LPC2148. This driver provides APIs to
 *                initialize the buzzer and control its ON/OFF operation.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "buzzer.h"
#include <LPC214x.h>

/******************************************************************************
 * Function Name : Buzzer_Init
 * Description   : Configures the buzzer pin as GPIO output and turns OFF
 *                 the buzzer during initialization.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Buzzer_Init(void)
{
    /* Configure buzzer pin as output */
    IODIR0 |= (1 << BUZZER);

    /* Ensure buzzer is OFF */
    IOCLR0 = (1 << BUZZER);
}

/******************************************************************************
 * Function Name : Buzzer_On
 * Description   : Turns ON the buzzer.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Buzzer_On(void)
{
    /* Drive buzzer pin HIGH */
    IOSET0 = (1 << BUZZER);
}

/******************************************************************************
 * Function Name : Buzzer_Off
 * Description   : Turns OFF the buzzer.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Buzzer_Off(void)
{
    /* Drive buzzer pin LOW */
    IOCLR0 = (1 << BUZZER);
}

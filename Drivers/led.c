/******************************************************************************
 * File Name    : led.c
 * Description  : LED driver for LPC2148. This driver provides APIs to
 *                initialize, turn ON, and turn OFF the LEDs connected to
 *                the development board.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "led.h"
#include <LPC214x.h>

/******************************************************************************
 * Function Name : Led_Init
 * Description   : Configures the LED pins as GPIO outputs and turns OFF
 *                 both LEDs during initialization.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Led_Init(void)
{
    /* Configure LED pins as output */
    IODIR0 |= (1 << LED_GREEN) | (1 << LED_RED);

    /* Turn OFF both LEDs */
    IOCLR0 = (1 << LED_GREEN) | (1 << LED_RED);
}

/******************************************************************************
 * Function Name : Led_On
 * Description   : Turns ON the specified LED.
 *
 * Parameters    :
 *      pinNo - GPIO pin number connected to the LED.
 *
 * Returns       : None
 ******************************************************************************/
void Led_On(u8 pinNo)
{
    IOSET0 = (1 << pinNo);
}

/******************************************************************************
 * Function Name : Led_Off
 * Description   : Turns OFF the specified LED.
 *
 * Parameters    :
 *      pinNo - GPIO pin number connected to the LED.
 *
 * Returns       : None
 ******************************************************************************/
void Led_Off(u8 pinNo)
{
    IOCLR0 = (1 << pinNo);
}

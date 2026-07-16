/******************************************************************************
 * File Name    : invalid.c
 * Description  : Handles the INVALID state of the application. Displays an
 *                authentication failure message, activates visual and audible
 *                alerts, and returns the system to the RFID waiting state.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/******************************************************************************
 * Function Name : Invalid_Card
 * Description   : Indicates an invalid RFID card by displaying an error
 *                 message, turning ON the red LED and buzzer, then returns
 *                 the application to the WAIT state for the next RFID scan.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Invalid_Card(void)
{
    /* Display authentication failure message */
    CmdLCD(CLEAR_LCD);
    StrLCD("    Invalid Card    ");

    /* Indicate invalid card using LED and buzzer */
    Led_On(LED_RED);
    Buzzer_On();

    /* Keep the indication active for 5 seconds */
    delay_ms(5000);

    /* Turn OFF LED and buzzer */
    Led_Off(LED_RED);
    Buzzer_Off();

    /* Return to RFID waiting state */
    state = WAIT;

    /* Clear UART receive buffer index for next card */
    uart_index = 0;
}

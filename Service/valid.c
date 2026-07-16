/******************************************************************************
 * File Name    : valid.c
 * Description  : Handles the VALID state of the application. Displays a
 *                successful authentication message, greets the user, and
 *                redirects the application to either the Admin or User menu
 *                based on the user type.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/******************************************************************************
 * Function Name : Valid_Card
 * Description   : Displays a successful authentication message, greets the
 *                 authenticated user, indicates success using the green LED,
 *                 and transfers control to the appropriate menu.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Valid_Card(void)
{
    /* Display authentication success message */
    CmdLCD(CLEAR_LCD);
    StrLCD("Valid Card");

    /* Display welcome message with user's name */
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Hai ");
    StrLCD(rfid[curr_card_index].Name);

    /* Indicate successful authentication */
    Led_On(LED_GREEN);

    /* Keep message visible for 2 seconds */
    delay_ms(2000);

    /* Turn OFF status LED */
    Led_Off(LED_GREEN);

    /* Redirect user based on access privilege */
    if(rfid[curr_card_index].user_flag == 1)
    {
        /* Admin User */
        state = OFF;
    }
    else
    {
        /* Normal User */
        state = DASHBOARD;
    }
}

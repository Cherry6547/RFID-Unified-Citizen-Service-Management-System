/******************************************************************************
 * File Name    : dashboard.c
 * Description  : Implements the user dashboard. Displays all available
 *                citizen services and transfers control to the selected
 *                module based on keypad input.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/******************************************************************************
 * Function Name : Menu
 * Description   : Displays the dashboard menu, waits for user input from the
 *                 keypad, and updates the application state according to the
 *                 selected service. If no key is pressed within the timeout,
 *                 the system returns to the WAIT state.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Menu(void)
{
    u8 temp;
    u8 flag = 0;

L1:

    /* Display dashboard menu */
    CmdLCD(CLEAR_LCD);

    StrLCD("Choose the Service");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("1.PAN");
    StrLCD("   ");
    StrLCD("4.D.L");

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("2.ATM   5.RESET P.W");

    CmdLCD(GOTO_LINE4_POS0);
    StrLCD("3.VOTE  6.EXIT");

    /* Wait for user input with timeout */
    temp = KeyScan_t(&flag);

    /* Display the selected key */
    CharLCD(temp);

    delay_ms(1000);

    /* Return to WAIT state if timeout occurs */
    if(flag)
    {
        state = WAIT;
        uart_index = 0;
        return;
    }

    /* Process user selection */
    if(temp == '1')
    {
        state = PAN;
    }
    else if(temp == '2')
    {
        state = ATM;
    }
    else if(temp == '3')
    {
        state = VOTE;
    }
    else if(temp == '4')
    {
        state = DL;
    }
    else if(temp == '5')
    {
        state = RESET;
    }
    else if(temp == '6')
    {
        state = EXIT;
    }
    else
    {
        /* Invalid menu option */
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Input");

        delay_ms(2000);

        /* Redisplay dashboard */
        goto L1;
    }
}

/******************************************************************************
 * File Name    : pan.c
 * Description  : Implements the PAN card service. After successful user
 *                authentication, the PAN card information of the currently
 *                authenticated user is displayed on the LCD.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/******************************************************************************
 * Function Name : Show_Pan_Info
 * Description   : Displays the PAN card information of the authenticated user.
 *                 The information remains on the LCD until any key is pressed
 *                 or the 30-second timeout expires.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Show_Pan_Info(void)
{
    u8 flag = 0;

    /* Display user's Name */
    CmdLCD(CLEAR_LCD);
    StrLCD("Name:");
    StrLCD(rfid[curr_card_index].Name);

    /* Display Date of Birth */
    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("DOB:");
    StrLCD(rfid[curr_card_index].DOB);

    /* Display Father's Name */
    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("FName:");
    StrLCD(rfid[curr_card_index].Father_Name);

    /* Display PAN Number */
    CmdLCD(GOTO_LINE4_POS0);
    StrLCD("PAN NO:");
    StrLCD(rfid[curr_card_index].pan.id);

    /* Wait for any key press or timeout */
    KeyScan_t(&flag);

    /* Return to Dashboard */
    state = DASHBOARD;
}

/******************************************************************************
 * Function Name : Get_Pan
 * Description   : Authenticates the user before displaying PAN card
 *                 information. Depending on the authentication result, the
 *                 application either displays PAN details, returns to the
 *                 dashboard, or exits to the RFID waiting state.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Get_Pan(void)
{
    s16 temp;

    /* Authenticate user */
    temp = Authentication();

    if(temp == -1)
    {
        /* Authentication timeout */
        state = DASHBOARD;
    }
    else if(temp == 1)
    {
        /* Authentication successful */
        Show_Pan_Info();
    }
    else if(temp == 0)
    {
        /* Authentication failed */
        state = WAIT;
    }
}

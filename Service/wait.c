/******************************************************************************
 * File Name    : wait.c
 * Description  : Implements the WAIT state of the application. This module
 *                waits for an RFID card, validates the received RFID ID, and
 *                changes the application state based on authentication result.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"
#include <LPC214x.h>

/*----------------------------------------------------------------------------
 * Global Variables
 *--------------------------------------------------------------------------*/

/* Temporary buffer used to store the scanned RFID card ID */
s8 tempid[9];

/* Stores the index of the currently authenticated RFID user */
s16 curr_card_index = -1;

/******************************************************************************
 * Function Name : Validate
 * Description   : Compares the scanned RFID ID with all registered RFID
 *                 cards. If a match is found, the corresponding user index
 *                 is stored.
 *
 * Parameters    :
 *      id - Pointer to scanned RFID card ID.
 *
 * Returns       :
 *      1 -> Valid RFID Card
 *      0 -> Invalid RFID Card
 ******************************************************************************/
u8 Validate(s8 *id)
{
    u8 i;

    /* Compare scanned ID with every registered RFID card */
    for(i = 0; i < TOTAL_CARDS; i++)
    {
        if(strcmp(rfid[i].id, id) == 0)
        {
            /* Store authenticated user index */
            curr_card_index = i;

            return 1;
        }
    }

    return 0;
}

/******************************************************************************
 * Function Name : Read_RFID
 * Description   : Waits for a complete RFID frame from UART. After receiving
 *                 the complete card ID, validates the card and changes the
 *                 application state accordingly.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Read_RFID(void)
{
    /* Check whether complete RFID data has been received */
    if(uart_index == 10)
    {
        /* Disable UART receive interrupt */
        U0IER &= ~(1 << 0);

        /*
         * Remove Start Byte.
         * Received Frame:
         * --------------------------------------------------
         * | Start | 8-byte RFID ID | Stop |
         * --------------------------------------------------
         *
         * After memmove():
         * --------------------------------------------------
         * | 8-byte RFID ID |
         * --------------------------------------------------
         */
        memmove(uart_data, uart_data + 1, 8);

        /* Append null terminator */
        uart_data[8] = '\0';

        /* Copy RFID ID into temporary buffer */
        strcpy(tempid, uart_data);

        /* Validate scanned RFID card */
        if(Validate(tempid))
        {
            state = VALID;
        }
        else
        {
            state = INVALID;
        }
    }
    else
    {
        /* Display waiting message */
        CmdLCD(CLEAR_LCD);
        StrLCD("Waiting for Card");

        /* Enable UART Receive Interrupt */
        U0IER |= (1 << 0);

        delay_ms(1000);
    }
}

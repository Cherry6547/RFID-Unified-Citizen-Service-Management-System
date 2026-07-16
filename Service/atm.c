/******************************************************************************
 * File Name    : atm.c
 * Description  : Implements the ATM service of the RFID Unified Citizen
 *                Service Management System. This module provides balance
 *                enquiry, cash withdrawal, cash deposit, and ATM menu
 *                management after successful user authentication.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/* Maximum account balance supported (u16 limit) */
#define MAX_ATM_LIMIT    65535

/*----------------------------------------------------------------------------
 * ATM Service State Machine
 *--------------------------------------------------------------------------*/
enum ATM_State
{
    AUTH,           /* Authenticate user */
    MENU,           /* Display ATM menu */
    BE,             /* Balance Enquiry */
    WITHDRAW,       /* Withdraw money */
    DEPOSIT         /* Deposit money */
};

/* Stores the current ATM state */
u8 atm_state = AUTH;

/******************************************************************************
 * Function Name : balance_enquiry
 * Description   : Displays the current account balance of the authenticated
 *                 user and returns to the ATM menu.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void balance_enquiry(void)
{
    u16 bal = rfid[curr_card_index].atm.balance;

    /* Display account balance */
    CmdLCD(CLEAR_LCD);
    StrLCD("ACC Balance : ");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Rs.");
    U32LCD(bal);

    /* Display balance for 5 seconds */
    delay_ms(5000);

    /* Return to ATM menu */
    atm_state = MENU;
}

/******************************************************************************
 * Function Name : withdraw
 * Description   : Allows the authenticated user to withdraw money from the
 *                 account after validating the entered amount. The updated
 *                 balance is stored in the SPI EEPROM.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void withdraw(void)
{
    u32 temp = 0;
    u16 minimum_balance = 500;
    u8 flag = 0;

    /* Prompt user to enter withdrawal amount */
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter Amount : ");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("(x 100 only)");

    temp = Read_Num(temp, &flag, GOTO_LINE3_POS0);

    /* Return on timeout */
    if(flag)
    {
        return;
    }

    /* Validate maximum input value */
    if(temp > MAX_ATM_LIMIT)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Withdraw Failed");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Amount Exceeded");

        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("MAX LIMIT");

        delay_ms(3000);

        atm_state = MENU;
        return;
    }

    /* Validate denomination */
    if(temp == 0 || (temp % 100) != 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Amount");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Rs.100 Multiples");

        delay_ms(3000);

        atm_state = MENU;
        return;
    }

    /* Check minimum balance constraint */
    if(temp <= (rfid[curr_card_index].atm.balance - minimum_balance))
    {
        /* Update account balance */
        rfid[curr_card_index].atm.balance -= temp;

        CmdLCD(CLEAR_LCD);
        StrLCD("Money Withdrawn");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Successfully");

        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("Balance : Rs.");

        U32LCD(rfid[curr_card_index].atm.balance);

        delay_ms(5000);

        /* Store updated balance in EEPROM */
        Update_Balance_Eeprom(curr_card_index);
    }
    else
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Insufficient");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Balance");

        delay_ms(5000);
    }

    /* Return to ATM menu */
    atm_state = MENU;
}

/******************************************************************************
 * Function Name : deposit
 * Description   : Allows the authenticated user to deposit money into the
 *                 account after validating the entered amount. The updated
 *                 balance is stored in the SPI EEPROM.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void deposit(void)
{
    u32 temp = 0;
    u8 flag = 0;

    /* Prompt user to enter deposit amount */
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter amount to be");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("deposit:");
    StrLCD("(x 100 only)");

    temp = Read_Num(temp, &flag, GOTO_LINE3_POS0);

    /* Return on timeout */
    if(flag)
    {
        return;
    }

    /* Validate maximum input value */
    if(temp > MAX_ATM_LIMIT)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Deposit Failed");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Amount Exceeded");

        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("MAX LIMIT");

        delay_ms(3000);

        atm_state = MENU;
        return;
    }

    /* Validate denomination */
    if(temp == 0 || (temp % 100) != 0)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Amount");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Rs.100 Multiples");

        delay_ms(3000);

        atm_state = MENU;
        return;
    }

    /* Check maximum account balance */
    if((temp + rfid[curr_card_index].atm.balance) >= MAX_ATM_LIMIT)
    {
        CmdLCD(CLEAR_LCD);
        StrLCD("Deposit Failed");

        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("Maximum Acc Balance");

        CmdLCD(GOTO_LINE3_POS0);
        StrLCD("Should be less than");

        CmdLCD(GOTO_LINE4_POS0);
        StrLCD("65536");

        delay_ms(5000);

        atm_state = MENU;
        return;
    }

    /* Update account balance */
    rfid[curr_card_index].atm.balance += temp;

    CmdLCD(CLEAR_LCD);
    StrLCD("Deposited");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Successfully");

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("Balance : Rs.");

    U32LCD(rfid[curr_card_index].atm.balance);

    delay_ms(3000);

    /* Store updated balance in EEPROM */
    Update_Balance_Eeprom(curr_card_index);

    /* Return to ATM menu */
    atm_state = MENU;
}

/******************************************************************************
 * Function Name : show_atm_menu
 * Description   : Displays the ATM service menu and allows the user to select
 *                 the required ATM operation. If no key is pressed within the
 *                 timeout period, control returns to the main dashboard.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void show_atm_menu(void)
{
    u8 flag = 0;
    u8 temp;

L2:

    /* Display ATM menu */
    CmdLCD(CLEAR_LCD);
    StrLCD("1.Bal Enquiry");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("2.Withdraw");

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("3.Deposit");

    CmdLCD(GOTO_LINE4_POS0);
    StrLCD("4.Exit");

    /* Wait for user selection */
    temp = KeyScan_t(&flag);

    /* Return to dashboard on timeout */
    if(flag)
    {
        state = DASHBOARD;
        return;
    }

    /* Process user selection */
    if(temp == '1')
    {
        atm_state = BE;
    }
    else if(temp == '2')
    {
        atm_state = WITHDRAW;
    }
    else if(temp == '3')
    {
        atm_state = DEPOSIT;
    }
    else if(temp == '4')
    {
        state = DASHBOARD;
        atm_state = AUTH;
    }
    else
    {
        /* Invalid menu option */
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Input");

        delay_ms(2000);

        /* Redisplay ATM menu */
        goto L2;
    }
}

/******************************************************************************
 * Function Name : Get_Atm
 * Description   : Controls the ATM service state machine. Depending on the
 *                 current ATM state, it performs authentication, displays the
 *                 ATM menu, balance enquiry, withdrawal, or deposit service.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Get_Atm(void)
{
    s16 temp;

    switch(atm_state)
    {
        case AUTH:

            /* Authenticate the user before ATM access */
            temp = Authentication();

            if(temp != 1)
            {
                state = DASHBOARD;
            }
            else
            {
                atm_state = MENU;
            }
            break;

        case MENU:

            /* Display ATM menu */
            show_atm_menu();
            break;

        case BE:

            /* Balance Enquiry */
            balance_enquiry();
            break;

        case DEPOSIT:

            /* Deposit Money */
            deposit();
            break;

        case WITHDRAW:

            /* Withdraw Money */
            withdraw();
            break;
    }
}		

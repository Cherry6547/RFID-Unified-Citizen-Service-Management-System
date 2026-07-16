/******************************************************************************
 * File Name    : main.c
 * Description  : Main application file for the RFID-based Citizen Service
 *                and Voting System.
 *
 * Author       : Charan Sai Mathkala
 * Platform     : LPC2148 (ARM7TDMI-S)
 * Compiler     : Keil uVision
 ******************************************************************************/
 
#include"service.h"

/* Stores the current state of the application state machine */
u32 state = INIT;


/******************************************************************************
 * Function Name : main
 * Description   : Initializes all peripherals and continuously executes
 *                 the application state machine.
 *
 * Returns       : Never returns.
 ******************************************************************************/

int main(void)
{
    /* Initialize all hardware peripherals */
    System_Init();

    while (1)
    {
        /* Reconfigure RTC if date/time has been updated */
        if (RTC_Flag == 1)
        {
            Configure_Rtc();
        }

        /* Execute application based on current state */
        switch (state)
        {
            case INIT:
                Display_Project();
                state = WAIT;
                break;

            case WAIT:
                Read_RFID();
                break;

            case VALID:
                Valid_Card();
                break;

            case INVALID:
                Invalid_Card();
                break;

            case DASHBOARD:
                Menu();
                break;

            case PAN:
                Get_Pan();
                break;

            case ATM:
                Get_Atm();
                break;

            case VOTE:
                Get_Vote_Info();
                break;

            case DL:
                Get_Dl_Info();
                break;

            case OFF:
                Edit_Vote_Machine();
                break;

            case RESET:
                Set_Password();
                break;

            case EXIT:
                /* Reset UART buffer and wait for next RFID card */
                state = WAIT;
                uart_index = 0;
                break;
        }
    }
}

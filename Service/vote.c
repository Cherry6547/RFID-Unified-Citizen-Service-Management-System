/******************************************************************************
 * File Name    : vote.c
 * Description  : Implements the Voting service of the RFID Unified Citizen
 *                Service Management System. This module authenticates the
 *                voter, allows eligible users to cast a vote, prevents
 *                multiple voting, and provides administrative functions to
 *                reset the voting machine.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"

/*----------------------------------------------------------------------------
 * Voting Service State Machine
 *--------------------------------------------------------------------------*/
enum VOTE_State
{
    VERIFY,        /* Authenticate voter */
    SHOW,          /* Display vote already cast message */
    CAST           /* Cast vote */
};

/* Current voting service state */
u16 vote_state = VERIFY;

/* Voting Machine Status Flag */
u8 VM_Flag = 0;

/******************************************************************************
 * Function Name : Show_Vote_Info
 * Description   : Displays a message indicating that the authenticated user
 *                 has already cast the vote.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Show_Vote_Info(void)
{
    /* Display voting status */
    CmdLCD(CLEAR_LCD);
    StrLCD("Your Vote is");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Already Casted");

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("Thank You");

    delay_ms(3000);

    /* Return to dashboard */
    state = DASHBOARD;
}

/******************************************************************************
 * Function Name : Cast_Vote
 * Description   : Displays the list of contesting parties, accepts the user's
 *                 selection, records the vote, and prevents duplicate voting.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Cast_Vote(void)
{
    u8 temp = 0;
    u8 flag = 0;

    /* Display available parties */
    CmdLCD(CLEAR_LCD);

    StrLCD("1.Party 1 - ");
    CharLCD(0);

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("2.Party 2 - ");
    CharLCD(1);

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("3.Party 3 - ");
    CharLCD(2);

    CmdLCD(GOTO_LINE4_POS0);
    StrLCD("4.Party 4 - ");
    CharLCD(3);

    /* Wait for user selection */
    temp = KeyScan_t(&flag);

    /* Return to dashboard on timeout */
    if(flag)
    {
        state = DASHBOARD;
        return;
    }

    /* Record invalid selections as NOTA */
    if(!(temp >= '1' && temp <= '4'))
    {
        rfid[curr_card_index].voterid.index = 5;

        delay_ms(2000);
    }else{

				/* Store selected party */
				rfid[curr_card_index].voterid.index = temp;
		}
    rfid[curr_card_index].voterid.voted = 1;

    /* Display success message */
    CmdLCD(CLEAR_LCD);
    StrLCD("Vote Successfully");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Cast");

    delay_ms(3000);

    /* Return to dashboard */
    state = DASHBOARD;
}

/******************************************************************************
 * Function Name : Reset_Votes
 * Description   : Resets the voting status of all registered users, allowing
 *                 them to participate in the next election. A progress bar is
 *                 displayed during the reset operation.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Reset_Votes(void)
{
    u8 i;

    /* Clear voting status of all registered users */
    for(i = 0; i < TOTAL_CARDS; i++)
    {
        rfid[i].voterid.voted = 0;
    }

    /* Display reset progress */
    CmdLCD(CLEAR_LCD);
    StrLCD("Resetting VM");

    CmdLCD(GOTO_LINE2_POS0);
    CharLCD('[');

    CmdLCD(GOTO_LINE2_POS0 + 19);
    CharLCD(']');

    CmdLCD(GOTO_LINE2_POS0 + 1);

    for(i = 0; i < 18; i++)
    {
        CharLCD('#');
        delay_ms(100);
    }

    /* Display completion message */
    CmdLCD(CLEAR_LCD);
    StrLCD("Voting Machine");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Reset Successfull");

    delay_ms(3000);
}

/******************************************************************************
 * Function Name : Edit_Vote_Machine
 * Description   : Provides administrative access to the voting machine.
 *                 After successful officer authentication, the administrator
 *                 can reset the voting machine or exit the application.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Edit_Vote_Machine(void)
{
    u8 temp = 0;

L3:

    /* Display administrator access message */
    CmdLCD(CLEAR_LCD);
    StrLCD("OFFICER Privilege");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("Detected");

    delay_ms(3000);

    /* Authenticate officer */
    CmdLCD(CLEAR_LCD);
    temp = Authentication();

    if(temp != 1)
    {
        uart_index = 0;
        state = WAIT;
        return;
    }

    /* Display administrator menu */
    StrLCD("Would you like to:");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("1.Reset VM");

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD("2.Exit");

    CmdLCD(GOTO_LINE4_POS0);

    temp = KeyScan();

    if(temp == '1')
    {
        /* Reset voting machine */
        Reset_Votes();

        uart_index = 0;
        state = WAIT;
        vote_state = VERIFY;
    }
    else if(temp == '2')
    {
        /* Exit administrator mode */
        uart_index = 0;
        state = WAIT;
        vote_state = VERIFY;
    }
    else
    {
        /* Invalid menu option */
        CmdLCD(CLEAR_LCD);
        StrLCD("Invalid Choice");

        delay_ms(3000);

        goto L3;
    }
}

/******************************************************************************
 * Function Name : Get_Vote_Info
 * Description   : Controls the voting service state machine. Based on the
 *                 current state, it authenticates the voter, checks whether
 *                 the vote has already been cast, displays the voting status,
 *                 or allows the user to cast a vote.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Get_Vote_Info(void)
{
    s16 temp = 0;

    switch(vote_state)
    {
        case VERIFY:

            /* Authenticate the voter */
            temp = Authentication();

            if(temp == 1)
            {
                /* Check whether the user has already voted */
                if(rfid[curr_card_index].voterid.voted == 1)
                {
                    vote_state = SHOW;
                }
                else
                {
                    vote_state = CAST;
                }
            }
            else
            {
                /* Authentication failed or timed out */
                state = DASHBOARD;
            }
            break;

        case SHOW:

            /* Display vote already cast information */
            Show_Vote_Info();

            /* Prepare for next voting session */
            vote_state = VERIFY;
            break;

        case CAST:

            /* Allow user to cast vote */
            Cast_Vote();

            /* Prepare for next voting session */
            vote_state = VERIFY;
            break;
    }
}

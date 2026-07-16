#include"service.h"

s32 Auth()
{
    u32 temp = 0, pin = 0, i = 0;
	  u8 flag=0;
    CmdLCD(CLEAR_LCD);
    StrLCD("Enter the PIN :");
    CmdLCD(GOTO_LINE2_POS0);
    while(i < 4)
    {
        /*------------------------------------------------------------------
         * Read key pressed from keypad
         *------------------------------------------------------------------*/
        temp = KeyScan_t(&flag);
				if(flag){
					state = DASHBOARD;
					return -1;
				}

        /*------------------------------------------------------------------
         * Validate input
         *
         * Accept only:
         * - Numeric digits ('0' to '9')
         * - Delete key ('d')
         *------------------------------------------------------------------*/
        if(!(((temp >= '0') && (temp <= '9')) || (temp == 'd')))
        {
            return 0;      // Invalid input detected
        }


        /*------------------------------------------------------------------
         * Handle delete/backspace operation
         *------------------------------------------------------------------*/
        if(temp == 'd')
        {
            /*
             * Prevent deletion below zero digits
             */
            if(i != 0)
            {
                i--;
            }

            /*
             * Move cursor to previous position
             */
            CmdLCD(GOTO_LINE2_POS0 + i);


            /*
             * Clear displayed character
             */
            CharLCD(' ');


            /*
             * Restore cursor position
             */
            CmdLCD(GOTO_LINE2_POS0 + i);


            /*
             * Remove last digit from entered PIN
             */
            pin /= 10;
        }
        else
        {
            /*--------------------------------------------------------------
             * Display entered digit temporarily
             *--------------------------------------------------------------*/
            CharLCD(temp);


            /*--------------------------------------------------------------
             * Small delay for visual feedback
             *--------------------------------------------------------------*/
            delay_ms(300);


            /*--------------------------------------------------------------
             * Move cursor back to entered position
             *--------------------------------------------------------------*/
            CmdLCD(GOTO_LINE2_POS0 + i);


            /*--------------------------------------------------------------
             * Mask entered digit with '*'
             *--------------------------------------------------------------*/
            CharLCD('*');


            /*--------------------------------------------------------------
             * Store digit into PIN variable
             *--------------------------------------------------------------*/
            pin = pin * 10 + (temp - 48);


            /*--------------------------------------------------------------
             * Move to next digit position
             *--------------------------------------------------------------*/
            i++;
        }
    }


    /*----------------------------------------------------------------------
     * Clear LCD after PIN entry completion
     *----------------------------------------------------------------------*/
    CmdLCD(CLEAR_LCD);


    /*----------------------------------------------------------------------
     * Return entered PIN
     *----------------------------------------------------------------------*/
    return pin;
}



/*----------------------------------------------------------------------------
 *                      User Authentication Handler
 *---------------------------------------------------------------------------*/

/******************************************************************************
 * @brief   Validates user authentication against stored PIN.
 *
 * This function:
 * - Calls Auth() to read user PIN
 * - Compares entered PIN with stored PIN
 * - Tracks failed attempts
 * - Locks device after 3 invalid attempts
 *
 *****************************************************************************/

s16 Authentication()
{
    /*
     * Static variable retains failed attempt count
     * between function calls
     */
    static u32 attempts = 0;
		s32 temp = 0;

    /*
     * Retry label for repeated authentication attempts
     */
L1:	temp = Auth();

    /*----------------------------------------------------------------------
     * Verify entered PIN
     *----------------------------------------------------------------------*/
    if(temp == rfid[curr_card_index].password)
    {
        /*
         * Reset attempt counter on successful authentication
         */
        attempts = 0;
			return 1;
    }
		else if(temp == -1)
		{
			return -1;
		}
    else
    {
        /*------------------------------------------------------------------
         * Increment invalid attempt counter
         *------------------------------------------------------------------*/
        attempts++;


        /*------------------------------------------------------------------
         * Display invalid PIN message
         *------------------------------------------------------------------*/
        StrLCD("Invalid PIN : ");


        CmdLCD(GOTO_LINE2_POS0);


        StrLCD("Try Again");


        delay_ms(1000);


        
        if(attempts == 3)
        {
            CmdLCD(CLEAR_LCD);


            StrLCD("Too many attemtps ");


            CmdLCD(GOTO_LINE2_POS0);


            StrLCD("try after some time");
					
            delay_s(10);
						return 0;
        }
        else
        {
            /*--------------------------------------------------------------
             * Display remaining attempts
             *--------------------------------------------------------------*/
            CmdLCD(CLEAR_LCD);


            StrLCD("Wrong pin : ");


            U32LCD(3 - attempts);


            CmdLCD(GOTO_LINE2_POS0);


            StrLCD("Attempts Left");


            delay_ms(1000);
        }


        /*------------------------------------------------------------------
         * Retry authentication
         *------------------------------------------------------------------*/
        goto L1;
    }
}

u32 Read_Num(u32 val,u8 *flag,u8 line)
{
    /*
     * temp -> Stores keypad input
     * num  -> Final numeric value
     * pos  -> LCD cursor position tracker
     */
    u32 temp = 0, num = 0, pos = 0;


    /*----------------------------------------------------------------------
     * Continuously read keypad input
     *----------------------------------------------------------------------*/
    while(1)
    {
        /*
         * Read keypad value
         */
				
        temp = KeyScan_t(flag);
				if(*flag==1){
					return 0;
				}
				if((!(temp>='0' || temp<='9')) && pos==0){
					return val;
				}
				CmdLCD(line);
				StrLCD("                ");
				CmdLCD(line);

        /*------------------------------------------------------------------
         * Exit input mode on invalid/non-numeric key
         *------------------------------------------------------------------*/
        if(!((temp >= '0' && temp <= '9') || (temp == 'd')))
        {
            break;
        }


        /*------------------------------------------------------------------
         * Handle delete/backspace operation
         *------------------------------------------------------------------*/
        if(temp == 'd')
        {
            /*
             * Prevent cursor underflow
             */
            if(pos != 0)
            {
                pos--;
            }


            /*
             * Move cursor to previous position
             */
            CmdLCD(line + pos);


            /*
             * Clear displayed character
             */
            CharLCD(' ');


            /*
             * Restore cursor position
             */
            CmdLCD(line + pos);


            /*
             * Remove last digit from number
             */
            num /= 10;
        }
        else
        {
            /*--------------------------------------------------------------
             * Append digit into final number
             *--------------------------------------------------------------*/
            num = num * 10 + (temp - 48);
						U32LCD(num);

            /*--------------------------------------------------------------
             * Increment cursor position
             *--------------------------------------------------------------*/
            pos++;
        }
    }


    /*----------------------------------------------------------------------
     * Clear LCD after input completion
     *----------------------------------------------------------------------*/
    CmdLCD(CLEAR_LCD);


    /*----------------------------------------------------------------------
     * Return entered number
     *----------------------------------------------------------------------*/
    return num;
}

void Set_Password()
{
    /*
     * Temporary variable to store new password
     */
    u32 temp1=0,temp2=0;
		u8 flag =0;

    /*----------------------------------------------------------------------
     * Verify user before allowing password change
     *----------------------------------------------------------------------*/
    temp1 = Authentication();
	if(temp1==0){
		state = DASHBOARD;
		return;
	}

    /*----------------------------------------------------------------------
     * Clear LCD before password setup screen
     *----------------------------------------------------------------------*/
    CmdLCD(CLEAR_LCD);


    /*----------------------------------------------------------------------
     * Position cursor on first line
     *----------------------------------------------------------------------*/
    CmdLCD(GOTO_LINE1_POS0);


    /*----------------------------------------------------------------------
     * Display password setup prompt
     *----------------------------------------------------------------------*/
    StrLCD("Set new PWD:(4Digit)");


    /*----------------------------------------------------------------------
     * Move cursor to second line
     *----------------------------------------------------------------------*/
    CmdLCD(GOTO_LINE2_POS0);


    /*----------------------------------------------------------------------
     * Read new password from user
     *----------------------------------------------------------------------*/
    temp1 = Read_Num(temp1,&flag,GOTO_LINE2_POS0);
		if(flag == 1){
			state = DASHBOARD;
			return;
		}
	if(temp1 > 9999){
		CmdLCD(CLEAR_LCD);
		StrLCD("invalid pwd length:");
		delay_ms(3000);
		state = DASHBOARD;
		return;
	}
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("Confirm pwd:(4Digit)");
	
	temp2 = Read_Num(temp2,&flag,GOTO_LINE2_POS0);
		if(flag == 1){
			state = DASHBOARD;
			return;
		}
	 if(temp2 > 9999){
		CmdLCD(CLEAR_LCD);
		StrLCD("invalid pwd length:");
		delay_ms(3000);
		state = DASHBOARD;
		return;
	}
		 if(temp1 == temp2){
    /*----------------------------------------------------------------------
     * Update system PIN
     *----------------------------------------------------------------------*/
    rfid[curr_card_index].password = temp1;
	Store_RFID();
	state = DASHBOARD;
	 }else{
    /*----------------------------------------------------------------------
     * Return application to display state
     *----------------------------------------------------------------------*/
    CmdLCD(CLEAR_LCD);
	StrLCD("Password reset");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("failed");
	delay_ms(2000);
	state = DASHBOARD;
	}
}

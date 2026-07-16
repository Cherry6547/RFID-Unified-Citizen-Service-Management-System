/******************************************************************************
 * File Name    : init.c
 * Description  : System initialization module for the RFID Unified Citizen
 *                Service Management System. This module initializes user
 *                information, stores/retrieves persistent data from EEPROM,
 *                initializes all hardware peripherals, and displays the
 *                project welcome screen.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#include "service.h"
#include "string.h"
#include <LPC214x.h>

/*----------------------------------------------------------------------------
 * RFID User Database
 * Stores complete information of all registered users.
 *--------------------------------------------------------------------------*/
struct RFID rfid[TOTAL_CARDS];

/******************************************************************************
 * Function Name : Init_RFID
 * Description   : Initializes the RFID user database with default user
 *                 information. Static information is initialized in RAM,
 *                 whereas RFID ID, password and balance are later restored
 *                 from EEPROM.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Init_RFID(void)
{
    /************************ User 0 ************************/

    rfid[0].user_flag = 1;
    rfid[0].password = 2209;

    strcpy(rfid[0].Name, "charan");
    strcpy(rfid[0].Father_Name, "Samba");
    strcpy(rfid[0].DOB, "14/12/2002");

    strcpy(rfid[0].pan.id, "CYZPC0093G");

    rfid[0].atm.balance = 10000;

    rfid[0].voterid.gender = 'M';
    rfid[0].voterid.voted = 0;
    rfid[0].voterid.index = 0;

    strcpy(rfid[0].dl.no, "KA0320150012345");
    strcpy(rfid[0].dl.validity, "14/12/2045");
    strcpy(rfid[0].dl.type, "LMV");
    strcpy(rfid[0].dl.state, "AP");

    rfid[0].dl.pincode = 521170;


    /************************ User 1 ************************/

    rfid[1].user_flag = 0;
    rfid[1].password = 1111;

    strcpy(rfid[1].Name, "Sai Kumar");
    strcpy(rfid[1].Father_Name, "Swami");
    strcpy(rfid[1].DOB, "14/10/1999");

    strcpy(rfid[1].pan.id, "CYZPC0093G");

    rfid[1].atm.balance = 10000;

    rfid[1].voterid.gender = 'M';
    rfid[1].voterid.voted = 0;
    rfid[1].voterid.index = 0;

    strcpy(rfid[1].dl.no, "KA0320150012345");
    strcpy(rfid[1].dl.validity, "14/12/2045");
    strcpy(rfid[1].dl.type, "LMV");
    strcpy(rfid[1].dl.state, "AP");

    rfid[1].dl.pincode = 521170;


    /************************ User 2 ************************/

    rfid[2].user_flag = 0;
    rfid[2].password = 1234;

    strcpy(rfid[2].Name, "shiva");
    strcpy(rfid[2].Father_Name, "Hari");
    strcpy(rfid[2].DOB, "30/10/2002");

    strcpy(rfid[2].pan.id, "HYZPC4429L");

    rfid[2].atm.balance = 10000;

    rfid[2].voterid.gender = 'F';
    rfid[2].voterid.voted = 0;
    rfid[2].voterid.index = 0;

    strcpy(rfid[2].dl.no, "AP0320150012345");
    strcpy(rfid[2].dl.validity, "14/12/2045");
    strcpy(rfid[2].dl.type, "LMV");
    strcpy(rfid[2].dl.state, "AP");

    rfid[2].dl.pincode = 521185;
}

/******************************************************************************
 * Function Name : Store_RFID
 * Description   : Stores the RFID card ID, user password, and ATM balance
 *                 of all registered users into the SPI EEPROM.
 *
 *                 This function is intended to be executed only once during
 *                 initial hardware setup to program the EEPROM with default
 *                 user information.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Store_RFID(void)
{
    u8 i, j;
    u16 pwd;
    u16 balance;

    /* Default RFID card numbers assigned to users */
    s8 id[TOTAL_CARDS][9] =
    {
        "12556450",
        "00354731",
        "12555657"
    };

    /* EEPROM address map for RFID IDs */
    u16 id_addr[TOTAL_CARDS] =
    {
        RFID0_ID,
        RFID1_ID,
        RFID2_ID
    };

    /* EEPROM address map for passwords */
    u16 pwd_addr[TOTAL_CARDS] =
    {
        RFID0_PWD,
        RFID1_PWD,
        RFID2_PWD
    };

    /* EEPROM address map for ATM balances */
    u16 bal_addr[TOTAL_CARDS] =
    {
        RFID0_BAL,
        RFID1_BAL,
        RFID2_BAL
    };

    /* Store information of every registered user */
    for(i = 0; i < TOTAL_CARDS; i++)
    {
        /*--------------------------------------------------------------
         * Store 8-byte RFID Card ID
         *-------------------------------------------------------------*/
        for(j = 0; j < 8; j++)
        {
            Byte_Write(id_addr[i] + j, id[i][j]);
            delay_ms(10);
        }

        /*--------------------------------------------------------------
         * Store User Password (16-bit)
         * MSB first followed by LSB
         *-------------------------------------------------------------*/
        pwd = (u16)rfid[i].password;

        Byte_Write(pwd_addr[i], (u8)(pwd >> 8));
        delay_ms(10);

        Byte_Write(pwd_addr[i] + 1, (u8)pwd);
        delay_ms(10);

        /*--------------------------------------------------------------
         * Store ATM Balance (16-bit)
         *-------------------------------------------------------------*/
        balance = (u16)rfid[i].atm.balance;

        Byte_Write(bal_addr[i], (u8)(balance >> 8));
        delay_ms(10);

        Byte_Write(bal_addr[i] + 1, (u8)balance);
        delay_ms(10);
    }
}

/******************************************************************************
 * Function Name : Retrieve_RFID
 * Description   : Retrieves the RFID card ID, user password, and ATM balance
 *                 of all registered users from the SPI EEPROM and updates
 *                 the RFID database in RAM.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Retrieve_RFID(void)
{
    u8 i, j;
    u8 high_byte;
    u8 low_byte;

    /* EEPROM address map for RFID IDs */
    u16 id_addr[TOTAL_CARDS] =
    {
        RFID0_ID,
        RFID1_ID,
        RFID2_ID
    };

    /* EEPROM address map for passwords */
    u16 pwd_addr[TOTAL_CARDS] =
    {
        RFID0_PWD,
        RFID1_PWD,
        RFID2_PWD
    };

    /* EEPROM address map for ATM balances */
    u16 bal_addr[TOTAL_CARDS] =
    {
        RFID0_BAL,
        RFID1_BAL,
        RFID2_BAL
    };

    /* Retrieve information of every registered user */
    for(i = 0; i < TOTAL_CARDS; i++)
    {
        /*--------------------------------------------------------------
         * Retrieve 8-byte RFID Card ID
         *-------------------------------------------------------------*/
        for(j = 0; j < 8; j++)
        {
            rfid[i].id[j] = ByteRead_25LC512(id_addr[i] + j);
        }

        /* Null terminate the RFID string */
        rfid[i].id[8] = '\0';

        /*--------------------------------------------------------------
         * Retrieve User Password (16-bit)
         *-------------------------------------------------------------*/
        high_byte = ByteRead_25LC512(pwd_addr[i]);
        low_byte  = ByteRead_25LC512(pwd_addr[i] + 1);

        rfid[i].password =
            ((u16)high_byte << 8) | low_byte;

        /*--------------------------------------------------------------
         * Retrieve ATM Balance (16-bit)
         *-------------------------------------------------------------*/
        high_byte = ByteRead_25LC512(bal_addr[i]);
        low_byte  = ByteRead_25LC512(bal_addr[i] + 1);

        rfid[i].atm.balance =
            ((u16)high_byte << 8) | low_byte;
    }
}

/******************************************************************************
 * Function Name : Update_Balance_Eeprom
 * Description   : Updates the ATM balance of the specified user in the
 *                 SPI EEPROM. Only the balance field is updated, avoiding
 *                 unnecessary writes to other EEPROM locations.
 *
 * Parameters    :
 *      card_index - Index of the RFID user whose balance is to be updated.
 *
 * Returns       : None
 ******************************************************************************/
void Update_Balance_Eeprom(s16 card_index)
{
    u16 addr;
    u16 balance;

    /* Determine EEPROM address corresponding to the selected user */
    switch(card_index)
    {
        case 0:
            addr = RFID0_BAL;
            break;

        case 1:
            addr = RFID1_BAL;
            break;

        case 2:
            addr = RFID2_BAL;
            break;

        default:
            /* Invalid card index */
            return;
    }

    /* Read updated balance from RFID database */
    balance = rfid[card_index].atm.balance;

    /* Store Most Significant Byte (MSB) */
    Byte_Write(addr, (u8)(balance >> 8));
    delay_ms(10);

    /* Store Least Significant Byte (LSB) */
    Byte_Write(addr + 1, (u8)balance);
    delay_ms(10);
}
/******************************************************************************
 * Function Name : System_Init
 * Description   : Initializes all hardware peripherals and application data
 *                 required for the RFID Unified Citizen Service Management
 *                 System. During startup, user information is initialized and
 *                 persistent data is retrieved from the SPI EEPROM.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void System_Init(void)
{
    /* Initialize communication peripherals */
    UART0_Init();
    SPI0_Init();

    /* Initialize user interface peripherals */
    InitLCD();
    KPM_Init();
    Led_Init();
    Buzzer_Init();

    /* Initialize system peripherals */
    RTC_Init();
    Timer0_Init();
    Exti_switch_Init();

    /* Initialize default user information */
    Init_RFID();

    /*
     * Store_RFID() is intended to be executed only once while programming
     * the hardware for the first time. After the EEPROM has been programmed,
     * it should remain commented to preserve the stored data.
     */
    //Store_RFID();

    /* Retrieve RFID IDs, passwords and balances from EEPROM */
    Retrieve_RFID();
}

/******************************************************************************
 * Function Name : Display_Project
 * Description   : Displays the project title on the LCD during system startup.
 *                 The welcome screen is displayed for 3 seconds before the
 *                 application enters the RFID waiting state.
 *
 * Parameters    : None
 *
 * Returns       : None
 ******************************************************************************/
void Display_Project(void)
{
    /* Display project title */
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("    RFID Unified    ");

    CmdLCD(GOTO_LINE2_POS0);
    StrLCD("  Citizen Service   ");

    CmdLCD(GOTO_LINE3_POS0);
    StrLCD(" Management System ");

    /* Display splash screen for 3 seconds */
    tdelay_ms(0, 3000);
}

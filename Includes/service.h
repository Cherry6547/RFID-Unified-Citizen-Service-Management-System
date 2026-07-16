/******************************************************************************
 * File Name    : service.h
 * Description  : Common header file for the RFID Unified Citizen Service
 *                Management System. This file contains application state
 *                definitions, data structures, global variables, EEPROM
 *                memory mapping, and function prototypes used throughout
 *                the project.
 *
 * Author       : Charan Sai Mathkala
 * Target MCU   : LPC2148 (ARM7TDMI-S)
 ******************************************************************************/

#ifndef SERVICE_H
#define SERVICE_H

/*----------------------------------------------------------------------------
 * Header Files
 *--------------------------------------------------------------------------*/
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "uart0.h"
#include "kpm.h"
#include "rtc.h"
#include "timer.h"
#include "delays.h"
#include "spi.h"
#include "spi_eeprom.h"
#include "led.h"
#include "buzzer.h"
#include "exti_switch.h"
#include <string.h>

/*----------------------------------------------------------------------------
 * EEPROM Memory Map
 *
 * Each RFID user occupies 20 bytes.
 *
 *  -------------------------------------------------------
 * | RFID ID | Gap | Password | Gap | Balance | Remaining |
 *  -------------------------------------------------------
 *--------------------------------------------------------------------------*/
#define RFID0_ID       0
#define RFID0_PWD      9
#define RFID0_BAL      12

#define RFID1_ID       20
#define RFID1_PWD      29
#define RFID1_BAL      32

#define RFID2_ID       40
#define RFID2_PWD      49
#define RFID2_BAL      52

/* Total number of registered RFID users */
#define TOTAL_CARDS    3


/*----------------------------------------------------------------------------
 * Global Variables
 *--------------------------------------------------------------------------*/

/* UART */
extern s8 uart_data[10];
extern u32 uart_index;

/* Application State */
extern u32 state;

/* Current Authenticated User */
extern s8 tempid[9];
extern s16 curr_card_index;

/* RTC Information */
extern u32 year;
extern u8 hour, min, sec;
extern u8 date, month;
extern u8 RTC_Flag;


/*----------------------------------------------------------------------------
 * Application State Machine
 *--------------------------------------------------------------------------*/
enum state_t
{
    INIT,
    WAIT,
    VALID,
    INVALID,
    DASHBOARD,
    PAN,
    ATM,
    VOTE,
    DL,
    OFF,
    EXIT,
    RESET
};


/*----------------------------------------------------------------------------
 * User Data Structures
 *--------------------------------------------------------------------------*/

/* PAN Card Information */
struct PAN_t
{
    s8 id[11];
};

/* ATM Information */
struct ATM_t
{
    u16 balance;
};

/* Voter Information */
struct VOTERID_t
{
    s8 gender;
    s8 voted;
    s8 index;
};

/* Driving License Information */
struct DL_t
{
    s8 no[17];
    s8 validity[11];
    s8 type[20];
    u32 pincode;
    s8 state[3];
};

/* Complete RFID User Record */
struct RFID
{
    s8 id[9];
    u16 password;
    s8 user_flag;

    s8 Name[30];
    s8 Father_Name[30];
    s8 DOB[11];

    struct PAN_t pan;
    struct ATM_t atm;
    struct VOTERID_t voterid;
    struct DL_t dl;
};

/* RFID User Database */
extern struct RFID rfid[TOTAL_CARDS];


/*----------------------------------------------------------------------------
 * System Initialization
 *--------------------------------------------------------------------------*/
void System_Init(void);
void Display_Project(void);


/*----------------------------------------------------------------------------
 * RFID Service
 *--------------------------------------------------------------------------*/
void Read_RFID(void);
void Store_RFID(void);
void Retrieve_RFID(void);
u8 Validate(s8 *id);


/*----------------------------------------------------------------------------
 * Authentication
 *--------------------------------------------------------------------------*/
s16 Authentication(void);
u32 Read_Num(u32 val, u8 *flag, u8 line);


/*----------------------------------------------------------------------------
 * User Authentication States
 *--------------------------------------------------------------------------*/
void Valid_Card(void);
void Invalid_Card(void);


/*----------------------------------------------------------------------------
 * Dashboard
 *--------------------------------------------------------------------------*/
void Menu(void);


/*----------------------------------------------------------------------------
 * PAN Service
 *--------------------------------------------------------------------------*/
void Get_Pan(void);
void Show_Pan_Info(void);


/*----------------------------------------------------------------------------
 * ATM Service
 *--------------------------------------------------------------------------*/
void Get_Atm(void);
void Update_Balance_Eeprom(s16 card_index);


/*----------------------------------------------------------------------------
 * Voting Service
 *--------------------------------------------------------------------------*/
void Get_Vote_Info(void);
void Show_Vote_Info(void);
void Cast_Vote(void);
void Edit_Vote_Machine(void);
void Reset_Votes(void);


/*----------------------------------------------------------------------------
 * RTC / Driving License
 *--------------------------------------------------------------------------*/
void Configure_Rtc(void);
void Get_Dl_Info(void);
u8 Get_DL_Validity(void);
void Show_DL_Info(void);
void Get_Dl_Info(void);

/*----------------------------------------------------------------------------
 * Password Service
 *--------------------------------------------------------------------------*/
void Set_Password(void);

#endif

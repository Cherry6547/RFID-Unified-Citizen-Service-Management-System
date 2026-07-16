# RFID Unified Citizen Service Management System

An Embedded C project developed on the **LPC2148 (ARM7TDMI-S)** microcontroller that provides multiple citizen services using a single RFID card.

The project integrates multiple peripherals such as **RFID, SPI EEPROM, UART, RTC, LCD, Matrix Keypad, LEDs, and Buzzer** to simulate a unified citizen service platform.

---

## Features

- RFID Based User Authentication
- Password Protected Services
- PAN Card Information
- ATM Services
  - Balance Enquiry
  - Cash Deposit
  - Cash Withdrawal
- Digital Voting System
- Driving License Information
- RTC Configuration
- Officer/Admin Mode
- SPI EEPROM Data Storage

---

## Hardware Used

- LPC2148 ARM7 Development Board
- RFID Reader
- RFID Cards
- 20x4 Character LCD
- 4x4 Matrix Keypad
- SPI EEPROM (25LC512)
- RTC (External Clock Source)
- LEDs
- Buzzer

---

## Software Used

- Embedded C
- Keil µVision
- Flash Magic
- Git & GitHub

---

## Project Architecture

```
Application
│
├── Drivers
│   ├── UART
│   ├── SPI
│   ├── RTC
│   ├── LCD
│   ├── Timer
│   ├── Keypad
│   ├── EEPROM
│   ├── LED
│   └── Buzzer
│
├── Services
│   ├── RFID Authentication
│   ├── PAN
│   ├── ATM
│   ├── Voting
│   ├── Driving License
│   └── Password Management
│
└── Application State Machine
```

---
## Project Workflow

```text
                 RFID Card
                     │
                     ▼
            RFID Authentication
                     │
         ┌───────────┴───────────┐
         │                       │
         ▼                       ▼
    Officer Card             User Card
         │                       │
         ▼                       ▼
  Officer Authentication      Dashboard
         │                       │
         ▼                       │
 Reset Voting Machine            │
                                 │
        ┌─────────────┬──────────┼───────────┐
        │             │          │           │
        ▼             ▼          ▼           ▼
      PAN           ATM       Voting   Driving License
        │             │          │           │
        ▼             ▼          ▼           ▼
 Password Auth  Password Auth Password Auth Display Details
        │             │          │
        ▼             ▼          ▼
 Display Info   ATM Operations  Cast / View Vote
```

---

## Repository Structure

```
App/
Drivers/
Includes/
Service/
```

---

## Author

**Charan Sai Mathkala**

Embedded Systems | ARM | Embedded C | Linux System Programming

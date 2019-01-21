/*********************************************************************
 * Collin Maker and Thomas Quist
 * EGR 436 - Lab 1 - UART Communication Controlled Metronome
 * - Code interfaces with C based console code through USB to UART dongle
 * - Includes files for setup and operation of a circular serial buffer
 * for future use
 ********************************************************************/

//Standard Includes
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//Custom Includes
#include "globals.h"
#include "hardware.h"
#include "serial.h"

//Local Prototypes
void InitHardware();
void InitSoftware();
void Append(char *string, char letter);
//================================================================================
int main(void)
{
    //Halting the Watchdog
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    //Hardware and interrupt setup
    InitHardware();
    InitSoftware();

    int bpm = BASE_BPM;   //Base BPM

    while (1)   //Main program loop
    {
        
        if(UARTFlag)    //Characters have been received but not full command
        {
            if(CheckFullCommand())  //Look for newline character in the buffer
            {
                ReadFromBuffer();   //Get full command
                bpm = atoi(RxRead);
                if(BPMToTicks(bpm))
                {
                    EchoCommand();
                    //Echo Back BPM
                }
                else
                {
                    //Echo back error message
                }
            }
        }

        /*
        if (UARTFlag == 1)
        {
            if (C == '\n')
            {
                UARTFlag = 1;
                num = atoi(string);
                if (num < 2 || num > 220)
                {
                 //Invalid number, default to 60
                 Tempo = 60;
                }
                else
                {
                    Tempo = num;
                }

                strcpy(string,"");
                C = 'null';
            }
            else
            {
            Append(string,C);
            C = 'null';
            }
            UARTFlag = 0;

        }
        else
        {

        }*/
    }
}
//================================================================================
//Initializes the hardware on the system
//================================================================================
void InitHardware()
{
    __disable_irq();
    LFXT_Init ();
    TimerA1_Init();
    LedOutput_Init();
    UART0_init();
    BPMToTicks(BASE_BPM);
    __enable_irq();
}
//================================================================================
//Initializes the software on the system
//================================================================================
void InitSoftware()
{
    UARTFlag = 0;

    //Define pointer index
    RxWriteIndex = 0;
    RxReadIndex = 0;

}
//================================================================================
//Interrupt vector for the UART Communications
//================================================================================
void EUSCIA0_IRQHandler(void)
{
    RxBuffer[RxWriteIndex] = EUSCI_A0->RXBUF;   //Takes char from buffer and puts writes to serial buffer
    RxWriteIndex = (RxWriteIndex + 1) % BUFFER_SIZE;    //Increments the circular buffer write index
    P2->OUT ^= 2;   //Toggles led for debug
    UARTFlag = 1;   //Set UART flag to begin parsing of the buffer
}
//================================================================================
//Interrupt vector for the timer A Capture Compare
// Used here as the handler of BPM Indicator LED
//================================================================================
void TA1_0_IRQHandler(void)
{
    TIMER_A1->CCTL[0] &=~ 1; //clear interrupt flag

    //This should allow for the led to flash on beats
    if(P1->OUT & 0x01)  //LED is on
    {
        TIMER_A1->CCR[0] = BPMTicks - FLASH_TICKS;
        P1->OUT &= ~0x01;
    }
    else    //LED is off
    {
        TIMER_A1->CCR[0] = FLASH_TICKS;
        P1->OUT |= 0x01;
    }
    //P1->OUT ^= 0x01; //Toggle LED

}

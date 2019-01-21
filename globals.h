/*
 * globals.h
 *
 *  Created on: Jan 14, 2019
 *      Author: colli
 */

#include <stdint.h>


#ifndef GLOBALS_H_
#define GLOBALS_H_
//Macros
#define BUFFER_SIZE 10
#define BASE_BPM    60
#define FLASH_TICKS 40

#define NEWLINE_CHAR    10  //Char to control the end of commands

//Globals
uint8_t UARTFlag;
float BPMTicks;

//Serial Buffer Handling
uint8_t RxBuffer[BUFFER_SIZE];
uint8_t RxRead[BUFFER_SIZE];
uint8_t TxBuffer[BUFFER_SIZE];
int RxWriteIndex;
int RxReadIndex;
int RxReadTo;





#endif /* GLOBALS_H_ */

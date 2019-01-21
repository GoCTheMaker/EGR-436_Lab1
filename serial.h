/*
 * serial.h
 *
 *  Created on: Jan 14, 2019
 *      Author: colli
 */

#ifndef SERIAL_H_
#define SERIAL_H_

//Prototypes
void UART0_init();
int CheckFullCommand();
void ReadFromBuffer();
void EchoCommand();


#endif /* SERIAL_H_ */

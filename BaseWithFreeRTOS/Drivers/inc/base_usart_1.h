/*
 * base_usart_1.h
 *
 *  Created on: 16. apr 2018
 *      Author: Sad
 */

#ifndef INC_BASE_USART_1_H_
#define INC_BASE_USART_1_H_



/*
 * Includes
 * */
#include "stm32f10x_usart.h"

/*
 * Defines, macros
 * */
#define BASE_USART_1_RX_BUFFER_SIZE 16
#define BASE_USART_1_TX_BUFFER_SIZE 16

/*
 * GLOBAL
 * */

/*
 * Functions
 * */
void base_usart_1_init(USART_InitTypeDef *USART_Conf);

/*
 * NVIC functions
 * */

/*
 * Threads
 * */

#endif /* INC_BASE_USART_1_H_ */

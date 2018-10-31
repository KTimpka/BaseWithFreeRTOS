/*
 * WS2812B.h
 *
 *  Created on: 17. apr 2018
 *      Author: Sad
 */

#ifndef INC_BASE_WS2812B_H_
#define INC_BASE_WS2812B_H_



/*
 * Includes
 * */
#include "stdint.h"

/*
 * Defines, macros
 * */
#define LED_COUNT 36
#define LED_RESET 15

#define	WS2812B_TX_BUFFER_SIZE  (uint32_t)(12 + 1) //One extra for 0

//Thread defines
#define FRONT_MIN	0	//Front is 8 + 8 from 0 to 15
#define BACK_MIN	16	//Back is 10 + 10 from 16 to 35

#define FRONT_MID	8
#define	BACK_MID	26

#define	FRONT_STEP	8
#define BACK_STEP	10

#define	FRONT_COLOR	0x96FFFF
#define BACK_COLOR	0xFF0000

#define	FRONT_COLOR_HALF	0x121111
#define BACK_COLOR_HALF		0x110000

/*
 * GLOBAL
 * */

/*
 * Functions
 * */
void base_WS2812B_init();

void base_WS2812B_hex_to_buffer(uint32_t value, uint16_t *buffer_p);

/*
 * NVIC functions
 * */

/*
 * Threads
 * */

#endif /* INC_BASE_WS2812B_H_ */

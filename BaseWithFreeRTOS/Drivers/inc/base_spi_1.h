/*
 * base_spi.h
 *
 *  Created on: 17. apr 2018
 *      Author: Sad
 */

#ifndef INC_BASE_SPI_1_H_
#define INC_BASE_SPI_1_H_



/*
 * Includes
 * */
#include "stdint.h"

/*
 * Defines, macros
 * */

/*
 * GLOBAL
 * */

/*
 * Functions
 * */
void base_spi_1_init();

void base_spi_1_dma_init(uint32_t buffer_address, uint32_t buffer_size);

void base_spi_1_send_buffer();

void base_spi_1_set_times(uint32_t value);

void base_spi_1_set_irq_callback(void (*spi_callback_p)());

/*
 * NVIC functions
 * */
void DMA1Channel3_IRQHandler(void);

/*
 * Threads
 * */

#endif /* INC_BASE_SPI_1_H_ */

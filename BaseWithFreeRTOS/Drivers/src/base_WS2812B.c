/*
 * WS2812B.c
 *
 *  Created on: 17. apr 2018
 *      Author: Sad
 */

#include <base_WS2812B.h>

#include "stdint.h"
#include "stddef.h"

#include "FreeRTOS.h"
#include "task.h"

#include "base_spi_1.h"

/*
 * SPI sends 12 bytes (8 bits for LED 1 bit) buffer
 * */
static uint16_t WS2812B_TX_BUFFER[WS2812B_TX_BUFFER_SIZE];

/*
 * All LED values
 * + 15 will all be 0, LED needs >50us low to start new cycle @4,5mhz = 222ns * 16 bit * 15 sends = 53us
 * */
static uint32_t LED_BUFFER[LED_COUNT + LED_RESET];
/*
 * Static value
 * */
static uint32_t CALLBACK_LED_COUNTER;


static void base_WS2812B_thread(void *pvParameters);
static void base_WS2812B_spi_callback();

void base_WS2812B_init()
{
	/*
	 * Start SPI and DMA for SPI
	 * */
	base_spi_1_init(NULL);
	base_spi_1_dma_init((uint32_t)WS2812B_TX_BUFFER, WS2812B_TX_BUFFER_SIZE);
	base_spi_1_set_irq_callback(&base_WS2812B_spi_callback);

	/*
	 * Zero
	 * */


	/*
	 * Add 0 to the end
	 * */
	WS2812B_TX_BUFFER[12] = 0;

	/*
	 * Clear LEDs
	 * */
	int c;
	for (c = 0; c < LED_COUNT + LED_RESET - 1; ++c)
		LED_BUFFER[c] = 0;

	/*
	 * Reset LED counter
	 * */
	CALLBACK_LED_COUNTER = LED_COUNT + LED_RESET - 1;

	/*
	 * Start thread for WS2812B LED driver
	 * */
	xTaskCreate( base_WS2812B_thread, "Base_WS2812B", configMINIMAL_STACK_SIZE, 0, 0, NULL );
}

void base_WS2812B_hex_to_buffer(uint32_t value, uint16_t *buffer_p)
{
	uint32_t c_value, c_buff, temp;

	/*
	 * Data from RGB to GRB
	 * No need for 0:0:0
	 * */
	if (value) {
		temp = value;							//Copy value
		value &= 	0x00FF0000;					//RGB and 100 = R__
		value >>= 	8;							//R__ to _R_
		value |= 	temp & 0x000000FF;			//_R_ or __B = _RB
		value |= 	(temp << 8) & 0x00FF0000;	//_RB or (RGB to GB_ and 100 = G__ ) = GRB
	}

	/*
	 * Fill buffer
	 * */
	for (c_value = 0, c_buff = WS2812B_TX_BUFFER_SIZE - 1; c_buff > 0; c_value += 2, c_buff -= 1) {
		temp = (value >> c_value) & 0x000003;

		/*
		 * 0 - 0xC0
		 * 1 - 0xF0
		 * */
		switch (temp) {
		case 0:
			buffer_p[c_buff - 1] = 0xC0C0;
			break;
		case 1:
			buffer_p[c_buff - 1] = 0xC0F0;
			break;
		case 2:
			buffer_p[c_buff - 1] = 0xF0C0;
			break;
		default:
			buffer_p[c_buff - 1] = 0xF0F0;
			break;
		}
	}
}

void base_WS2812B_clear_tx_buffer()
{
	uint32_t c;
	for (c = 0; c < WS2812B_TX_BUFFER_SIZE ;++c)
		WS2812B_TX_BUFFER[c] = 0;
}

static void base_WS2812B_spi_callback()
{
	if (CALLBACK_LED_COUNTER == LED_COUNT + LED_RESET - 1) {	//Send RESET code
		base_WS2812B_clear_tx_buffer();
		CALLBACK_LED_COUNTER -= 1;
		return;
	} else if (CALLBACK_LED_COUNTER >= LED_COUNT) {				//Send zeros without reseting buffer
		CALLBACK_LED_COUNTER -= 1;
	} else if (!CALLBACK_LED_COUNTER){							//Reset LED counter
		base_WS2812B_hex_to_buffer(	LED_BUFFER[CALLBACK_LED_COUNTER], WS2812B_TX_BUFFER);
		CALLBACK_LED_COUNTER = LED_COUNT + LED_RESET - 1;
	} else {													//Regular
		base_WS2812B_hex_to_buffer(	LED_BUFFER[CALLBACK_LED_COUNTER], WS2812B_TX_BUFFER);
		CALLBACK_LED_COUNTER -= 1;
	}
}

static void base_WS2812B_pattern_1()
{
	static uint32_t FRONT_START;
	static uint32_t BACK_START;

	uint32_t status, c, color;

	if (FRONT_START >= FRONT_MID)
		FRONT_START = FRONT_MID - 1;
	if (BACK_START >= BACK_MID)
		BACK_START = BACK_MIN;

	status 	= FRONT_START;
	color 	= FRONT_COLOR;
	for (c = 0; c < 4; ++c) {
		LED_BUFFER[status] 				= color;
		LED_BUFFER[status + FRONT_STEP] = color;

		status -= 1;
		if (status >= FRONT_MID)
			status = FRONT_MID - 1;

		if (color == FRONT_COLOR_HALF)
			color = FRONT_COLOR;
		else
			color = FRONT_COLOR_HALF;
	}

	status 	= BACK_START;
	color 	= BACK_COLOR;
	for (c = 0; c < 4; ++c) {
		LED_BUFFER[status] 				= color;
		LED_BUFFER[status + BACK_STEP] 	= color;

		status += 1;
		if (status >= BACK_MID)
			status = BACK_MIN;

		if (color == BACK_COLOR_HALF)
			color = BACK_COLOR;
		else
			color = BACK_COLOR_HALF;
	}


	FRONT_START -= 1;
	BACK_START 	+= 1;

}

static void base_WS2812B_thread(void *pvParameters)
{
	/*
	 * Start SPI
	 * */
	base_spi_1_send_buffer(WS2812B_TX_BUFFER_SIZE);

	int32_t	c;
	/*
	 * Init colors
	 * */
	for (c = 0; c < 8; ++c) {			//First LEDs "BLUE"
		LED_BUFFER[c] 		= 0x96FFFF;
		LED_BUFFER[c + 8] 	= 0x96FFFF;
	}
	for (c = 16; c < 16 + 10; ++c) {
		LED_BUFFER[c] 		= 0xFF0000;	//Back LEDs "RED"
		LED_BUFFER[c + 10] 	= 0xFF0000;
	}

	//Thread loop
	while (1) {

		base_WS2812B_pattern_1();

		//Wait
		vTaskDelay((TickType_t) (100 / portTICK_PERIOD_MS));
	}
	//Never exit thread
	while (1);
}

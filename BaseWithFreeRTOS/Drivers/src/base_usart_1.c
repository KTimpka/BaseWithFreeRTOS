/*
 * base_usart_1.c
 *
 *  Created on: 16. apr 2018
 *      Author: Sad
 */

#include "base_usart_1.h"

#include "stddef.h"

#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "misc.h"


static uint8_t	RX_BUFFER[BASE_USART_1_RX_BUFFER_SIZE];
static uint8_t	TX_BUFFER[BASE_USART_1_TX_BUFFER_SIZE];

static void (*rx_full_p)();
static void (*tx_done_p)();


/*
 * Initialize USART 1
 * */
void base_usart_1_init(USART_InitTypeDef *USART_Conf)
{
	/*
	 * If USART_Conf is NULL use default
	 * USART configured as follow:
	 * 		- BaudRate = 115200 baud
	 * 		- Word Length = 8 Bits
	 * 		- One Stop Bit
	 * 		- No parity
	 * 		- Hardware flow control disabled (RTS and CTS signals)
	 * 		- Receive and transmit enabled
	 * */
	if (USART_Conf == NULL) {
		USART_InitTypeDef USART_Conf_default;

		USART_Conf_default.USART_BaudRate 				= (uint32_t)115200;					//115200 Baud
		USART_Conf_default.USART_WordLength 			= USART_WordLength_8b;				//8 Bits
		USART_Conf_default.USART_StopBits 				= USART_StopBits_1;					//1 Stop bit
		USART_Conf_default.USART_Parity 				= USART_Parity_No;					//No parity
		USART_Conf_default.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;	//No hardware control
		USART_Conf_default.USART_Mode 					= USART_Mode_Tx | USART_Mode_Rx;	//Use both RX and TX

		USART_Conf = &USART_Conf_default;
	}

	/*
	 * NULL function pointers
	 * */
	rx_full_p = NULL;
	tx_done_p = NULL;

	/*
	 * Start clock for USART1, IOPorts, AFIOPorts, DMA1
	 * */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//Using Port B pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//Using USART1 alternate pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1 clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//DMA1 clock

	/*
	 * Configure GPIO pins
	 * Remap USART1 pins
	 * PB6 TX as AF_PP
	 * PB7 RX as IN_FLOAT
	 * */
	GPIO_InitTypeDef GPIO_Conf;

	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);			//PB 6/7, UART1 TX/RX, remap

	//PB 6, UART1 TX
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_6;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_Conf);

	//PB 7, UART1 RX
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_7;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_Conf);

	/*
	 * Set USART parameters to hardware
	 * */
	USART_Init(USART1, USART_Conf);

	/*
	 * Configure DMA for USART
	 * */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);			//Enable DMA interrupt for RX
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);			//Enable DMA interrupt for TX

	/*
	 * DMA configuration for RX
	 * RX buffer is 16 bytes (random number)	TODO: RX Buffer size ok?
	 * DMA transfer complete interruption is used to set buffer full bit
	 * */
	DMA_InitTypeDef DMA_Conf;

	DMA_Conf.DMA_PeripheralBaseAddr	=	(uint32_t)&(USART1->DR);	//USART1 Data register address
	DMA_Conf.DMA_MemoryBaseAddr		=	(uint32_t)RX_BUFFER;		//Buffer in RAM for RX
	DMA_Conf.DMA_DIR				=	DMA_DIR_PeripheralSRC;		//Peripheral as source
	DMA_Conf.DMA_BufferSize			=	BASE_USART_1_RX_BUFFER_SIZE;//Buffer size
	DMA_Conf.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;	//Disable address inc
	DMA_Conf.DMA_MemoryInc			=	DMA_MemoryInc_Enable;		//Enable buffer address inc
	DMA_Conf.DMA_PeripheralDataSize	= 	DMA_PeripheralDataSize_Byte;//USART buffer is 1 byte
	DMA_Conf.DMA_MemoryDataSize		=	DMA_MemoryDataSize_Byte;	//Write 1 byte to buffer
	DMA_Conf.DMA_Mode				=	DMA_Mode_Circular;			//Buffer is used as circle half and half
	DMA_Conf.DMA_Priority			= 	DMA_Priority_Medium;		//Medium priority IDK TODO: Is medium priority OK?
	DMA_Conf.DMA_M2M				=	DMA_M2M_Disable;			//Not using memory to memory

	DMA_Init(DMA1_Channel5, &DMA_Conf);								//USART1 RX is on Channel 5

	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);					//Enable interruption when full buffer is filled

	DMA_Cmd(DMA1_Channel5, ENABLE);									//Start DMA

	/*
	 * DMA configuration for TX
	 * TX buffer is 16 bytes (random number)	TODO: TX Buffer size ok?
	 * DMA transfer complete interruption is used to set done bit
	 * */
	DMA_Conf.DMA_PeripheralBaseAddr	=	(uint32_t)&(USART1->DR);	//USART1 Data register address
	DMA_Conf.DMA_MemoryBaseAddr		=	(uint32_t)TX_BUFFER;		//Buffer in RAM for RX
	DMA_Conf.DMA_DIR				=	DMA_DIR_PeripheralDST;		//Peripheral as source
	DMA_Conf.DMA_BufferSize			=	BASE_USART_1_TX_BUFFER_SIZE;//Buffer size
	DMA_Conf.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;	//Disable address inc
	DMA_Conf.DMA_MemoryInc			=	DMA_MemoryInc_Enable;		//Enable buffer address inc
	DMA_Conf.DMA_PeripheralDataSize	= 	DMA_PeripheralDataSize_Byte;//USART buffer is 1 byte
	DMA_Conf.DMA_MemoryDataSize		=	DMA_MemoryDataSize_Byte;	//Write 1 byte to buffer
	DMA_Conf.DMA_Mode				=	DMA_Mode_Normal;			//After buffer is sent, stop DMA
	DMA_Conf.DMA_Priority			= 	DMA_Priority_Medium;		//Medium priority IDK TODO: Is medium priority OK?
	DMA_Conf.DMA_M2M				=	DMA_M2M_Disable;			//Not using memory to memory

	DMA_Init(DMA1_Channel4, &DMA_Conf);								//USART1 TX is on Channel 4

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);					//Enable interruption when full buffer is transfered

	/*
	 * Configure NVIC for USART
	 * */
	NVIC_InitTypeDef NVIC_conf;

	//DMA interrupt
	NVIC_conf.NVIC_IRQChannel						=	DMA1_Channel5_IRQn;	//NVIC interrupt vector
	NVIC_conf.NVIC_IRQChannelPreemptionPriority		=	5;					//Priority 0...15	TODO: Priority ok?
	NVIC_conf.NVIC_IRQChannelSubPriority			=	0;					//Sub Priority not used
	NVIC_conf.NVIC_IRQChannelCmd					=	ENABLE;				//Enable DMA1_Channel5 interrupt
	NVIC_Init(&NVIC_conf);													//Apply to hardware

	//DMA interrupt
	NVIC_conf.NVIC_IRQChannel						=	DMA1_Channel4_IRQn;	//NVIC interrupt vector
	NVIC_conf.NVIC_IRQChannelPreemptionPriority		=	5;					//Priority 0...15	TODO: Priority ok?
	NVIC_conf.NVIC_IRQChannelSubPriority			=	0;					//Sub Priority not used
	NVIC_conf.NVIC_IRQChannelCmd					=	ENABLE;				//Enable DMA1_Channel4 interrupt
	NVIC_Init(&NVIC_conf);													//Apply to hardware

	/*
	 * Start USART 1
	 * */
	USART_Cmd(USART1, ENABLE);
}

/*
 * NVIC function for TX DMA
 * */
void DMA1Channel4_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC4)) {
		DMA_ClearFlag(DMA1_FLAG_TC4);

		/*
		 * Call user function
		 * */
		if (tx_done_p)
			tx_done_p();
		else
			DMA_Cmd(DMA1_Channel4, DISABLE);
	}
}

/*
 * NVIC function for RX DMA
 * */
void DMA1Channel5_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC5)) {
		DMA_ClearFlag(DMA1_FLAG_TC5);

		/*
		 * Call user function
		 * */
		if (rx_full_p)
			rx_full_p();
	}
}

uint32_t base_usart_1_print(uint8_t *buf, uint32_t size)
{
	uint32_t c;

	//DMA still working
	if (DMA_GetCurrDataCounter(DMA1_Channel4))
		return 0;

	//Copy buffer
	for (c = 0; c < size && c < BASE_USART_1_TX_BUFFER_SIZE; ++c) {
		TX_BUFFER[c] = buf[c];
	}

	//Set buffer size
	DMA1_Channel4->CNDTR =	c;
	//Start DMA
	DMA_Cmd(DMA1_Channel4, ENABLE);

	return c;
}

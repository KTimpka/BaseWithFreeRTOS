/*
 * base_spi.c
 *
 *  Created on: 17. apr 2018
 *      Author: Sad
 */
#include <base_spi_1.h>

#include "stdint.h"
#include "stddef.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"


static uint32_t IRQ_HANDLER_TIMES;
static uint32_t BUFFER_SIZE;

static void (*spi_callback)();

void base_spi_1_init(SPI_InitTypeDef *SPI_Conf)
{
	/*
	 * Default SPI configure values if SPI_Conf is NULL
	 * */
	if (SPI_Conf == NULL) {
		SPI_InitTypeDef SPI_Conf_Default;

		SPI_Conf_Default.SPI_Direction			= SPI_Direction_1Line_Tx;		//Only send data
		SPI_Conf_Default.SPI_Mode				= SPI_Mode_Master;				//Master mode
		SPI_Conf_Default.SPI_DataSize			= SPI_DataSize_16b;				//Lets use 16 bits
		SPI_Conf_Default.SPI_CPOL				= SPI_CPOL_High;				//Idle is high
		SPI_Conf_Default.SPI_CPHA				= SPI_CPHA_1Edge;				//Capture data on firts edge, not used
		SPI_Conf_Default.SPI_NSS				= SPI_NSS_Soft;					//Software slave select, not used
		SPI_Conf_Default.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_16;		//Baud 4,5Mhz
		SPI_Conf_Default.SPI_FirstBit			= SPI_FirstBit_MSB;				//First bit is LSB
		SPI_Conf_Default.SPI_CRCPolynomial		= (uint16_t)8;					//Random number, not used

		SPI_Conf = &SPI_Conf_Default;
	}

	/*
	 * Zero out globals
	 * */
	IRQ_HANDLER_TIMES 	= 1;
	BUFFER_SIZE 		= 0;

	spi_callback = NULL;

	/*
	 * Start clock for SPI, IOports, AFIOPins
	 * */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//Using Port A pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//AFIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	//SPI1 clock


	/*
	 * Configure GPIO
	 * For SPI1 PA7 as MOSI
	 * */
	GPIO_InitTypeDef GPIO_Conf;

	//PB 7, SPI 1 MOSI
	GPIO_Conf.GPIO_Pin 		= GPIO_Pin_7;
	GPIO_Conf.GPIO_Speed 	= GPIO_Speed_10MHz;
	GPIO_Conf.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_Conf);

	/*
	 * Apply configure to hardware
	 * */

	SPI_Init(SPI1, SPI_Conf);

	/*
	 * Enable DMA request if TX buffer is empty
	 * */
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

	/*
	 * Start SPI1
	 * */
	SPI_Cmd(SPI1, ENABLE);
}

void base_spi_1_dma_init(uint32_t buffer_address, uint32_t buffer_size)
{
	/*
	 * Start clock for DMA1
	 * */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//DMA1 clock

	/*
	 * Configure DMA1
	 * SPI is on channel 3
	 * */
	DMA_InitTypeDef DMA_Conf;

	DMA_Conf.DMA_PeripheralBaseAddr	=	(uint32_t)&(SPI1->DR);		//SPI Data register address
	DMA_Conf.DMA_MemoryBaseAddr		=	buffer_address;				//Buffer
	DMA_Conf.DMA_DIR				=	DMA_DIR_PeripheralDST;		//Memory as source
	DMA_Conf.DMA_BufferSize			=	buffer_size;				//Buffer size
	DMA_Conf.DMA_PeripheralInc		=	DMA_PeripheralInc_Disable;	//Disable address inc
	DMA_Conf.DMA_MemoryInc			=	DMA_MemoryInc_Enable;		//Enable buffer address inc
	DMA_Conf.DMA_PeripheralDataSize	= 	DMA_PeripheralDataSize_HalfWord;//SPI buffer is 2 byte
	DMA_Conf.DMA_MemoryDataSize		=	DMA_MemoryDataSize_HalfWord;	//Write 2 byte to buffer
	DMA_Conf.DMA_Mode				=	DMA_Mode_Normal;			//Buffer is used as normal
	DMA_Conf.DMA_Priority			= 	DMA_Priority_Medium;		//Medium priority IDK TODO: Is medium priority OK?
	DMA_Conf.DMA_M2M				=	DMA_M2M_Disable;			//Not using memory to memory

	DMA_Init(DMA1_Channel3, &DMA_Conf);								//SPI is on Channel 3

	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);					//Enable interruption when full buffer is transfered to disable DMA

	/*
	 * Enable NVIC
	 * */
	NVIC_InitTypeDef NVIC_conf;

	//DMA interrupt
	NVIC_conf.NVIC_IRQChannel						=	DMA1_Channel3_IRQn;	//NVIC interrupt vector
	NVIC_conf.NVIC_IRQChannelPreemptionPriority		=	5;					//Priority 0...15
	NVIC_conf.NVIC_IRQChannelSubPriority			=	0;					//Sub Priority not used
	NVIC_conf.NVIC_IRQChannelCmd					=	ENABLE;				//Enable DMA1_Channel3 interrupt
	NVIC_Init(&NVIC_conf);													//Apply to hardware
}

void base_spi_1_send_buffer(uint32_t buffer_size) {

	/*
	 * Set and save buffer size to transfer
	 * */
	DMA1_Channel3->CNDTR 	= buffer_size;
	BUFFER_SIZE 			= buffer_size;
	/*
	 * Start DMA
	 * */
	DMA_Cmd(DMA1_Channel3, ENABLE);
}

void base_spi_1_set_times(uint32_t value)
{
	IRQ_HANDLER_TIMES = value;
}

void base_spi_1_set_irq_callback(void (*spi_callback_p)())
{
	spi_callback = spi_callback_p;
}

void DMA1Channel3_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC3)) {
		DMA_ClearFlag(DMA1_FLAG_TC3);

		/*
		 * Disable DMA
		 * */
		DMA_Cmd(DMA1_Channel3, DISABLE);

		if (!BUFFER_SIZE){

		}else if (spi_callback) {
			spi_callback();
			DMA1_Channel3->CNDTR = BUFFER_SIZE;
			DMA_Cmd(DMA1_Channel3, ENABLE);
		} else if (IRQ_HANDLER_TIMES > 1){
			/*
			 * Rerun DMA if needed
			 * */
			IRQ_HANDLER_TIMES -= 1;
			DMA1_Channel3->CNDTR = BUFFER_SIZE;
			DMA_Cmd(DMA1_Channel3, ENABLE);

		}
	}
}


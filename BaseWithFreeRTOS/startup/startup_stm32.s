/**
  ******************************************************************************
  * @file      startup_stm32.s
  * @author    Ac6
  * @version   V1.0.0
  * @date      12-June-2014
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m3
  .thumb

.global	g_pfnVectors
.global	Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss

.equ  BootRAM,        0xF1E0F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

    .section	.text.Reset_Handler
	.weak	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs	r1, #0
  b	LoopCopyDataInit

CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4

LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
	movs r3, #0
 	str  r3, [r2]
	adds r2, r2, #4

LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss

/* Call the clock system intitialization function.*/
    bl  SystemInit
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
	bl	main

LoopForever:
    b LoopForever

.size	Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section	.text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
	.size	Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex-M.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
 	.section	.isr_vector,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
	.word	_estack
	.word	Reset_Handler
	.word	NMI_Handler
	.word	HardFault_Handler
	.word	MemManage_Handler
	.word	BusFault_Handler
	.word	UsageFault_Handler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	SVC_Handler
	.word	DebugMon_Handler
	.word	0
	.word	PendSV_Handler
	.word	SysTick_Handler
	.word	0//WWDG_IRQHandler	/*Window watchdog interrupt */
	.word	0//PVD_IRQHandler	/*PVD through EXTI Line detection interrupt*/
	.word	0//TAMPER_IRQHandler	/*Tamper interrupt*/
	.word	0//RTC_IRQHandler	/*RTC global interrupt*/
	.word	0//FLASH_IRQHandler	/*Flash global interrupt*/
	.word	0//RCC_IRQHandler	/*RCC global interrupt */
	.word	0//EXTI0_IRQHandler	/*EXTI Line0 interrupt*/
	.word	0//EXTI1_IRQHandler	/*EXTI Line1 interrupt*/
	.word	0//EXTI2_IRQHandler	/*EXTI Line2 interrupt*/
	.word	0//EXTI3_IRQHandler	/*EXTI Line3 interrupt*/
	.word	0//EXTI4_IRQHandler	/*EXTI Line4 interrupt*/
	.word	0//DMA1Channel1_IRQHandler	/*DMA1 Channel1 global interrupt*/
	.word	0//DMA1Channel2_IRQHandler	/*DMA1 Channel2 global interrupt*/
	.word	DMA1Channel3_IRQHandler	/*DMA1 Channel3 global interrupt*/
	.word	DMA1Channel4_IRQHandler	/*DMA1 Channel4 global interrupt*/
	.word	DMA1Channel5_IRQHandler	/*DMA1 Channel5 global interrupt*/
	.word	0//DMA1Channel6_IRQHandler	/*DMA1 Channel6 global interrupt*/
	.word	0//DMA1Channel7_IRQHandler	/*DMA1 Channel7 global interrupt*/
	.word	0//ADC_IRQHandler
	.word	0//USB_HP_CAN_TX_IRQHandler
	.word	0//USB_LP_CAN_RX0_IRQHandler
	.word	0//CAN_RX1_IRQHandler
	.word	0//CAN_SCE_IRQHandler
	.word	0//EXTI9_5_IRQHandler
	.word	0//TIM1_BRK_IRQHandler
	.word	0//TIM1_UP_IRQHandler
	.word	0//TIM1_TRG_COM_IRQHandler
	.word	0//TIM1_CC_IRQHandler
	.word	0//TIM2_IRQHandler
	.word	0//TIM3_IRQHandler
	.word	0//TIM4_IRQHandler
	.word	0//I2C1_EV_IRQHandler
	.word	0//I2C1_ER_IRQHandler
	.word	0//I2C2_EV_IRQHandler
	.word	0//I2C2_ER_IRQHandler
	.word	0//SPI1_IRQHandler
	.word	0//SPI2_IRQHandler
	.word	0//USART1_IRQHandler
	.word	0//USART2_IRQHandler
	.word	0//USART3_IRQHandler
	.word	0//EXTI15_10_IRQHandler
	.word	0//RTCAlarm_IRQHandler
	.word	0//USBWakeUp_IRQHandler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  	.weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

  	.weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

  	.weak	MemManage_Handler
	.thumb_set MemManage_Handler,Default_Handler

  	.weak	BusFault_Handler
	.thumb_set BusFault_Handler,Default_Handler

	.weak	UsageFault_Handler
	.thumb_set UsageFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	DebugMon_Handler
	.thumb_set DebugMon_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler

	.weak	SystemInit

/************************ (C) COPYRIGHT Ac6 *****END OF FILE****/

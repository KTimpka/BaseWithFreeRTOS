/*
 * base_usb.h
 *
 *  Created on: 27. mai 2018
 *      Author: Sad
 */

#ifndef INC_BASE_USB_H_
#define INC_BASE_USB_H_



/*
 * Includes
 * */
#include "stm32f10x.h"

/*
 * Defines, macros
 * */
/**
  * @brief USB
  */

typedef struct
{
  __IO uint32_t CNTR;
  __IO uint32_t ISTR;
  uint16_t  RESERVED1;
  __IO uint16_t SR;
  uint16_t  RESERVED2;
  __IO uint16_t DR;
  uint16_t  RESERVED3;
  __IO uint16_t CRCPR;
  uint16_t  RESERVED4;
  __IO uint16_t RXCRCR;
  uint16_t  RESERVED5;
  __IO uint16_t TXCRCR;
  uint16_t  RESERVED6;
  __IO uint16_t I2SCFGR;
  uint16_t  RESERVED7;
  __IO uint16_t I2SPR;
  uint16_t  RESERVED8;
} usb_TypeDef;

/*
 * GLOBAL
 * */

/*
 * Functions
 * */

/*
 * NVIC functions
 * */

/*
 * Threads
 * */

#endif /* INC_BASE_USB_H_ */

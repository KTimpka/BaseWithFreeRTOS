/*
 * base_io.c
 *
 *  Created on: 12. okt 2018
 *      Author: Toftan2-elekter
 */


/*
 * Function to avoid unnecessary lines when configuring pins
 * */
void base_gpio_init(GPIO_TypeDef * 		GPIOx, //Port A,B,C,D,E...
					uint16_t			GPIO_Pin, //Pin number 0 - 15
					GPIOSpeed_TypeDef 	GPIO_Speed, //Speed 2,10,50MHz
					GPIOMode_TypeDef 	GPIO_Mode, //Mode Ai, Di, Do, ...
					)
{

}

/*
 * test.c
 *
 *  Created on: 6. mai 2018
 *      Author: Sad
 */

#include "test.h"

#include "FreeRTOS.h"
#include "task.h"

static void test_thread(void *pvParameters);

void test_thread_init()
{
	xTaskCreate( test_thread, "test_thread", configMINIMAL_STACK_SIZE, 0, 0, NULL );
}

static void test_thread(void *pvParameters)
{

}

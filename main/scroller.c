/*
 * scroller.c
 *
 *  Created on: Mar 18, 2020
 *      Author: Bakeromso
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "user_defines.h"
#include "scroller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include <stdbool.h>
#include <math.h>


void scrollTick(bool isLeft){
	/*
	 *  configTICK_RATE_HZ = 100
	 *  portTICK_PERIOD_MS = 1000 / configTICK_RATE_HZ = 10
	 *  period single pulse 6 ms
	 *  phase shift 3 ms
	 */
	gpio_num_t pin1;
	gpio_num_t pin2;
	if(isLeft){
		pin1 = GPIO_OUTPUT_IO_D0;
		pin2 = GPIO_OUTPUT_IO_D3;
	}
	else{
		pin1 = GPIO_OUTPUT_IO_D3;
		pin2 = GPIO_OUTPUT_IO_D0;
	}
	static uint32_t level = 0; // remember level as we're only toggling
	TickType_t xLastWakeTime = xTaskGetTickCount ();
	const TickType_t xFrequency = 1;
	const TickType_t xPhaseShift = 1;
	// First set GPIO OUTPUT 0 to high
	gpio_set_level(pin1, level);
	// Wait for the phase shift
	vTaskDelayUntil( &xLastWakeTime, xPhaseShift);
	// Set the other GPIO, OUTPUT 1 to high
	gpio_set_level(pin2, level);
	// Wait for half the cycle, minus the phase shift that was already waited for
	vTaskDelayUntil( &xLastWakeTime, xFrequency);
	level ^= 1; // toggle between 1 and 0;
}

void scrollTickLeft(void){
	scrollTick(true);
}

void scrollTickRight(void){
	scrollTick(false);
}

void scrollFullRight(void){
	for(int i = 0; i < 80; i ++){
		scrollTickRight();
	}
}

void scrollFullLeft(void){
	for(int i = 0; i < 80; i ++){
		scrollTickLeft();
	}
}

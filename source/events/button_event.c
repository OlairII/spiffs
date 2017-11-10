/*
 * button_event.c
 *
 *  Created on: Oct 20, 2017
 *      Author: houz03
 */

#include "esp_common.h"

#include "dev_config.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "tasks/blink.h"

void ISR_Button(void * arg) {

	os_delay_us(10000);

	GPIO_Pin intStatus = GPIO_IntGetStatus();

	if (intStatus & GPIO_PIN_13) { //verifica se a int foi gerada pelo pushbutton
		if (GPIO_GetPin(GPIO_PIN_13) == 0) {
			GPIO_SetPinToggle(GPIO_PIN_2);

		}
	}
	GPIO_IntClear();
}


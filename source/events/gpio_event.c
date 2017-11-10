/*
 * int_gpio.c
 *
 *  Created on: 6 de ago de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"

#include "dev_config.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "tasks/measure.h"

void ISR_Gpio(void *arg) {

  uint32_t debounce;

  GPIO_Pin intStatus = GPIO_IntGetStatus();

  if (intStatus & PIN_WATER_DROP)
    incDropCounter();

  GPIO_IntClear();
}

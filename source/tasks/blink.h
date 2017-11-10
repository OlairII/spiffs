#ifndef SOURCE_TASKS_BLINK_H_
#define SOURCE_TASKS_BLINK_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"


// funções

void blink_test();
void blink_config(GPIO_Pin PIN);
void taskBlink(void *args);

#ifdef __cplusplus
}
#endif

#endif

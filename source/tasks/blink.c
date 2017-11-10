#include "esp_common.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#include "FreeRTOS.h"
#include "task.h"

#include "tasks/blink.h"
extern void ISR_Button(void * arg);

void blink_test() {

	printf("\n\n");
	printf("TEST BLINK");
	printf("\n");
}

void blink_config(GPIO_Pin PIN) {

	GPIO_Init(GPIO_PIN_2);
	GPIO_SetPinType(GPIO_PIN_2, GPIO_TYPE_OUTPUT);
	GPIO_Init(GPIO_PIN_13);
	GPIO_SetPinType(GPIO_PIN_13, GPIO_TYPE_INPUT);

}

void taskBlink(void *args) {

	blink_test();
	blink_config(GPIO_PIN_2);
	GPIO_SetPinLow(GPIO_PIN_2);



	GPIO_IntRegisterIsr(ISR_Button, NULL);

	GPIO_IntSetType(GPIO_PIN_13, GPIO_INT_FALLING_EDGE);

	GPIO_IntEnable();


	while (1) {

		/*if(GPIO_GetPin(GPIO_PIN_13)){
		 GPIO_SetPinHigh(GPIO_PIN_2);

		 }
		 else GPIO_SetPinLow(GPIO_PIN_2);
		 //vTaskDelay(pdMS_TO_TICKS(10000));
		 */


//		GPIO_SetPinToggle(GPIO_PIN_5);
		vTaskDelay(pdMS_TO_TICKS(100));		//asm(NOP)


	}

}

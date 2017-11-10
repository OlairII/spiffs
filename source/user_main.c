#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"

#include "dev_config.h"
#include "driver/uart.h"
#include "tasks/startup.h"
#include "tasks/blink.h"
#include "tasks/getwforecast.h"

void printHeader() {
	printf("\n\n\n\n");
	printf(
			"--------------------------------------------------------------------------------\n");
	printf("Koppen\n");
	printf("Product: %s - %d\n", PRODUCT_NAME, system_get_chip_id());
	printf("Hardware Version: %s\n", HARDWARE_VERSION);
	printf("Firmware Version: %s\n", FIRMWARE_VERSION);
	printf("SDK Version: %s\n", system_get_sdk_version());
	printf("Free heap size: %d\n", system_get_free_heap_size());
	printf("System reset cause: %d\n", system_get_rst_info()->reason);
	printf(
			"--------------------------------------------------------------------------------\n");
}

void user_init(void) {

	UART_Init0();

//  printHeader();

	xTaskCreate(taskStartup, "Main", 1000, NULL, 4, NULL);
//	xTaskCreate(taskBlink, "Main", 500, (void *) 2, 3, NULL);

}

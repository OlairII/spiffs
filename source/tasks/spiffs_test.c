/*
 * spiffs_test.c
 *
 *  Created on: Nov 23, 2017
 *      Author: houz03
 */

#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cJSON.h"
#include "lwip/apps/sntp.h"
#include "lwip/apps/sntp_time.h"
#include "lwip/api.h"
#include "lwip/netbuf.h"
#include "utils/spiffs/esp_spiffs.h"
#include "utils/spiffs/spiffs.h"
#include "fcntl.h"
#include "unistd.h"

#include "dev_config.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "tasks/net_status.h"

#define FS1_FLASH_SIZE	(128*1024)
#define FS1_FLASH_ADDR	(1024*1024)
#define SECTOR_SIZE		(4*1024)
#define LOG_BLOCK		(SECTOR_SIZE)
#define LOG_PAGE		(128)
#define FD_BUF_SIZE   	(32*4)
#define CACHE_BUF_SIZE 	(LOG_PAGE + 32)*8

#define LOG_PAGE_SIZE       256

void taskSpiffsTest(void *args) {

	/*
	static spiffs fs;

	static u8_t spiffs_work_buf[LOG_PAGE_SIZE * 2];
	static u8_t spiffs_fds[32 * 4];
	static u8_t spiffs_cache_buf[(LOG_PAGE_SIZE + 32) * 4];

	struct esp_spiffs_config config;
	config.phys_size = FS1_FLASH_SIZE;
	config.phys_addr = FS1_FLASH_ADDR;
	config.phys_erase_block = SECTOR_SIZE;
	config.log_block_size = LOG_BLOCK;
	config.log_page_size = LOG_PAGE;
	config.fd_buf_size = FD_BUF_SIZE * 2;
	config.cache_buf_size = CACHE_BUF_SIZE;
	esp_spiffs_init(&config);

	printf("1\n");

//	cfg.hal_read_f = esp_spiffs_read();
//	cfg.hal_write_f = esp_spiffs_write();
//	cfg.hal_erase_f = esp_spiffs_erase();

	int res = SPIFFS_mount(&fs, &config, spiffs_work_buf, spiffs_fds,
			sizeof(spiffs_fds), spiffs_cache_buf, sizeof(spiffs_cache_buf), 0);
	printf("2\n");
	printf("mount res: %i\n", res);

	char buf[12];

	// Surely, I've mounted spiffs before entering here

	spiffs_file fd = SPIFFS_open(&fs, "my_file",
	SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
	if (SPIFFS_write(&fs, fd, (u8_t *) "Hello world", 12) < 0)
		printf("errno %i\n", SPIFFS_errno(&fs));
	SPIFFS_close(&fs, fd);

	fd = SPIFFS_open(&fs, "my_file", SPIFFS_RDWR, 0);
	if (SPIFFS_read(&fs, fd, (u8_t *) buf, 12) < 0)
		printf("errno %i\n", SPIFFS_errno(&fs));
	SPIFFS_close(&fs, fd);

	printf("--> %s <--\n", buf);*/


	struct esp_spiffs_config config;
	config.phys_size = FS1_FLASH_SIZE;
	config.phys_addr = FS1_FLASH_ADDR;
	config.phys_erase_block = SECTOR_SIZE;
	config.log_block_size = LOG_BLOCK;
	config.log_page_size = LOG_PAGE;
	config.fd_buf_size = FD_BUF_SIZE * 2;
	config.cache_buf_size = CACHE_BUF_SIZE;
	esp_spiffs_init(&config);

	char *buf = "hello	world";

	char out[20] = { 0 };

	int pfd = open("myfile", O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if (pfd <= 3) {

		printf("open	file	error	\n");

	}

	int write_byte = write(pfd, buf, strlen(buf));

	if (write_byte <= 0) {

		printf("write	file	error	\n");

	}

	close(pfd);

	open("myfile", O_RDWR);
	if(read(pfd, out, 20) < 0 ){
		printf("read error\n");
	}
	close(pfd);
	printf("%s\n", out);

	while (1) {

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

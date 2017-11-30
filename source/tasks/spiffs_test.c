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
#include "utils/rest/rest.h"
#include "utils/base64/base64.h"
#include "math.h"

#define FS1_FLASH_SIZE	(128*1024)
#define FS1_FLASH_ADDR	(1024*1024)
#define SECTOR_SIZE		(4*1024)
#define LOG_BLOCK		(SECTOR_SIZE)
#define LOG_PAGE		(128)
#define FD_BUF_SIZE   	(32*4)
#define CACHE_BUF_SIZE 	(LOG_PAGE + 32)*8

#define LOG_PAGE_SIZE       256

void taskSpiffsTest(void *args) {

	struct esp_spiffs_config config;
	config.phys_size = FS1_FLASH_SIZE;
	config.phys_addr = FS1_FLASH_ADDR;
	config.phys_erase_block = SECTOR_SIZE;
	config.log_block_size = LOG_BLOCK;
	config.log_page_size = LOG_PAGE;
	config.fd_buf_size = FD_BUF_SIZE * 2;
	config.cache_buf_size = CACHE_BUF_SIZE;
	esp_spiffs_init(&config);

	/*
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
	 */

	printf("\n\nentrou na taskRestComm\n\n");
	Request req;
	req.host = "api.apixu.com";
	req.port = 80;
	req.lport = 8000;
	req.url =
			"http://api.apixu.com/v1/current.json?key=46712e1bd82748309f9183131172710&q=Londrina";
	req.body = NULL;
	req.cacheCtrl = NO_CACHE;
	req.contLength = 0;
	req.contType = CONTENT_JSON;
	req.restType = GET;
	req.authType = AUTH_NONE;

	Response resp;

	while (1) {

		printf("entrando na func rest\n");
		if (rest(&req, &resp) == REST_OK) {
			printf("saiu da rest\n");
			printf("%s\n", resp.body);
			cJSON *response = cJSON_Parse(resp.body);
			char *wfdata = cJSON_Print(response);
			printf("1\n%s\n", wfdata);

			char *buf = wfdata;
			size_t datalen = strlen(wfdata);
			char *out = malloc(datalen+1);

			int pfd = open("myfile", O_TRUNC | O_CREAT | O_RDWR,
			S_IRUSR | S_IWUSR);

			if (pfd <= 3) {

				printf("open	file	error	\n");

			}

			int write_byte = write(pfd, buf, strlen(buf));

			if (write_byte <= 0) {

				printf("write	file	error	\n");

			}

			close(pfd);

			pfd = open("myfile", O_RDWR);

			if (read(pfd, out, datalen) < 0) {
				printf("read error\n");
			}
			close(pfd);
			out[datalen] = '\0';
			printf("2\n%s\n", out);
			free(out);

		}

		vTaskDelay(pdMS_TO_TICKS(10000));
	}
}
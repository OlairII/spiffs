/*
 * net_status.h
 *
 *  Created on: 20 de ago de 2017
 *      Author: dkrepsky
 */

#ifndef SOURCE_TASKS_NET_STATUS_H_
#define SOURCE_TASKS_NET_STATUS_H_

#include <stdbool.h>

bool getIsLanConnected();

bool getIsWebConnected();

bool getIsServerConnected();

void netStatusUpdate();

void netStatusUpdate_ISR();

void taskNetStatus(void *args);

#endif /* SOURCE_TASKS_NET_STATUS_H_ */

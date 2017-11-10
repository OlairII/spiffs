/*
 * local_udp.h
 *
 *  Created on: 28 de ago de 2017
 *      Author: dkrepsky
 */

#ifndef SOURCE_TASKS_LOCAL_UDP_H_
#define SOURCE_TASKS_LOCAL_UDP_H_

#define LOCAL_REQ_HEADER        0xA5
#define LOCAL_RES_HEADER        0xAA

#define LOCAL_REQ_KEEP_ALIVE    0x01
#define LOCAL_RES_KEEP_ALIVE    0x02

void taskLocalUdp(void *args);

#endif /* SOURCE_TASKS_LOCAL_UDP_H_ */

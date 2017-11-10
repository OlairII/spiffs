/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \file ping.h
 *
 * \brief ICMP ping.
 *
 * \author David Krepsky
 * \version 1.0.1
 * \date 07/2017
 *
 * \bug None known.
 */

#ifndef _UTILS_PING_H_
#define _UTILS_PING_H_

#include "lwip/inet.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Error creating the socket.
#define PING_ERROR_SOCKET -1

/// Error sending ping.
#define PING_ERROR_SEND   -2

/// Error receiving ping (timeout).
#define PING_ERROR_RECV   -3

/*!
 * ICMP Ping implementation.
 *
 * # Requires
 *
 * Enable the receiver buffer in "SDK_PATH"/include/lwip/lwipopts.h by adding the following line:
 * #define DEFAULT_RAW_RECVMBOX_SIZE 6
 *
 * After that, rebuild lwip lib:
 *
 * rm $SDK_PATH/lib/liblwip.a
 * cd $SDK_PATH/third_party
 * ./make_lib.sh lwip
 *
 * # Usage
 *
 * ```
 *    ip_addr_t ip;
 *    IP4_ADDR(&ip, 8, 8, 8, 8);
 *    int32_t delay = ping(&ip);
 *
 *    if(delay >= 0)
 *      printf("Success [%d]\n", delay);
 *    else
 *      printf("Timeout\n");
 * ```
 * @param addr Echo destination address.
 *
 * @return If success, returns the delay time in ms. On error returns PING_ERROR_{cause}.
 */
int32_t ping(ip_addr_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* _UTILS_PING_H_ */

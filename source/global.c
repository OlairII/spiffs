/*
 * global.c
 *
 *  Created on: 18 de ago de 2017
 *      Author: dkrepsky
 */

#include <stdint.h>
#include <stdbool.h>
#include "esp_common.h"

#include "global.h"

volatile bool g_isTimeSync = false;
volatile bool g_isTimeUpdated = false;


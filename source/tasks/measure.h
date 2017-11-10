/*
 * measure.h
 *
 *  Created on: 27 de ago de 2017
 *      Author: dkrepsky
 */

#ifndef SOURCE_TASKS_MEASURE_H_
#define SOURCE_TASKS_MEASURE_H_

#define SAMPLE_PERIOD_SEC  9

void incDropCounter();

char *lastMeasureJSON();

void taskMeasure(void *args);

#endif /* SOURCE_TASKS_MEASURE_H_ */

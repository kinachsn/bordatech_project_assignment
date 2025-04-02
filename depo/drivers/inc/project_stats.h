/*
 * project_stats.h
 *
 *  Created on: Apr 1, 2025
 *      Author: hasan
 */

#ifndef INC_PROJECT_STATS_H_
#define INC_PROJECT_STATS_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

float calculate_median(float arr[], uint8_t size);
float find_min(float arr[], uint8_t size);
float find_max(float arr[], uint8_t size);
float calculate_sample_std(float arr[], uint8_t size);
void calculate_statistics(float arr[], uint8_t size, float arr_calculated[4]);

#endif /* INC_PROJECT_STATS_H_ */

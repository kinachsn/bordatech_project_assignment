/*
 * project_stats.c
 *
 *  Created on: Apr 1, 2025
 *      Author: hasan
 */

#include "project_stats.h"

/**
 * @brief  Calculates the median value of an array of floats
 * @param  arr: Array of float values
 * @param  size: Size of the array
 * @return The median value of the array
 */
float calculate_median(float arr[], uint8_t size) {
    float sorted[size];
    memcpy(sorted, arr, sizeof(float) * size);

    // Bubble Sort (Sort array in ascending order)
    for (uint8_t i = 0; i < size - 1; i++) {
        for (uint8_t j = 0; j < size - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                float temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }
    return sorted[size / 2];
}

/**
 * @brief  Finds the minimum value in an array of floats
 * @param  arr: Array of float values
 * @param  size: Size of the array
 * @return The minimum value in the array
 */
float find_min(float arr[], uint8_t size) {
    float min_val = arr[0];
    for (uint8_t i = 1; i < size; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    return min_val;
}

/**
 * @brief  Finds the maximum value in an array of floats
 * @param  arr: Array of float values
 * @param  size: Size of the array
 * @return The maximum value in the array
 */
float find_max(float arr[], uint8_t size) {
    float max_val = arr[0];
    for (uint8_t i = 1; i < size; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}

/**
 * @brief Calculates the sample standard deviation of an array of float values.
 *
 * This function calculates the standard deviation of a sample of data, applying the
 * Bessel correction (dividing by n-1 instead of n) to adjust for the bias in estimating
 * the population standard deviation from a sample. The formula used is:
 *
 *    std_dev = sqrt( (1 / (n - 1)) * sum((x_i - mean)^2) )
 *
 * @param arr An array of float values representing the data sample.
 * @param size The number of elements in the array (the sample size).
 *
 * @return The standard deviation of the sample data.
 */
float calculate_sample_std(float arr[], uint8_t size) {
    float sum = 0, mean, std_dev = 0;

    // Calculate mean
    for (uint8_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    mean = sum / size;

    // Calculate sample standard deviation (n-1 denominator)
    for (uint8_t i = 0; i < size; i++) {
        std_dev += pow(arr[i] - mean, 2);
    }
    return sqrt(std_dev / (size - 1));  // Bessel correction (n-1)
}

/**
 * @brief Computes statistical values (min, max, median, standard deviation) from an array.
 * @param arr: Input array of float values
 * @param size: Size of the input array
 * @param arr_calculated: Output array where results will be stored
 *
 * The calculated values are stored in arr_calculated as follows:
 *   arr_calculated[0] -> Minimum value
 *   arr_calculated[1] -> Maximum value
 *   arr_calculated[2] -> Median value
 *   arr_calculated[3] -> Sample standard deviation
 */
void calculate_statistics(float arr[], uint8_t size, float arr_calculated[4]) {
    arr_calculated[0] = find_min(arr, size);
    arr_calculated[1] = find_max(arr, size);
    arr_calculated[2] = calculate_median(arr, size);
    arr_calculated[3] = calculate_sample_std(arr, size);
}


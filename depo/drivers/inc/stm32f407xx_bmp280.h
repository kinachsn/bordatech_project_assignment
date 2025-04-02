/*
 * stm32f407xx_bmp280.h
 *
 *  Created on: Mar 31, 2025
 *      Author: hasan
 */

#ifndef INC_STM32F407XX_BMP280_H_
#define INC_STM32F407XX_BMP280_H_

#include "stm32f407xx.h"

/*
 * Typedefs for signed and unsigned 32-bit values
 * Used for sensor calculations and compensation
 */
typedef int32_t  BMP280_S32_t;
typedef uint32_t BMP280_U32_t;

//------------------------------------------

/*
 * BMP280 I2C Address and Register Definitions
 */
#define BMP280_ADDR 	0x77 	// Default I2C address (SDO -> GND)
#define BMP280_CHIP_ID 	0xD0 	// Chip ID register
#define BMP280_RESET	0xE0 	// Reset register

/*
 * Status Register and Flags
 */
#define BMP280_STATUS	0x73 	// Status register
#define STATUS_CONV		0x08 	// Measurement in progress
#define STATUS_COPY		0x01 	// NVM data copying in progress

//---------------------------------------------------

/*
 * Control Measurement Register (CTRL_MEAS)
 * Defines oversampling settings and operating modes
 */
#define CTRL_MEAS			0xF4

/*
 * Oversampling settings for pressure and temperature measurements
 */
#define OVERSAMPLING_SKIPPED	0x00
#define OVERSAMPLING_16BIT 		0x01
#define OVERSAMPLING_17BIT 		0x02
#define OVERSAMPLING_18BIT 		0x03
#define OVERSAMPLING_19BIT 		0x04
#define OVERSAMPLING_20BIT 		0x05

/*
 * Bit offsets for configuring CTRL_MEAS register
 */
#define CTRL_M_T_OFFSET	  0x05 	// Temperature measurement offset
#define CTRL_M_P_OFFSET	  0x02 	// Pressure measurement offset
#define CTRL_M_M_OFFSET   0x00  // Mode selection offset

/*
 * Temperature Oversampling Macros
 */
#define OVERSAMPLING_T_SKIPPED  (OVERSAMPLING_SKIPPED << CTRL_M_T_OFFSET)
#define OVERSAMPLING_T_16BIT	(OVERSAMPLING_16BIT << CTRL_M_T_OFFSET)
#define OVERSAMPLING_T_17BIT	(OVERSAMPLING_17BIT << CTRL_M_T_OFFSET)
#define OVERSAMPLING_T_18BIT	(OVERSAMPLING_18BIT << CTRL_M_T_OFFSET)
#define OVERSAMPLING_T_19BIT	(OVERSAMPLING_19BIT << CTRL_M_T_OFFSET)
#define OVERSAMPLING_T_20BIT	(OVERSAMPLING_20BIT << CTRL_M_T_OFFSET)

/*
 * Pressure Oversampling Macros
 */
#define OVERSAMPLING_P_SKIPPED  (OVERSAMPLING_SKIPPED << CTRL_M_P_OFFSET)
#define OVERSAMPLING_P_16BIT	(OVERSAMPLING_16BIT << CTRL_M_P_OFFSET)
#define OVERSAMPLING_P_17BIT	(OVERSAMPLING_17BIT << CTRL_M_P_OFFSET)
#define OVERSAMPLING_P_18BIT	(OVERSAMPLING_18BIT << CTRL_M_P_OFFSET)
#define OVERSAMPLING_P_19BIT	(OVERSAMPLING_19BIT << CTRL_M_P_OFFSET)
#define OVERSAMPLING_P_20BIT	(OVERSAMPLING_20BIT << CTRL_M_P_OFFSET)

/*
 * Operating Modes
 */
#define SLEEP	0x00 	// Low power mode, no measurements
#define FORCE	0x01 	// Perform a single measurement, then return to sleep mode
#define NORMAL  0x03 	// Continuous measurements with periodic updates

//-----------------------------------------------------------

/*
 * Configuration Register (CONFIG)
 * Used to configure standby time, filter settings, and SPI mode
 */
#define CONFIG 0xF5

/*
 * Standby time settings (T_standby) in Normal mode
 */
#define T_SB_OFFSET		0x05

#define T_SB_0_5		(0x00 << T_SB_OFFSET) // 0.5 ms
#define T_SB_62_5		(0x01 << T_SB_OFFSET) // 62.5 ms
#define T_SB_125		(0x02 << T_SB_OFFSET) // 125 ms
#define T_SB_250		(0x03 << T_SB_OFFSET) // 250 ms
#define T_SB_500		(0x04 << T_SB_OFFSET) // 500 ms
#define T_SB_1000		(0x05 << T_SB_OFFSET) // 1000 ms
#define T_SB_2000		(0x06 << T_SB_OFFSET) // 2000 ms
#define T_SB_4000		(0x07 << T_SB_OFFSET) // 4000 ms

/*
 * IIR Filter Coefficients
 * Used to smooth measurement noise
 */
#define FILTER_OFFSET	0x02

#define FILTER_COEFF_2		(0x02 << FILTER_OFFSET)
#define FILTER_COEFF_5		(0x04 << FILTER_OFFSET)
#define FILTER_COEFF_11		(0x08 << FILTER_OFFSET)
#define FILTER_COEFF_22		(0x10 << FILTER_OFFSET)

//-------------------------------------------------------------

/*
 * Data Registers for Raw Sensor Measurements
 */
#define START_RAW_DATA_POINT	0xF7 // Start address for pressure & temperature data
#define RAW_DATA_LENGTH			6 	// Length of raw data (3 bytes pressure, 3 bytes temperature)

//-------------------------------------------------------------

/*
 * Calibration Data Storage Address
 * Calibration coefficients stored in BMP280's internal memory
 */
#define CALIB_DATA_ADDR		0x88

/*
 * Raw Data Register Addresses
 */
#define Press_register			0xF7 // Pressure data register
#define Temperature_register	0xFA // Temperature data register

//--------------------------------------------------------------

/*
 * Global Variables for Calibration Data
 * These variables store factory calibration coefficients for pressure and temperature calculations
 */
extern uint16_t dig_T1, dig_P1;
extern int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

//--------------------------------------------------------------
extern BMP280_S32_t t_fine; // Fine temperature value used in pressure calculations

/*
 * Function Prototypes
 */

/**
 * @brief  Initializes the BMP280 sensor
 * @param  pI2CHandle: Pointer to the I2C handle structure
 */
void BMP280_Init(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Reads the calibration data from the BMP280's ROM
 * @param  pI2CHandle: Pointer to the I2C handle structure
 */
void BMP280_ReadCalibrationData(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Reads the raw pressure data from BMP280
 * @param  pI2CHandle: Pointer to the I2C handle structure
 * @return Raw pressure value
 */
uint32_t BMP280_ReadPressureRaw(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Reads the raw temperature data from BMP280
 * @param  pI2CHandle: Pointer to the I2C handle structure
 * @return Raw temperature value
 */
uint32_t BMP280_ReadTemperatureRaw(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Compensates temperature using factory calibration data
 * @param  adc_T: Raw temperature value
 * @return Compensated temperature in degrees Celsius
 */
double bmp280_compensate_T_double(BMP280_S32_t adc_T);

/**
 * @brief  Compensates pressure using factory calibration data
 * @param  adc_P: Raw pressure value
 * @return Compensated pressure in Pascals
 */
double bmp280_compensate_P_double(BMP280_S32_t adc_P);

/**
 * @brief  Reads and processes the pressure value from BMP280
 */
void BMP280_ReadPressure();

#endif /* INC_STM32F407XX_BMP280_H_ */

/*
 * stm32f407xx_bmp280.c
 *
 *  Created on: Mar 31, 2025
 *      Author: hasan
 */

#include "stm32f407xx_bmp280.h"

// Calibration coefficients for temperature and pressure calculations
uint16_t dig_T1=27504;
int16_t dig_T2=26435, dig_T3=-1000;
uint16_t dig_P1=36477;
int16_t dig_P2=-10685, dig_P3=3024, dig_P4=2855, dig_P5=150, dig_P6=-7, dig_P7=15500, dig_P8=-14600, dig_P9=6000;
int32_t t_fine;

/**
 * @brief  Initializes the BMP280 sensor by configuring control and configuration registers.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @retval None
 */
void BMP280_Init(I2C_Handle_t *pI2CHandle) {
    uint8_t config[2];

    // Set control register (0xF4) -> Normal mode, 16x oversampling
    config[0] = CTRL_MEAS;
    config[1] = 0x5F;

    I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);
    I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
    I2C_MasterSendData(pI2CHandle, config, 2, BMP280_ADDR, I2C_DISABLE_SR);

    // Set configuration register (0xF5) -> Standard filtering and standby time
    config[0] = CONFIG;
    config[1] = 0x90;
    I2C_MasterSendData(pI2CHandle, config, 2, BMP280_ADDR, I2C_DISABLE_SR);
}

/**
 * @brief  Reads calibration data from BMP280's non-volatile memory.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @retval None
 */
void BMP280_ReadCalibrationData(I2C_Handle_t *pI2CHandle) {
    uint8_t calib_data[24];
    uint8_t reg_addr = CALIB_DATA_ADDR;

    I2C_PeripheralControl(I2C1, ENABLE);
    I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);
    I2C_MasterSendData(pI2CHandle, &reg_addr, 1, BMP280_ADDR, I2C_DISABLE_SR);
    I2C_MasterReceiveData(pI2CHandle, calib_data, 24, BMP280_ADDR, I2C_DISABLE_SR);

    // Parse calibration data
    dig_T1 = (calib_data[0]  << 8) | calib_data[1];
    dig_T2 = (calib_data[2]  << 8) | calib_data[3];
    dig_T3 = (calib_data[4]  << 8) | calib_data[5];

    dig_P1 = (calib_data[6]  << 8) | calib_data[7];
    dig_P2 = (calib_data[8]  << 8) | calib_data[9];
    dig_P3 = (calib_data[10] << 8) | calib_data[11];
    dig_P4 = (calib_data[12] << 8) | calib_data[13];
    dig_P5 = (calib_data[14] << 8) | calib_data[15];
    dig_P6 = (calib_data[16] << 8) | calib_data[17];
    dig_P7 = (calib_data[18] << 8) | calib_data[19];
    dig_P8 = (calib_data[20] << 8) | calib_data[21];
    dig_P9 = (calib_data[22] << 8) | calib_data[23];
}

/**
 * @brief  Reads raw pressure data from BMP280.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @retval Raw pressure value (20-bit ADC reading)
 */
uint32_t BMP280_ReadPressureRaw(I2C_Handle_t *pI2CHandle) {
    uint8_t pressure_raw[3];
    uint8_t reg_addr = Press_register;

    I2C_PeripheralControl(I2C1, ENABLE);
    I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);
    I2C_MasterSendData(pI2CHandle, &reg_addr, 1, BMP280_ADDR, I2C_DISABLE_SR);
    I2C_MasterReceiveData(pI2CHandle, pressure_raw, 3, BMP280_ADDR, I2C_DISABLE_SR);

    // Combine bytes into a 20-bit ADC value
    return ((uint32_t)pressure_raw[0] << 12) | ((uint32_t)pressure_raw[1] << 4) | (pressure_raw[2] >> 4);
}

/**
 * @brief  Reads raw temperature data from BMP280.
 * @param  pI2CHandle: Pointer to the I2C handle structure.
 * @retval Raw temperature value (20-bit ADC reading)
 */
uint32_t BMP280_ReadTemperatureRaw(I2C_Handle_t *pI2CHandle) {
    uint8_t temp_raw[3];
    uint8_t reg_addr = Temperature_register;

    I2C_PeripheralControl(I2C1, ENABLE);
    I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);
    I2C_MasterSendData(pI2CHandle, &reg_addr, 1, BMP280_ADDR, I2C_DISABLE_SR);
    I2C_MasterReceiveData(pI2CHandle, temp_raw, 3, BMP280_ADDR, I2C_DISABLE_SR);

    // Combine bytes into a 20-bit ADC value
    return ((uint32_t)temp_raw[0] << 12) | ((uint32_t)temp_raw[1] << 4) | (temp_raw[2] >> 4);
}

/**
 * @brief  Compensates raw temperature reading using BMP280 calibration coefficients.
 * @param  adc_T: Raw temperature ADC value
 * @retval Compensated temperature in degrees Celsius
 */
double bmp280_compensate_T_double(BMP280_S32_t adc_T) {
    double var1, var2, T;
    var1  = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
    var2  = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) *
            (((double)adc_T)/131072.0 - ((double) dig_T1)/8192.0)) * ((double)dig_T3);
    t_fine = (BMP280_S32_t)(var1 + var2);
    T  = (var1 + var2) / 5120.0;
    return T;
}

/**
 * @brief  Compensates raw pressure reading using BMP280 calibration coefficients.
 * @param  adc_P: Raw pressure ADC value
 * @retval Compensated pressure in Pascal (Pa)
 */
double bmp280_compensate_P_double(BMP280_S32_t adc_P) {
    double var1, var2, p;
    var1 = ((double)t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
    if (var1 == 0.0) return 0;
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)dig_P8) / 32768.0;
    return p + (var1 + var2 + ((double)dig_P7)) / 16.0;
}

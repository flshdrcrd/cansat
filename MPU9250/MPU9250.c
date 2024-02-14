/*
 * MPU9250.c
 *
 *  Created on: Feb 11, 2024
 *      Author: berci
 */

#include "MPU9250.h"

void MPU9250_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t config_data[] = {0b00000100, 0b00011000, 0b00011000, 0b00000100};
    HAL_I2C_Mem_Write(hi2c, MPU9250_I2C_ADDR, MPU9250_REG_CONFIG, I2C_MEMADD_SIZE_8BIT, config_data, 4, HAL_MAX_DELAY);
}

void MPU9250_Read(I2C_HandleTypeDef *hi2c, MPU9250 *data) {
    uint8_t sensor_data_raw[14];
    HAL_I2C_Mem_Read(hi2c, MPU9250_I2C_ADDR, MPU9250_REG_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, sensor_data_raw, 14, HAL_MAX_DELAY);

    // Accelerometer data
    for (int i = 0; i < 3; ++i) {
        int16_t raw_value = (sensor_data_raw[i*2] << 8) | sensor_data_raw[i*2 + 1];
        data->accel[i] = (float)raw_value / 32768.0f * 16.0f;
    }

    // Gyroscope data
    for (int i = 0; i < 3; ++i) {
        int16_t raw_value = (sensor_data_raw[i*2 + 8] << 8) | sensor_data_raw[i*2 + 9];
        data->gyro_dps[i] = (float)raw_value / 32768.0f * 2000.0f;
    }

    // Temperature data
    int16_t temp_raw = (sensor_data_raw[6] << 8) | sensor_data_raw[7];
    data->temperature = ((float)temp_raw / 333.87f) + 21.0;
}

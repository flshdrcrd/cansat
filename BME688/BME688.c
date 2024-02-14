/*
 * BME688.c
 *
 *  Created on: Jan 23, 2024
 *      Author: berci
 */

#include "BME688.h"


void BME688_Init(I2C_HandleTypeDef *hi2c) {
	uint8_t write_data;
	write_data = BME688_SET_OSRS_H;
	HAL_I2C_Mem_Write(hi2c, BME688_I2C_ADDR, BME688_REG_CTRL_HUM, I2C_MEMADD_SIZE_8BIT, &write_data, 1, HAL_MAX_DELAY);
}

void BME688_Read(I2C_HandleTypeDef *hi2c, BME688 *data) {
	uint8_t write_data;
	write_data = (BME688_SET_OSRS_T << 5) | (BME688_SET_OSRS_P << 2) | BME688_SET_MODE;
	HAL_I2C_Mem_Write(hi2c, BME688_I2C_ADDR, BME688_REG_CTRL_MEAS, I2C_MEMADD_SIZE_8BIT, &write_data, 1, HAL_MAX_DELAY);

	uint16_t par_t1;
	int16_t par_t2;
	int8_t par_t3;

	uint16_t par_p1;
	int16_t par_p2;
	int8_t par_p3;
	int16_t par_p4;
	int16_t par_p5;
	int8_t par_p6;
	int8_t par_p7;
	int16_t par_p8;
	int16_t par_p9;
	uint8_t par_p10;

	uint16_t par_h1;
	uint16_t par_h2;
	int8_t par_h3;
	int8_t par_h4;
	int8_t par_h5;
	uint8_t par_h6;
	int8_t par_h7;

	uint8_t read_data[8];
	HAL_I2C_Mem_Read(hi2c, BME688_I2C_ADDR, BME688_REG_PRESS_MSB_0, I2C_MEMADD_SIZE_8BIT, read_data, 8, HAL_MAX_DELAY);

	uint8_t calib_temp[3];
	HAL_I2C_Mem_Read(hi2c, BME688_I2C_ADDR, 0xE9, I2C_MEMADD_SIZE_8BIT, calib_temp, 2, HAL_MAX_DELAY);
	par_t1 = (calib_temp[1] << 8) | calib_temp[0];
	HAL_I2C_Mem_Read(hi2c, BME688_I2C_ADDR, 0x8A, I2C_MEMADD_SIZE_8BIT, calib_temp, 2, HAL_MAX_DELAY);
	par_t2 = (calib_temp[1] << 8) | calib_temp[0];
	par_t3 = calib_temp[2];

	uint32_t press_adc = ((read_data[0] << 16) | (read_data[1] << 8) | read_data[2]) >> 4;
	uint32_t temp_adc = ((read_data[3] << 16) | (read_data[4] << 8) | read_data[5]) >> 4;
	uint16_t hum_adc = (read_data[6] << 8) | read_data[7];

	float var1;
	float var2;
	float var3;
	float var4;

	float t_fine;
	float temp_comp;

	var1 = ((((float) temp_adc / 16384.0f) - ((float) par_t1 / 1024.0f)) * ((float) par_t2));
	var2 = (((((float) temp_adc / 131072.0f) - ((float) par_t1 / 8192.0f)) * (((float) temp_adc / 131072.0f) - ((float) par_t1 / 8192.0f))) * ((float) par_t3 * 16.0f));
	t_fine = (var1 + var2);
	temp_comp = (t_fine / 5120.0f);
	data->temperature = temp_comp;

	uint8_t calib_press[19];
	HAL_I2C_Mem_Read(hi2c, BME688_I2C_ADDR, BME688_REG_PAR_P1_LSB, I2C_MEMADD_SIZE_8BIT, calib_press, 19, HAL_MAX_DELAY);

	par_p1 = (calib_press[1] << 8) | calib_press[0];
	par_p2 = (calib_press[3] << 8) | calib_press[2];
	par_p3 = calib_press[4];
	par_p4 = (calib_press[7] << 8) | calib_press[6];
	par_p5 = (calib_press[9] << 8) | calib_press[8];
	par_p6 = calib_press[11];
	par_p7 = calib_press[10];
	par_p8 = (calib_press[15] << 8) | calib_press[14];
	par_p9 = (calib_press[17] << 8) | calib_press[16];
	par_p10 = calib_press[18];

	float press_comp;

	var1 = (((float)t_fine / 2.0f) - 64000.0f);
	var2 = var1 * var1 * (((float)par_p6) / (131072.0f));
	var2 = var2 + (var1 * ((float)par_p5) * 2.0f);
	var2 = (var2 / 4.0f) + (((float)par_p4) * 65536.0f);
	var1 = (((((float)par_p3 * var1 * var1) / 16384.0f) + ((float)par_p2 * var1)) / 524288.0f);
	var1 = ((1.0f + (var1 / 32768.0f)) * ((float)par_p1));
	press_comp = (1048576.0f - ((float)press_adc));

	if ((int)var1 != 0) {
		press_comp = (((press_comp - (var2 / 4096.0f)) * 6250.0f) / var1);
		var1 = (((float)par_p9) * press_comp * press_comp) / 2147483648.0f;
		var2 = press_comp * (((float)par_p8) / 32768.0f);
		var3 = ((press_comp / 256.0f) * (press_comp / 256.0f) * (press_comp / 256.0f) * (par_p10 / 131072.0f));
		press_comp = (press_comp + (var1 + var2 + var3 + ((float)par_p7 * 128.0f)) / 16.0f);
	} else {
		press_comp = 0;
	}
	data->pressure = press_comp;


	uint8_t calib_hum[9];
	HAL_I2C_Mem_Read(hi2c, BME688_I2C_ADDR, BME688_REG_PAR_H2_MSB, I2C_MEMADD_SIZE_8BIT, calib_hum, 9, HAL_MAX_DELAY);

	par_h1 = (calib_hum[3] << 4) | (calib_hum[2] & 0x0F);
	par_h2 = (calib_hum[1] << 4) | (calib_hum[2] >> 4);
	par_h3 = calib_hum[4];
	par_h4 = calib_hum[5];
	par_h5 = calib_hum[6];
	par_h6 = calib_hum[7];
	par_h7 = calib_hum[8];

	float hum_comp;

	var1 = (float)((float)hum_adc) - (((float)par_h1 * 16.0f) + (((float)par_h3 / 2.0f) * temp_comp));
	var2 = var1 * ((float)(((float)par_h2 / 262144.0f) * (1.0f + (((float)par_h4 / 16384.0f) * temp_comp) + (((float)par_h5 / 1048576.0f) * temp_comp * temp_comp))));
	var3 = (float)par_h6 / 16384.0f;
	var4 = (float)par_h7 / 2097152.0f;
	hum_comp = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);

	if (hum_comp > 100.0f) {
		hum_comp = 100.0f;
	} else if (hum_comp < 0.0f) {
		hum_comp = 0.0f;
	}
	data->humidity = hum_comp;

}

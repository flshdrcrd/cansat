/*
 * BME688.h
 *
 *  Created on: Jan 23, 2024
 *      Author: berci
 */

#ifndef INC_BME688_H_
#define INC_BME688_H_

#include "stm32f1xx_hal.h"

/*
 * DEFINES
 */

#define BME688_I2C_ADDR (0x77 << 1) /* SDO to GND -> 0x76, SDO to VDDIO -> 0x77 */

#define BME688_CHIP_ID 0x61
#define BME688_VARIANT_ID 0x01

#define BME688_SET_OSRS_H 0b001
#define BME688_SET_OSRS_T 0b010
#define BME688_SET_OSRS_P 0b101
#define BME688_SET_MODE 0b11

/*
 * REGISTERS
 */

#define BME688_REG_STATUS 0x73
#define BME688_REG_VARIANT_ID 0xF0
#define BME688_REG_RESET 0xE0
#define BME688_REG_CHIP_ID 0xD0
#define BME688_REG_CONFIG 0x75
#define BME688_REG_CTRL_MEAS 0x74
#define BME688_REG_CTRL_HUM 0x72
#define BME688_REG_CTRL_GAS_1 0x71
#define BME688_REG_CTRL_GAS_0 0x70
#define BME688_REG_GAS_WAIT_SHARED 0x6E
#define BME688_REG_GAS_WAIT_9 0x6D
#define BME688_REG_GAS_WAIT_8 0x6C
#define BME688_REG_GAS_WAIT_7 0x6B
#define BME688_REG_GAS_WAIT_6 0x6A
#define BME688_REG_GAS_WAIT_5 0x68
#define BME688_REG_GAS_WAIT_4 0x67
#define BME688_REG_GAS_WAIT_3 0x67
#define BME688_REG_GAS_WAIT_2 0x66
#define BME688_REG_GAS_WAIT_1 0x65
#define BME688_REG_GAS_WAIT_0 0x64
#define BME688_REG_RES_HEAT_9 0x63
#define BME688_REG_RES_HEAT_8 0x62
#define BME688_REG_RES_HEAT_7 0x61
#define BME688_REG_RES_HEAT_6 0x60
#define BME688_REG_RES_HEAT_5 0x5F
#define BME688_REG_RES_HEAT_4 0x5E
#define BME688_REG_RES_HEAT_3 0x5D
#define BME688_REG_RES_HEAT_2 0x5C
#define BME688_REG_RES_HEAT_1 0x5B
#define BME688_REG_RES_HEAT_0 0x5A
#define BME688_REG_IDAC_HEAT_9 0x59
#define BME688_REG_IDAC_HEAT_8 0x58
#define BME688_REG_IDAC_HEAT_7 0x57
#define BME688_REG_IDAC_HEAT_6 0x56
#define BME688_REG_IDAC_HEAT_5 0x55
#define BME688_REG_IDAC_HEAT_4 0x54
#define BME688_REG_IDAC_HEAT_3 0x53
#define BME688_REG_IDAC_HEAT_2 0x52
#define BME688_REG_IDAC_HEAT_1 0x51
#define BME688_REG_IDAC_HEAT_0 0x50
#define BME688_REG_GAS_R_LSB_2 0x4F
#define BME688_REG_GAS_R_MSB_2 0x4E
#define BME688_REG_HUM_LSB_2 0x48
#define BME688_REG_HUM_MSB_2 0x47
#define BME688_REG_TEMP_XLSB_2 0x46
#define BME688_REG_TEMP_LSB_2 0x45
#define BME688_REG_TEMP_MSB_2 0x44
#define BME688_REG_PRESS_XLSB_2 0x43
#define BME688_REG_PRESS_LSB_2 0x42
#define BME688_REG_PRESS_MSB_2 0x41
#define BME688_REG_SUB_MEAS_INDEX_2 0x40
#define BME688_REG_MEAS_STATUS_2 0x3F
#define BME688_REG_GAS_R_LSB_1 0x3E
#define BME688_REG_GAS_R_MSB_1 0x3D
#define BME688_REG_HUM_LSB_1 0x37
#define BME688_REG_HUM_MSB_1 0x36
#define BME688_REG_TEMP_XLSB_1 0x35
#define BME688_REG_TEMP_LSB_1 0x34
#define BME688_REG_TEMP_MSB_1 0x33
#define BME688_REG_PRESS_XLSB_1 0x32
#define BME688_REG_PRESS_LSB_1 0x31
#define BME688_REG_PRESS_MSB_1 0x30
#define BME688_REG_SUB_MEAS_INDEX_1 0x2F
#define BME688_REG_MEAS_STATUS_1 0x2E
#define BME688_REG_GAS_R_LSB_0 0x2D
#define BME688_REG_GAS_R_MSB_0 0x2C
#define BME688_REG_HUM_LSB_0 0x26
#define BME688_REG_HUM_MSB_0 0x25
#define BME688_REG_TEMP_XLSB_0 0x24
#define BME688_REG_TEMP_LSB_0 0x23
#define BME688_REG_TEMP_MSB_0 0x22
#define BME688_REG_PRESS_XLSB_0 0x21
#define BME688_REG_PRESS_LSB_0 0x20
#define BME688_REG_PRESS_MSB_0 0x1F
#define BME688_REG_SUB_MEAS_INDEX_0 0x1E
#define BME688_REG_MEAS_STATUS_0 0x1D

#define BME688_REG_PAR_T1_LSB 0xE9
#define BME688_REG_PAR_T1_MSB 0xEA
#define BME688_REG_PAR_T2_LSB 0x8A
#define BME688_REG_PAR_T2_MSB 0x8B
#define BME688_REG_PAR_T3 0x8C
#define BME688_REG_PAR_P1_LSB 0x8E
#define BME688_REG_PAR_P1_MSB 0x8F
#define BME688_REG_PAR_P2_LSB 0x90
#define BME688_REG_PAR_P2_MSB 0x91
#define BME688_REG_PAR_P3 0x92
#define BME688_REG_PAR_P4_LSB 0x94
#define BME688_REG_PAR_P4_MSB 0x95
#define BME688_REG_PAR_P5_LSB 0x96
#define BME688_REG_PAR_P5_MSB 0x97
#define BME688_REG_PAR_P6 0x99
#define BME688_REG_PAR_P7 0x98
#define BME688_REG_PAR_P8_LSB 0x9C
#define BME688_REG_PAR_P8_MSB 0x9D
#define BME688_REG_PAR_P9_LSB 0x9E
#define BME688_REG_PAR_P9_MSB 0x9F
#define BME688_REG_PAR_P10 0xA0
#define BME688_REG_PAR_H1_LSB 0xE2
#define BME688_REG_PAR_H1_MSB 0xE3
#define BME688_REG_PAR_H2_LSB 0xE2
#define BME688_REG_PAR_H2_MSB 0xE1
#define BME688_REG_PAR_H3 0xE4
#define BME688_REG_PAR_H4 0xE5
#define BME688_REG_PAR_H5 0xE6
#define BME688_REG_PAR_H6 0xE7
#define BME688_REG_PAR_H7 0xE8

typedef struct {
	float temperature;
	float pressure;
	float humidity;
} BME688;

void BME688_Init(I2C_HandleTypeDef *hi2c);
void BME688_Read(I2C_HandleTypeDef *hi2c, BME688 *data);

#endif /* INC_BME688_H_ */
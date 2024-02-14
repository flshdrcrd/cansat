/*
 * NEO_M8N.h
 *
 *  Created on: Feb 10, 2024
 *      Author: berci
 */

#ifndef INC_NEOM8N_H_
#define INC_NEOM8N_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define GPS_MAX_BUFFER 128

typedef struct {
	uint32_t time;
	float latitude;
	float longitude;
	uint8_t positioning_status;
	uint8_t satellites_used;
	float hdop;
	float altitude;
} NEOM8N;

void NEOM8N_Read(UART_HandleTypeDef *huart, NEOM8N *gnss_data);

#endif /* INC_NEOM8N_H_ */

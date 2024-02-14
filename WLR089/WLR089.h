/*
 * WLR089U0.h
 *
 *  Created on: Feb 14, 2024
 *      Author: berci
 */

#ifndef INC_WLR089_H_
#define INC_WLR089_H_

#include <string.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

#define WLR089_PWR 20
#define WLR089_FREQ 868100000
#define WLR089_SYNC 34

void WLR089_Init(UART_HandleTypeDef *huart);
void WLR089_Transmit(const char *input, UART_HandleTypeDef *huart);

#endif /* INC_WLR089_H_ */

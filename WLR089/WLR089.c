/*
 * WLR089U0.c
 *
 *  Created on: Jan 31, 2024
 *      Author: berci
 */


#include <WLR089.h>

void WLR089_Init(UART_HandleTypeDef *huart) {
	char cmd[128];
	sprintf(cmd, "radio set pwr %d\r\nradio set freq %d\r\nradio set sync %d\r\n", WLR089_PWR, WLR089_FREQ, WLR089_SYNC);
	HAL_UART_Transmit(huart, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
}

void WLR089_Transmit(const char *input, UART_HandleTypeDef *huart) {
	int len = 2 * strlen(input);

	char tx_buffer[9 + len + 4];

	strcpy(tx_buffer, "radio tx ");
	for (int i = 0, j = 9; i < (len / 2); ++i, j += 2) {
		sprintf(tx_buffer + j, "%02X", input[i]);
	}
	strcpy(tx_buffer + 9 + len, " 1\r\n");

	HAL_UART_Transmit(huart, (uint8_t*) tx_buffer, 9 + len + 4, HAL_MAX_DELAY);
}


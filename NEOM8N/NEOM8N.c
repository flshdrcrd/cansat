/*
 * NEO_M8N.c
 *
 *  Created on: Feb 10, 2024
 *      Author: berci
 */

#include <NEOM8N.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned char calculateChecksum(const char *sentence) {
	unsigned char checksum = 0;
	for (int i = 1; sentence[i] != '*'; i++) {
		checksum ^= sentence[i];
	}
	return checksum;
}

void NEOM8N_Read(UART_HandleTypeDef *huart, NEOM8N *gnss_data) {
	char sentence[GPS_MAX_BUFFER];
	uint8_t rx_char;
	uint8_t n = 0;

	do {
		HAL_UART_Receive(huart, &rx_char, 1, HAL_MAX_DELAY);
		sentence[n++] = rx_char;
	} while (rx_char != '\n' && n < GPS_MAX_BUFFER - 1);

	if (strncmp(sentence, "$GNGGA", 6) != 0) {
		return;
	}

	unsigned char checksum = calculateChecksum(sentence);
	char receivedChecksum[3];
	sprintf(receivedChecksum, "%02X", checksum);
	if (strncmp(receivedChecksum, &sentence[strlen(sentence) - 4], 2) != 0) {
		return;
	}

	uint8_t index = 0;
	char *p1 = (char*) sentence;
	char *p2;

	while ((p2 = strchr(p1, ',')) != NULL) {
		char data[p2 - p1];
		memcpy(data, p1, p2 - p1);

		switch (index) {
		case 1:
			if (p1 != p2) {
				data[6] = data[7];
				data[7] = '\0';
				gnss_data->time = atoi(data);
			} else {
				gnss_data->time = 0;
			}
			break;
		case 2:
			if (p1 != p2) {
				int degrees;
				double minutes;
				double decimalDegrees;

				sscanf(data, "%2d%lf", &degrees, &minutes);
				double fraction = minutes / 60.0;
				decimalDegrees = degrees + fraction;

				gnss_data->latitude = decimalDegrees;
			} else {
				gnss_data->latitude = 0.0f;
			}
			break;
		case 3:
			if (data[0] == 'S') {
				gnss_data->latitude = -gnss_data->latitude;
			}
			break;
		case 4:
			if (p1 != p2) {
				int degrees;
				double minutes;
				double decimalDegrees;

				sscanf(data, "%3d%lf", &degrees, &minutes);
				double fraction = minutes / 60.0;
				decimalDegrees = degrees + fraction;

				gnss_data->longitude = decimalDegrees;
			} else {
				gnss_data->longitude = 0.0;
			}
			break;
		case 5:
			if (data[0] == 'W') {
				gnss_data->longitude = -gnss_data->longitude;
			}
			break;
		case 6:
			if (p1 != p2) {
				gnss_data->positioning_status = atoi(data);
			} else {
				gnss_data->positioning_status = 0;
			}
			break;
		case 7:
			if (p1 != p2) {
				gnss_data->satellites_used = atoi(data);
			} else {
				gnss_data->satellites_used = 0;
			}
			break;
		case 8:
			if (p1 != p2) {
				gnss_data->hdop = atof(data);
			} else {
				gnss_data->hdop = 0.0f;
			}
			break;
		case 9:
			if (p1 != p2) {
				gnss_data->altitude = atof(data);
			} else {
				gnss_data->altitude = 0.0f;
			}
			break;
		default:
			break;
		}

		p1 = p2 + 1;
		index++;
	}
}

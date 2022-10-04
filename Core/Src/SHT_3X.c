/*
 * SHT_3X.c
 *
 *  Created on: Oct 4, 2022
 *      Author: Muhammad Asghar
 */

#include "SHT3x.h"
#include "main.h"
#include <stdio.h>
#include <stdint.h>


void Measure(I2C_HandleTypeDef I2CHandle, float *temperature, float *humidity)
{
	 uint8_t Recieve_buffer[6];
	 Send_command(I2CHandle, 0x2c06);
	 HAL_Delay(1);
	 HAL_I2C_Master_Receive(&I2CHandle, 0x44 << 1u, Recieve_buffer, sizeof(Recieve_buffer), 30);

	uint8_t temperature_crc = calculate_crc(Recieve_buffer, 2);
	uint8_t humidity_crc = calculate_crc(Recieve_buffer+3 , 2);

//	if (temperature_crc != Recieve_buffer[2] || humidity_crc != Recieve_buffer[5])
//	{
//		HAL_UART_Transmit(&huart1, (uint8_t*)"not okay\n", strlen("not okay\n"), 1000);
//	}

	int16_t temperature_raw = (int16_t)uint8_to_uint16(Recieve_buffer[0], Recieve_buffer[1]);
	uint16_t humidity_raw = uint8_to_uint16(Recieve_buffer[3], Recieve_buffer[4]);

		*temperature = -45.0f + 175.0f * (float)temperature_raw / 65535.0f;
		*humidity = 100.0f * (float)humidity_raw / 65535.0f;

//		HAL_UART_Transmit(&huart1, (uint8_t*)"okay\n", strlen("okay\n"), 1000);

}





uint8_t calculate_crc(const uint8_t *data, size_t length)
{
	uint8_t crc = 0xff;
	for (size_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (size_t j = 0; j < 8; j++) {
			if ((crc & 0x80u) != 0) {
				crc = (uint8_t)((uint8_t)(crc << 1u) ^ 0x31u);
			} else {
				crc <<= 1u;
			}
		}
	}
	return crc;
}

void Send_command(I2C_HandleTypeDef I2CHandle, uint16_t command)
{
	uint8_t command_buffer[2];
	command_buffer[0] = ((command & 0xff00) >> 8U);
	command_buffer[1] = ((command & 0x00ff));
	HAL_I2C_Master_Transmit(&I2CHandle, 0x44 << 1u, command_buffer, sizeof(command_buffer), 30);
}


uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb)
{
	return (uint16_t)((uint16_t)msb << 8u) | lsb;
}


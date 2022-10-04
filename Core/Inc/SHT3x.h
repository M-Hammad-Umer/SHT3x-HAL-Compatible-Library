/*
 * SHT3x.h
 *
 *  Created on: Oct 4, 2022
 *      Author: Muhammad Asghar
 */

#ifndef INC_SHT3X_H_
#define INC_SHT3X_H_

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"

void Measure(I2C_HandleTypeDef I2CHandle, float *temperature, float *humidity);
void UART_send_float(char *data);


uint8_t calculate_crc(const uint8_t *data, size_t length);
void Send_command(I2C_HandleTypeDef I2CHandle, uint16_t command);
uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb);


#endif /* INC_SHT3X_H_ */

#ifndef __DHT_H
#define __DHT_H

#include "stm32f0xx_hal.h"
uint8_t RHI, RHD, TCI, TCD, SUM;
uint32_t pMillis, cMillis;

typedef struct
{
	TIM_HandleTypeDef* Timer;
	uint16_t Pin;
	GPIO_TypeDef* PORT;
	float Temp;
	float Humi;
} DHT_Name;


void DHT_Init(DHT_Name* DHT, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
void DHT_ReadTempHum(DHT_Name* DHT);
#endif

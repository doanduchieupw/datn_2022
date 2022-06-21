#include "DHT.h"
//************************** Low Level Layer ********************************************************//
void Delay_us (DHT_Name* DHT, uint16_t Time)
{
  __HAL_TIM_SET_COUNTER(DHT->Timer, 0);
  while (__HAL_TIM_GET_COUNTER(DHT->Timer) < Time);
}

void Delay_ms(DHT_Name* DHT, uint16_t Time)
{
	__HAL_TIM_SET_COUNTER(DHT->Timer, 0);
	while(Time--)
	{
		while(__HAL_TIM_GET_COUNTER(DHT->Timer)<1000){}
	}
}

// set the pin as output
static void DHT_SetPinOut(DHT_Name* DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT->PORT, &GPIO_InitStruct);
}
static void DHT_SetPinIn(DHT_Name* DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT->PORT, &GPIO_InitStruct);
}


static void DHT_WritePin(DHT_Name* DHT, uint8_t Value)
{
	HAL_GPIO_WritePin(DHT->PORT, DHT->Pin, Value);
}
static uint8_t DHT_ReadPin(DHT_Name* DHT)
{
	uint8_t Value;
	Value =  HAL_GPIO_ReadPin(DHT->PORT, DHT->Pin);
	return Value;
}

//********************************* Middle level Layer ****************************************************//
static uint8_t DHT11_Start (DHT_Name* DHT)
{
  uint8_t Response = 0;
  DHT_SetPinOut(DHT);
  DHT_WritePin(DHT, 0);		// pull the pin low
  HAL_Delay(20);   			// wait for 20ms
  DHT_WritePin(DHT, 1);   	// pull the pin high
  Delay_us(DHT, 30);   			// wait for 30us
  DHT_SetPinIn(DHT);		// set the pin as input
  Delay_us(DHT, 40);

  if (!DHT_ReadPin(DHT)) {
	  Delay_us(DHT, 80);
	  if (DHT_ReadPin(DHT)) Response = 1;
  }

  pMillis = HAL_GetTick();
  cMillis = HAL_GetTick();
  while (DHT_ReadPin(DHT) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}

uint8_t DHT11_Read (DHT_Name* DHT)
{
  uint8_t value;
  for (uint8_t i = 0; i < 8; i++)
  {
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (!DHT_ReadPin(DHT) && (pMillis + 2 > cMillis))
    	{  // wait for the pin to go high
    		cMillis = HAL_GetTick();
    	}
    Delay_us(DHT, 40);   // wait for 40 us
    if (!DHT_ReadPin(DHT))   // if the pin is low
    	value &= ~(1<<(7-i));
    else
    	value |= (1<<(7-i));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (DHT_ReadPin(DHT) && pMillis + 2 > cMillis)
    {  // wait for the pin to go low
      cMillis = HAL_GetTick();
    }
  }
  return value;
}

//************************** High Level Layer ********************************************************//
void DHT_Init(DHT_Name* DHT, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin)
{

	DHT->PORT = DH_PORT;
	DHT->Pin = DH_Pin;
	DHT->Timer = Timer;
}

void DHT_ReadTempHum(DHT_Name* DHT)
{
	if(DHT11_Start(DHT))
	{
		RHI = DHT11_Read(DHT); // Relative humidity integral
		RHD = DHT11_Read(DHT); // Relative humidity decimal
		TCI = DHT11_Read(DHT); // Celsius integral
		TCD = DHT11_Read(DHT); // Celsius decimal
		SUM = DHT11_Read(DHT); // Check sum
		if (RHI + RHD + TCI + TCD == SUM)
		{
			// Can use RHI and TCI for any purposes if whole number only needed
			DHT->Temp = (float)TCI + (float)(TCD/10.0);
			DHT->Humi = (float)RHI + (float)(RHD/10.0);
		}
	}
}

//end DHT-11___________________________________________________________________

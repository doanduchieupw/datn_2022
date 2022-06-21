#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f0xx_hal.h"

#define MPU6050_ADDR 0xD0
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

typedef struct
{
	int16_t Accel_X_RAW;
	int16_t Accel_Y_RAW;
	int16_t Accel_Z_RAW;
	float Ax;
	float Ay;
	float Az;

	int16_t Gyro_X_RAW;
	int16_t Gyro_Y_RAW;
	int16_t Gyro_Z_RAW;
	float Gx;
	float Gy;
	float Gz;

	float Temperature;

}MPU6050_t;


void MPU6050_Init (I2C_HandleTypeDef *I2Cx);
void MPU6050_Read_Accel(I2C_HandleTypeDef *I2Cx, MPU6050_t *MPU);
void MPU6050_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *MPU);
void MPU6050_Read_Temp(I2C_HandleTypeDef *I2Cx, MPU6050_t *MPU);

#endif

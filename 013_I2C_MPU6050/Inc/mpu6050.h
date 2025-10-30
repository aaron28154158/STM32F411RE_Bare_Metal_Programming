#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>
#include "systick.h"
#include "i2c.h"

extern uint8_t mpu6050_data_buffer[14];
extern char read_data;

/* Device ID register */
#define WHO_AM_I					0x75

/* I2C address */
#define DEVICE_ADDRESS				0x68

#define ACCEL_CONFIG_REGISTER 		0x1C
#define AFS_SEL_4G					(1UL << 3)
#define GYRO_CONFIG_REGISTER    	0x1B
#define FS_SEL_250DPS          		(0x00)

#define PWR_MGMT_1_REGISTER			0x6B
#define PWR_MGMT_2_REGISTER			0x6C
#define PWR_MGMT_1_DEVICE_RESET		(1UL << 7)
#define PWR_MGMT_1_WAKE_UP			(0x00)

/* ACCEL_XOUT_H */
#define DATA_START_ADDR				(0x3B)

void mpu6050_INIT(void);
void mpu6050_Read_Values(uint8_t reg);
void mpu6050_Read_Address(uint8_t reg);

#endif

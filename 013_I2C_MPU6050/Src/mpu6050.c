#include "mpu6050.h"

uint8_t mpu6050_data_buffer[14];
char read_data;

void mpu6050_Read_Address(uint8_t reg)
{
	I2C1_byteRead(DEVICE_ADDRESS, reg, &read_data);
}

void mpu6050_Write(uint8_t reg, char value)
{
	char write_data[1];
	write_data[0] = value;

	I2C1_burstWrite(DEVICE_ADDRESS, reg, 1, write_data);
}

void mpu6050_Read_Values(uint8_t reg)
{
	I2C1_burstRead(DEVICE_ADDRESS, reg, 14, (char*) mpu6050_data_buffer);
}

void mpu6050_INIT(void)
{
	/* Enable I2C */
	I2C1_INIT();

	SysTick_Delay_ms(10);

	/* Read the DEVICE ID, this should return 0x68 */
	mpu6050_Read_Address(WHO_AM_I);

	SysTick_Delay_ms(1);

	/* Reset all bits - 這會重置整個 MPU6050 */
	mpu6050_Write(PWR_MGMT_1_REGISTER, PWR_MGMT_1_DEVICE_RESET);

	SysTick_Delay_ms(100);

	/* Configure power control - Wake up device (清除 SLEEP bit) */
	mpu6050_Write(PWR_MGMT_1_REGISTER, PWR_MGMT_1_WAKE_UP);

	SysTick_Delay_ms(10);

	/* Set data format range to ±4g */
	mpu6050_Write(ACCEL_CONFIG_REGISTER, AFS_SEL_4G);

	SysTick_Delay_ms(10);

	mpu6050_Write(GYRO_CONFIG_REGISTER, FS_SEL_250DPS);

	SysTick_Delay_ms(10);
}

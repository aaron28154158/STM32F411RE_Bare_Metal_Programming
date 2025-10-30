#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "systick.h"
#include "mpu6050.h"
#include "uart.h"
#include "systick.h"
#include "fpu.h"

int16_t ax, ay, az;      // 加速度原始值
int16_t temp;            // 溫度原始值
int16_t gx, gy, gz;      // 陀螺儀原始值

#define ACCEL_SCALE_FACTOR_4G     	8192.0f
#define GYRO_SCALE_FACTOR_250_DPS 	131.0f

float acc_x, acc_y, acc_z; // 單位為 g
float gyro_x, gyro_y, gyro_z; // 單位為 °/s

uint32_t g_measurement_count = 0;

int main(void)
{
	FPU_Enable();
	SySTick_INIT();
	UART2_TXRX_INIT();

	printf("MPU6050 INIT... \n");
	mpu6050_INIT();

	mpu6050_Read_Address(WHO_AM_I);
	if(read_data == 0x68)
	{
	        printf("MPU6050 Connected! (ID: 0x%02X)\r\n", read_data);
	}
	else
	{
	        printf("ERROR: MPU6050 Not Found! (ID: 0x%02X)\r\n", read_data);
	        while(1);  // Stop execute
	}

	printf("Starting measurements... \n");

    while(1)
    {
    	/* 讀取 14 bytes 感測器數據 */
    	mpu6050_Read_Values(DATA_START_ADDR);

        ax = (int16_t)((mpu6050_data_buffer[0] << 8) | mpu6050_data_buffer[1]);
        ay = (int16_t)((mpu6050_data_buffer[2] << 8) | mpu6050_data_buffer[3]);
        az = (int16_t)((mpu6050_data_buffer[4] << 8) | mpu6050_data_buffer[5]);

        temp = (int16_t)((mpu6050_data_buffer[6] << 8) | mpu6050_data_buffer[7]);

        gx = (int16_t)((mpu6050_data_buffer[8] << 8) | mpu6050_data_buffer[9]);
        gy = (int16_t)((mpu6050_data_buffer[10] << 8) | mpu6050_data_buffer[11]);
        gz = (int16_t)((mpu6050_data_buffer[12] << 8) | mpu6050_data_buffer[13]);

        acc_x = (float)ax / ACCEL_SCALE_FACTOR_4G;   // 單位: g
        acc_y = (float)ay / ACCEL_SCALE_FACTOR_4G;
        acc_z = (float)az / ACCEL_SCALE_FACTOR_4G;

        gyro_x = (float)gx / GYRO_SCALE_FACTOR_250_DPS;  // 單位: °/s
        gyro_y = (float)gy / GYRO_SCALE_FACTOR_250_DPS;
        gyro_z = (float)gz / GYRO_SCALE_FACTOR_250_DPS;

        printf("=============================\n");
        printf("Measurement Count : %lu \n", g_measurement_count);
        printf("Acc: X=%.2f Y=%.2f Z=%.2f \n", acc_x, acc_y, acc_z);
        printf("Gyro: X=%.1f Y=%.1f Z=%.1f \n", gyro_x, gyro_y, gyro_z);
        printf("=============================\n");
        printf("\n");

        SysTick_Delay_ms(1000);
    }
}

#ifndef IMU_DRIVER_H
#define IMU_DRIVER_H

#include <stdint.h>
#include "pico/stdlib.h"

// --- Constantes ---
#define IMU_I2C_PORT i2c0
#define IMU_SDA_PIN 0
#define IMU_SCL_PIN 1

void imu_init();

void imu_read_raw(int16_t accel[3], int16_t gyro[3]);

#endif // IMU_DRIVER_H
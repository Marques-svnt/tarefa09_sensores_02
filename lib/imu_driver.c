#include "imu_driver.h"
#include "hardware/i2c.h"

// Endereço I2C do MPU6050
static const int ADDR = 0x68;

// Função privada para resetar o MPU6050
static void mpu6050_reset() {
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(IMU_I2C_PORT, ADDR, buf, 2, false);
    sleep_ms(100);
    buf[1] = 0x00;
    i2c_write_blocking(IMU_I2C_PORT, ADDR, buf, 2, false);
    sleep_ms(10);
}

// Implementação da função pública imu_init
void imu_init() {
    i2c_init(IMU_I2C_PORT, 400 * 1000);
    gpio_set_function(IMU_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(IMU_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(IMU_SDA_PIN);
    gpio_pull_up(IMU_SCL_PIN);
    mpu6050_reset();
}

// Implementação da função pública imu_read_raw
void imu_read_raw(int16_t accel[3], int16_t gyro[3]) {
    uint8_t buffer[6];
    
    // Lê Acelerômetro
    uint8_t val = 0x3B;
    i2c_write_blocking(IMU_I2C_PORT, ADDR, &val, 1, true);
    i2c_read_blocking(IMU_I2C_PORT, ADDR, buffer, 6, false);
    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];
    }

    // Lê Giroscópio
    val = 0x43;
    i2c_write_blocking(IMU_I2C_PORT, ADDR, &val, 1, true);
    i2c_read_blocking(IMU_I2C_PORT, ADDR, buffer, 6, false);
    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];
    }
}
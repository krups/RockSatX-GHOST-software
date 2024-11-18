// sensor1.c
#include "bme280_sensor.h"

// Function prototypes
void configBME280(void);
{
    // Example configuration data for BME280
    uint8_t configData[2] = {0x01, 0x27}; // Example configuration data
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SENSOR1_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, SENSOR1_CONFIG_REGISTER, true);
    i2c_master_write(cmd, configData, 2, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    printf("Configuring Sensor 1\n");
}

void handleBNE280(void);
{
uint8_t data[8];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SENSOR1_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, SENSOR1_DATA_REGISTER, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SENSOR1_I2C_ADDRESS << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 8, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    // Process the data read from the sensor
    int32_t pressure = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t temperature = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    int32_t humidity = (data[6] << 8) | data[7];

    printf("Sensor 1 Data - Pressure: %d, Temperature: %d, Humidity: %d\n", pressure, temperature, humidity);
}
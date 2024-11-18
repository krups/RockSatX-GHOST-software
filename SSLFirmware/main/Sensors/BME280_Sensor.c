// Sensors/BME280_sensor.c
#include "BME280_sensor.h"
#include "main/Libraries/BME280_SensorAPI/bme280.h"
#include <stdio.h>

// I2C address for BME280 sensor
#define BME280_I2C_ADDRESS BME280_I2C_ADDR_PRIM

// BME280 device structure
struct bme280_dev bme280_dev;

// I2C read function
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    i2c_port_t i2c_num = (i2c_port_t)intf_ptr;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BME280_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BME280_I2C_ADDRESS << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, reg_data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK ? BME280_OK : BME280_E_COMM_FAIL;
}

// I2C write function
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    i2c_port_t i2c_num = (i2c_port_t)intf_ptr;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (BME280_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write(cmd, reg_data, len, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret == ESP_OK ? BME280_OK : BME280_E_COMM_FAIL;
}

// Delay function
void user_delay_ms(uint32_t period, void *intf_ptr) {
    vTaskDelay(period / portTICK_RATE_MS);
}

// Sensor configuration function
void configBME280(void) {
    int8_t rslt;
    bme280_dev.dev_id = BME280_I2C_ADDRESS;
    bme280_dev.intf = BME280_I2C_INTF;
    bme280_dev.read = user_i2c_read;
    bme280_dev.write = user_i2c_write;
    bme280_dev.delay_us = user_delay_ms;
    bme280_dev.intf_ptr = (void *)I2C_NUM_0;

    rslt = bme280_init(&bme280_dev);
    if (rslt != BME280_OK) {
        printf("Failed to initialize BME280 sensor\n");
        return;
    }

    uint8_t settings_sel;
    bme280_dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    bme280_dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    bme280_dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    bme280_dev.settings.filter = BME280_FILTER_COEFF_16;
    bme280_dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL | BME280_STANDBY_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, &bme280_dev);
    if (rslt != BME280_OK) {
        printf("Failed to set BME280 sensor settings\n");
        return;
    }

    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme280_dev);
    if (rslt != BME280_OK) {
        printf("Failed to set BME280 sensor mode\n");
        return;
    }

    printf("Configuring BME280 sensor\n");
}

// Sensor handler function
void handleBME280(void) {
    struct bme280_data comp_data;
    int8_t rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &bme280_dev);
    if (rslt == BME280_OK) {
        printf("Temperature: %.2f °C, Pressure: %.2f hPa, Humidity: %.2f %%\n",
               comp_data.temperature, comp_data.pressure / 100, comp_data.humidity);
    } else {
        printf("Failed to read data from BME280 sensor\n");
    }
}
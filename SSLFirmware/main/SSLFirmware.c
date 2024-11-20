//SSL Firmware
//Firmware for all current SSL missions and devices
//Developed for RockSat-X GHOST
//Based on Matt Ruffner's KREPE 2 firmware
//Developed by Hersch Nathan, Alex Barrera

#include "config.h" 
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h" 
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <driver/i2c.h>
#include <stdio.h>
#include "BME280_Sensor.h"
#include "i2c_manager.h"
#include "Fake_Sensor.h"

// Define sensor intervals (in milliseconds)
const uint32_t sensorIntervals[SENSOR_EVENT_COUNT] = {
    [SENSOR1_EVENT] = 1000,
    [SENSOR2_EVENT] = 2000,
    [SENSOR3_EVENT] = 3000,
    [SENSOR4_EVENT] = 4000,
    [SENSOR5_EVENT] = 5000
};

void app_main() {
    // Define I2C bus configurations
    I2CBusConfig bus1Config = {
        .i2cPort = I2C_NUM_0,
        .sensorEvents = {SENSOR1_EVENT, SENSOR2_EVENT},
        .sensorIntervals = {sensorIntervals[SENSOR1_EVENT], sensorIntervals[SENSOR2_EVENT]},
        .sensorConfigs = {configBME280, configFakeSensor}, // Set sensors 1-2 config
        .sensorCount = 2
    };

    I2CBusConfig bus2Config = {
        .i2cPort = I2C_NUM_1,
        .sensorEvents = {SENSOR3_EVENT, SENSOR4_EVENT, SENSOR5_EVENT},
        .sensorIntervals = {sensorIntervals[SENSOR3_EVENT], sensorIntervals[SENSOR4_EVENT], sensorIntervals[SENSOR5_EVENT]},
        .sensorConfigs = {configFakeSensor, configFakeSensor, configFakeSensor}, // Set sensors 3-5 config as fake sensors
        .sensorCount = 3
    };

    // Create event queues for each bus
    bus1Config.eventQueue = xQueueCreate(10, sizeof(SensorEventType));
    bus2Config.eventQueue = xQueueCreate(10, sizeof(SensorEventType));

    // Create mutexes for each bus
    bus1Config.mutex = xSemaphoreCreateMutex();
    bus2Config.mutex = xSemaphoreCreateMutex();

    // Create tasks for each I2C bus
    xTaskCreate(i2cBusTask, "I2CBusTask1", 2048, &bus1Config, 5, NULL);
    xTaskCreate(i2cBusTask, "I2CBusTask2", 2048, &bus2Config, 5, NULL);
}


//SSL Firmware
//Firmware for all current SSL missions and devices
//Developed for RockSat-X GHOST
//Based on Matt Ruffner's KREPE 2 firmware
//Developed by Hersch Nathan, Alex Barrera

#include <config.h> 
#include "freertos/task.h" //backbone of this firmware
#include "freertos/FreeRTOS.h" //backbone of this firmware
#include <freertos/timers.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <driver/i2c.h>
#include <stdio.h>
#include "Sensors/BME280_sensor.h"
#include "i2c_manager.h"

// Define sensor polling intervals in milliseconds
#define SENSOR1_INTERVAL 1000
#define SENSOR2_INTERVAL 2000
#define SENSOR3_INTERVAL 1500
#define SENSOR4_INTERVAL 2500
#define SENSOR5_INTERVAL 3000

// Define the maximum number of sensors per bus
#define MAX_SENSORS_PER_BUS 5

// Lookup table for sensor handlers
SensorHandler sensorHandlers[SENSOR_EVENT_COUNT] = {
    handleBME280,
    handleSensor2,
    handleSensor3,
    handleSensor4,
    handleSensor5
};

void app_main() {
    // Define I2C bus configurations
    I2CBusConfig bus1Config = {
        .i2cPort = I2C_NUM_0,
        .sensorEvents = {SENSOR1_EVENT, SENSOR2_EVENT},
        .sensorIntervals = {SENSOR1_INTERVAL, SENSOR2_INTERVAL},
        .sensorConfigs = {configBME280, configSensor2},
        .sensorCount = 2
    };

    I2CBusConfig bus2Config = {
        .i2cPort = I2C_NUM_1,
        .sensorEvents = {SENSOR3_EVENT, SENSOR4_EVENT, SENSOR5_EVENT},
        .sensorIntervals = {SENSOR3_INTERVAL, SENSOR4_INTERVAL, SENSOR5_INTERVAL},
        .sensorConfigs = {configSensor3, configSensor4, configSensor5},
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


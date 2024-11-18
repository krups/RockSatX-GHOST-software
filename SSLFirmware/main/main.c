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


#include "Sensors/bme280/bme280_sensor.h"

// Define sensor polling intervals in milliseconds
#define SENSOR1_INTERVAL 1000
#define SENSOR2_INTERVAL 2000
#define SENSOR3_INTERVAL 1500
#define SENSOR4_INTERVAL 2500
#define SENSOR5_INTERVAL 3000

// Define the maximum number of sensors per bus
#define MAX_SENSORS_PER_BUS 5

// Define sensor event types
typedef enum {
    SENSOR1_EVENT,
    SENSOR2_EVENT,
    SENSOR3_EVENT,
    SENSOR4_EVENT,
    SENSOR5_EVENT,
    SENSOR_EVENT_COUNT // This should always be the last element
} SensorEventType;

// Function pointer type for sensor handlers
typedef void (*SensorHandler)(void);

// Function pointer type for sensor configuration functions
typedef void (*SensorConfig)(void);

// Lookup table for sensor handlers
SensorHandler sensorHandlers[SENSOR_EVENT_COUNT] = {
    handleSensor1,
    handleSensor2,
    handleSensor3,
    handleSensor4,
    handleSensor5
};

// Structure for I2C bus configuration
typedef struct {
    i2c_port_t i2cPort;
    TimerHandle_t timers[MAX_SENSORS_PER_BUS];
    QueueHandle_t eventQueue;
    SemaphoreHandle_t mutex;
    SensorEventType sensorEvents[MAX_SENSORS_PER_BUS];
    uint32_t sensorIntervals[MAX_SENSORS_PER_BUS];
    SensorConfig sensorConfigs[MAX_SENSORS_PER_BUS];
    uint8_t sensorCount;
} I2CBusConfig;

// Function to add sensor event to the queue
void addSensorEventToQueue(QueueHandle_t queue, SensorEventType eventType) {
    xQueueSend(queue, &eventType, portMAX_DELAY);
}

// Timer callback function
void sensorTimerCallback(TimerHandle_t xTimer) {
    SensorEventType eventType = (SensorEventType)pvTimerGetTimerID(xTimer);
    I2CBusConfig *busConfig = (I2CBusConfig *)pvTimerGetTimerID(xTimer);
    addSensorEventToQueue(busConfig->eventQueue, eventType);
}

// Task to handle I2C bus
void i2cBusTask(void *pvParameters) {
    I2CBusConfig *busConfig = (I2CBusConfig *)pvParameters;

    // Configure sensors before starting timers
    for (int i = 0; i < busConfig->sensorCount; i++) {
        if (busConfig->sensorConfigs[i] != NULL) {
            busConfig->sensorConfigs[i]();
        }
    }

    // Create timers for each sensor
    for (int i = 0; i < busConfig->sensorCount; i++) {
        busConfig->timers[i] = xTimerCreate("SensorTimer", pdMS_TO_TICKS(busConfig->sensorIntervals[i]), pdTRUE, (void *)busConfig->sensorEvents[i], sensorTimerCallback);
        xTimerStart(busConfig->timers[i], 0);
    }

    // Task loop
    while (1) {
        SensorEventType eventType;
        if (xQueueReceive(busConfig->eventQueue, &eventType, portMAX_DELAY) == pdPASS) {
            // Take the mutex before accessing the I2C bus
            if (xSemaphoreTake(busConfig->mutex, portMAX_DELAY) == pdTRUE) {
                // Handle the sensor event using the lookup table
                if (eventType < SENSOR_EVENT_COUNT) {
                    sensorHandlers[eventType]();
                }
                // Give the mutex back after accessing the I2C bus
                xSemaphoreGive(busConfig->mutex);
            }
        }
    }
}

void app_main() {
    // Define I2C bus configurations
    I2CBusConfig bus1Config = {
        .i2cPort = I2C_NUM_0,
        .sensorEvents = {SENSOR1_EVENT, SENSOR2_EVENT},
        .sensorIntervals = {SENSOR1_INTERVAL, SENSOR2_INTERVAL},
        .sensorConfigs = {configSensor1, configSensor2},
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


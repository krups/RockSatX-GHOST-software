// i2c_manager.h
#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <driver/i2c.h>
#include <stdio.h>
#include "esp_mac.h"
//#include "BME280_sensor.h"
#include "Fake_Sensor.h"
#include "config.h" // Include config.h for MAX_SENSORS_PER_BUS

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

// Lookup table for sensor handlers
extern SensorHandler sensorHandlers[SENSOR_EVENT_COUNT];

extern const uint32_t sensorIntervals[SENSOR_EVENT_COUNT];

// Function pointer type for sensor configuration functions
typedef void (*SensorConfig)(void);

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

// Declare sensorIntervals as an external variable
extern const uint32_t sensorIntervals[SENSOR_EVENT_COUNT];

// Function prototypes
void addSensorEventToQueue(QueueHandle_t queue, SensorEventType eventType);
void sensorTimerCallback(TimerHandle_t xTimer);
void i2cBusTask(void *pvParameters);

#endif // I2C_MANAGER_H
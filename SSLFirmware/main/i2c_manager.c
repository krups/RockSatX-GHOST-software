// i2c_manager.c
#include "i2c_manager.h"


// Lookup table for sensor handlers
SensorHandler sensorHandlers[SENSOR_EVENT_COUNT] = {
    handleFakeSensor,
    handleFakeSensor, // Set sensor 2 as fake sensor
    handleFakeSensor, // Set sensor 3 as fake sensor
    handleFakeSensor, // Set sensor 4 as fake sensor
    handleFakeSensor  // Set sensor 5 as fake sensor
};

// Lookup table for sensor intervals
const uint32_t sensorIntervals[SENSOR_EVENT_COUNT] = {
    Fake_INTERVAL,
    Fake_INTERVAL,
    Fake_INTERVAL,
    Fake_INTERVAL,
    Fake_INTERVAL
};

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
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
extern "C" {
#include "BucketManager.h"
}
#include <string.h>
#include "Arduino.h"
//included arduino libraries
//IridiumSBD -- builds
//Adafruit_BNO086 -- builds
//BME280 -- builds
//Adafruit_I2C
//Adafruit_SPI



extern "C" {
void app_main() {
   
    

    // I2CBusConfig bus2Config = {
    //     .i2cPort = I2C_NUM_1,
    //     .sensorEvents = {SENSOR3_EVENT, SENSOR4_EVENT, SENSOR5_EVENT},
    //     .sensorIntervals = {sensorIntervals[SENSOR3_EVENT], sensorIntervals[SENSOR4_EVENT], sensorIntervals[SENSOR5_EVENT]},
    //     .sensorConfigs = {configFakeSensor, configFakeSensor, configFakeSensor}, // Set sensors 3-5 config as fake sensors
    //     .sensorCount = 3
    // };

    // Create event queues for each bus
    // bus1Config.eventQueue = xQueueCreate(10, sizeof(SensorEventType));
    // bus2Config.eventQueue = xQueueCreate(10, sizeof(SensorEventType));

    // Create mutexes for each bus
    // bus1Config.mutex = xSemaphoreCreateMutex();
    // bus2Config.mutex = xSemaphoreCreateMutex();

    // Create tasks for each I2C bus
    // xTaskCreate(i2cBusTask, "I2CBusTask1", 2048, &bus1Config, 5, NULL);
    // xTaskCreate(i2cBusTask, "I2CBusTask2", 2048, &bus2Config, 5, NULL);
    
    
    
    
}

}
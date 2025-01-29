//SSL Firmware
//Firmware for all current SSL missions and devices
//Developed for RockSat-X GHOST
//Based on Matt Ruffner's KREPE 2 firmware
//Developed by Hersch Nathan, Alex Barrera
#include <string.h>
#include "config.h" 
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h" 
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <driver/i2c.h>
#include <stdio.h>
#include "Arduino.h"
//included arduino libraries
//IridiumSBD -- builds
//Adafruit_BNO086 -- builds
//BME280 -- builds
//Adafruit_BusIO -- builds
//Adafruit_Sensor --builds
//Adafruit_I2C
//Adafruit_SPI

extern "C" {
#include "BucketManager.h"
}
#include "Adafruit_BME280.h"
#include "Adafruit_BNO08x.h"
#include "Honeywell_ABP.h"
#include "IridiumSBD.h"





void app_main() {
    //for i2c bus it defaults to the &wire defined in arduino wire library
    
    Serial.begin(9600);

    TwoWire theWire = TwoWire(0);
    
    Serial.println("freak");

    Adafruit_BME280 BME280;
    BME280.begin(96,&theWire);
    
    // this technically says that the reset pin is on 0 but that is just a filler value for now
    Adafruit_BNO08x BNO = Adafruit_BNO08x(0);
    BNO.begin_I2C(74,&theWire,1);

    IridiumSBD modem();
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

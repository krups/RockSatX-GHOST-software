// this file is solely for testing purposes and will not be flown it will also be where the overall software testing script will be
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
void app_main(){
    
}
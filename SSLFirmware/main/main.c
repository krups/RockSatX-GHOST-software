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


void app_main() {
  
}


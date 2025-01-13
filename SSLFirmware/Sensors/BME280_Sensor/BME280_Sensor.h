// Sensors/BME280_sensor.h
#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include <stdint.h>
#include <driver/i2c.h>
#include "BME280_SensorAPI/bme280.h"
#include "BME280_SensorAPI/bme280_defs.h"
#include "config.h"
#include <stdio.h>

// Function prototypes
void configBME280(void);
void handleBME280(void);

#endif // BME280_SENSOR_H
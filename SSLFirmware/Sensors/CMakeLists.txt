# List all components within the Sensor folder

idf_component_register(SRCS "Fake_Sensor/Fake_Sensor.c" "BME280_Sensor/BME280_Sensor.c"  "BME280_Sensor/BME280_SensorAPI/bme280.c" 
                        INCLUDE_DIRS "Fake_Sensor" "BME280_Sensor" 
                        PRIV_REQUIRES driver
                        REQUIRES main)


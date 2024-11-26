# List all components within the Sensor folder

idf_component_register(SRCS "BME280_Sensor.c" "BME280_SensorAPI/bme280.c"
                        INCLUDE_DIRS "." "BME280_SensorAPI" 
                        PRIV_REQUIRES driver
                        REQUIRES main) 


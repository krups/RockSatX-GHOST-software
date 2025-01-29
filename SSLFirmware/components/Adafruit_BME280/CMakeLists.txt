
idf_component_register(SRCS "src/Adafruit_BME280.cpp"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES arduino-esp32 Adafruit_Sensor
                       REQUIRES driver main)
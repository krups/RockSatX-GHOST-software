idf_component_register(SRCS "SSLFirmware.c"
                    INCLUDE_DIRS "." 
                    INCLUDE_DIR "Sensors"
                    REQUIRES BME280_Sensor
                    REQUIRES Fake_Sensor)

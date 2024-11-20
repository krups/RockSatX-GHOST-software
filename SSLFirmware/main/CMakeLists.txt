set(srcs "SSLFirmware.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver
                    INCLUDE_DIRS "Sensors"
                                 "Managers" 
                                 "Sensors/BME280_Sensor" 
                                 "Managers/i2c_manager"
                                 "Sensors/Fake_Sensor")

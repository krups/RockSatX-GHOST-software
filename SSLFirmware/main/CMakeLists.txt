set(srcs "SSLFirmware.c" "i2c_manager.c" "Fake_Sensor.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver)

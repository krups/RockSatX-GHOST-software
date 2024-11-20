idf_component_register(SRCS "i2c_manager.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver
                    INCLUDE_DIRS "Sensors"
                                 "Managers"  
                                 "Managers/i2c_manager")

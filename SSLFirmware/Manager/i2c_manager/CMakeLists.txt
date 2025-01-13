idf_component_register(SRCS "i2c_manager.c"
                    INCLUDE_DIRS "."
                    REQUIRES Sensors main
                    PRIV_REQUIRES driver )
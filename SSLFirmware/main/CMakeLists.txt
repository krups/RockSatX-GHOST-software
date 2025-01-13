set(srcs "SSLFirmware.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver
                    REQUIRES Manager Sensors)

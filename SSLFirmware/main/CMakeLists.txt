

idf_component_register(SRCS "main_capsule.cpp"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver
                    REQUIRES BucketManager arduino-esp32)

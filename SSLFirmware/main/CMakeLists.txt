set(srcs "SSLFirmware.c" "BucketManager.c" "BucketManager.h" )
idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver)

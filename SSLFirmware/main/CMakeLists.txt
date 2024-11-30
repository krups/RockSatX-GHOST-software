set(srcs "SSLFirmware.cpp" "BucketManager.cpp" "BucketManager.h" )
idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES driver)

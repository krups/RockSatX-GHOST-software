set(requires
    BucketManager
    arduino-esp32
    Adafruit_BME280
    Adafruit_BNO08x
    Adafruit_BusIO
    Adafruit_Sensor
    Honeywell_ABP
    IridiumSBD
    H3LIS100)

idf_component_register(SRCS "main_capsule.cpp"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES driver
                       REQUIRES ${requires})

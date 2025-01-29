
idf_component_register(SRCS "src/Adafruit_BNO08x.cpp" "src/sh2_SensorValue.c" "src/sh2_util.c" "src/sh2.c" "src/shtp.c"
                       INCLUDE_DIRS "src"
                       PRIV_REQUIRES arduino-esp32 Adafruit_BusIO Adafruit_Sensor
                       REQUIRES driver main)
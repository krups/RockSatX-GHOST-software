# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(SSLFirmware)

# Add subdirectories
add_subdirectory(main)
add_subdirectory(components/BME280_SensorAPI)

# Add your source files here
set(SOURCES
    main.c
    Managers/i2c_manager.c
    Sensors/BME280_sensor.c
    Libraries/BME280_SensorAPI/bme280.c
    # Add other source files as needed
)

# Add executable
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS .
                       INCLUDE_DIRS main
                       INCLUDE_DIRS main/Managers
                       INCLUDE_DIRS main/Sensors)
# List all components within the Manager folder

idf_component_register(SRCS "Fake_Sensor/Fake_Sensor" 
                        INCLUDE_DIRS "Fake_Sensor"
                        PRIV_REQUIRES driver)

#idf_component_register(SRCS "BME280_Sensor/BME280_Sensor" 
 #                       INCLUDE_DIRS "BME280_Sensor"
  #                      PRIV_REQUIRES driver)


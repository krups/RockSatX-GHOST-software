// Defines which board we are compiling for
// Written by: Hersch Nathan

// current options are:
// Kentucky Flight Computer for KANGS (KFC_KANGS)
// Fempto Sats for KANGS (Fempto_KANGS)
// Rocketstation Transmitter for Kangs (RST_KANGS)
// Ground Sation (GS)


//Constant Addresses are defined in this file

// configre format:
// if(current option)
//   define if i2c is enabled
//   define if spi is enabled
//   define if sensors are enabled
//   define if which communicatio is enabled
//   define usb pins
//   define I2C pins
//   define I2C devices
//   define SPI pins
//   define SPU devices
//   define Special Pins for different devices
//
//   for each group define pins and define which sensors are available

#ifndef CONFIG_H
#define CONFIG_H

#define FULL_FEATURE_DEBUG 

// Define the maximum number of sensors per bus
#define MAX_SENSORS_PER_BUS 5

// Constant I2C Addresses
    //Move to each sensor file
#define BME280_I2C_ADDRESS 0x76
#define BN0066_I2C_ADDRESS 0x4A

#define CONFIG_I2CDEV_TIMEOUT 1000

#if defined(KFC_KANGS)
    // Define sensor polling intervals in milliseconds
    #define SENSOR1_INTERVAL 1000
    #define SENSOR2_INTERVAL 2000
    #define SENSOR3_INTERVAL 1500
    #define SENSOR4_INTERVAL 2500
    #define SENSOR5_INTERVAL 3000

    #define BME280_SETTINGS_SEL (BME280_PRESS | BME280_TEMP | BME280_HUM | BME280_FILTER_COEFF_16 | BME280_STANDBY)

#elif defined(Fempto_KANGS)
    // Define sensor polling intervals in milliseconds
    #define SENSOR1_INTERVAL 1000
    #define SENSOR2_INTERVAL 2000
    #define SENSOR3_INTERVAL 1500
    #define SENSOR4_INTERVAL 2500
    #define SENSOR5_INTERVAL 3000

    #define I2C1 
    #define SPI1 
   
    #define LORA 
    #define SENSORS 

    // USB Pins
    #define USB1_DP 14
    #define USB1_DN 13

    // I2C pins 
    #define SDA1_PIN 6
    #define SCL1_PIN 7

    // I2C Sensors
    #define BME280_I2C 1
    #define BNO086_I2C 1

    // SPI Pins
    #define MOSI1_PIN 18
    #define MISO1_PIN 19
    #define SCK1_PIN 17
	   #define NSS1_PIN 15
    
    // BN0086 Pins for 
    #define BNO086_HINTN 4
    #define BNO086_RESET 5

    // LoRa Pins
    #define LORA1_TXEN 39
    #define LORA1_RXEN 38
    #define LORA1_RESET 20
    #define LORA1_BUSY 21
    #define LORA1_DIO1 22
    #define LORA1_DIO2 23

    #define BME280_SETTINGS_SEL (BME280_PRESS | BME280_TEMP | BME280_HUM | BME280_FILTER_COEFF_16 | BME280_STANDBY)

#elif defined(RST_KANGS)
    // Define sensor polling intervals in milliseconds
    #define SENSOR1_INTERVAL 1000
    #define SENSOR2_INTERVAL 2000
    #define SENSOR3_INTERVAL 1500
    #define SENSOR4_INTERVAL 2500
    #define SENSOR5_INTERVAL 3000

	//communications
	#define I2C1
	#define SPI2
	#define UART1
	#define UART2
	#define UART4
	
	#define LORA
	#define SENSORS
	
	// USB pins
	#define USB1_DP 14
	#define USB1_DN 13
	
	//I2C pins
	#define SDA1_PIN 36
	#define SCL1_PIN 37
	
	// I2C sensors
	#define BME280_I2C 1
 #define BNO086_I2C 1
	
	// SPI pins
	#define MOSI1_PIN 18
	#define MISO1_PIN 19
	#define SCK1_PIN 17
	#define NSS1_PIN 15
	
	// BN0086 pins
	#define BNO086_HINTN 39
	#define BNO086_RESET 38
	
	// LoRa pins
	#define LORA1_TXEN 5
 #define LORA1_RXEN 4
 #define LORA1_RESET 20
 #define LORA1_BUSY 21
 #define LORA1_DIO1 22
 #define LORA1_DIO2 23
	
	// UART pins
	#define UART_TX1 6
	#define UART_TX2 7
	#define UART_TX4 10
	#define UART_RX4 11
	
	// radio pin
	#define TE_SENSE 8

#define BME280_SETTINGS_SEL (BME280_PRESS | BME280_TEMP | BME280_HUM | BME280_FILTER_COEFF_16 | BME280_STANDBY)

#elif defined(GS)
    // Define sensor polling intervals in milliseconds
    #define SENSOR1_INTERVAL 1000
    #define SENSOR2_INTERVAL 2000
    #define SENSOR3_INTERVAL 1500
    #define SENSOR4_INTERVAL 2500
    #define SENSOR5_INTERVAL 3000

    #define BME280_SETTINGS_SEL (BME280_PRESS | BME280_TEMP | BME280_HUM | BME280_FILTER_COEFF_16 | BME280_STANDBY)

#elif defined(FULL_FEATURE_DEBUG)
    // Define sensor polling intervals in milliseconds
    #define BME280_INTERVAL 1000
    #define Fake_INTERVAL 2000


    #define I2C1
    #define I2C2
    #define I2C3
    #define I2C4

    #define SPI1
    #define SPI2

	   #define UART1
	   #define UART2
	   #define UART4

    #define LORA
    #define SENSORS

    // USB Pins
    #define USB1_DP 14
    #define USB1_DN 13

    // I2C pins
    #define SDA1_PIN 21
    #define SCL1_PIN 22
    #define SDA2_PIN 25
    #define SCL2_PIN 26
    #define SDA3_PIN 27
    #define SCL3_PIN 32
    #define SDA4_PIN 33
    #define SCL4_PIN 34

    // I2C Sensors
    #define BME280_I2C 1
    #define BNO086_I2C 1

    // SPI Pins
    #define MOSI1_PIN 18
    #define MISO1_PIN 19
    #define SCK1_PIN 17
	   #define NSS1_PIN 15
    #define MOSI2_PIN 23
    #define MISO2_PIN 25
    #define SCK2_PIN 26

    // BN0086 Pins
    #define BNO086_HINTN 4
    #define BNO086_RESET 5

    // LoRa Pins
    #define LORA1_TXEN 39
    #define LORA1_RXEN 38
    #define LORA1_RESET 20
	
	  // UART pins
	  #define UART_TX1 6
	  #define UART_TX2 7
	  #define UART_TX4 10
	  #define UART_RX4 11
	
	  // radio pin
	  #define TE_SENSE 8

    #define BME280_SETTINGS_SEL (BME280_PRESS | BME280_TEMP | BME280_HUM | BME280_FILTER_COEFF_16 | BME280_STANDBY_TIME_0_5_MS)

#else
    #error "No valid configuration defined"
#endif
    



#endif

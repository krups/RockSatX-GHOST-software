## DEV BRANCH
This Branch is for testing ESP-IDF and try to get away from Arduino IDE
## RockSat-X Software

Software supporting the [RockSat-X Ghost flight computer, Femptosats, and Ground Station]([https://github.com/krups/RockSatX-Ghost-software]) RockSat-X Flight (Ghost) November 2025.

This codebase is built on the [KREPE Flight Computer (KFC)](https://github.com/krups/iss-software). The major change was updating the MCU to a ESP32-S3. The cobebase is being rewritten in ESP-IDF. See the ```doc``` folder for the full documentation.


### Firmware 
SSL firmware is in ```SSLFirmware``` folder.

Spectrometer subsystem (FeatherM0/M4) firmware is in ```BSMSFirmware``` folder.

Rocket Station (Raspberry Pi 5) firmware is in  ```RocketStationFirmware``` folder.

Shared code and parameters between these three firmwares is in ```src/``` directory.


### Data Analysis
Tools to decompress received iridium packets and convert them from binary format to CSV format are in the ```tools/``` directory.

### Hardware

The hardware that this code runs on is in seperate repositories. See Below.

[RockSatX GHOST Hardware](https://github.com/krups/RockSatX-GHOST-Hardware)

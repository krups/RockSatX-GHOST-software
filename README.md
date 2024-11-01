## RockSat-X Software

Software supporting the [RockSat-X Ghost flight computer, Femptosats, and Ground Station]([https://github.com/krups/RockSatX-Ghost-software]) RockSat-X Flight (Ghost) November 2025.

This codebase is built on the [KREPE Flight Computer (KFC)](https://github.com/krups/iss-software). The major change to the Flight Computer is changing the MCU and upgrading the LoRa module.

### Firmware 
Main flight computer (Feather M4 processor) firmware is in ```KREPE2Firmware``` folder.

Ground station firmware (Feather M0) processor is in ```GroundstationFirmware``` folder.

Spectrometer subsystem (FeatherM0/M4) firmware is in ```BSMSFirmware``` folder.

FemptoSats  (ESP32-S3-WROOM) is in  ```FemptoSatsFirmware``` folder.

Shared code and parameters between these three firmwares is in ```src/``` directory.

Groundstation Image processing is in the ```GoundstationImage``` folder.

### Data Analysis
Tools to decompress received iridium packets and convert them from binary format to CSV format are in the ```tools/``` directory.

### Hardware

The hardware that this code runs on is in seperate repositories. See Below.

[FemptoSats hardware](https://github.com/krups/KRUPS-FemtoSat)

# FoodTags: Ultra-Low power tags for IoT food monitoring

FoodTags is meant to be a platform for suppliers and consumers to be able to estimate the status of food by measuring properties of the food storage environment using batteryless IoT sensors. These sensors are powered using energy harvesting from the RF waves being sent by a reciever, such as a smartphone. 

FoodTags can currently use analog thermistors/photoresistors to measure temperature and illuminance respectively.

## Software: Code Composer 10 Project for the embedded software on the MSP430G2553.

### This code can:
- Work in ultra-low power settings.
- Scan the thermistor and photoresistor using the 10 bit ADC at the lowest reasonable clock setting
- Send NFC NDEF messages with all of the stored measurements on the device.
- Use a custom algorithm to save samples in a circular queue without needing to rely on information that needs to be written on every NFC detection.

## AlternativeDesign: Folder that contains the LTspice files and informational diagram of the AM transmitter design.

### This includes:
- A BOM of around < 0.50$ in component parts.
- Image of simulation results.
- All LTspice files used for developing this design.

## Hardware: Folder that contains the PCB and schematic files for the current iteration of the FoodTags hardware.

### This includes:
- A BOM for the current design.
- PCB design files in both JSON (EasyEDA) and Gerber for manufacturing.
- Schematic files as both an image and JSON (EasyEDA) file.

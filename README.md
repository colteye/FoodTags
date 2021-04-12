# FoodTags: Ultra-Low power tags for IoT food monitoring

FoodTags is meant to be a platform for suppliers and consumers to be able to estimate the status of food using batteryless IoT sensors. These sensors are powered using energy harvesting from the RF waves being sent by a reciever, such as a smartphone. 

It can currently use analog thermistors/photoresistors to measure temperature and illuminance respectively.

## FoodTagsApp: Folder that contains the source code for the FoodTags iOS application.

### This app allows you to:
- Scan FoodTags using the NFC NDEF protocol
- Measure and visualize the temperature (in Fahrenheit) and illuminance (in Lux) e of the tag's immediate area

<img src="https://user-images.githubusercontent.com/6888483/114378631-e61b3400-9b55-11eb-9812-6d40a019fdf4.PNG" width="50%" height="50%">

## FoodTagsHardware: Folder that contains the source code and hardware design files for the FoodTag itself.
### This includes:
- PCB and Circuit Schematics
- Embedded C code for the MSP430G2553 and RF430CL330H
- An alternative LTspice design for a low power (maximum current draw of 4.8 mA @ 3V) AM transmitter version of FoodTags with much lower BOM costs.

<img src="https://user-images.githubusercontent.com/6888483/114380420-106df100-9b58-11eb-8701-e8c3cd75c9e1.PNG" width="70%" height="70%">

## Main Contributors: Erik Coltey, Regina Monteavaro

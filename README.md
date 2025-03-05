# Project VC 5

Note: This project is the base for my upcoming project VC 5 and won't be updated anymore. Please see ... for an up-to-date version.


## General description
The aim of this project is to create an external device, which allows the user to adjust a variety of settings (e.g. volume, display brightness, settings within a program, ...) on a host PC. 

## Hardware
A number of rotaries are available as input (number set to 5 within the config file of this project) and LED's and displays to show any kind of status.
The schematic for the device can be found at _Harware/schematic_.

## Software
The software consists of two parts.

### 1. Firmware
The firmware is written for the Raspberry Pi RP2040. It communicates via USB with the host PC. The firmware only transmits data from the rotaries to the host PC and executes commands from the host PC. These commands can for example set the color of the LED and control the content shown on the displays.


### 2. PC application
  The PC application (only Windows) acts as the executing intelligence. It takes the data from the rotaries and processes it. In this case, as a demo, the volume of five different artificial audio channels is controlled. The application can also send commands to the device to e.g. set the LED color or write information on the displays.

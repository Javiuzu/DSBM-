# DSBM- System Design Based on Microcomputers

Project for the subject of System Design Based on Microcomputers at Facultat d'Informàtica de Barcelona, UPC. 

This project uses a Raspberry Pi, a Arduino and a PIC to read different values of a Gyroscope, a touch screen and a ultrasonic sensor, the Raspberry stores the values of the sensors on a file, and a script uploads them to a server. 

The Raspberry reads the screen values and writes the stored sensor values on it, also hosts it to the server, and is connected via the GPIO ports to the microchip and by bluetooth to the Arduino.

The Arduino is the one tasked to read the values of the ultrasonic sensor, and send it to the Raspberry, while the PIC24F is tasked with reading the gyroscope and sending the values using an SPI communication to the Raspberry-

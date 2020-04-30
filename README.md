# PES_Project6
#### Katherine Hemzacek

## Project Description
This project utilizes FreeRTOS and Kinetis SDK functions to output, read in, and process voltages in a sine wave function. The program generates a lookup table of register values which are used to output a sine wave on the DAC output. These voltages are then read via the ADC input channel. The values are read into a circular buffer until the buffer is full, at which point a DMA transfer of the data is initiated. After being transferred, the data is processed to determine minimum, maximum, average, and standard deviation values for the set of data. These values can then be displayed on a PC terminal program.

## Repo Contents
This repository contains Program 1, which validates the lookup table creation and DAC functionality, and Program 2, which builds off of Program 1 to read in the voltages and process the data.

This repository contains all project files from these programs in MCUXpresso. All source code written for the project can be found in the folder "source".

## Installation and Execution Notes
The contents of the either program folder can be imported to MCUXpresso as a new project in the MCUXpresso IDE.

The project was created to be run on an NXP KL25Z development board. The board should be connected to a PC, and a PC terminal program should be used to establish a connection with the board's UART port. The PC terminal program should be set up for a baud rate of 115200, 8 bits, no partity bits, no start bit, one stop bit.
 

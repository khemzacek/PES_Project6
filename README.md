# PES_Project6
#### Katherine Hemzacek

## Project Description
This project utilizes FreeRTOS and Kinetis SDK functions to output, read in, and process voltages in a sine wave function. The program generates a lookup table of register values which are used to output a sine wave on the DAC output. These voltages are then read via the ADC input channel. The values are read into a circular buffer until the buffer is full, at which point a DMA transfer of the data is initiated. After being transferred, the data is processed to determine minimum, maximum, average, and standard deviation values for the set of data. These values can then be displayed on a PC terminal program.

## Repo Contents
This repository contains Program 1, which validates the lookup table creation and DAC functionality, and Program 2, which builds off of Program 1 to read in the voltages and process the data.

This repository contains all project files from these programs in MCUXpresso. All source code written for the project can be found in the folder "source".

## Observations
Several design decisions in this project involved implementing the required math. To create the lookup table, the some function in the standard math library was used. Because this table was only created once, before tasks with tighter time constraints were started, the extra processing time and power made sense. Once in the RTOS, processing data as it came in, as much math as possible was done on raw, integer register values. This saved time and processing power as the data was streaming in.

In terms of prioritizing tasks, for this application, the tasks were relatively ordered, rather than certain tasks being much higher priority. Therefore, only the signal processing task was set to have a lower priority (because the deadlines for processing the data were farther spread apart). To manage ordering the ordered tasks, each one successively resumed the next task. The other priority management was related to interrupts, which always execute before any FreeRTOS tasks. These were intentionally kept as short as possible.

## Installation and Execution Notes
The contents of the either program folder can be imported to MCUXpresso as a new project in the MCUXpresso IDE.

The project was created to be run on an NXP KL25Z development board. The board should be connected to a PC, and a PC terminal program should be used to establish a connection with the board's UART port. The PC terminal program should be set up for a baud rate of 115200, 8 bits, no partity bits, no start bit, one stop bit.
 

/*
 * LED_control.h
 *
 *  Created on: Mar 10, 2020
 *      Author: Katherine
 */

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#include "MKL25Z4.h"

// referenced https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_2/Source/main.c
// define LED pins/ports
#define R_LED_PORT PTB //Port B
#define R_LED_PIN (18)
#define G_LED_PORT PTB //Port B
#define G_LED_PIN (19)
#define B_LED_PORT PTD //Port D
#define B_LED_PIN (1)

// macro to help mask right pin
#define MASK(x) (1UL << (x))


// enum to track color being turned on
enum LEDcolor{red, green, blue};
extern enum LEDcolor color;


/* @brief Initialize LED pins
 * taken from https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_2/Source/main.c
 */
void LED_init(void);

/* @brief Turns on particular LED color
 * referenced https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_2/Source/main.c
 */
void LED_on(enum LEDcolor color);

/* @brief Turns off LEDs
 * referenced https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_2/Source/main.c
 */
void LED_off(enum LEDcolor color);

/* @brief Toggles LEDs
 * referenced https://github.com/alexander-g-dean/ESF/blob/master/Code/Chapter_2/Source/main.c
 */
void LED_toggle(enum LEDcolor color);

#endif /* LED_CONTROL_H_ */


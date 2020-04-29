/*
 * delay.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Katherine
 *
 *      https://github.com/alexander-g-dean/ESF/blob/master/NXP/Code/Chapter_8/Serial-Demo/src/delay.c
 */

#include <MKL25Z4.H>

void Delay (uint32_t dly) {
  volatile uint32_t t;

	for (t=dly*10000; t>0; t--)
		;
}

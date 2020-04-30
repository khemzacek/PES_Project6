/*
 * LED_control.c
 *
 *  Created on: Mar 10, 2020
 *      Author: Katherine
 */
#include "LED_control.h"

GPIO_Type *LEDport;
uint32_t LEDpin;

void LED_init(void)
{
	//Enable clock to Port B and Port D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	//Make 3 pins (to RGB LEDs) GPIO
	PORTB->PCR[R_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[R_LED_PIN] |= PORT_PCR_MUX(1);
	PORTB->PCR[G_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[G_LED_PIN] |= PORT_PCR_MUX(1);
	PORTD->PCR[B_LED_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[B_LED_PIN] |= PORT_PCR_MUX(1);

	//Set pins to outputs
	PTB->PDDR |= MASK(R_LED_PIN) | MASK(G_LED_PIN);
	PTD->PDDR |= MASK(B_LED_PIN);

	PTB->PCOR |= MASK(R_LED_PIN) | MASK(G_LED_PIN);
	PTD->PCOR |= MASK(B_LED_PIN);

	//Turn all off to start
	R_LED_PORT->PSOR = MASK(R_LED_PIN);
	G_LED_PORT->PSOR = MASK(G_LED_PIN);
	B_LED_PORT->PSOR = MASK(B_LED_PIN);
#ifdef PROGRAM_DEBUG
    printf("LEDs Initialized\r\n;")
#endif
}

void LED_on(enum LEDcolor color)
{
	// set port/pin for right color
	if(color == red){
		LEDport = R_LED_PORT;
		LEDpin = R_LED_PIN;
	}else if(color == green){
		LEDport = G_LED_PORT;
		LEDpin = G_LED_PIN;
	}else if(color == blue){
		LEDport = B_LED_PORT;
		LEDpin = B_LED_PIN;
	}

	// turn on that color pin
	LEDport->PCOR = MASK(LEDpin);

}

void LED_off(enum LEDcolor color){
	// set port/pin for right color
	if(color == red){
		LEDport = R_LED_PORT;
		LEDpin = R_LED_PIN;
	}else if(color == green){
		LEDport = G_LED_PORT;
		LEDpin = G_LED_PIN;
	}else if(color == blue){
		LEDport = B_LED_PORT;
		LEDpin = B_LED_PIN;
	}

	// turn off that color pin
	LEDport->PSOR = MASK(LEDpin);

}

void LED_toggle(enum LEDcolor color){
	// set port/pin for right color
	if(color == red){
		LEDport = R_LED_PORT;
		LEDpin = R_LED_PIN;
	}else if(color == green){
		LEDport = G_LED_PORT;
		LEDpin = G_LED_PIN;
	}else if(color == blue){
		LEDport = B_LED_PORT;
		LEDpin = B_LED_PIN;
	}

	// toggle that color pin
	LEDport->PTOR = MASK(LEDpin);

}

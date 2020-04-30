/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*  This program utilized
 * 	- the FreeRTOS Hello World demo application
 * 	- the FreeRTOS swtimer demo application
 * 	- the Kinetis dac_adc demo application
 */


/**
 * @file    Program1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_dac.h"
#include "fsl_adc16.h"
#include "fsl_device_registers.h"
#include "LED_control.h"
#include "delay.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
//#define PROGRAM_DEBUG	//define for debug, disable for normal


/* The software timer period. */
#define SW_TIMER_PERIOD_MS (100 / portTICK_PERIOD_MS)	// 0.1 sec
/* Task priorities. */
#define DAC_task_PRIORITY (configMAX_PRIORITIES - 1)
#define ADCstart_task_PRIORITY (configMAX_PRIORITIES - 3)
#define ADCread_task_PRIORITY (configMAX_PRIORITIES - 2)
/* Task Handles */
TaskHandle_t xDACtaskHandle  = NULL;
TaskHandle_t xADCstartTaskHandle  = NULL;
TaskHandle_t xADCreadTaskHandle  = NULL;
/* DAC defines */
#define DEMO_DAC_BASEADDR DAC0

#define DAC_1_0_VOLTS 1241U
#define DAC_1_5_VOLTS 1862U
#define DAC_2_0_VOLTS 2482U
#define DAC_2_5_VOLTS 3103U
#define DAC_3_0_VOLTS 3724U

#define PI	3.14159265
#define DAC_levels	4096.0
#define DAC_max_volt 3.300

/* ADC defines */
#define DEMO_ADC16_BASEADDR ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 0U /* PTE20, ADC0_SE0 */

#define DEMO_ADC16_IRQn ADC0_IRQn
#define DEMO_ADC16_IRQ_HANDLER_FUNC ADC0_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* The callback function. */
static void SwTimerCallback(TimerHandle_t xTimer);
/* DAC task */
static void DAC_task(void *pvParameters);
/* ADC start task */
static void ADCstart_task(void *pvParameters);
/* ADC read task */
static void ADCread_task(void *pvParameters);
/* Initialize DAC */
static void InitDAC(void);
/* Initialize ADC16 */
static void InitADC(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
// Sine lookup table
uint32_t g_sine_table[50];
uint8_t g_index = 0;

// ADC variables
volatile bool g_Adc16ConversionDoneFlag = false;
volatile uint32_t g_Adc16ConversionValue = 0;
adc16_channel_config_t g_adc16ChannelConfigStruct;
uint32_t g_ADC_buffer[64];

uint32_t g_DSP_buffer[64];

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initializes DAC peripheral
 * from Kinetis SDK dac_adc demo application
 */
static void InitDAC(void)
{
    dac_config_t dacConfigStruct;

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */
#ifdef PROGRAM_DEBUG
    printf("DAC Initialized\r\n");
#endif

}

/*!
 * @brief Initializes ADC peripheral
 * from Kinetis SDK dac_adc demo application
 */
static void InitADC(void)
{
	adc16_config_t adc16ConfigStruct;

	/* Configure the ADC16. */
	    /*
	     * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
	     * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
	     * adc16ConfigStruct.enableAsynchronousClock = true;
	     * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
	     * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
	     * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
	     * adc16ConfigStruct.enableHighSpeed = false;
	     * adc16ConfigStruct.enableLowPower = false;
	     * adc16ConfigStruct.enableContinuousConversion = false;
	     */
	    ADC16_GetDefaultConfig(&adc16ConfigStruct);
	#if defined(BOARD_ADC_USE_ALT_VREF)
	    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
	#endif
	    ADC16_Init(DEMO_ADC16_BASEADDR, &adc16ConfigStruct);

	    /* Make sure the software trigger is used. */
	    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASEADDR, false);
	#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
	    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASEADDR))
	    {
	        PRINTF("\r\nADC16_DoAutoCalibration() Done.");
	    }
	    else
	    {
	        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
	    }
	#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

	    /* Prepare ADC channel setting */
	    g_adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
	    g_adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true;

	#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
	    g_adc16ChannelConfigStruct.enableDifferentialConversion = false;
	#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
}

/*!
 * @brief Defines values in sine lookup table
 * sine wave 1V to 3V, no phase shift, period of 5 sec, 0.1 sec steps
 * voltages converted to 12-bit, 0-3.3V DAC register values
 * table is one cycle
 */
void Define_SineTable(void){
    double voltage;
    double time = 0;

    for(uint8_t i = 0; i < 50; i++){
    	voltage = sin((2*PI/5)*time) + 2;	// calculate voltage (sine function)
    	g_sine_table[i] = (uint32_t)(voltage*DAC_levels/DAC_max_volt);	//convert to DAC reg values

    	time =+ time + 0.1;	//increment time
    }
#ifdef PROGRAM_DEBUG
    printf("Sine Lookup Table Defined\r\n");
#endif
}

/*!
 * @brief ADC Interrupt Handler
 * from Kinetis SDK dac_adc demo application
 */
void DEMO_ADC16_IRQ_HANDLER_FUNC(void)
{
	/* Read conversion result to clear the conversion completed flag. */
	g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP);
	//after ADC conversion is complete...
    xTaskResumeFromISR(xADCreadTaskHandle);
}

/*!
 * @brief Main function
 */
int main(void) {

    TimerHandle_t SwTimerHandle = NULL;

  	/* Init system. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    SystemCoreClockUpdate();
#ifdef PROGRAM_DEBUG
    printf("Board Initialized\r\n");
#endif
    LED_init();
    InitDAC();
    InitADC();

    Define_SineTable();

#ifdef PROGRAM_DEBUG
    printf("Cycle known DAC values\r\n");
    // Verify DAC output
    printf("1.0 V\r\n");
    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_1_0_VOLTS);
    Delay(3000);
    printf("1.5 V\r\n");
    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_1_5_VOLTS);
    Delay(3000);
    printf("2.0 V\r\n");
    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_2_0_VOLTS);
    Delay(3000);
    printf("2.5 V\r\n");
    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_2_5_VOLTS);
    Delay(3000);
    printf("3.0 V\r\n");
    DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, DAC_3_0_VOLTS);
    Delay(3000);
#endif


    /* Create DAC Task */
    // modified from hello_world task in demo app
    xTaskCreate(DAC_task,		// function that implements task
    		"DAC_task", 		// human readable task name
			configMINIMAL_STACK_SIZE + 50, // task stack size (words)
			NULL, 			// task parameters (pointer to them)
			DAC_task_PRIORITY, 	// task priority
			&xDACtaskHandle);		//task handle (used for suspending and resuming)
    vTaskSuspend(xDACtaskHandle);
    /* Create ADCstart Task */
    // modified from hello_world task in demo app
    xTaskCreate(ADCstart_task,
    		"ADCstart_task",
			configMINIMAL_STACK_SIZE + 100,
			NULL,
			ADCstart_task_PRIORITY,
			&xADCstartTaskHandle);
    vTaskSuspend(xADCstartTaskHandle);
    /* Create ADCread Task */
    // modified from hello_world task in demo app
    xTaskCreate(ADCread_task,
    		"ADCread_task",
			configMINIMAL_STACK_SIZE + 100,
			NULL,
			ADCread_task_PRIORITY,
			&xADCreadTaskHandle);
    vTaskSuspend(xADCreadTaskHandle);
    /* Create the software timer. */
    // from FreeRTOS swtimer demo application
    SwTimerHandle = xTimerCreate("SwTimer",          /* Text name. */
    		SW_TIMER_PERIOD_MS, /* Timer period. */
			pdTRUE,             /* Enable auto reload. */
			0,                  /* ID is not used. */
			SwTimerCallback);   /* The callback function. */
    /* Start timer. */
#ifdef PROGRAM_DEBUG
    printf("Starting Timer\r\n");
#endif
    xTimerStart(SwTimerHandle, 0);
    /* Start scheduling. */
#ifdef PROGRAM_DEBUG
    printf("Starting Scheduler\r\n");
#endif
    EnableIRQ(DEMO_ADC16_IRQn);
    vTaskStartScheduler();
    for (;;)
    	;
}


/*!
 * @brief Task responsible for updating DAC output
 */
static void DAC_task(void *pvParameters)
{
    for (;;)
    {
    	// set DAC output (according to lookup table)
    	DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, g_sine_table[g_index]);
    	LED_toggle(green);
    	// print DAC register value
    	//PRINTF("%d\r\n", g_sine_table[g_index]);
    	vTaskResume(xADCstartTaskHandle); // Start ADC read
    	vTaskSuspend(NULL);		// Task suspends itself after finishing
    }
}

/*!
 * @brief Task responsible for starting ADC read
 */
static void ADCstart_task(void *pvParameters)
{
    for (;;)
    {
    	// start read
    	g_Adc16ConversionDoneFlag = false;
    	ADC16_SetChannelConfig(DEMO_ADC16_BASEADDR, DEMO_ADC16_CHANNEL_GROUP, &g_adc16ChannelConfigStruct);
    	vTaskSuspend(NULL);		// Task suspends itself after finishing
    }
}

/*!
 * @brief Task responsible for getting ADC value
 */
static void ADCread_task(void *pvParameters)
{
    for (;;)
    {
    	static uint8_t ADC_index = 0;

    	g_ADC_buffer[ADC_index] = g_Adc16ConversionValue;
    	printf("%d\r\n", g_ADC_buffer[ADC_index]);

    	if(ADC_index < 63){
    		ADC_index++;
    	}else{
    		ADC_index = 0;
    	}

    	vTaskSuspend(NULL);		// Task suspends itself after finishing
    }
}

/*!
 * @brief Software timer callback.
 */
static void SwTimerCallback(TimerHandle_t xTimer)
{
	// increment index each 0.1 seconds
	if(g_index < 49){
    	g_index++;
    }else{
    	g_index = 0;
#ifdef PROGRAM_DEBUG
    printf("Cycle Done\r\n");
#endif
    }
    vTaskResume(xDACtaskHandle);		// DAC_task is resumed with each tick
}

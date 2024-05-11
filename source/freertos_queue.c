/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*System includes.*/
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "LedSws_Drivers_Init.h"
#include "CAN.h"
#include "fsl_flexcan.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Globals
 ******************************************************************************/
/* Logger queue handle */
//static QueueHandle_t log_queue = NULL;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
static void write_task_1(void *pvParameters);
static void write_task_2(void *pvParameters);
static void ADC_Period_task(void *pvParameters);



/*******************************************************************************
 * Code
 ******************************************************************************/

volatile bool button1_pressed;
volatile bool button2_pressed;
static uint8_t Period;

void BOARD_SW2_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
    UpAdc();
	button1_pressed = 1;
}

void BOARD_SW3_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
    DownAdc();
	button2_pressed = 1;
}


/*!
 * @brief Main function
 */
int main(void)
{
    NVIC_SetPriority(CAN0_ORed_Message_buffer_IRQn, 5); /*Important to modify the priority*/
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CAN_Init();
    SwitchLed_Init();

    /* Initialize logger for 10 logs with maximum lenght of one log 20 B */

    if (xTaskCreate(ADC_Period_task, "WRITE_TASK_ADC_3", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

/*******************************************************************************
 * Application functions
 ******************************************************************************/

static void ADC_Period_task(void *pvParameters)
{
    static uint8_t Cnt = 0;
    while(1)
    {
    	
        if(Period <= Cnt)
        {
        	PRINTF("TASK_ADC.\r\n");
            Send_CAN_ADC();
            Cnt = 0;
        }
        else
        {
            Cnt++;
        }
        
        vTaskDelay(2u);
    }
}

void PeriodUpdate(uint8_t NewPeriod)
{
    Period = NewPeriod;
}

/*******************************************************************************
 * Logger functions
 ******************************************************************************/

/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "Functions.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void (*state)(); /*Pointer to function for state machine*/
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false; /*Switch 3 status*/
volatile bool g_ButtonPress_TWO = false; /*Switch 2 status*/

uint8_t TaskN = 0;
/*           Active, Function       , Time, Task   */
FIFO Task1 = {1    , BlinkRed       , 50  , STATE_1};
FIFO Task2 = {0    , BlinkGreen     , 75  , STATE_2};
FIFO Task3 = {1    , BlinkBlue      , 100 , STATE_3};
FIFO Task4 = {0    , BlinkYellow    , 125 , STATE_4};
FIFO Task5 = {1    , BlinkLightBlue , 150 , STATE_5};
FIFO Task6 = {1    , BlinkPurple    , 200 , STATE_6};
FIFO Tasks[5]; /*Array of tasks, here is the FIFO configured.*/


/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_SW_IRQ_HANDLER(void) /*Handler for SW3*/
{
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#else
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
#endif
    /* Change state of button. */
    g_ButtonPress = true;
    SDK_ISR_EXIT_BARRIER;
}

void BOARD_SW_IRQ_HANDLER_TWO(void) /*Handler for SW2*/
{
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    /* Clear external interrupt flag. */
    GPIO_GpioClearInterruptFlags(BOARD_SW_GPIO_TWO, 1U << BOARD_SW_GPIO_PIN_TWO);
#else
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BOARD_SW_GPIO_TWO, 1U << BOARD_SW_GPIO_PIN_TWO);
#endif
    /* Change state of button. */
    g_ButtonPress_TWO = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Define the init structure for the input switch pin */
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    /* Define the init structure for the output LED pin */
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();


/* Init input switch 3 GPIO. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
    GPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, kGPIO_InterruptFallingEdge);
#else
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
#endif

    EnableIRQ(BOARD_SW_IRQ);
    GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

/* Init input switch 2 GPIO. */
#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
	GPIO_SetPinInterruptConfig(BOARD_SW_GPIO_TWO, BOARD_SW_GPIO_PIN_TWO, kGPIO_InterruptFallingEdge);
#else
	PORT_SetPinInterruptConfig(BOARD_SW_PORT_TWO, BOARD_SW_GPIO_PIN_TWO, kPORT_InterruptFallingEdge);
#endif

    EnableIRQ(BOARD_SW_IRQ_TWO);
    GPIO_PinInit(BOARD_SW_GPIO_TWO, BOARD_SW_GPIO_PIN_TWO, &sw_config);


    /* Init output LED GPIO RED. */
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

    /* Init output LED GPIO GREEN. */
    GPIO_PinInit(BOARD_LED_GPIO_TWO, BOARD_LED_GPIO_PIN_TWO, &led_config);

    /* Init output LED GPIO BLUE. */
    GPIO_PinInit(BOARD_LED_GPIO_THREE, BOARD_LED_GPIO_PIN_THREE, &led_config);

    state = BlinkRed; /*Initial State*/
    /*Assing tasks to tasks array*/
    Tasks[0] = Task1;
    Tasks[1] = Task2;
    Tasks[2] = Task3;
    Tasks[3] = Task4;
    Tasks[4] = Task5;
    Tasks[5] = Task6;

    while (1)
    {   
        StateSelect(Tasks[TaskN].TaskNum);
    }
}

void BlinkRed(void)/*STATE_1*/
{
    uint64_t WaitTime = 200000u * Tasks[TaskN].Time;
    static uint64_t WaitCnt = 0;

	LED_GREEN_OFF();
	LED_BLUE_OFF();

    LED_RED_ON();

    PRINTF("\r\n STATE 1\r\n");

    while(WaitCnt <  WaitTime)
    {
        WaitCnt++;
    }
    WaitCnt = 0;
}

void BlinkGreen(void)/*STATE_2*/
{
    uint64_t WaitTime = 200000u * Tasks[TaskN].Time;
    static uint64_t WaitCnt = 0;  

	LED_RED_OFF();
	LED_BLUE_OFF();

    LED_GREEN_ON();


    PRINTF("\r\n STATE 2\r\n");
    while(WaitCnt <  WaitTime)
    {
        WaitCnt++;
    }
    WaitCnt = 0;

}

void BlinkBlue(void)/*STATE_3*/
{
    uint64_t WaitTime = 100000u * Tasks[TaskN].Time;
    static uint64_t WaitCnt = 0; 

	LED_RED_OFF();
	LED_GREEN_OFF();

    LED_BLUE_ON();

    PRINTF("\r\n STATE 3\r\n");
    while(WaitCnt <  WaitTime)
    {
        WaitCnt++;
    }
    WaitCnt = 0;
}

void BlinkYellow(void)/*STATE_4*/
{
    uint64_t WaitTime = 100000u * Tasks[TaskN].Time;
    static uint64_t WaitCnt = 0;

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();

    LED_RED_ON();
    LED_GREEN_ON();

    PRINTF("\r\n STATE 4\r\n");
    while(WaitCnt <  WaitTime)
    {
        WaitCnt++;
    }
    WaitCnt = 0;
}

void BlinkLightBlue(void)/*STATE_5*/
{
    uint64_t WaitTime = 100000u * Tasks[TaskN].Time;
    static uint64_t WaitCnt = 0;

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();

    LED_BLUE_ON();
    LED_GREEN_ON();
    PRINTF("\r\n STATE 5\r\n");
    while(WaitCnt <  WaitTime)
    {
        WaitCnt++;
    }
    WaitCnt = 0;
}

void BlinkPurple(void)/*STATE_6*/
{
    uint64_t WaitTime = 100000u * Tasks[TaskN].Time;
    static uint64_t WaitCnt = 0;

    LED_RED_OFF();
    LED_GREEN_OFF();
    LED_BLUE_OFF();

    LED_RED_ON();
    LED_BLUE_ON();

    PRINTF("\r\n STATE 6\r\n");
    while(WaitCnt <  WaitTime)
    {
        WaitCnt++;
    }
    WaitCnt = 0;
}

void StateSelect(STATES NextState) /*Function used to switch states.*/
{
    if( TaskN > STATE_6)
    {
        TaskN = 0;
    }
    switch(NextState)
        {
            case STATE_1:
            {
                state = Tasks[TaskN].state;
                if(Tasks[TaskN].IsActive == 1)/*Validate if the Task is active or not.*/
                {
                    state();
                }
                TaskN++;
                
            }               
            break;
            case STATE_2:
            {             
                state = Tasks[TaskN].state;
                if(Tasks[TaskN].IsActive == 1)
                {
                    state();
                }
                TaskN++;
                
            }
            break;
            case STATE_3:
            {
                state = Tasks[TaskN].state;
                if(Tasks[TaskN].IsActive == 1)
                {
                    state();
                }
                TaskN++;
               
            }
            break;
            case STATE_4:
            {
            	state = Tasks[TaskN].state;
                if(Tasks[TaskN].IsActive == 1)
                {
                    state();
                }
                TaskN++;
               
            }
            break;
            case STATE_5:
            {
            	state = Tasks[TaskN].state;
                if(Tasks[TaskN].IsActive == 1)
                {
                    state();
                }
                TaskN++;
                
            }
            break;
            case STATE_6:
            {
            	state = Tasks[TaskN].state;
                if(Tasks[TaskN].IsActive == 1)
                {
                    state();
                }        
                TaskN++;        
            }
            break; 
            default:
            {
                /*Out of range. For security return to default conditions*/
                //TaskN++;
                state = main;
                PRINTF("\r\n OUT OF STATES\r\n");
                state();
            }
            break;
        }
}

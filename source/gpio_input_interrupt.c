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
unsigned long Counter = 0;
unsigned long CurrentCounter = 0;
uint16_t Interval = 200; /*Blink Interval, change to toggle led to a faster frequency*/
uint8_t ChangeFlag = 0; /*Start flag. used to synchronize outputs when a state change is requested.*/

/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false; /*Switch 3 status*/
volatile bool g_ButtonPress_TWO = false; /*Switch 2 status*/
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

    while (1)
    {  
        if(__LONG_MAX__ > Counter) 
        {
            Counter++; /*Counter used to blink outputs.*/
        }
        else
        {
            Counter = 0; /*Counter reset to avoid overflows*/
            CurrentCounter = 0;
        }    
        state();/*Call pointed function*/
    }
}

void BlinkRed(void)/*STATE_1*/
{
	LED_GREEN_OFF();
	LED_BLUE_OFF();
    if( (Counter - CurrentCounter) >= Interval ) /*Toggle activated when the interval time is reached.*/
    {
    	LED_RED_TOGGLE();
        CurrentCounter = Counter;
    }
    PRINTF("\r\n STATE 1\r\n");
    PRINTF("Counter: %u\n",Counter);
    StateSelect(NextStateF());/*Call the next state.*/
}

void BlinkGreen(void)/*STATE_2*/
{
	LED_RED_OFF();
	LED_BLUE_OFF();
    if( (Counter - CurrentCounter) >= Interval )
    {
    	LED_GREEN_TOGGLE();
        CurrentCounter = Counter;
    }
    PRINTF("\r\n STATE 2\r\n");
    PRINTF("Counter: %u\n",Counter);
    StateSelect(NextStateF());
}

void BlinkBlue(void)/*STATE_3*/
{
	LED_RED_OFF();
	LED_GREEN_OFF();
    if( (Counter - CurrentCounter) >= Interval )
    {
    	LED_BLUE_TOGGLE();
        CurrentCounter = Counter;
    }
    PRINTF("\r\n STATE 3\r\n");
    PRINTF("Counter: %u\n",Counter);
    StateSelect(NextStateF());

}

void BlinkYellow(void)/*STATE_4*/
{
	if(0u == ChangeFlag)/*Condition used to synchronize outputs before the state starts*/
	{
		LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		ChangeFlag = 1;
	}

    if( (Counter - CurrentCounter) >= Interval )
    {
    	LED_RED_TOGGLE();
    	LED_GREEN_TOGGLE();
        CurrentCounter = Counter;
    }
    PRINTF("\r\n STATE 4\r\n");
    PRINTF("Counter: %u\n",Counter);
    StateSelect(NextStateF());

}

void BlinkLightBlue(void)/*STATE_5*/
{
	if(0u == ChangeFlag)
	{
		LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		ChangeFlag = 1;
	}

    if( (Counter - CurrentCounter) >= Interval )
    {
    	LED_BLUE_TOGGLE();
    	LED_GREEN_TOGGLE();
        CurrentCounter = Counter;
    }
    PRINTF("\r\n STATE 5\r\n");
    PRINTF("Counter: %u\n",Counter);
    StateSelect(NextStateF());
}

void BlinkPurple(void)/*STATE_6*/
{
	if(0u == ChangeFlag)
	{
		LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		ChangeFlag = 1;
	}

    if( (Counter - CurrentCounter) >= Interval )
    {
    	LED_RED_TOGGLE();
    	LED_BLUE_TOGGLE();
        CurrentCounter = Counter;
    }
    PRINTF("\r\n STATE 6\r\n");
    PRINTF("Counter: %u\n",Counter);
    StateSelect(NextStateF());

}

void StateSelect(STATES NextState) /*Function used to switch states.*/
{
    switch(NextState)
        {
            case STATE_1:
            {
                state = BlinkRed;
            }               
            break;
            case STATE_2:
            {             
                state = BlinkGreen;
            }
            break;
            case STATE_3:
            {
                state = BlinkBlue;
            }
            break;
            case STATE_4:
            {
            	if(state != BlinkYellow)
            	{
            		ChangeFlag = 0;
            	}
            	state = BlinkYellow;
            }
            break;
            case STATE_5:
            {
            	if(state != BlinkLightBlue)
            	{
            		ChangeFlag = 0;
            	}
            	state = BlinkLightBlue;
            }
            break;
            case STATE_6:
            {
            	if(state != BlinkPurple)
            	{
            		ChangeFlag = 0;
            	}
            	state = BlinkPurple;
            }
            break; 
            default:
            {
                /*Out of range. For security return to default conditions*/
                state = main;
                Counter = 0; /*Counter reset to avoid overflows*/
                CurrentCounter = 0;
                PRINTF("\r\n OUT OF STATES\r\n");
            }
            break;
        }
}

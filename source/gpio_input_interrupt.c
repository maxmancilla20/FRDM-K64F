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
/*           IsActive, Function       , Time, Task   */
FIFO Task1 = {1      , BlinkRed       , 50  , STATE_1, 6};
FIFO Task2 = {0      , BlinkGreen     , 75  , STATE_2, 2};
FIFO Task3 = {1      , BlinkBlue      , 100 , STATE_3, 3};
FIFO Task4 = {0      , BlinkYellow    , 125 , STATE_4, 6};
FIFO Task5 = {1      , BlinkLightBlue , 150 , STATE_5, 5};
FIFO Task6 = {1      , BlinkPurple    , 200 , STATE_6, 6};
FIFO Tasks[]; /*Array of tasks, here is the FIFO configured.*/
STATES PriorityOrder[] = {};
FIFO CopyTasks[]; /*Copy used to re ogranize the array in order to priority*/

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
    /*Assing taskN to tasks array*/
    Tasks[0] = Task1;
    Tasks[1] = Task2;
    Tasks[2] = Task3;
    Tasks[3] = Task4;
    Tasks[4] = Task5;
    Tasks[5] = Task6;
    CopyTasks[0] = Task1;
    CopyTasks[1] = Task2;
    CopyTasks[2] = Task3;
    CopyTasks[3] = Task4;
    CopyTasks[4] = Task5;
    CopyTasks[5] = Task6;
    PriorityTasks();
    while (1)
    {   
        StateSelect(CopyTasks[TaskN].TaskNum); /*Infinite loop that choose the next state.*/
        if(g_ButtonPress == true)
        {
            g_ButtonPress = false;
            TaskCreateDelete();           
        }
    }
}

void BlinkRed(void)/*STATE_1*/
{
    uint64_t WaitTime = 200000u * CopyTasks[TaskN].Time;
    static uint64_t WaitCnt = 0;

	LED_GREEN_OFF();
	LED_BLUE_OFF();

    LED_RED_ON();

    PRINTF("\r\n STATE 1\r\n");

    while(WaitCnt <  WaitTime) /*Delay*/
    {
        WaitCnt++;
    }
    WaitCnt = 0;
}

void BlinkGreen(void)/*STATE_2*/
{
    uint64_t WaitTime = 200000u * CopyTasks[TaskN].Time;
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
    uint64_t WaitTime = 100000u * CopyTasks[TaskN].Time;
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
    uint64_t WaitTime = 100000u * CopyTasks[TaskN].Time;
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
    uint64_t WaitTime = 100000u * CopyTasks[TaskN].Time;
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
    uint64_t WaitTime = 100000u * CopyTasks[TaskN].Time;
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
    if( TaskN > STATE_6) /*Threshold for TaskN*/
    {
        TaskN = 0;
    }

    switch(NextState)
    {
        case STATE_1:
        {
            state = CopyTasks[TaskN].state; /*Asigning function to be executed*/
            if(CopyTasks[TaskN].IsActive == 1)/*Validate if the Task is active or not.*/
            {
                state(); /*Execute function*/
            }
            TaskN++; /*Go to the next task*/   
        }               
        break;
        case STATE_2:
        {             
            state = CopyTasks[TaskN].state;
            if(CopyTasks[TaskN].IsActive == 1)
            {
                state();
            }
            TaskN++;                
        }
        break;
        case STATE_3:
        {
            state = CopyTasks[TaskN].state;
            if(CopyTasks[TaskN].IsActive == 1)
            {
                state();
            }
            TaskN++;              
        }
        break;
        case STATE_4:
        {
            state = CopyTasks[TaskN].state;
            if(CopyTasks[TaskN].IsActive == 1)
            {
                state();
            }
            TaskN++;             
        }
        break;
        case STATE_5:
        {
            state = CopyTasks[TaskN].state;
            if(CopyTasks[TaskN].IsActive == 1)
            {
                state();
            }
            TaskN++;               
        }
        break;
        case STATE_6:
        {
            state = CopyTasks[TaskN].state;
            if(CopyTasks[TaskN].IsActive == 1)
            {
                state();
            }        
            TaskN++;        
        }
        break; 
        default:
        {
            /*Out of range. For security return to default conditions*/
            TaskN = 0;
            state = main;
            PRINTF("\r\n OUT OF STATES\r\n");
            state();
        }
        break;
    }
}

void TaskCreateDelete()
{
    /*When the SW is pressed, this function is called.           */
    /*Each time the SW is pressed, the var NumTask will increment*/
    /*The value of NumTask will be the selected task to toggle-  */

    uint64_t WaitTime = 20000000u; /*Limit time to select the task to toggle.*/
    static uint64_t WaitCnt = 0;
    PRINTF("\r\nTOGGLE STATE");
    static uint8_t NumTask;
    uint8_t i;
    NumTask = 0;
    while(WaitCnt <  WaitTime) /*Delay*/
    {
        if(g_ButtonPress == true)
        {
            NumTask++;
            if(NumTask > STATE_6) /*Threshold used to don't pass STATE_6*/
            {
                NumTask = STATE_6;
            }           
            g_ButtonPress = false;
        }
        WaitCnt++;
    }
    WaitCnt = 0; /*Reset Delay.*/
    PRINTF(" -> : %d\r\n", NumTask+1 );

    for(i = STATE_1; i <= STATE_6; i++)/*For used to sincronize the active/inactive state to the correct array numer.*/
    {
        if(NumTask == CopyTasks[i].TaskNum)/*If the states match then toggle the state.*/
        {
            if(CopyTasks[i].IsActive == 1)
            {
                CopyTasks[i].IsActive = 0;
            }
            else
            {
                CopyTasks[i].IsActive = 1;
            }            
        }
    }
    
}


void PriorityTasks()
{
    STATES j, k, i, temp; /*TaskNum*/
    uint8_t temp2;  /*Priority*/
    uint16_t temp3; /*Time Active*/
    void (*temp4)(); /*Ptr to Fct*/
    uint8_t temp5; /*IsActive*/

    for(i = STATE_1; i <= STATE_6; i++)
    {
        for(j = STATE_1, k = STATE_2; k <= STATE_6; j++, k++)
        {
            if(CopyTasks[j].Priority < CopyTasks[k].Priority) /*Usted to re order by components. All components are moved in order to priority*/
            {
                temp = CopyTasks[k].TaskNum;
                CopyTasks[k].TaskNum = CopyTasks[j].TaskNum;
                CopyTasks[j].TaskNum = temp;

                temp2 = CopyTasks[k].Priority;
                CopyTasks[k].Priority = CopyTasks[j].Priority;
                CopyTasks[j].Priority = temp2;

                temp3 = CopyTasks[k].Time;
                CopyTasks[k].Time = CopyTasks[j].Time;
                CopyTasks[j].Time = temp3;

                temp4 = CopyTasks[k].state;
                CopyTasks[k].state = CopyTasks[j].state;
                CopyTasks[j].state = temp4;

                temp5 = CopyTasks[k].IsActive;
                CopyTasks[k].IsActive = CopyTasks[j].IsActive;
                CopyTasks[j].IsActive = temp5;
                                
            }
        }
    }
    for(i = STATE_1; i <= STATE_6; i++)
    {
        PRINTF("\r\n RE-ORDERED STATE %d\r\n", CopyTasks[i].TaskNum+1); /*Print Re-Ordered array.*/
    }
}

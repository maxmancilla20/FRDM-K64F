/*
 * LedSws_Drivers_Init.c
 *
 *  Created on: 10 may. 2024
 *      Author: USER
 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

#include "pin_mux.h"
#include "clock_config.h"

/*Macros*/
#define BOARD_LED_GPIO     BOARD_LED_RED_GPIO       /*Red Led*/
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define BOARD_LED_GPIO_TWO     BOARD_LED_GREEN_GPIO /*Green Led*/
#define BOARD_LED_GPIO_PIN_TWO BOARD_LED_GREEN_GPIO_PIN

#define BOARD_LED_GPIO_THREE     BOARD_LED_BLUE_GPIO /*Blue Led*/
#define BOARD_LED_GPIO_PIN_THREE BOARD_LED_BLUE_GPIO_PIN


gpio_pin_config_t sw2_config = {
        kGPIO_DigitalInput,
        0,
    };

gpio_pin_config_t sw3_config = {
    kGPIO_DigitalInput,
    0,
    };
    
gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

uint64_t ADC_Val = 0;


void SwitchLed_Init(void)
{
    PORT_SetPinInterruptConfig(BOARD_SW2_PORT, BOARD_SW2_GPIO_PIN, kPORT_InterruptFallingEdge);
	EnableIRQ(BOARD_SW2_IRQ);
	GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw2_config);

    PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptFallingEdge);
    EnableIRQ(BOARD_SW3_IRQ);
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw3_config);

    //Set PORTA Interrupt level to 3 (higher than SYSCALL), configMAX_SYSCALL_INTERRUPT_PRIORITY priority is 2.
    (void) NVIC_GetPriority(PORTA_IRQn);
	NVIC_SetPriority(PORTA_IRQn,3);        //PORTA vector is 5
	(void) NVIC_GetPriority(PORTC_IRQn);
	NVIC_SetPriority(PORTC_IRQn,4);

      /* RED*/
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAsGpio);
    /* GREEN */
	PORT_SetPinMux(PORTE, 26U, kPORT_MuxAsGpio);
    /* BLUE */
    PORT_SetPinMux(PORTB, 21U, kPORT_MuxAsGpio);

    /* Init output LED GPIO RED. */
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

    /* Init output LED GPIO GREEN. */
    GPIO_PinInit(BOARD_LED_GPIO_TWO, BOARD_LED_GPIO_PIN_TWO, &led_config);

    /* Init output LED GPIO BLUE. */
    GPIO_PinInit(BOARD_LED_GPIO_THREE, BOARD_LED_GPIO_PIN_THREE, &led_config);

    GPIO_PortSet(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1u <<BOARD_LED_GREEN_GPIO_PIN);
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
}

void UpAdc(void)
{
    if(ADC_Val < 400)
    {
        ADC_Val = ADC_Val + 1;
    }
}
void DownAdc(void)
{
    if(ADC_Val > 1)
    {
        ADC_Val = ADC_Val - 1;
    }
}

uint64_t Get_ADC_Val(void)
{
    return ADC_Val;
}

void LedUpdate(uint8_t LedStatus)
{
    switch(LedStatus)
    {
        case 0:
            GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
            break;
        case 1:
            GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1u << BOARD_LED_BLUE_GPIO_PIN);
            break;
        default:
            break;
    }
} 
/*
 * Functions.h
 *
 *  Created on: 31 ago. 2023
 *      Author: Mats
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/*Includes*/
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

/*Macros*/
#define BOARD_LED_GPIO     BOARD_LED_RED_GPIO       /*Red Led*/
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define BOARD_LED_GPIO_TWO     BOARD_LED_GREEN_GPIO /*Green Led*/
#define BOARD_LED_GPIO_PIN_TWO BOARD_LED_GREEN_GPIO_PIN

#define BOARD_LED_GPIO_THREE     BOARD_LED_BLUE_GPIO /*Blue Led*/
#define BOARD_LED_GPIO_PIN_THREE BOARD_LED_BLUE_GPIO_PIN

#define BOARD_SW_GPIO        BOARD_SW3_GPIO     /*Switch 3 configurations*/
#define BOARD_SW_PORT        BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW3_GPIO_PIN

#define BOARD_SW_GPIO_TWO        BOARD_SW2_GPIO /*Switch 2 configurations*/
#define BOARD_SW_PORT_TWO        BOARD_SW2_PORT
#define BOARD_SW_GPIO_PIN_TWO    BOARD_SW2_GPIO_PIN

#define BOARD_SW_IRQ         BOARD_SW3_IRQ      /*Switch 3 configurations*/
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW3_NAME

#define BOARD_SW_IRQ_TWO         BOARD_SW2_IRQ /*Switch 2 configurations*/
#define BOARD_SW_IRQ_HANDLER_TWO BOARD_SW2_IRQ_HANDLER
#define BOARD_SW_NAME_TWO        BOARD_SW2_NAME

#define LED_RED_ON()  GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)   /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */


#define LED_GREEN_ON() GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */


#define LED_BLUE_ON() GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE \ \*/
#define LED_BLUE_OFF() GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE \ \*/
#define LED_BLUE_TOGGLE() GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

/*Typedefs*/
typedef enum /*Variable used to select states.*/
{
    STATE_1,
    STATE_2,
    STATE_3,
	STATE_4,
    STATE_5,
    STATE_6
}STATES;

typedef struct _FIFO
{
    uint8_t IsActive;
    void (*state)();
    uint8_t Time;
}FIFO;


/*Function prototypes*/
STATES NextStateF(void);
void StateSelect(STATES);
void BlinkRed(void);
void BlinkGreen(void);
void BlinkBlue(void);
void BlinkYellow(void);
void BlinkLightBlue(void);
void BlinkPurple(void);

#endif /* FUNCTIONS_H_ */

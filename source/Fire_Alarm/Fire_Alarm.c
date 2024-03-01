/*
 * Fire_Alarm.c
 *
 *  Created on: 17 feb. 2024
 *      Author: USER
 */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "Fire_Alarm\Fire_Alarm.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "lwip/api.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dhcp.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"
#include "lwip_mqtt_id.h"

#include "ctype.h"
#include "stdio.h"
#include <string.h>

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_GPIO     BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief delay a while.
 */
void delay(void);
void Fire_Alarm_Init(void);

uint8_t AlarmState = 1;
/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1600000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

 void SaveLastData(const u8_t *data)
{
    AlarmState = *data;
}

/*uint8_t Get_Forest_Status(void)
{
    static uint8_t status = 0;

    return status;
}*/

uint8_t Get_Alarm_Status(void)
{
    return AlarmState;
}

/*!
 * @brief Main function
 */
void Fire_Alarm_Monitor(void *arg)
{
	LWIP_UNUSED_ARG(arg);
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };
    static uint32_t BlinkLed = 0;
    int err;
    static uint8_t SentFlag1 = 0;
    static uint8_t SentFlag2 = 0;
    
    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

    /*Loop Task*/
    while(1)
    {
        switch(AlarmState)
        {
            case 49: /*49 = 1 in ASCII*/
            if(SentFlag1 == 0)
            {
                err = tcpip_callback(publish_message, NULL);
                if (err != ERR_OK)
                {
                    PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
                }
            	GPIO_PortSet(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
                SentFlag1 = 1;
                SentFlag2 = 0;
            }

            break;
            case 50: /*50 = 2 in ASCII*/
                //delay();
                BlinkLed = (BlinkLed + 1) % (1600000 + 1);

                if ( BlinkLed == 0 )
                {
                    GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
                }

                if(SentFlag2 == 0)
                {
                    err = tcpip_callback(publish_message, NULL);
                    if (err != ERR_OK)
                    {
                        PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
                    }
                    SentFlag1 = 0;
                    SentFlag2 = 1;
                }
            break;
            default:
        }

        //AlarmState = 2;
    }
}

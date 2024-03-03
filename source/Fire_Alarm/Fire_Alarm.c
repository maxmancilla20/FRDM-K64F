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

#include "FreeRTOS.h"
#include "task.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
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
uint8_t AlarmState = 1;
static uint8_t Emergency = 0;
static uint8_t ManualState = 0;

gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput,
    0,
};

// Task handles
TaskHandle_t FireAlarmActivated_Handle = NULL;
TaskHandle_t MQTTPublisher_Handle = NULL;
TaskHandle_t FireAlarmMonitor_Handle = NULL;
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

 void SaveLastData(const u8_t *data, uint8_t operation, const char *topic)
{ 
    static uint8_t PowerFlag = 0;
    static uint8_t ManualFlag = 0;

    /* Define the init structure for the output LED pin*/
    
    /* Init output LED GPIO. */

    if((1 == operation) && (0 == strcmp("maximiliano_p2024/power", topic)))
    {
        PowerFlag = 1;
    }
    else if((1 == operation) && (0 == strcmp("maximiliano_p2024/manual", topic)))
    {
        ManualFlag = 1;
    }
    else
    {
        /*Do nothing*/
    }
    
    if((1 == PowerFlag) && (0 == operation))
    {
        AlarmState = *data;
        PowerFlag = 0;
        PRINTF("POWER FLAG CHANGED TO %d\n", *data);
        if(48 == AlarmState)
        {
            vTaskSuspend(FireAlarmMonitor_Handle);
            vTaskSuspend(FireAlarmActivated_Handle);
            GPIO_PortSet(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
        }
        else
        {
            vTaskResume(MQTTPublisher_Handle);
            vTaskResume(FireAlarmMonitor_Handle);
            vTaskResume(FireAlarmActivated_Handle);
        }
    } 
    else if((1 == ManualFlag) && (0 == operation))
    {
        ManualState = *data;
        ManualFlag = 0;
    }
    else
    {
        /*Do nothing*/
    }
}

uint8_t Get_Alarm_Status(void)
{
    return AlarmState;
}

/*!
 * @brief Main function
 */
void InitMonitoringSys(void *arg)/*Task0*/
{   
    LWIP_UNUSED_ARG(arg);

    /* MQTT Publisher Task Creation */
    if (NULL == MQTTPublisher_Handle) {
        MQTTPublisher_Handle = sys_thread_new("MQTTPublisher", MQTTPublisher, NULL, INIT_THREAD_STACKSIZE, INIT_THREAD_PRIO);
        if (MQTTPublisher_Handle == NULL) {
            LWIP_ASSERT("MQTTPublisher(): Task creation failed.", 0);
            printf("Error: MQTTPublisher task creation failed.\n");
        }
    } else {
        LWIP_ASSERT("MQTTPublisher(): Task already created.", 0);
        printf("Error: MQTTPublisher task already created.\n");
    }

    /* Fire Alarm Monitor Task Creation */
    if (NULL == FireAlarmMonitor_Handle) {
        FireAlarmMonitor_Handle = sys_thread_new("Fire_Alarm_Monitor", Fire_Alarm_Monitor, NULL, SIMPLE_STACK_SIZE, INIT_THREAD_PRIO);
        if (FireAlarmMonitor_Handle == NULL) {
            LWIP_ASSERT("Fire_Alarm_Monitor(): Task creation failed.", 0);
            printf("Error: Fire_Alarm_Monitor task creation failed.\n");
        }
    } else {
        LWIP_ASSERT("Fire_Alarm_Monitor(): Task already created.", 0);
        printf("Error: Fire_Alarm_Monitor task already created.\n");
    }

        /* Fire Alarm Activated Task Creation */
    if (NULL == FireAlarmActivated_Handle) {
        FireAlarmActivated_Handle = sys_thread_new("FireAlarmActivated", FireAlarmActivated, NULL, configMINIMAL_STACK_SIZE, INIT_THREAD_PRIO);
        if (FireAlarmActivated_Handle == NULL) {
            LWIP_ASSERT("FireAlarmActivated(): Task creation failed.", 0);
            printf("Error: FireAlarmActivated task creation failed.\n");
        }
    } else {
        LWIP_ASSERT("FireAlarmActivated(): Task already created.", 0);
        printf("Error: FireAlarmActivated task already created.\n");
    }

    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
    vTaskDelete(NULL);
}

void MQTTPublisher(void *arg)/*TASK 1*/
{
	LWIP_UNUSED_ARG(arg);
    int err;
    /*Loop Task*/
    while(1)
    {
        static uint8_t ShutDownCnt = 0;
        err = tcpip_callback(publish_message, NULL);
        if (err != ERR_OK)
        {
            PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
        }
        if(48 == AlarmState)
        {  
            if(5 < ShutDownCnt)
            {
                vTaskSuspend(MQTTPublisher_Handle);
                
            }
            else
            {
                ShutDownCnt++;
            }
        }
        else
        {
            ShutDownCnt = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));/*Delay for 1s*/
    }

}

void Fire_Alarm_Monitor(void *arg)/*TASK 2*/
{   
    LWIP_UNUSED_ARG(arg);

    static uint8_t Last_Temp_Val = 0;
    static uint8_t Last_Humidity_Val = 0;

    while (1)
    {
        Last_Temp_Val = atoi(Get_Temp_Msg());
        Last_Humidity_Val = atoi(Get_Humidity_Msg());
        if( (( 80 <= Last_Temp_Val) && (20 >= Last_Humidity_Val)) || (49 == ManualState) )
        {
            Emergency = 1;
            vTaskResume(FireAlarmActivated_Handle);
        }
        else
        {   
            Emergency = 0;
            vTaskSuspend(FireAlarmActivated_Handle);
            GPIO_PortSet(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));/*Delay for 1s*/
    }
}

void FireAlarmActivated(void *arg)/*TASK 3*/
{           
    LWIP_UNUSED_ARG(arg);
    vTaskSuspend(FireAlarmActivated_Handle);

    while (1)
    {
        GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
        vTaskDelay(pdMS_TO_TICKS(500));/*Delay for 500ms*/
    }
}

char* Get_Temp_Msg(void)
{
    static char TempMsg[4];
    static uint8_t NumericTemp = 0;

    if(48 == AlarmState)
    {
        strcpy(TempMsg, "0");
    }
    else
    {
        strcpy(TempMsg, "95");
    }
     
    return TempMsg;
}

char* Get_Humidity_Msg(void)
{
    static char HumidityMsg[4];

    if(48 == AlarmState)
    {
        strcpy(HumidityMsg, "0");
    }
    else
    {
        strcpy(HumidityMsg, "60");
    }
    
    return HumidityMsg;
}

char* Get_Image_Msg(void)
{
    static char ImageMsg[200];
    memset(ImageMsg, '\0', sizeof(ImageMsg));

    // Check the state of the task
    if(48 == AlarmState)
    {
        strcpy(ImageMsg, "https://allthings.how/content/images/wordpress/2021/07/allthings.how-how-to-shutdown-a-windows-11-pc-shut-down-computer.png");
    }
    else
    {
        if((1 == Emergency) || (49 == ManualState))
        {
            strcpy(ImageMsg, "https://www.treehugger.com/thmb/i3riIyXip9Qg6iCJKi_ukf8C9hI=/750x0/filters:no_upscale():max_bytes(150000):strip_icc():format(webp)/elkfire-56af58b45f9b58b7d017af72.jpg");
        }
        else
        {
            strcpy(ImageMsg, "https://images.unsplash.com/photo-1542273917363-3b1817f69a2d?q=80&w=1774&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D");
        }
    }
    return ImageMsg;
}


 

/*
 * Fire_Alarm.c
 *
 *  Created on: 17 feb. 2024
 *      Author: USER
 */
#include "Fire_Alarm.h"
//#include "Fire_Alarm\Fire_Alarm.h"
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

 void SaveLastData(const u8_t *data, uint8_t operation, const char *topic)
{ 

    /*
        OPERATION 1 = INDICATE THAT WE RECEIVE SOMETHING
        OPERATION 0 = USED AFTER OPERATION 1 RECEIVED. THIS OPERATION STORES THE RECEIVED DATA.  
    */
    static uint8_t PowerFlag = 0;
    static uint8_t ManualFlag = 0;

    if((1 == operation) && (0 == strcmp("maximiliano_p2024/power", topic)))
    {
        PowerFlag = 1;/*Flag that indicates that we receive something from power topic*/
    }
    else if((1 == operation) && (0 == strcmp("maximiliano_p2024/manual", topic)))
    {
        ManualFlag = 1;/*Flag that indicates that we receive something from manual topic*/
    }
    else
    {
        /*Do nothing*/
    }
    
    if((1 == PowerFlag) && (0 == operation))
    {
        AlarmState = *data;
        PowerFlag = 0;
        
        if(48 == AlarmState)
        {   
            PRINTF("POWER OFF");
            vTaskSuspend(FireAlarmMonitor_Handle);
            vTaskSuspend(FireAlarmActivated_Handle);
            GPIO_PortSet(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
        }
        else
        {
            PRINTF("POWER ON");
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

void InitMonitoringSys(void *arg)/*Task0*/
{   
    /* TASK CREATED AFTER "app_thread" FINISHED TO INITIALIZE MQTT, THIS TASK INITIALIZES THE SYSTEM*/
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
    /*MAIN TASK, THIS TASK CALLS publish_message THAT PUBLISH ALL THE NECESSARY TOPICS*/
	LWIP_UNUSED_ARG(arg);
    int err;
    /*Loop Task*/
    while(1)
    {
        static uint8_t ShutDownCnt = 0; /*FLAG USED TO LET THE SYSTEM SET ALL IN CERO BEFORE IT WILL BE TURNED OFF*/
        err = tcpip_callback(publish_message, NULL);
        if (err != ERR_OK)
        {
            PRINTF("Failed to invoke publishing of a message on the tcpip_thread: %d.\r\n", err);
        }
        if(48 == AlarmState)/*48 = "0" IN ASCII*/
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

    /*TASK USED TO MONITOR TEMPERATURE AND HUMIDITY AND DETERMINE IF THERES A FIRE EMERGENCY IN THE FOREST*/
    /*ALSO IF THE ALARM WAS ACTIVATED MANUALLY IT ACTIVATES THE ACTUATORS OF THE ALARM*/
    static uint8_t Last_Temp_Val = 0;
    static uint8_t Last_Humidity_Val = 0;

    while (1)
    {
        Last_Temp_Val = atoi(Get_Temp_Msg());
        Last_Humidity_Val = atoi(Get_Humidity_Msg());
        if( (( 80 <= Last_Temp_Val) && (20 >= Last_Humidity_Val)) || (49 == ManualState) )
        {
            /*EMERGENCY IN PROGRESS*/
            Emergency = 1;
            vTaskResume(FireAlarmActivated_Handle);
        }
        else
        {   
            /*NO EMERGENCY DETECTED*/
            Emergency = 0;
            vTaskSuspend(FireAlarmActivated_Handle);
            GPIO_PortSet(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));/*Delay for 1s*/
    }
}

void FireAlarmActivated(void *arg)/*TASK 3*/
{           
    /*THIS IS THE "DRIVER" OR ACTUATOR OF THE ALARM. IF THIS TASK IS RUNNING IT WILL TOGGLE A RED LED THAT INDICATES THE EMERGENCY.*/
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
    /*API USED TO RETURN THE TEMPERATURE VALUE. IT READS THE "SENSORS" AND DISCARDS THE TEMPERATURE READING IF THE POWER IS OFF*/
    if (48 == AlarmState) {
        return "0";
    } else {
        /* Call TempSimulation and return the pointer to the static buffer*/
        return TempSimulation();
    }
}

char* Get_Humidity_Msg(void)
{
    /*API USED TO RETURN THE HUMIDITY VALUE. IT READS THE "SENSORS" AND DISCARDS THE HUMIDITY READING IF THE POWER IS OFF*/   
    if(48 == AlarmState){
        return "0";
    }else{
        return HumiditySimulation();
    }
}

char* Get_Image_Msg(void)
{
    /* "CAMERA CONNECTION", IT SHOWS A VISUAL STATE OF THE FOREST.*/
    static char ImageMsg[200];
    memset(ImageMsg, '\0', sizeof(ImageMsg));

    if(48 == AlarmState)
    {   
        /*IF THE POWER IS OFF. SHOW A DISCONNECTION IMAGE*/
        strcpy(ImageMsg, "https://allthings.how/content/images/wordpress/2021/07/allthings.how-how-to-shutdown-a-windows-11-pc-shut-down-computer.png");
    }
    else
    {
        if((1 == Emergency) || (49 == ManualState))
        {
            /*IF A FIRE EMERGENCY IS DETECTED OR THE MANUAL ACTIAVTION IS SET. SHOWS THE STATE OF THE FOREST IN FIRE.*/
            strcpy(ImageMsg, "https://www.treehugger.com/thmb/i3riIyXip9Qg6iCJKi_ukf8C9hI=/750x0/filters:no_upscale():max_bytes(150000):strip_icc():format(webp)/elkfire-56af58b45f9b58b7d017af72.jpg");
        }
        else
        {
            strcpy(ImageMsg, "https://images.unsplash.com/photo-1542273917363-3b1817f69a2d?q=80&w=1774&auto=format&fit=crop&ixlib=rb-4.0.3&ixid=M3wxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8fA%3D%3D");
        }
    }
    return ImageMsg;
}
 

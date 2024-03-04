/*
 * Fire_Alarm.h
 *
 *  Created on: 17 feb. 2024
 *      Author: USER
 */

#ifndef FIRE_ALARM_FIRE_ALARM_H_
#define FIRE_ALARM_FIRE_ALARM_H_


#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"

#include "FreeRTOS.h"
#include "task.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include <time.h>

#include "HumidiySensor.h"
#include "TempSensor.h"

typedef uint8_t   u8_t;
#define MAX_VALUE 0xFFFFFFFFu

/*! @brief MQTT server host name or IP address. */
#define EXAMPLE_MQTT_SERVER_HOST "broker.hivemq.com"

/*! @brief MQTT server port number. */
#define EXAMPLE_MQTT_SERVER_PORT 1883

/*! @brief Stack size of the temporary lwIP initialization thread. */
#define INIT_THREAD_STACKSIZE 1500
#define SIMPLE_STACK_SIZE  300
#define MEDIUM_STACK_SIZE  700


/*! @brief Priority of the temporary lwIP initialization thread. */
#define INIT_THREAD_PRIO DEFAULT_THREAD_PRIO

/*! @brief Stack size of the temporary initialization thread. */
#define APP_THREAD_STACKSIZE 1024

/*! @brief Priority of the temporary initialization thread. */
#define APP_THREAD_PRIO DEFAULT_THREAD_PRIO

void Fire_Alarm_Monitor(void *);
void FireAlarmActivated(void *);
void MQTTPublisher(void *);
void InitMonitoringSys(void *);
void SaveLastData(const u8_t *, uint8_t, const char *);
void publish_message(void *ctx);
char* Get_Temp_Msg(void);
char* Get_Humidity_Msg(void);
char* Get_Image_Msg(void);


#endif /* FIRE_ALARM_FIRE_ALARM_H_ */

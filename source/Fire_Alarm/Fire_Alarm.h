/*
 * Fire_Alarm.h
 *
 *  Created on: 17 feb. 2024
 *      Author: USER
 */

#ifndef FIRE_ALARM_FIRE_ALARM_H_
#define FIRE_ALARM_FIRE_ALARM_H_

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
char* TempSimulation(void);
char* HumiditySimulation(void);

#endif /* FIRE_ALARM_FIRE_ALARM_H_ */

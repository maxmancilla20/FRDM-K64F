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

void Fire_Alarm_Monitor(void *);
uint8_t Get_Alarm_Status(void);
uint8_t Get_Forest_Status(void);
void SaveLastData(const u8_t *);
void publish_message(void *ctx);

#endif /* FIRE_ALARM_FIRE_ALARM_H_ */

/*
 * LedSws_Drivers_Init.h
 *
 *  Created on: 10 may. 2024
 *      Author: USER
 */

#ifndef LEDSWS_DRIVERS_INIT_H_
#define LEDSWS_DRIVERS_INIT_H_

void SwitchLed_Init(void);
void UpAdc(void);
void DownAdc(void);
uint64_t Get_ADC_Val(void);
void LedUpdate(uint8_t);

#endif /* LEDSWS_DRIVERS_INIT_H_ */

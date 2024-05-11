/*
 * CAN.h
 *
 *  Created on: 10 may. 2024
 *      Author: USER
 */

#ifndef CAN_H_
#define CAN_H_

#define EXAMPLE_FLEXCAN_IRQn       CAN0_ORed_Message_buffer_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler CAN0_ORed_Message_buffer_IRQHandler

void CAN_Init(void);

void Send_CAN_ADC(void);
static void log_task(void *pvParameters);
void PeriodUpdate(uint8_t);



#endif /* CAN_H_ */

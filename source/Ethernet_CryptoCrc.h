/*
 * Ethernet_CryptoCrc.h
 *
 *  Created on: 7 feb. 2024
 *      Author: USER
 */

#ifndef ETHERNET_CRYPTOCRC_H_
#define ETHERNET_CRYPTOCRC_H_

#include "stdint.h"
#include "fsl_debug_console.h"
#include "aes.h"
#include "fsl_crc.h"
#include "MK64F12.h"


static void InitCrc32(CRC_Type *, uint32_t);
uint32_t Get_CRC32(uint8_t);
uint8_t * EncryptMsg(uint8_t *);
size_t Get_Msg_Lenght();


#endif /* ETHERNET_CRYPTOCRC_H_ */

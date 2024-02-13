/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "Driver_ETH_MAC.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_enet_cmsis.h"
#include "fsl_enet_phy_cmsis.h"
#include "fsl_phy.h"
#include "stdlib.h"

#include "fsl_common.h"
#include "fsl_sysmpu.h"
#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "RTE_Device.h"

#include "Ethernet_CryptoCrc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* ENET base address */
#define EXAMPLE_ENET     Driver_ETH_MAC0
#define EXAMPLE_ENET_PHY Driver_ETH_PHY0
#define ENET_DATA_LENGTH        (1000)
#define ENET_EXAMPLE_LOOP_COUNT (20U)

/* @TEST_ANCHOR*/

#ifndef MAC_ADDRESS
#define MAC_ADDRESS {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x61}
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_frame[ENET_DATA_LENGTH + 14];
volatile uint32_t g_testTxNum  = 0;
uint8_t g_macAddr[6]           = MAC_ADDRESS;
volatile uint32_t g_rxIndex    = 0;
volatile uint32_t g_rxCheckIdx = 0;
volatile uint32_t g_txCheckIdx = 0;
uint32_t globaldata;
/*******************************************************************************
 * Code
 ******************************************************************************/
mdio_handle_t mdioHandle = {.ops = &enet_ops};
phy_handle_t phyHandle   = {.phyAddr = RTE_ENET_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &phyksz8081_ops};

uint32_t ENET0_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}

void ENET_SignalEvent_t(uint32_t event)
{
    if (event == ARM_ETH_MAC_EVENT_RX_FRAME)
    {
        uint32_t size;
        uint32_t len;
        uint32_t index;

        /* Get the Frame size */
        size = EXAMPLE_ENET.GetRxFrameSize();
        /* Call ENET_ReadFrame when there is a received frame. */
        if (size != 0)
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
            uint8_t *data = (uint8_t *)malloc(size);
            if (data)
            {
                len = EXAMPLE_ENET.ReadFrame(data, size);
                if (size == len)
                {
                    /* Increase the received frame numbers. */
                    if (g_rxIndex < ENET_EXAMPLE_LOOP_COUNT)
                    {
                        g_rxIndex++;
                    }
                }
                free(data);
            }
        }
    }
    if (event == ARM_ETH_MAC_EVENT_TX_FRAME)
    {
        g_testTxNum++;
    }
}
//uint8_t MyName[100];
/*! @brief Build Frame for transmit. */
static void ENET_BuildBroadCastFrame(void)
{
    //MyName = "MAXIMILIANO_MT";
    uint32_t count  = 0;
    uint32_t length = ENET_DATA_LENGTH - 46;

    memset(g_frame, 0, sizeof(g_frame));/*Clear Frame for a new request*/
    /*Msg to send*/
    PRINTF("\r\n ----------------------------RAW MESSAGE---------------------------------.\r\n");

    for(int i=0; i < 23; i++) {
		PRINTF("0x%x,", MsgtoSend[i]);
	}
    PRINTF("\r\n");

    /*Encrypt Msg*/
    uint8_t * Encrypted_Msg = EncryptMsg(MsgtoSend);


    /*Build Ethernet Buffer*/
    for (count = 0; count < 6U; count++)
    {
        g_frame[count] = 0xFF;
    }
    memcpy(&g_frame[6], &g_macAddr[0], 6U);
    g_frame[12] = (length >> 8) & 0xFFU;
    g_frame[13] = length & 0xFFU;

    /*Add Encrypted data to the buffer*/
    PRINTF("\r\n ---------------------Encrypted Msg on Buffer----------------------------------.\r\n");
    for (count = 0; count < Get_Msg_Lenght(); count++)
    {
        g_frame[count + 18] = Encrypted_Msg[count];
        PRINTF("0x%02x,", g_frame[count + 18] );
    }
    PRINTF("\r\n ---------------------CRC on Buffer----------------------------------.\r\n");

    /*32 bits CRC divided in 4 different bytes*/
    uint32_t CRCObtained = Get_CRC32(0, Encrypted_Msg);
    g_frame[Get_Msg_Lenght() + 18] = (uint8_t)(CRCObtained >> 24);
    g_frame[Get_Msg_Lenght() + 19] = (uint8_t)(CRCObtained >> 16);
    g_frame[Get_Msg_Lenght() + 20] = (uint8_t)(CRCObtained >> 8);
    g_frame[Get_Msg_Lenght() + 21] = (uint8_t)CRCObtained;
    PRINTF("0x%02x,", g_frame[Get_Msg_Lenght() + 18] );
    PRINTF("0x%02x,", g_frame[Get_Msg_Lenght() + 19] );
    PRINTF("0x%02x,", g_frame[Get_Msg_Lenght() + 20] );
    PRINTF("0x%02x,", g_frame[Get_Msg_Lenght() + 21] );
    (void)Get_CRC32(1, Encrypted_Msg);/*Print obtained CRC*/
    //DecryptMsg();


    uint8_t * Decrypted_Msg = DecryptMsgandCRC(Encrypted_Msg, CRCObtained);

    
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t txnumber = 0;
    ARM_ETH_LINK_INFO linkInfo;

    /* Hardware Initialization. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Disable SYSMPU. */
    SYSMPU_Enable(SYSMPU, false);

    mdioHandle.resource.base        = ENET;
    mdioHandle.resource.csrClock_Hz = ENET0_GetFreq();

    PRINTF("\r\nENET example start.\r\n");

    /* Initialize the ENET module. */
    EXAMPLE_ENET.Initialize(ENET_SignalEvent_t);
    EXAMPLE_ENET.PowerControl(ARM_POWER_FULL);
    EXAMPLE_ENET.SetMacAddress((ARM_ETH_MAC_ADDR *)g_macAddr);

    PRINTF("Wait for PHY init...\r\n");
    while (EXAMPLE_ENET_PHY.PowerControl(ARM_POWER_FULL) != ARM_DRIVER_OK)
    {
        PRINTF("PHY Auto-negotiation failed, please check the cable connection and link partner setting.\r\n");
    }

    EXAMPLE_ENET.Control(ARM_ETH_MAC_CONTROL_RX, 1);
    EXAMPLE_ENET.Control(ARM_ETH_MAC_CONTROL_TX, 1);
    PRINTF("Wait for PHY link up...\r\n");
    do
    {
        if (EXAMPLE_ENET_PHY.GetLinkState() == ARM_ETH_LINK_UP)
        {
            linkInfo = EXAMPLE_ENET_PHY.GetLinkInfo();
            EXAMPLE_ENET.Control(ARM_ETH_MAC_CONFIGURE, linkInfo.speed << ARM_ETH_MAC_SPEED_Pos |
                                                            linkInfo.duplex << ARM_ETH_MAC_DUPLEX_Pos |
                                                            ARM_ETH_MAC_ADDRESS_BROADCAST);
            break;
        }
    } while (1);

#if defined(PHY_STABILITY_DELAY_US) && PHY_STABILITY_DELAY_US
    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

    /* Build broadcast for sending. */
    ENET_BuildBroadCastFrame();
    uint8_t FrameCnt = 0;
    while (1)
    {
        /* Check the total number of received number. */
        if (g_testTxNum && (g_txCheckIdx != g_testTxNum))
        {
            switch(FrameCnt)
            {
                case 0:
                    UpdateMsgtoSend("Hola este es el primer update");
                    ENET_BuildBroadCastFrame();
                    FrameCnt++;
                break;
                case 1:
                    UpdateMsgtoSend("Hola este es el segundo update");
                    ENET_BuildBroadCastFrame();
                    FrameCnt++;
                break;
                case 2:
                    UpdateMsgtoSend("Hola este es el tercer update");
                    ENET_BuildBroadCastFrame();
                    FrameCnt++;
                break;
                case 3:
                    UpdateMsgtoSend("Hola este es el cuarto update");
                    ENET_BuildBroadCastFrame();
                    FrameCnt++;
                break; 
                case 4:
                    UpdateMsgtoSend("Hola este es el quinto update");
                    ENET_BuildBroadCastFrame();
                    FrameCnt++;
                break;  
                case 5:
                    UpdateMsgtoSend("Hola este es el sexto update");
                    ENET_BuildBroadCastFrame();
                    FrameCnt++;
                break; 
                default:
                /*Do Nothing*/
            }
            g_txCheckIdx = g_testTxNum;
            PRINTF("The %d frame transmitted success!\r\n", g_txCheckIdx);
        }
        if (g_rxCheckIdx != g_rxIndex)
        {
            g_rxCheckIdx = g_rxIndex;
            PRINTF("A total of %d frame(s) has been successfully received!\r\n", g_rxCheckIdx);
        }
        /* Get the Frame size */
        if (txnumber < ENET_EXAMPLE_LOOP_COUNT)
        {
            txnumber++;
            /* Send a multicast frame when the PHY is link up. */
            if (EXAMPLE_ENET.SendFrame(&g_frame[0], ENET_DATA_LENGTH, ARM_ETH_MAC_TX_FRAME_EVENT) == ARM_DRIVER_OK)
            {
                SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
            }
            else
            {
                PRINTF(" \r\nTransmit frame failed!\r\n");
            }
        }
    }
}

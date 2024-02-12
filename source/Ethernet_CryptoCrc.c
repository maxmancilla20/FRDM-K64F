
/*================================================================================================================*/
/*                                                  Includes                                                      */
/*================================================================================================================*/
#include "Ethernet_CryptoCrc.h"

/*================================================================================================================*/
/*                                             Variable prototypes                                                */
/*================================================================================================================*/
uint8_t key[] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
uint8_t iv[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
struct AES_ctx ctx;
size_t MyName_Len, padded_len;
uint8_t padded_msg[512] = {0};
uint8_t padded_msg_copy[512] = {0};


/*================================================================================================================*/
/*                                                   Functions                                                    */
/*================================================================================================================*/

/*Initialization of CRC32*/
static void InitCrc32(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    config.polynomial         = 0x04C11DB7U;
    config.seed               = seed;
    config.reflectIn          = true;
    config.reflectOut         = true;
    config.complementChecksum = true;
    config.crcBits            = kCrcBits32;
    config.crcResult          = kCrcFinalChecksum;

    CRC_Init(base, &config);
}

uint32_t Get_CRC32(uint8_t Option, uint8_t * padded_msg_c)
{
    CRC_Type *base = CRC0;
    static uint32_t CKSM;

    switch(Option)
    {
        case 0: /*Calculate CRC*/
            InitCrc32(base, 0xFFFFFFFFU);
            CRC_WriteData(base, (uint8_t *)&padded_msg_c[0], padded_len);
            CKSM = CRC_Get32bitResult(base); 
        break;
        case 1: /*PRINT CRC*/
            PRINTF("\r\nCRC-32 xd: 0x%08x\r\n", CKSM);
        break;

        default:
        /*Do nothing*/
    }


    return CKSM;
}

uint8_t * EncryptMsg(uint8_t * MyName)
{
    AES_init_ctx_iv(&ctx, key, iv);
	MyName_Len = strlen(MyName);
	padded_len = MyName_Len + (16 - (MyName_Len%16) );
	memcpy(padded_msg, MyName, MyName_Len);
	AES_CBC_encrypt_buffer(&ctx, padded_msg, padded_len);

    return padded_msg;
}

uint8_t * DecryptMsgandCRC(uint8_t * Decrypt, uint32_t CRCBase)
{   
    uint32_t CRCCheck = Get_CRC32(0, Decrypt);

    if(CRCCheck != CRCBase)
    {
        PRINTF("\r\nCRC NOT OK \r\n");
    }
    else
    {   
        PRINTF("\r\nCRC OK");
        AES_init_ctx_iv(&ctx, key, iv);    
        AES_CBC_decrypt_buffer(&ctx, Decrypt, padded_len);

        PRINTF("\r\nDECRYPTED MSG: ");
        PRINTF(Decrypt);
        PRINTF("\r\n");
    }
}

size_t Get_Msg_Lenght()
{
    return padded_len;
}
#include "./trust_interface.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/TCM/tcm.h"

uint16_t respSize = 0;
uint8_t commandBuf[TCM_COMMAND_BUFF_SIZE];

void writeUint16ToBuffer(uint8_t *buffer, uint16_t index, uint16_t data)
{
    // 小端序存储：先存低字节，再存高字节
    buffer[index + 1] = data & 0xFF;    // 低字节
    buffer[index] = (data >> 8) & 0xFF; // 高字节
}

void writeUint32ToBuffer(uint8_t *buffer, uint16_t index, uint32_t data)
{
    // 小端序存储：依次存低字节到高字节
    buffer[index + 3] = data & 0xFF; // 存储最低字节
    buffer[index + 2] = (data >> 8) & 0xFF;
    buffer[index + 1] = (data >> 16) & 0xFF;
    buffer[index] = (data >> 24) & 0xFF; // 存储最高字节
}

int lp_tcm_startup(uint8_t *respBuf)
{
    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, 12);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_Startup);
    writeUint16ToBuffer(commandBuf, 10, TCM_SU_CLEAR);

    respSize = sendCommand(commandBuf, 12, respBuf);

    return respSize;
}

int lp_tcm_shutdown(uint8_t *respBuf)
{
    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, 12);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_Shutdown);
    writeUint16ToBuffer(commandBuf, 10, TCM_SU_CLEAR);

    respSize = sendCommand(commandBuf, 12, respBuf);

    return respSize;
}

int lp_tcm_selftest(uint8_t *respBuf)
{
    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, 11);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_SelfTest);
    writeUint16ToBuffer(commandBuf, 10, TCM_YES);

    respSize = sendCommand(commandBuf, 11, respBuf);

    return respSize;
}

int lp_tcm_clearup(uint8_t *respBuf)
{
    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, 12);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_Startup);
    writeUint16ToBuffer(commandBuf, 10, TCM_SU_CLEAR);

    respSize = sendCommand(commandBuf, 12, respBuf);

    return respSize;
}

int lp_tcm_getrandom(uint16_t bytesRequested, uint8_t *respBuf)
{
    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, 12);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_GetRandom);
    writeUint16ToBuffer(commandBuf, 10, bytesRequested);

    respSize = sendCommand(commandBuf, 12, respBuf);

    return respSize;
}

/* sm3哈希接口 */
int lp_tcm_hash_sha256(uint8_t *data, uint16_t dataSize, uint8_t *respBuf)
{
    uint16_t commandSize = 2 + 4 + 4 + 2 + dataSize + 2 + 4; /* 计算命令长度 */

    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, commandSize);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_Hash);
    writeUint16ToBuffer(commandBuf, 10, dataSize);

    uint16_t i, j;
    for (i = 12, j = 0; j < dataSize; i++, j++)
    {
        commandBuf[i] = data[j];
    }

    writeUint16ToBuffer(commandBuf, 12 + dataSize, TCM_ALG_SHA256);
    writeUint32ToBuffer(commandBuf, 12 + dataSize + 2, TCM_RH_NULL);

    respSize = sendCommand(commandBuf, commandSize, respBuf);

    return respSize;
}

int lp_tcm_hash_sm3(uint8_t *data, uint16_t dataSize, uint8_t *respBuf)
{
    uint16_t commandSize = 2 + 4 + 4 + 2 + dataSize + 2 + 4; /* 计算命令长度 */

    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS);
    writeUint32ToBuffer(commandBuf, 2, commandSize);
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_Hash);
    writeUint16ToBuffer(commandBuf, 10, dataSize);

    uint16_t i, j;
    for (i = 12, j = 0; j < dataSize; i++, j++)
    {
        commandBuf[i] = data[j];
    }

    writeUint16ToBuffer(commandBuf, 12 + dataSize, TCM_ALG_SM3_256);
    writeUint32ToBuffer(commandBuf, 12 + dataSize + 2, TCM_RH_NULL);

    respSize = sendCommand(commandBuf, commandSize, respBuf);

    return respSize;
}

int lp_tcm_pcrread(uint8_t pcrIndex, uint8_t *respBuf)
{
    writeUint16ToBuffer(commandBuf, 0, TCM_ST_NO_SESSIONS); /* 命令头 */
    writeUint32ToBuffer(commandBuf, 2, 20); /* 命令总长度 */
    writeUint32ToBuffer(commandBuf, 6, TCM_CC_PCR_Read); /* 命令码 */
    writeUint32ToBuffer(commandBuf, 10, (uint32_t)1); /* 选择PCR的个数 */
    
    writeUint16ToBuffer(commandBuf, 14, TCM_ALG_SHA256); /* PCR所属的算法 */
    commandBuf[16] = TCM_PCR_SELECT_MAX;

    if (pcrIndex >= 0 && pcrIndex < 8)
    {
        commandBuf[17] = (uint8_t)(1 << (pcrIndex % 8));
        commandBuf[18] = 0x00;
        commandBuf[19] = 0x00;
    }
    else if(pcrIndex >= 8 && pcrIndex < 16)
    {
        commandBuf[17] = 0x00;
        commandBuf[18] = (uint8_t)(1 << (pcrIndex % 8));
        commandBuf[19] = 0x00;
    }
    else if(pcrIndex >= 16 && pcrIndex < 24)
    {
        commandBuf[17] = 0x00;
        commandBuf[18] = 0x00;
        commandBuf[19] = (uint8_t)(1 << (pcrIndex % 8));
    }
    
    for (uint8_t i = 0; i < 20; i++)
    {
        printf("%02x ", commandBuf[i]);
    }
    
    printf("\r\n");
    respSize = sendCommand(commandBuf, 20, respBuf);

    return respSize;
}
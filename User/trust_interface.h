#ifndef __TRUST_INTERFACE_H
#define __TRUST_INTERFACE_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/TCM/tcm.h"

typedef struct TCMS_PCR_SELECTION TCMS_PCR_SELECTION;
struct TCMS_PCR_SELECTION {
    TCM_ALG_ID hash; /* the hash algorithm associated with the selection */
    uint8_t sizeofSelect; /* the size in octets of the pcrSelect array */
    uint8_t pcrSelect[TCM_PCR_SELECT_MAX]; /* the bit map of selected PCR */
};

/* Definition of TCML_PCR_SELECTION Structure */
typedef struct TCML_PCR_SELECTION TCML_PCR_SELECTION;
struct TCML_PCR_SELECTION {
    uint32_t count; /* number of selection structures. A value of zero is allowed. */
    TCMS_PCR_SELECTION pcrSelections[TPM2_NUM_PCR_BANKS]; /* list of selections */
};


int lp_tcm_startup(uint8_t *respBuf);
int lp_tcm_shutdown(uint8_t *respBuf);
int lp_tcm_selftest(uint8_t *respBuf);
int lp_tcm_clearup(uint8_t *respBuf);
int lp_tcm_getrandom(uint16_t bytesRequested, uint8_t *respBuf);
int lp_tcm_hash_sha256(uint8_t *data, uint16_t dataSize, uint8_t *respBuf);
int lp_tcm_hash_sm3(uint8_t *data, uint16_t data_size, uint8_t *respBuf);
int lp_tcm_pcrread(uint8_t pcrIndex, uint8_t *respBuf);
int lp_tcm_pcrextend(uint8_t pcrIndex, uint8_t *extendData, uint8_t extendDataSize, uint8_t *respBuf);
#endif
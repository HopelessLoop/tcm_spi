#include "./stm32f1xx_it.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TCM/tcm.h"
#include "./trust_interface.h"

int main(void)
{
  HAL_Init();                         /* 初始化HAL库 */
  sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟为72Mhz */
  delay_init(72);                     /* 延时初始化 */
  usart_init(115200);                 /* 串口初始化为115200 */
  led_init();                         /* 初始化LED */
  tcm_init();                         /* 初始化TCM */

  printf("Waiting...\r\n");

  uint16_t i;

  /* TCM相关变量 */
  uint8_t respBuf[TCM_SPI_BUFF_SIZE];
  uint16_t respSize = 0;

  // TCM_STARTUP
  lp_tcm_startup(respBuf);
  printf("OK: ");

  for (i = 0; i < 10; i++)
  {
    printf("%02x ", respBuf[i]);
  }
  printf("\r\n");
  
  printf("tcm startup successfully!\r\n");
  printf("\r\n");
  
    // TCM_STARTUP
  respSize = lp_tcm_pcrread(0, respBuf);
  printf("OK: ");

  for (i = 0; i < respSize; i++)
  {
    printf("%02x ", respBuf[i]);
  }
  printf("\r\n");
  
  printf("tcm readpcr successfully!\r\n");
  printf("\r\n");

  // uint8_t hash_data[] = "\xb9\x7b\x6d\x49\xfd\x5e\xfe\x81\x64\xeb\x15\xf6\x9a\x99\xe5\xc0\x80\x39\xc1\x73\x7a\x0b\xbd\xb9\x07\xe4\x5d\x9d\xe6\x4d\x66\x27\x05\x75\x31\x4e\xd4\x76\xb7\xd6\xcf\x95\xf5\xe1\xee\x7b\x30\x07\xdf\x0b\xfa\x02\x18\x5c\x86\x7d\x5c\xf0\xc9\x61\x1c\x6e\x84\x14\x78\x59\x66\xee\xb4\x08\x6b\x64\xb1\x47\xf4\xe3\x9a\x70\xf0\xba\x37\xca\x49\x24\xe6\x0c\xae\x7d\x67\xf0\xd4\x3a\xae\x37\x34\xdb\x61\xb1\x99\xa6\x4b\x33\xf2\xb9\x59\x88\xde\x60\x94\xa1\x96\xe7\x39\x43\xe1\xd8\x71\x4e\x01\x53\x5c\x9d\xba\x21\xf5\x5e\xb5\x9d";

  // respSize = lp_tcm_hash_sha256(hash_data, 128, respBuf);
  // printf("OK: ");
  // for (i = 0; i < respSize; i++)
  // {
  //   printf("%02x ", respBuf[i]);
  // }
  // printf("\r\n");

  // printf("tcm hash successfully!\r\n");
  // printf("\r\n");

  while(1){};
}
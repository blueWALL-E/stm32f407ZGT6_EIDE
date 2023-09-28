/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:01:33
 * @LastEditTime: 2023-09-28 21:56:03
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\stm32f407ZGT6_EIDE\1_user\main.c
 * @Description: stm32f407zgt6 工程模板 vscode
 * @Wearing:  Read only, do not modify place!!!
 * @Shortcut keys:  ctrl+alt+/ ctrl+alt+t
 */
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"

int main(void)
{
  u32 t = 0;
  delay_init(168);
  uart_init(115200);

  while (1)
  {
    printf("t:%ld\r\n", t);
    delay_ms(500);
    t++;
    if (t == 100)
    {
      t = 0;
    }
  }
}

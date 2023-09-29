/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-09-29 11:11:56
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\stm32f407ZGT6_EIDE\1_user\usart\usart.h
 * @Description: 正点原子usart文件
 * @Wearing:  Read only, do not modify place!!!
 * @Shortcut keys:  ctrl+alt+/ ctrl+alt+t
 */
#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#include "stdarg.h"
#include "string.h"
#include "stdlib.h"

#define USART_MAX_RECV_LEN 60  // 最大接收缓存字节数
#define USART_MAX_SEND_LEN 600 // 最大发送缓存字节数

extern u8 USART_RX_BUF[USART_MAX_RECV_LEN]; // 接收缓冲,最大USART_MAX_RECV_LEN个字节.
extern u8 USART_TX_BUF[USART_MAX_SEND_LEN]; // 发送缓冲,最大USART_MAX_SEND_LEN个字节
extern vu16 USART_RX_STA;                   // 接收数据状态  volatile

void uart_init(u32 bound);

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);

void Usart_printf(USART_TypeDef *pUSARTx, char *fmt, ...);
#endif

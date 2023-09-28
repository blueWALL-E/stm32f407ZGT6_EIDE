/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-09-28 21:46:23
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

#define USART_REC_LEN 200 // 定义最大接收字节数 200
#define EN_USART1_RX 1    // 使能（1）/禁止（0）串口1接收

extern u8 USART_RX_BUF[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;               // 接收状态标记

void uart_init(u32 bound);

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);

#endif

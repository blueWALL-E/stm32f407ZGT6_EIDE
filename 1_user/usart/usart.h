/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-09-29 11:11:56
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\stm32f407ZGT6_EIDE\1_user\usart\usart.h
 * @Description: ����ԭ��usart�ļ�
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

#define USART_MAX_RECV_LEN 60  // �����ջ����ֽ���
#define USART_MAX_SEND_LEN 600 // ����ͻ����ֽ���

extern u8 USART_RX_BUF[USART_MAX_RECV_LEN]; // ���ջ���,���USART_MAX_RECV_LEN���ֽ�.
extern u8 USART_TX_BUF[USART_MAX_SEND_LEN]; // ���ͻ���,���USART_MAX_SEND_LEN���ֽ�
extern vu16 USART_RX_STA;                   // ��������״̬  volatile

void uart_init(u32 bound);

void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);

void Usart_printf(USART_TypeDef *pUSARTx, char *fmt, ...);
#endif

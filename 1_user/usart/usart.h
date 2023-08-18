/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-08-18 21:30:37
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\project\1_user\usart\usart.h
 * @Description: ����ԭ��usart�ļ�
 * @Wearing:  Read only, do not modify place!!!
 * @Shortcut keys:  ctrl+alt+/ ctrl+alt+t
 */
#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define USART_REC_LEN 200 // �����������ֽ��� 200
#define EN_USART1_RX 1    // ʹ�ܣ�1��/��ֹ��0������1����

extern u8 USART_RX_BUF[USART_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA;               // ����״̬���


void uart_init(u32 bound);
#endif

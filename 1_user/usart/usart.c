/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-09-28 21:48:55
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\stm32f407ZGT6_EIDE\1_user\usart\usart.c
 * @Description: 正点原子usart文件
 * @Wearing:  Read only, do not modify place!!!
 * @Shortcut keys:  ctrl+alt+/ ctrl+alt+t
 */
#include "sys.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
// 如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos 使用
#endif

// gcc下这样写有bug待修改
//////////////////////////////////////////////////////////////////
// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; // 循环发送,直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}
#endif

#if EN_USART1_RX // 如果使能了接收
// 串口1中断服务程序
// 注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0; // 接收状态标记

/**
 * @brief 初始化串口1函数
 * @note NUULL
 * @param bound 波特率
 * @return {*}
 */
void uart_init(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 使能USART1时钟

	// 串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  // GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); // GPIOA10复用为USART1

	// USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 初始化PA9，PA10

	// USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART1, &USART_InitStructure);										// 初始化串口1

	USART_Cmd(USART1, ENABLE); // 使能串口1

	// USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启相关中断

	// Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  // 串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、

#endif
}

void USART1_IRQHandler(void) // 串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS // 如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);	//读取接收到的数据

		if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
		{
			if (USART_RX_STA & 0x4000) // 接收到了0x0d
			{
				if (Res != 0x0a)
					USART_RX_STA = 0; // 接收错误,重新开始
				else
					USART_RX_STA |= 0x8000; // 接收完成了
			}
			else // 还没收到0X0D
			{
				if (Res == 0x0d)
					USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))
						USART_RX_STA = 0; // 接收数据错误,重新开始接收
				}
			}
		}
	}
#if SYSTEM_SUPPORT_OS // 如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();
#endif
}
#endif

/**
 * @brief 发送一个字节 0xff
 * @note NUULL
 * @param pUSARTx USART pointer
 * @param ch send data 0x00-0xff
 * @return {*}
 */
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{

	USART_SendData(pUSARTx, ch);

	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

/**
 * @brief 发送2个字节 0x0d0a
 * @note NUULL
 * @param pUSARTx USART pointer
 * @param ch send data 0x0000-0xffff 0x0a0d
 * @return {*}
 */
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	temp_h = (ch & 0xff00) >> 8;
	temp_l = (ch & 0xff);
	USART_SendData(pUSARTx, temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(pUSARTx, temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

/**
 * @brief 发送8位的数组
 * @note NUULL
 * @param pUSARTx USART pointer
 * @param array First address of array
 * @param num Array length
 * @return {*}
 */
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num)
{
	uint16_t i;

	for (i = 0; i < num; i++)
	{

		USART_SendData(pUSARTx, array[i]);
		while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
			;
	}

	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
		;
}

/**
 * @brief 发送字符串
 * @note NUULL
 * @param pUSARTx USART pointer
 * @param str First address of string \0 end
 * @return {*}
 */
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');

	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
	{
	}
}
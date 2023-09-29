/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-09-29 11:08:35
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

u8 USART_RX_BUF[USART_MAX_RECV_LEN]; // 接收缓冲,最大USART_MAX_RECV_LEN个字节.
u8 USART_TX_BUF[USART_MAX_SEND_LEN]; // 发送缓冲,最大USART_MAX_SEND_LEN个字节
vu16 USART_RX_STA;					 // 接收数据状态  volatile

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

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启相关中断

	// Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  // 串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、
}

/**
 * @brief 串口1 中断服务函数
 * @note:
 * @return {*}
 */
void USART1_IRQHandler(void) // 串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS // 如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1); //(USART1->DR);	//读取接收到的数据
	}
#if SYSTEM_SUPPORT_OS // 如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();
#endif
}

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

/**
 * @brief 串口 printf 函数 类似于printf("ABC%d",a)函数
 * @note 确保一次发送数据不超过USART2_MAX_SEND_LEN字节
 * @param pUSARTx 串口号
 * @param fmt 需要发送的信息
 * @return {*}
 */
void Usart_printf(USART_TypeDef *pUSARTx, char *fmt, ...) //...表示可变参数（多个可变参数组成一个列表，后面有专门的指针指向他），不限定个数和类型
{
	u16 i, j;
	va_list ap;								 // 初始化指向可变参数列表的指针
	va_start(ap, fmt);						 // 将第一个可变参数的地址付给ap，即ap指向可变参数列表的开始
	vsprintf((char *)USART_TX_BUF, fmt, ap); // 将参数fmt、ap指向的可变参数一起转换成格式化字符串，放(char*)USART_TX_BUF数组中，其作用同sprintf（），只是参数类型不同
	va_end(ap);
	i = strlen((const char *)USART_TX_BUF); // 此次发送数据的长度
	for (j = 0; j < i; j++)					// 循环发送数据
	{
		while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
			;									  // 循环发送,直到发送完毕
		USART_SendData(pUSARTx, USART_TX_BUF[j]); // 把格式化字符串从开发板串口送出去
	}
}
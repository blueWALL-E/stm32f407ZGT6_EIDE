/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-08-18 21:28:15
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\project\1_user\sys\sys.c
 * @Description: 正点原子sys文件
 * @Wearing:  Read only, do not modify place!!!
 * @Shortcut keys:  ctrl+alt+/ ctrl+alt+t
 */
#include "sys.h"

// THUMB指令不支持汇编内联
// 采用如下方法实现执行汇编指令WFI
// THUMB指令不支持汇编内联
// 采用如下方法实现执行汇编指令WFI
void WFI_SET(void)
{
	__ASM volatile("wfi");
}
// 关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
// 开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");
}
// 设置栈顶地址
// addr:栈顶地址
void MSR_MSP(u32 addr)
{
	__ASM volatile("MSR MSP, r0");
	__ASM volatile("BX r14");
}

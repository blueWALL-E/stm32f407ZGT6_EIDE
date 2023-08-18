/*
 * @Author: Dragon
 * @Date: 2023-08-18 20:43:25
 * @LastEditTime: 2023-08-18 21:26:57
 * @FilePath: \projectf:\Vscode_programming\Embedded\STM32\project\1_user\delay\delay.h
 * @Description: 正点原子delay文件
 * @Wearing:  Read only, do not modify place!!!
 * @Shortcut keys:  ctrl+alt+/ ctrl+alt+t
 */
#ifndef __DELAY_H
#define __DELAY_H
#include <sys.h>

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif

#ifndef __LCD_eUSCI_B0_SPI_H
#define __LCD_eUSCI_B0_SPI_H

#include "ti/devices/msp432p4xx/inc/msp.h"

// ==========================================================================
// 参数宏定义
#define  RS_CMD            0xCC    // 命令
#define  RS_DAT            0xDD    // 数据

// GPIO 引脚操作宏定义
#define  LCD_TFT_CSn_H      (P6->OUT |= BIT0)
#define  LCD_TFT_CSn_L      (P6->OUT &=~BIT0)
#define  LCD_TFT_RS_DAT     (P6->OUT |= BIT1)   // 选择数据寄存器
#define  LCD_TFT_RS_CMD     (P6->OUT &=~BIT1)   // 选择指令寄存器

// ==========================================================================
void DelayMs(uint32_t dat);     // 软件延时函数
void LCD_SPI_Init(void);        // LCD 引脚及 SPI 初始化
// SPI 写1个字节, 控制 CSn 和 D/CX 信号线
void LCD_TFT_SPI_WR_RS(uint8_t reg, uint8_t data);
// SPI 写1个字节, 不控制 CSn 和 D/CX 信号线
void LCD_TFT_SPI_WR(uint8_t data);


#endif

// ==========================================================================
// End of file.

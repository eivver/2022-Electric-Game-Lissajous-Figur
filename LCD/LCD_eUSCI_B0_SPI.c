#include "LCD_eUSCI_B0_SPI.h"   // LCD 底层SPI驱动函数

// ==========================================================================
// 连线：核心板与 DY-IoT-PB 扩展板对接时，无需额外连线
// TFT LCD引脚    DY-IoT-PB 扩展板    MSP432P401R 核心板    备注
//  GND           GND                   GND                 接地
//  VCC           VDD                   +3V3                +3.3v 供电
//  SCL           P14/SCLK              P1.5/UCB0CLK        SPI 时钟
//  SDI           P16/MOSI              P1.6/UCB0SIMO       SPI 主出从入
//  SDO           P15/MISO              P1.7/UCB0SOMI       SPI 主入从出
//  CSn           P07                   P6.0/A7             SPI 片选
//  D/CX          P06                   P6.1/A12            H=数据; L=寄存器
//  RESET         RESET                 RESET               系统复位

// ==========================================================================
// ★★★注意★★★ 软件延时时间与单片机时钟主频设置、是否有中断占用、编译器版本、
// 编译优化级别等都有关，需要精确延时应采用定时器产生
void DelayMs(uint32_t dat)
{
    uint32_t i;
    while(dat--)
    {
        i = 428;        // 大约延迟 1ms @ MCLK = 3MHz
        while(i--);
    }
}

// ==========================================================================
// EUSCI_B0 模块初始化, 配置为 SPI 模式
void LCD_SPI_Init(void)
{
    P1->SEL0 |= BIT4 | BIT5 | BIT6;         // 配置模块使用的引脚
    // ------------------------------------------------
    // 设置 EUSCI_B0 工作在 SPI 模式, 不开中断
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST;      // 先将模块设置为复位状态
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST |    // Remain in reset state
            EUSCI_B_CTLW0_MST |                 // SPI master
            EUSCI_B_CTLW0_SYNC |                // Synchronous mode
            EUSCI_B_CTLW0_MSB |                 // MSB first
            EUSCI_B_CTLW0_MODE_0 |              // 3-pin mode
            EUSCI_B_CTLW0_SSEL__SMCLK;          // SMCLK
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_CKPH;      // CKPH = 1
    EUSCI_B0->CTLW0 &=~EUSCI_B_CTLW0_CKPL;      // CKPL = 0
    
    EUSCI_B0->BRW = 0x01;                       // fBitClock = fBRCLK/(UCBRx+1)
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;    // Release eUSCI from reset
    
    // ------------------------------------------------
    // P6.0 = CSn, SPI 片选
    P6->DIR |= BIT0;    // 设置为输出
    LCD_TFT_CSn_H;
    
    // P6.1 = D/CX, 数据/指令寄存器选择
    P6->DIR |= BIT1;    // 设置为输出
    LCD_TFT_RS_DAT;
}

// ==========================================================================
// SPI 写1个字节, 控制 CSn 和 D/CX 信号线
// 入口参数: reg = RS_CMD, 写寄存器; reg = RS_DAT, 写数据
void LCD_TFT_SPI_WR_RS(uint8_t reg, uint8_t data)
{
    if(reg == RS_CMD)
        LCD_TFT_RS_CMD;     // 输出高电平: 寄存器
    else if(reg == RS_DAT)
        LCD_TFT_RS_DAT;    // 输出低电平: 数据
    else
        LCD_TFT_RS_DAT;    // 输出低电平: 数据
    // ------------------------------------------------------
    LCD_TFT_CSn_L;    // 片选输出: 低电平
    
    while((EUSCI_B0->STATW & EUSCI_A_STATW_BUSY));
    EUSCI_B0->TXBUF = data;     // 发1字节
    
    while((EUSCI_B0->STATW & EUSCI_A_STATW_BUSY));

    LCD_TFT_CSn_H;    // 片选输出: 高电平
}
// ==========================================================================
// SPI 写1个字节, 不控制 CSn 和 D/CX 信号线
// 入口参数:  data 待写入的数据
void LCD_TFT_SPI_WR(uint8_t data)
{
    while((EUSCI_B0->STATW & EUSCI_A_STATW_BUSY));
    EUSCI_B0->TXBUF = data;     // 发1字节
    
    while((EUSCI_B0->STATW & EUSCI_A_STATW_BUSY));
}

// ==========================================================================
// End of file.

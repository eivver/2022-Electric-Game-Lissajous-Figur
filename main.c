/****************************************************/
// MSP432P401R
// 配置Keil独立空工程 (已将ti固件库打包至工程)
// Keil工程已配置开启FPU
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/28
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "tim32.h"
#include "adc.h"
#include "key4x4.h"
#include "oled.h"

#include "LCD_eUSCI_B0_SPI.h" // LCD 底层SPI驱动函数
#include "LCD_Display_API.h"  // LCD 显示接口函数
#include "LCD_Font.h"		  // ASCII 码字库点阵, 以及自定义字符点阵表

uint16_t tft_clean_cnt = 0;
uint8_t key_value = 0;
uint8_t waveform = 0;  // 波形:0正弦波,1三角波
uint8_t frequency = 1; // 频率倍数:1~5
uint8_t iic_sent = 50; 
uint8_t iic_cnt = 0;
char iic_num[5];

int main(void)
{
	SysInit();		   // 第3讲 时钟配置
	uart_init(115200); // 第7讲 串口配置
	delay_init();	   // 第4讲 滴答延时
	KEY4x4_Init();
	// 4052: B->P2.3, A->P2.4
	// 4053: C->P2.5, B->P2.6, A->P2.7
	P2DIR |= BIT3 | BIT4 | BIT5 | BIT6 | BIT7; // 模拟开关选择

	// 计数器 N_or_1->P3.0, D2->P3.5, D1->P3.6, D0->P3.7
	P3DIR |= BIT0 | BIT5 | BIT6 | BIT7; // 计数器置位

	uint16_t ChNum = 0;	 // 临时变量
	uint16_t yStart = 0; // 临时变量

	// ------------------------------------------------
	// WDT定时器在复位时默认开启，溢出将触发微控制器复位
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // 停止 WDT 定时器功能

	// ------------------------------------------------
	// 闪烁 LED 灯指示程序是否在运行
	P1->DIR |= BIT0;			  // P1.0 设置为输出
	P1->OUT |= BIT0;			  // P1.0 LED 亮
	MAP_Interrupt_enableMaster(); // 开启总中断
	// ------------------------------------------------
	LCD_SPI_Init(); // LCD 引脚及 SPI 初始化
	LCD_TFT_Init(); // 初始化 320x240 TFT LCD, 复位LCD,并全屏填充白色

	// ====================================================

	// 相位差：
	yStart = 0;
	for (ChNum = 0; ChNum <= 2; ChNum++)
	{
		LCD_Show_ChFont1616(145, yStart, ChNum, Red, White);
		yStart += 16;
	}
	LCD_TFT_ShowString(yStart, 145, ":", FONT1608, Red, White);

	// 波形：
	yStart = 0;
	for (ChNum = 3; ChNum <= 4; ChNum++)
	{
		LCD_Show_ChFont1616(167, yStart, ChNum, Red, White);
		yStart += 16;
	}
	LCD_TFT_ShowString(yStart, 167, ":", FONT1608, Red, White);

	yStart = 48;
	for (ChNum = 14; ChNum <= 16; ChNum++)
	{
		LCD_Show_ChFont1616(167, yStart, ChNum, Red, White);
		yStart += 16;
	}

	// 频率倍数：
	yStart = 0;
	for (ChNum = 5; ChNum <= 8; ChNum++)
	{
		LCD_Show_ChFont1616(189, yStart, ChNum, Red, White);
		yStart += 16;
	}
	LCD_TFT_ShowString(yStart, 189, ":", FONT1608, Red, White);

	LCD_TFT_ShowString(80, 189, "1", FONT1608, Red, White);
	yStart = 96;
	for (ChNum = 20; ChNum <= 21; ChNum++)
	{
		LCD_Show_ChFont1616(189, yStart, ChNum, Red, White);
		yStart += 16;
	}

	// 电压峰峰值：
	yStart = 0;
	for (ChNum = 9; ChNum <= 13; ChNum++)
	{
		LCD_Show_ChFont1616(211, yStart, ChNum, Red, White);
		yStart += 16;
	}
	LCD_TFT_ShowString(yStart, 211, ":", FONT1608, Red, White);

	LCD_TFT_ShowString(96, 211, "50", FONT1608, Red, White);
//	LCD_TFT_ShowString(112, 211, "V", FONT1608, Red, White);

	// 初始状态
	P2OUT |= BIT5 | BIT6 | BIT7;
	P2OUT &= ~(BIT3 | BIT4);

	P3OUT &= ~(BIT5 | BIT6 | BIT7 | BIT0);

	ADC_Config();
	while (1) // 死循环
	{
		key_value = KEY4x4_Scan(0);
		switch (key_value)
		{
		case 0:
			break;
		case 1:
			// 绘制
			LCD_TFT_fillrect(167, 183, 48, 96, White);
			yStart = 48;
			for (ChNum = 14; ChNum <= 16; ChNum++)
			{
				LCD_Show_ChFont1616(167, yStart, ChNum, Red, White);
				yStart += 16;
			}
			// 操作
			waveform = 0;
			break;
		case 2:
			// 绘制
			LCD_TFT_fillrect(167, 183, 48, 96, White);
			yStart = 48;
			for (ChNum = 17; ChNum <= 19; ChNum++)
			{
				LCD_Show_ChFont1616(167, yStart, ChNum, Red, White);
				yStart += 16;
			}
			// 操作
			waveform = 1;
			break;
		case 3:
			// 绘制
			LCD_TFT_fillrect(80, 189, 96, 197, White);
			LCD_TFT_ShowString(80, 189, "1", FONT1608, Red, White);
			// 操作
			frequency = 1;
			break;
		case 5:
			// 绘制
			LCD_TFT_fillrect(80, 189, 96, 197, White);
			LCD_TFT_ShowString(80, 189, "2", FONT1608, Red, White);
			// 操作
			frequency = 2;
			break;
		case 6:
			// 绘制
			LCD_TFT_fillrect(80, 189, 96, 197, White);
			LCD_TFT_ShowString(80, 189, "3", FONT1608, Red, White);
			// 操作
			frequency = 3;
			break;
		case 7:
			// 绘制
			LCD_TFT_fillrect(80, 189, 96, 197, White);
			LCD_TFT_ShowString(80, 189, "4", FONT1608, Red, White);
			// 操作
			frequency = 4;
			break;
		case 9:
			// 绘制
			LCD_TFT_fillrect(80, 189, 96, 197, White);
			LCD_TFT_ShowString(80, 189, "5", FONT1608, Red, White);
			// 操作
			frequency = 5;
			break;
		case 10:
			// 绘制
//			LCD_TFT_fillrect(96, 211, 112, 227, White);
//			LCD_TFT_ShowString(96, 211, "1", FONT1608, Red, White);
			// 操作
			iic_sent--;
			OLED_WR_Byte(iic_sent,0);
			LCD_TFT_fillrect(96, 211, 112+32, 227, White);
			iic_cnt = IntToStr(iic_sent , iic_num);
			LCD_TFT_ShowString(96, 211, iic_num, FONT1608, Red, White);
			break;
		case 11:
			// 绘制
//			LCD_TFT_fillrect(96, 211, 112, 227, White);
//			LCD_TFT_ShowString(96, 211, "2", FONT1608, Red, White);
			// 操作
			iic_sent++;
			OLED_WR_Byte(iic_sent,0);
			LCD_TFT_fillrect(96, 211, 112+32, 227, White);
			iic_cnt = IntToStr(iic_sent , iic_num);
			LCD_TFT_ShowString(96, 211, iic_num, FONT1608, Red, White);
			break;
		case 13:
			// 绘制
//			LCD_TFT_fillrect(96, 211, 112, 227, White);
//			LCD_TFT_ShowString(96, 211, "3", FONT1608, Red, White);
			// 操作

			break;
		default:
			break;
		}
		if (waveform == 0) // 正弦波
			switch (frequency)
			{
			case 1:
				P2OUT |= BIT5 | BIT6 | BIT7;
				P2OUT &= ~(BIT3 | BIT4);

				P3OUT &= ~(BIT5 | BIT6 | BIT7 | BIT0);
				break;
			case 2:
				P2OUT |= BIT4 | BIT5 | BIT6 | BIT7;
				P2OUT &= ~(BIT3);

				P3OUT |= BIT5 | BIT6 | BIT0;
				P3OUT &= ~(BIT7);
				break;
			case 3:
				P2OUT |= BIT3 | BIT5 | BIT6 | BIT7;
				P2OUT &= ~(BIT4);

				P3OUT |= BIT5 | BIT7 | BIT0;
				P3OUT &= ~(BIT6);
				break;
			case 4:
				P2OUT |= BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
				// P2OUT &= ~BIT6;

				P3OUT |= BIT5 | BIT0;
				P3OUT &= ~(BIT6 | BIT7);
				break;
			case 5:
				P2OUT |= BIT5;
				P2OUT &= ~(BIT3 | BIT4 | BIT6 | BIT7);

				P3OUT |= BIT6 | BIT7 | BIT0;
				P3OUT &= ~(BIT5);
				break;
			default:
				break;
			}
		else
			switch (frequency)
			{
			case 1:
				P2OUT |= BIT7;
				P2OUT &= ~(BIT3 | BIT4 | BIT5 | BIT6);

				P3OUT &= ~(BIT5 | BIT6 | BIT7 | BIT0);
				break;
			case 2:
				P2OUT |= BIT4 | BIT7;
				P2OUT &= ~(BIT3 | BIT5 | BIT6);

				P3OUT |= BIT5 | BIT6 | BIT0;
				P3OUT &= ~(BIT7);
				break;
			case 3:
				P2OUT |= BIT3 | BIT7;
				P2OUT &= ~(BIT4 | BIT5 | BIT6);

				P3OUT |= BIT5 | BIT7 | BIT0;
				P3OUT &= ~(BIT6);
				break;
			case 4:
				P2OUT |= BIT3 | BIT4 | BIT7;
				P2OUT &= ~(BIT5 | BIT6);

				P3OUT |= BIT5 | BIT0;
				P3OUT &= ~(BIT6 | BIT7);
				break;
			case 5:
				P2OUT |= BIT7;
				P2OUT &= ~(BIT3 | BIT4 | BIT5 | BIT6);

				P3OUT |= BIT6 | BIT7 | BIT0;
				P3OUT &= ~(BIT5);
				break;
			default:
				break;
			}

		//printf("%d\r\t", key_value);
		if (ADC_Flag == 0)
		{
			tft_clean_cnt++;
			if (tft_clean_cnt > 10)
			{
				LCD_TFT_fillrect(10, 10, 10 + 128, 10 + 128, White);
				tft_clean_cnt = 0;
			}

			for (int j = 0; j < 1024; j++){
				LCD_TFT_pixel(10 + X[j] / 128, 10 + 128 - Y[j] / 128, Blue);
				//printf("X=%d,Y=%d\r\t", X[j]/128,Y[j]/128);
			}
			//printf("======================================\r\t");
			ADC_Flag = 1;
			ii = 0;
		}
		//				LCD_TFT_pixel(10 + resultsBuffer[0]/128 ,10 + 128 - resultsBuffer[1]/128, Blue);
		P1->OUT ^= BIT0; // 运行指示灯(闪烁)
						 //        DelayMs(500);
	}
}

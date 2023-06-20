#ifndef __LCD_Display_API_H
#define __LCD_Display_API_H

#include "ti/devices/msp432p4xx/inc/msp.h"

// ------------------------------------------------------
// 用户自定义头文件
#include "LCD_eUSCI_B0_SPI.h"
#include "LCD_Font.h"

// ==========================================================================
// 屏幕方向: 共4种, 横向宽高=320*240; 纵向宽高=240*320
// 0=屏幕横向放置, 右侧引出电缆线; 屏幕纵向放置, 上侧引出电缆线; 
// 1=屏幕横向放置, 左侧引出电缆线; 屏幕纵向放置, 下侧引出电缆线; 
// 2=屏幕纵向放置, 下侧引出电缆线; 屏幕横向放置, 左侧引出电缆线;
// 3=屏幕纵向放置, 上侧引出电缆线; 屏幕横向放置, 右侧引出电缆线;
#define Orientation     2       // 0=横向宽高=320*240; 1=纵向宽高=240*320

// 配置字体大小
#define FONT1206   (1)  //字体12*6
#define FONT1608   (2)  //字体16*8

#define ROW  320        //Y 显示的行数
#define COL  240        //X 显示的列数

#define M_Space_ASCII       (0x20)  //"空格"的ASCII码

#define M_Vertical_FONT     (16)    //字体"垂直宽度" , 显示"行距"
#define M_Horizontal_FONT   (8)     //字体"水平宽度" , 显示"字符间距"

#define MAX_MSG_LEN             256 // generic message length

// ==========================================================================
// 液晶屏颜色, 前景色/背景色均可使用
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */

#define BPP             16          // Bits per pixel

// ==========================================================================
// LCD 初始化
void LCD_TFT_Init(void);
// TFT LCD 清屏, 并填充单色
void LCD_TFT_Clear(uint16_t color);
// 液晶模块复位
void LCD_TFT_Reset(void);

// 获取屏幕宽/高
uint16_t LCD_TFT_Get_Width(void);
uint16_t LCD_TFT_Get_Height(void);
// 设置屏幕最大区域
void LCD_TFT_Set_WindowMax(void);
// 设置显示方向
void LCD_TFT_Set_Orientation(void);
// 设置窗口区域
void LCD_TFT_Set_Window (uint16_t x, uint16_t y, uint16_t w, uint16_t h);
// 格式转换: 整数-->字符串
uint32_t IntToStr(int32_t uiInteger, char *cStr);

// 显示一个字符( 指定坐标,字体大小,颜色)
void LCD_TFT_ShowChar(uint16_t line,uint16_t column,uint8_t AscNum,uint8_t Font,uint16_t pointColor,uint16_t backColor);
// 显示字符串( 指定坐标,字体大小,颜色)
void LCD_TFT_ShowString(uint16_t column,uint16_t line,uint8_t *ArrayPoint,uint8_t Font,uint16_t pointColor,uint16_t backColor);
// 显示1个24*24的汉字
void LCD_Show_ChFont2424(uint16_t x,uint16_t y,uint8_t index,uint16_t pointColor,uint16_t backColor);
void LCD_Show_ChFont1616(uint16_t x,uint16_t y,uint8_t index,uint16_t pointColor,uint16_t backColor);
// 显示"待机页面
void LCD_Show_StandbyPage(void);

// 显示像素点
void LCD_TFT_pixel(uint16_t x, uint16_t y, uint16_t color);
// 绘制直线 "水平"
void LCD_TFT_hline(int x0, int x1, int y, uint16_t color);
// 绘制直线 "垂直"
void LCD_TFT_vline(int x, int y0, int y1, uint16_t color);
// 绘制直线 "斜线"
void LCD_TFT_line(int x0, int y0, int x1, int y1, uint16_t color);
// 绘制空心矩形
void LCD_TFT_rect(int x0, int y0, int x1, int y1, uint16_t color);
// 绘制实心矩形
void LCD_TFT_fillrect(int x0, int y0, int x1, int y1, uint16_t color);
// 绘制空心圆
void LCD_TFT_circle(int x0, int y0, int r, uint16_t color);
// 绘制实心圆
void LCD_TFT_fillcircle(int x, int y, int r, uint16_t color);

#endif

// ==========================================================================
// End of file.

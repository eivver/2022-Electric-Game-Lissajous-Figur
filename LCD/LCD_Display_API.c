#include "LCD_Display_API.h"    // LCD 显示接口函数

// ==========================================================================
#define  RS_CMD            0xCC    // 命令
#define  RS_DAT            0xDD    // 数据

// ==========================================================================
// LCD 初始化
void LCD_TFT_Init(void)
{
    LCD_TFT_Reset();            // 液晶模块复位
    LCD_TFT_Clear(White);       // LCD 清屏, 并填充单色
}

// ==========================================================================
// TFT LCD 清屏, 并填充单色
void LCD_TFT_Clear(uint16_t color)
{
    uint32_t i;

    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    LCD_TFT_Set_WindowMax();

    //LCD_TFT_RS_CMD;     // 输出高电平: 寄存器
    LCD_TFT_RS_DAT;     // 输出低电平: 数据
    LCD_TFT_CSn_L;      // 片选输出: 低电平

    for (i = 0; i < 240*320; i++) 
    {
        LCD_TFT_SPI_WR(color_h);
        LCD_TFT_SPI_WR(color_l);
    }
    LCD_TFT_CSn_H;    // 片选输出: 高电平
}

// ==========================================================================
// 获取屏幕宽/高
uint16_t LCD_TFT_Get_Width(void)
{
    if((Orientation == 0) || (Orientation == 1))
        return 320;
    else
        return 240;
}
uint16_t LCD_TFT_Get_Height(void)
{
    if((Orientation == 0) || (Orientation == 1))
        return 240;
    else
        return 320;
}

// ==========================================================================
// 设置屏幕最大区域
void LCD_TFT_Set_WindowMax(void)
{
    LCD_TFT_Set_Window(0, 0, LCD_TFT_Get_Width(), LCD_TFT_Get_Height());
}

// ==========================================================================
// 设置显示方向
void LCD_TFT_Set_Orientation(void) 
{
    switch (Orientation) 
    {
        case 0:
            LCD_TFT_SPI_WR_RS(RS_CMD, 0x36);
            LCD_TFT_SPI_WR_RS(RS_DAT, 0x28);
            break;
        case 1:
            LCD_TFT_SPI_WR_RS(RS_CMD, 0x36);
            LCD_TFT_SPI_WR_RS(RS_DAT, 0xE8);
            break;
        case 2:
            LCD_TFT_SPI_WR_RS(RS_CMD, 0x36);
            LCD_TFT_SPI_WR_RS(RS_DAT, 0x48);
            break;
        case 3:
            LCD_TFT_SPI_WR_RS(RS_CMD, 0x36);
            LCD_TFT_SPI_WR_RS(RS_DAT, 0x88);
            break;
    }
}

// ==========================================================================
// 设置窗口区域
void LCD_TFT_Set_Window (uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  LCD_TFT_SPI_WR_RS(RS_CMD, 0x2A);     // column addr set
  LCD_TFT_SPI_WR_RS(RS_DAT, x>>8);
  LCD_TFT_SPI_WR_RS(RS_DAT, x);        // XSTART
  LCD_TFT_SPI_WR_RS(RS_DAT, (x+w-1)>>8);
  LCD_TFT_SPI_WR_RS(RS_DAT, (x+w-1));  // XEND

  LCD_TFT_SPI_WR_RS(RS_CMD, 0x2B);     // row addr set
  LCD_TFT_SPI_WR_RS(RS_DAT, y>>8);
  LCD_TFT_SPI_WR_RS(RS_DAT, y);        // YSTART
  LCD_TFT_SPI_WR_RS(RS_DAT, (y+h-1)>>8);
  LCD_TFT_SPI_WR_RS(RS_DAT, (y+h-1));  // YEND

  LCD_TFT_SPI_WR_RS(RS_CMD, 0x2C);     // write to RAM
}

// ==========================================================================
// 液晶模块复位
void LCD_TFT_Reset(void)
{
    // Start Initial Sequence --------------------------------------------------
    LCD_TFT_SPI_WR_RS(RS_CMD, 0x01);       // SW Reset
    DelayMs(150);
    LCD_TFT_SPI_WR_RS(RS_CMD, 0x28);       // display off
    DelayMs(500);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xCF);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x83);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x30);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xED);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x64);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x03);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x12);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x81);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xEB);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x85);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x01);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x79);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xCB);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x39);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x2C);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x34);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x02);


    LCD_TFT_SPI_WR_RS(RS_CMD, 0xF7);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x20);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xEA);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xC0);     // POWER_CONTROL_1
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x26);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0XC1);     // POWER_CONTROL_2
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x11);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xC5);     // VCOM_CONTROL_1
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x35);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x3E);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0XC7);     // VCOM_CONTROL_2
    LCD_TFT_SPI_WR_RS(RS_DAT, 0xBE);

    //LCD_TFT_SPI_WR_RS(RS_CMD, 0x36);     // MEMORY_ACCESS_CONTROL
    //LCD_TFT_SPI_WR_RS(RS_DAT, 0x48);
    LCD_TFT_Set_Orientation();

    LCD_TFT_SPI_WR_RS(RS_CMD, 0x3A);     // COLMOD_PIXEL_FORMAT_SET
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x55);     // 16 bit pixel


    LCD_TFT_SPI_WR_RS(RS_CMD, 0xB1);     // Frame Rate
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x1B);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0XF2);     // Gamma Function Disable
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x08);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0x26);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x01);     // gamma set for curve 01/2/04/08

    // Gamma settings  -----------------------------------------------------------

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xE0);     // positive gamma correction
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x1F);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x1A);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x18);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x0A);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x0F);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x06);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x45);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x87);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x32);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x0A);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x07);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x02);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x07);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x05);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xE1);    // negativ gamma correction
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x25);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x27);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x05);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x10);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x09);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x3A);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x78);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x4D);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x05);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x18);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x0D);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x38);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x3A);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x1F);

    LCD_TFT_Set_WindowMax ();


    LCD_TFT_SPI_WR_RS(RS_CMD, 0xB7);   // entry mode
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x07);   //

    LCD_TFT_SPI_WR_RS(RS_CMD, 0xB6);   // display function control
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x0A);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x82);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x27);
    LCD_TFT_SPI_WR_RS(RS_DAT, 0x00);


    LCD_TFT_SPI_WR_RS(RS_CMD, 0x11);    // sleep out
    DelayMs(100);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0x29);    // display on
    DelayMs(100);

}

// ==========================================================================
// 格式转换: 整数-->字符串
uint32_t IntToStr(int32_t uiInteger, char *cStr)
{
    uint16_t ilen = 0, i;
    char ctemp;

    *cStr = uiInteger % 10;
    while(uiInteger)
    {
        *(cStr + ilen) = uiInteger % 10 + '0';
        uiInteger = uiInteger / 10;
        ilen ++;
    }

    for(i = 0; i < ilen / 2; i++)
    {
        ctemp = cStr[i];
        cStr[i] = cStr[ilen - i-1];
        cStr[ilen - i - 1] = ctemp;
    }
    return ilen;
}


// ==========================================================================
// 显示一个字符( 指定坐标,字体大小,颜色)
void LCD_TFT_ShowChar(uint16_t line,uint16_t column,uint8_t AscNum,uint8_t Font,uint16_t pointColor,uint16_t backColor)
{
    uint8_t  AscChar = 0;
    uint8_t  i = 0;
    uint8_t  j = 0;

    uint8_t  VerticalNum = 0;
    uint8_t  horizontalNum = 0;

    AscNum = AscNum - M_Space_ASCII;    // 得到偏移后的值; M_Space_ASCII 为空格值


    uint16_t xBuf = 0;
    xBuf = column;          // 备份 x 坐标值

    switch(Font)
    {
        case FONT1206:      // 字体 12*6
            VerticalNum = 12;
            horizontalNum = 6;
            break;
        case FONT1608:      // 字体 16*8
            VerticalNum = 16;
            horizontalNum = 8;
            break;
        default:   break;
    }

    // ------------------------------------------------
    for(i = 0; i < VerticalNum; i++)
    {
        switch(Font)
        {
            case FONT1206:  // 字体 12*6
                AscChar = ASCII_12x06[AscNum][i];   // 调用 1206 字体
                break;

            case FONT1608:  // 字体 16*8
                AscChar = ASCII_16x8[AscNum][i];    // 调用 1608 字体
                break;
            default:   break;
        }

        // ------------------------------------------------
        for(j = 0; j < horizontalNum; j++)
        {
            if(AscChar & 0x01) 
                LCD_TFT_pixel(line, column, pointColor);    // 画字符处的点
            else  
                LCD_TFT_pixel(line, column, backColor);     // 画空白处的点

            AscChar >>= 1;
            column ++;
        }

        column = xBuf;
        line ++;
    }

}

// ==========================================================================
// 显示字符串( 指定坐标,字体大小,颜色)
void LCD_TFT_ShowString(uint16_t column,uint16_t line,uint8_t *ArrayPoint,uint8_t Font,uint16_t pointColor,uint16_t backColor)
{
    uint16_t x = 0;
    uint16_t y = 0;

    uint8_t  VerticalNum = 0;
    uint8_t  horizontalNum = 0;

    uint16_t max_x = 0;
    uint16_t max_y = 0;

    x = column;
    y = line;
    switch(Font)
    {
        case FONT1206:      // 字体 12*6
            VerticalNum = 12;
            horizontalNum = 6;
            max_x = 320;
            max_y = 240;
            break;
        case FONT1608:      // 字体 16*8
            VerticalNum = 16;
            horizontalNum = 8;
            max_x = 320;
            max_y = 240;
            break;
        default:   break;
    }

    // ------------------------------------------------
    while(*ArrayPoint != '\0')
    {
        if(x > max_x) 
        {
            x = 0; 
            y += VerticalNum;
        }
        if(y > max_y) 
        {
            x = y = 0; 
            LCD_TFT_Clear(backColor);
        }

        LCD_TFT_ShowChar(y,x,*ArrayPoint,Font,pointColor,backColor);

        x += horizontalNum;     // 坐标地址累加，指向下一个字符的显示位置
        ArrayPoint ++;          // 数组指针累加，指向下一个字符
    }
}
// ==========================================================================
// 功能 : 显示1个24*24点阵的汉字
// 输入参数: (x,y): 汉字显示的位置      index: tfont24 数组里面的第几个汉字
//           pointColor: 汉字画笔颜色;  backColor: 背景颜色;
// 输出参数: None
void LCD_Show_ChFont2424(uint16_t x,uint16_t y,uint8_t index,uint16_t pointColor,uint16_t backColor)
{
    uint8_t byteNum = 0;
    uint8_t i = 0;
    uint8_t temp;

    uint16_t x0=x;

    for(byteNum = 0; byteNum < 72; byteNum++)   // 每个 24*24 的字点阵72个字节
    {
        // 二维数组, 每一维最大24字节
        if(byteNum < 24) 
            temp = Front_24x24[index*3][byteNum];           // 前24个字节
        else if(byteNum < 48) 
            temp=Front_24x24[index*3 + 1][byteNum - 24];    // 中24个字节
        else
            temp = Front_24x24[index*3 + 2][byteNum - 48];  // 后24个字节

        for(i = 0; i < 8; i++)     // 每个字节8位画点
        {
            if(temp & 0x80) 
                LCD_TFT_pixel(x,y,pointColor);  // 画汉字的点
            else 
                LCD_TFT_pixel(x,y,backColor);   // 画空白点（使用背景色）
            temp <<= 1;
            x ++;
            if((x-x0) == 24)    // 达到点阵的最大值则开始新的一行
            {
                x=x0;
                y++;
                break;
            }
        }
    }
}

// ==========================================================================
// 功能 : 显示1个24*24点阵的汉字
// 输入参数: (x,y): 汉字显示的位置      index: tfont24 数组里面的第几个汉字
//           pointColor: 汉字画笔颜色;  backColor: 背景颜色;
// 输出参数: None
void LCD_Show_ChFont1616(uint16_t x,uint16_t y,uint8_t index,uint16_t pointColor,uint16_t backColor)
{
    uint8_t byteNum = 0;
    uint8_t i = 0;
    uint8_t temp;

    uint16_t x0=x;

    for(byteNum = 0; byteNum < 32; byteNum++)   // 每个 16*16 的字点阵72个字节
    {
        // 二维数组, 每一维最大24字节
        if(byteNum < 16) 
            temp = Front_16x16[index*2][byteNum];           // 前16个字节
				else
            temp = Front_16x16[index*2 + 1][byteNum - 16];  // 后16个字节

        for(i = 0; i < 8; i++)     // 每个字节8位画点
        {
            if(temp & 0x80) 
                LCD_TFT_pixel(x,y,pointColor);  // 画汉字的点
            else 
                LCD_TFT_pixel(x,y,backColor);   // 画空白点（使用背景色）
            temp <<= 1;
            x ++;
            if((x-x0) == 16)    // 达到点阵的最大值则开始新的一行
            {
                x=x0;
                y++;
                break;
            }
        }
    }
}

// ==========================================================================
// 显示像素点
void LCD_TFT_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    unsigned char color_h;
    unsigned char color_l;
    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    if ((x >= LCD_TFT_Get_Width()) || (y >= LCD_TFT_Get_Height())) 
        return;

    LCD_TFT_Set_Window(x,y,x+1,y+1);

    LCD_TFT_RS_DAT;     // 输出低电平: 数据
    LCD_TFT_CSn_L;      // 片选输出: 低电平

    LCD_TFT_SPI_WR(color_h);
    LCD_TFT_SPI_WR(color_l);

    LCD_TFT_CSn_H;    // 片选输出: 高电平
}

// ==========================================================================
// 绘制直线 "水平"
void LCD_TFT_hline(int x0, int x1, int y, uint16_t color)
{
    int x;
    int w;
    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    w = x1 - x0 + 1;

    LCD_TFT_Set_Window(x0,y,w,1);


    LCD_TFT_SPI_WR_RS(RS_CMD, 0x2C);

    LCD_TFT_RS_DAT;     // 输出低电平: 数据
    LCD_TFT_CSn_L;      // 片选输出: 低电平

    for (x = 0; x < w; x++)
    {

        LCD_TFT_SPI_WR(color_h);
        LCD_TFT_SPI_WR(color_l);
    }

    LCD_TFT_CSn_H;    // 片选输出: 高电平
}

// ==========================================================================
// 绘制直线 "垂直"
void LCD_TFT_vline(int x, int y0, int y1, uint16_t color)
{
    int y;
    int h;

    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    h = y1 - y0 + 1;
    LCD_TFT_Set_Window(x,y0,1,h);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0x2C);

    LCD_TFT_RS_DAT;     // 输出低电平: 数据
    LCD_TFT_CSn_L;      // 片选输出: 低电平

    for (y = 0; y < h; y++)
    {

        LCD_TFT_SPI_WR(color_h);
        LCD_TFT_SPI_WR(color_l);
    }

    LCD_TFT_CSn_H;    // 片选输出: 高电平
}

// ==========================================================================
// 绘制直线 "斜线"
void LCD_TFT_line(int x0, int y0, int x1, int y1, uint16_t color)
{
    LCD_TFT_Set_WindowMax();

    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;

    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0)        // vertical line
    {
        if (y1 > y0) 
            LCD_TFT_vline(x0,y0,y1,color);
        else 
            LCD_TFT_vline(x0,y1,y0,color);
        return;
    }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0)        // horizontal line
    {
        if (x1 > x0) 
            LCD_TFT_hline(x0,x1,y0,color);
        else  
            LCD_TFT_hline(x1,x0,y0,color);
        return;
    }

    if (dy > 0)
        dy_sym = 1;
    else
        dy_sym = -1;

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy)
    {
        di = dy_x2 - dx;
        while (x0 != x1)
        {

            LCD_TFT_pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0)
            {
                di += dy_x2;
            } 
            else
            {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        LCD_TFT_pixel(x0, y0, color);
    }
    else
    {
        di = dx_x2 - dy;
        while (y0 != y1)
        {
            LCD_TFT_pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0)
            {
                di += dx_x2;
            } 
            else
            {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        LCD_TFT_pixel(x0, y0, color);
    }
}

// ==========================================================================
// 绘制空心矩形
void LCD_TFT_rect(int x0, int y0, int x1, int y1, uint16_t color) 
{
    if (x1 > x0) 
        LCD_TFT_hline(x0,x1,y0,color);
    else  
        LCD_TFT_hline(x1,x0,y0,color);

    if (y1 > y0) 
        LCD_TFT_vline(x0,y0,y1,color);
    else 
        LCD_TFT_vline(x0,y1,y0,color);

    if (x1 > x0) 
        LCD_TFT_hline(x0,x1,y1,color);
    else  
        LCD_TFT_hline(x1,x0,y1,color);

    if (y1 > y0) 
        LCD_TFT_vline(x1,y0,y1,color);
    else 
        LCD_TFT_vline(x1,y1,y0,color);
}

// ==========================================================================
// 绘制实心矩形
void LCD_TFT_fillrect(int x0, int y0, int x1, int y1, uint16_t color) 
{
    int p;
    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    int h = y1 - y0 + 1;
    int w = x1 - x0 + 1;
    int pixel = h * w;

    LCD_TFT_Set_Window(x0,y0,w,h);

    LCD_TFT_SPI_WR_RS(RS_CMD, 0x2C);

    LCD_TFT_RS_DAT;     // 输出低电平: 数据
    LCD_TFT_CSn_L;      // 片选输出: 低电平

    for (p=0; p<pixel; p++) 
    {
        LCD_TFT_SPI_WR(color_h);
        LCD_TFT_SPI_WR(color_l);
    }

    LCD_TFT_CSn_H;    // 片选输出: 高电平
}

// ==========================================================================
// 绘制空心圆
void LCD_TFT_circle(int x0, int y0, int r, uint16_t color)
{
    int draw_x0, draw_y0;
    int draw_x1, draw_y1;
    int draw_x2, draw_y2;
    int draw_x3, draw_y3;
    int draw_x4, draw_y4;
    int draw_x5, draw_y5;
    int draw_x6, draw_y6;
    int draw_x7, draw_y7;
    int xx, yy;
    int di;
    LCD_TFT_Set_WindowMax();
    if (r == 0)       // no radius
    {
        return;
    }
    
    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < LCD_TFT_Get_Height())
    {
        LCD_TFT_pixel(draw_x0, draw_y0, color);     // 90 degree
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0)
    {
        LCD_TFT_pixel(draw_x2, draw_y2, color);    // 270 degree
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < LCD_TFT_Get_Width())
    {
        LCD_TFT_pixel(draw_x4, draw_y4, color);     // 0 degree
    }

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5>=0)
    {
        LCD_TFT_pixel(draw_x5, draw_y5, color);     // 180 degree
    }

    if (r == 1)
    {
        return;
    }

    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while (xx < yy)
    {
        if (di < 0)
        {
            di += 4*xx + 6;
        } 
        else
        {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        if ( (draw_x0 <= LCD_TFT_Get_Width()) && (draw_y0>=0) )
        {
            LCD_TFT_pixel(draw_x0, draw_y0, color);
        }
        if ( (draw_x1 >= 0) && (draw_y1 >= 0) )
        {
            LCD_TFT_pixel(draw_x1, draw_y1, color);
        }
        if ( (draw_x2 <= LCD_TFT_Get_Width()) && (draw_y2 <= LCD_TFT_Get_Height()) )
        {
            LCD_TFT_pixel(draw_x2, draw_y2, color);
        }
        if ( (draw_x3 >=0 ) && (draw_y3 <= LCD_TFT_Get_Height()) )
        {
            LCD_TFT_pixel(draw_x3, draw_y3, color);
        }
        if ( (draw_x4 <= LCD_TFT_Get_Width()) && (draw_y4 >= 0) )
        {
            LCD_TFT_pixel(draw_x4, draw_y4, color);
        }
        if ( (draw_x5 >= 0) && (draw_y5 >= 0) )
        {
            LCD_TFT_pixel(draw_x5, draw_y5, color);
        }
        if ( (draw_x6 <=LCD_TFT_Get_Width()) && (draw_y6 <= LCD_TFT_Get_Height()) )
        {
            LCD_TFT_pixel(draw_x6, draw_y6, color);
        }
        if ( (draw_x7 >= 0) && (draw_y7 <= LCD_TFT_Get_Height()) )
        {
            LCD_TFT_pixel(draw_x7, draw_y7, color);
        }
    }
}
// ==========================================================================
// 绘制实心圆
void LCD_TFT_fillcircle(int x, int y, int r, uint16_t color)
{
    int i;
    for (i = 0; i <= r; i++)
    {
        LCD_TFT_circle(x,y,i,color);
    }
}

// ==========================================================================
// End of file.


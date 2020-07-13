#ifndef _OLED_H_
#define _OLED_H_

#include <Arduino.h>
#include "font.h"
#include "Periph.h"

#define OLED_SCL PB4
#define OLED_SDA PB3
#define OLED_RES PD2

#define OLED_SCLK_Clr() digitalWrite(OLED_SCL, LOW) //SCL
#define OLED_SCLK_Set() digitalWrite(OLED_SCL, HIGH)

#define OLED_SDIN_Clr() digitalWrite(OLED_SDA, LOW) //SDA
#define OLED_SDIN_Set() digitalWrite(OLED_SDA, HIGH)

#define OLED_RST_Clr() digitalWrite(OLED_RES, LOW) //RES   注：此引脚是为了配合SPI驱动模块改成I2C驱动模块使用的（改装的话必须接），如果买的是I2C模块，请忽略此引脚。
#define OLED_RST_Set() digitalWrite(OLED_RES, HIGH)

#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据

void OLED_Init(void);             //OLED的初始化
void OLED_ColorTurn(uint8_t i);   //反显函数
void OLED_DisplayTurn(uint8_t i); //屏幕旋转180度

void I2C_Start(void);   //起始信号
void I2C_Stop(void);    //结束信号
void I2C_WaitAck(void); //等待信号响应,测数据信号的电平

void Send_Byte(uint8_t dat); //写入一个字节

void OLED_WR_Byte(uint8_t dat, uint8_t mode); //发送一个字节
void OLED_Refresh(void);                      //更新显存到OLED
void OLED_Clear(void);                        //清屏函数

uint32_t OLED_Pow(uint8_t m, uint8_t n); //m^n

void OLED_DrawPoint(uint8_t x, uint8_t y);                                                  //画点
void OLED_ClearPoint(uint8_t x, uint8_t y);                                                 //清除一个点
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);                         //画线
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r);                                      //画圆
void OLED_ShowChar(uint8_t x, uint8_t y, const char chr, uint8_t size1);                    //在指定位置显示一个字符,包括部分字符
void OLED_ShowString(uint8_t x, uint8_t y, char *chr, uint8_t size1);                 //显示字符串
void OLED_ShowNum(uint8_t x, uint8_t y, int num, uint8_t len, uint8_t size1);               //显示2个数字
void OLED_ShowChinese(uint8_t x, uint8_t y, const uint8_t num, uint8_t size1);              //显示汉字
void OLED_WR_BP(uint8_t x, uint8_t y);                                                      //配置写入数据的起始位置
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]); //显示图片
void OLED_ShowSmallPicture(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, const unsigned char * picture);//


enum Signal_Level{Wait_Connect,One_cell,Two_cell,Three_cell,Four_cell,No_signal};

class OLED_Display{
    public:
        void Clear_UP_Screen(void);//清除上半部分屏
        void Clear_down_Screen(void);//清除下部分屏
        void Display_Collecting(void);//显示正在采集
        void Display_WaitSetting(void);//显示等待配置
        void Display_Electricity(unsigned int Vol);//显示电量
        void Display_Signal(unsigned char Level = Wait_Connect);//显示信号
        void Display_Sensor_Data(void);//显示传感器数据
        void Display_Collect_time(void);//显示采集时间
    protected:
};


extern OLED_Display MyOLED;


#endif
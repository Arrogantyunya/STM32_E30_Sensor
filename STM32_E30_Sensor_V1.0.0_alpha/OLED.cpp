#include "OLED.h"
#include "Private_Sensor.h"
#include "Private_RTC.h"

/*Create EEPROM project*/
OLED_Display MyOLED;

uint8_t OLED_GRAM[128][8]; //将要显示的缓存内容

//OLED的初始化
void OLED_Init(void)
{
    pinMode(OLED_SCL, OUTPUT); //设置数字8
    pinMode(OLED_SDA, OUTPUT); //设置数字9
    pinMode(OLED_RES, OUTPUT); //设置数字10

    OLED_RST_Set();
    delay(100);
    OLED_RST_Clr(); //复位
    delay(200);
    OLED_RST_Set();

    OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
    OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
    OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
    OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); //-not offset
    OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
    OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
    OLED_WR_Byte(0x40, OLED_CMD); //Set VCOM Deselect Level
    OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02, OLED_CMD); //
    OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
    OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF, OLED_CMD);
    OLED_Clear();
}

//反显函数
void OLED_ColorTurn(uint8_t i)
{
    if (!i)
        OLED_WR_Byte(0xA6, OLED_CMD); //正常显示
    else
        OLED_WR_Byte(0xA7, OLED_CMD); //反色显示
}

//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); //正常显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    else
    {
        OLED_WR_Byte(0xC0, OLED_CMD); //反转显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

//起始信号
void I2C_Start(void)
{
    OLED_SDIN_Set();
    OLED_SCLK_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

//结束信号
void I2C_Stop(void)
{
    OLED_SCLK_Set();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}

//写入一个字节
void Send_Byte(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_SCLK_Clr(); //将时钟信号设置为低电平
        if (dat & 0x80)  //将dat的8位从最高位依次写入
        {
            OLED_SDIN_Set();
        }
        else
        {
            OLED_SDIN_Clr();
        }
        OLED_SCLK_Set(); //将时钟信号设置为高电平
        OLED_SCLK_Clr(); //将时钟信号设置为低电平
        dat <<= 1;
    }
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    if (mode)
    {
        Send_Byte(0x40);
    }
    else
    {
        Send_Byte(0x00);
    }
    I2C_WaitAck();
    Send_Byte(dat);
    I2C_WaitAck();
    I2C_Stop();
}

//更新显存到OLED
void OLED_Refresh(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
        OLED_WR_Byte(0x00, OLED_CMD);     //设置低列起始地址
        OLED_WR_Byte(0x10, OLED_CMD);     //设置高列起始地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
    }
}

//清屏函数
void OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0; //清除所有数据
        }
    }
    OLED_Refresh(); //更新显示
}

//m^n
uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}

//画点
//x:0~127
//y:0~63
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    OLED_GRAM[x][i] |= n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(uint8_t x, uint8_t y)
{
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    OLED_GRAM[x][i] |= n;
    OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

//画线
//x:0~128
//y:0~64
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    uint8_t i, k, k1, k2, y0;
    if (x1 == x2) //画竖线
    {
        for (i = 0; i < (y2 - y1); i++)
        {
            OLED_DrawPoint(x1, y1 + i);
        }
    }
    else if (y1 == y2) //画横线
    {
        for (i = 0; i < (x2 - x1); i++)
        {
            OLED_DrawPoint(x1 + i, y1);
        }
    }
    else //画斜线
    {
        k1 = y2 - y1;
        k2 = x2 - x1;
        k = k1 * 10 / k2;
        for (i = 0; i < (x2 - x1); i++)
        {
            OLED_DrawPoint(x1 + i, y1 + i * k / 10);
        }
    }
}

//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    a = 0;
    b = r;
    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r * r; //计算画的点离圆心的距离
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(uint8_t x, uint8_t y, const char chr, uint8_t size1)
{
    uint8_t i, m, temp, size2, chr1;
    uint8_t y0 = y;
    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
    chr1 = chr - ' ';                                          //计算偏移后的值
    for (i = 0; i < size2; i++)
    {
        if (size1 == 12)
        {
            temp = pgm_read_byte(&asc2_1206[chr1][i]);
        } //调用1206字体
        else if (size1 == 16)
        {
            temp = pgm_read_byte(&asc2_1608[chr1][i]);
        } //调用1608字体
        else if (size1 == 24)
        {
            temp = pgm_read_byte(&asc2_2412[chr1][i]);
        } //调用2412字体
        else
            return;
        for (m = 0; m < 8; m++) //写入数据
        {
            if (temp & 0x80)
                OLED_DrawPoint(x, y);
            else
                OLED_ClearPoint(x, y);
            temp <<= 1;
            y++;
            if ((y - y0) == size1)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
void OLED_ShowString(uint8_t x, uint8_t y, char *chr, uint8_t size1)
{
    while ((*chr >= ' ') && (*chr <= '~')) //判断是不是非法字符!
    {
        OLED_ShowChar(x, y, *chr, size1);
        x += size1 / 2;
        if (x > 128 - size1 / 2) //换行
        {
            x = 0;
            y += size1;
        }
        chr++;
    }
}

////显示2个数字
////x,y :起点坐标
////len :数字的位数
////size:字体大小
void OLED_ShowNum(uint8_t x, uint8_t y, int num, uint8_t len, uint8_t size1)
{
    uint8_t t, temp;
    for (t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0)
        {
            OLED_ShowChar(x + (size1 / 2) * t, y, '0', size1);
        }
        else
        {
            OLED_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1);
        }
    }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
void OLED_ShowChinese(uint8_t x, uint8_t y, const uint8_t num, uint8_t size1)
{
    uint8_t i, m, n = 0, temp, chr1;
    uint8_t x0 = x, y0 = y;
    uint8_t size3 = size1 / 8;
    while (size3--)
    {
        chr1 = num * size1 / 8 + n;
        n++;
        for (i = 0; i < size1; i++)
        {
            if (size1 == 16)
            {
                temp = pgm_read_byte(&Hzk1[chr1][i]);
            } //调用16*16字体
            else if (size1 == 24)
            {
                temp = pgm_read_byte(&Hzk2[chr1][i]);
            } //调用24*24字体
            else if (size1 == 32)
            {
                temp = pgm_read_byte(&Hzk3[chr1][i]);
            } //调用32*32字体
            else if (size1 == 64)
            {
                temp = pgm_read_byte(&Hzk4[chr1][i]);
            } //调用64*64字体
            else
                return;

            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    OLED_DrawPoint(x, y);
                else
                    OLED_ClearPoint(x, y);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == size1)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}

//配置写入数据的起始位置
void OLED_WR_BP(uint8_t x, uint8_t y)
{
    OLED_WR_Byte(0xb0 + y, OLED_CMD); //设置行起始地址
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[])
{
    int j = 0;
    uint8_t t;
    uint8_t x, y;
    for (y = y0; y < y1; y++)
    {
        OLED_WR_BP(x0, y);
        for (x = x0; x < x1; x++)
        {
            t = pgm_read_byte(&BMP[j++]);
            OLED_WR_Byte(t, OLED_DATA);
        }
    }
}


/*
 *brief   : 显示小图片
 *para    : 起点位置的x坐标
            起点位置的Y坐标
            图片的宽度
            图片的高度
            图片存放的数组
 *return  : 无
 */
void OLED_ShowSmallPicture(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, const unsigned char * picture)
{
    // 图片大小17*8     OLED_ShowSmallPicture(0,0,17,8,Four_grid_electric_quantity);
    //起点0，0  终点17，8

    if(x0>=128 || y0>=64 || width<=0 || height<= 0)  return;

    unsigned char Now_height_copies = 0;//当前高度份数
    unsigned char bit = 0;//

    /* 先写横线 */
    for (unsigned char i = 0; i < width; i++)
    {
        Now_height_copies = 0;bit = 0;
        for (unsigned char j = 0; j < height; j++)
        {   
            if((picture[i + (Now_height_copies*width)] >> bit) & 0b1)
                OLED_DrawPoint(i+x0,j+y0);
            else
                OLED_ClearPoint(i+x0,j+y0);

            bit++;
            if (bit>=8){
                bit = 0;Now_height_copies++;
                // Serial.println(String("Now_height_copies = ") + Now_height_copies);
            }
        }
    }
}



//清除上半部分屏
void  OLED_Display::Clear_UP_Screen(void)
{
    uint8_t i, n;
    for (i = 0; i < 1; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0; //清除所有数据
        }
    }
    OLED_Refresh(); //更新显示
}

//清除下部分屏
void OLED_Display::Clear_down_Screen(void)
{
    uint8_t i, n;
    for (i = 1; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
        {
            OLED_GRAM[n][i] = 0; //清除所有数据
        }
    }
    OLED_Refresh(); //更新显示
}

//显示正在采集
void OLED_Display::Display_Collecting(void)
{
    Clear_down_Screen();//先清屏
    // 显示：【开始采集传感器数据】
	OLED_ShowChinese(32,16,1,16);
    OLED_ShowChinese(48,16,2,16);
	OLED_ShowChinese(64,16,3,16);
    OLED_ShowChinese(80,16,4,16);

	OLED_ShowChinese(16,32,5,16);
    OLED_ShowChinese(32,32,6,16);
	OLED_ShowChinese(48,32,7,16);
    OLED_ShowChinese(64,32,8,16);
	OLED_ShowChinese(80,32,9,16);
    OLED_Refresh(); //更新显示
}

//显示等待配置
void OLED_Display::Display_WaitSetting(void)
{
    Clear_down_Screen();//先清屏
    // 显示：【等待服务器配置参数】
	OLED_ShowChinese(16,16,10,16);
    OLED_ShowChinese(32,16,11,16);
	OLED_ShowChinese(48,16,12,16);
    OLED_ShowChinese(64,16,13,16);
	OLED_ShowChinese(80,16,14,16);

    OLED_ShowChinese(32,32,15,16);
	OLED_ShowChinese(48,32,16,16);
    OLED_ShowChinese(64,32,17,16);
	OLED_ShowChinese(80,32,18,16);
    OLED_Refresh(); //更新显示
}

//显示电量
void OLED_Display::Display_Electricity(unsigned int Vol)
{
    if (Vol>=4020)
        OLED_ShowSmallPicture(20,0,17,8,&Four_grid_electric_quantity[0]);//4格电量
    else if (Vol >= 3840 && Vol < 4020)
        OLED_ShowSmallPicture(20,0,17,8,&Three_grid_electric_quantity[0]);//3格电量
    else if (Vol >= 3660 && Vol < 3840)
        OLED_ShowSmallPicture(20,0,17,8,&Two_grid_electric_quantity[0]);//2格电量
    else if (Vol >= 3480 && Vol < 3660)
        OLED_ShowSmallPicture(20,0,17,8,&One_grid_electric_quantity[0]);//1格电量
    else if (Vol >= 3300 && Vol < 3480)
        OLED_ShowSmallPicture(20,0,17,8,&Zero_grid_electric_quantity[0]);//0格电量
    
    OLED_Refresh(); //更新显示
}

//显示信号
void OLED_Display::Display_Signal(unsigned char Level)
{
    if (Level == Wait_Connect)
        OLED_ShowSmallPicture(0,0,17,8,&Wait_for_signal[0]);//信号
    else if (Level == One_cell)
        OLED_ShowSmallPicture(0,0,17,8,&One_cell_signal[0]);//信号
    else if (Level == Two_cell)
        OLED_ShowSmallPicture(0,0,17,8,&Two_cell_signal[0]);//信号
    else if (Level == Three_cell)
        OLED_ShowSmallPicture(0,0,17,8,&Three_cell_signal[0]);//信号
    else if (Level == Four_cell)
        OLED_ShowSmallPicture(0,0,17,8,&Four_cell_signal[0]);//信号
    else if (Level == No_signal)
        OLED_ShowSmallPicture(0,0,17,8,&No_signal_connection[0]);//信号
    
    OLED_Refresh(); //更新显示
}

//显示传感器数据
void OLED_Display::Display_Sensor_Data(void)
{
    String Str_OLED_Temp = "";  char *_OLED_Temp = NULL;
    String Str_OLED_Hum = "";   char *_OLED_Hum = NULL;
    String Str_OLED_Lux = "";   char *_OLED_Lux = NULL;
    String Str_OLED_UV = "";    char *_OLED_UV = NULL; 

    Clear_down_Screen();//先清屏

    /* 显示温度 */
    if (Muti_Sensor_Data.GreenHouse_Temp < 100)
	{
		Str_OLED_Temp = String(Muti_Sensor_Data.GreenHouse_Temp) + ".0";
		_OLED_Temp = (char *)Str_OLED_Temp.c_str();
    	OLED_ShowString(0,16,_OLED_Temp,16);
		OLED_ShowChinese(40,16,0,16);/* ℃ */
	}
    else
    {
        Str_OLED_Temp = String("NULL") + ".0";
		_OLED_Temp = (char *)Str_OLED_Temp.c_str();
    	OLED_ShowString(0,16,_OLED_Temp,16);
		OLED_ShowChinese(40,16,0,16);/* ℃ */
    }
    
    /* 显示湿度 */
    if (Muti_Sensor_Data.GreenHouse_Humi < 100)
	{
		Str_OLED_Hum = String(Muti_Sensor_Data.GreenHouse_Humi) + "%RH";
		_OLED_Hum = (char *)Str_OLED_Hum.c_str();
    	OLED_ShowString(64,16,_OLED_Hum,16);
	}
    else
    {
        Str_OLED_Hum = String("NULL") + "%RH";
		_OLED_Hum = (char *)Str_OLED_Hum.c_str();
    	OLED_ShowString(64,16,_OLED_Hum,16);
    }

    /* 显示光照强度 */
	if (Muti_Sensor_Data.GreenHouse_Lux < 500000)
	{
		Str_OLED_Lux = String(Muti_Sensor_Data.GreenHouse_Lux) + "Lx";
		_OLED_Lux = (char *)Str_OLED_Lux.c_str();
    	OLED_ShowString(0,32,_OLED_Lux,16);
	}
    else
    {
        Str_OLED_Lux = String("NULL") + "Lx";
		_OLED_Lux = (char *)Str_OLED_Lux.c_str();
    	OLED_ShowString(0,32,_OLED_Lux,16);
    }
	
    /* 显示紫外强度 */
	if (Muti_Sensor_Data.GreenHouse_UV < 100)
	{
		Str_OLED_UV = String(Muti_Sensor_Data.GreenHouse_UV) + "W/m2";
		_OLED_UV = (char *)Str_OLED_UV.c_str();
    	OLED_ShowString(64,32,_OLED_UV,16);
	}
    else
    {
        Str_OLED_UV = String("NULL") + "W/m2";
		_OLED_UV = (char *)Str_OLED_UV.c_str();
    	OLED_ShowString(64,32,_OLED_UV,16);
    }

    OLED_Refresh(); //更新显示
}

//显示采集时间
void OLED_Display::Display_Collect_time(void)
{
    String Str_OLED_time = "";  char *_OLED_time = NULL;
    Str_OLED_time = String(RtcTime.month)+"/"+String(RtcTime.day)+" "+String(RtcTime.hour)+":"+String(RtcTime.minutes)+":"+String(RtcTime.seconds);
	_OLED_time = (char *)Str_OLED_time.c_str();
    OLED_ShowString(0,48,_OLED_time,16);

    OLED_Refresh(); //更新显示
}
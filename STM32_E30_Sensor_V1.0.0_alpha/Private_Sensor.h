#ifndef _PRIVATE_SENSOR_H
#define _PRIVATE_SENSOR_H

#include "board_config.h"
#include <RTClock.h>

//RS485传感器地址定义
#if E30_DEVICE_V1_0
  #define USE_CJMCU6750                       false
  #define SOIL_SENSOR_ADDR                    0x01
  #define SOIL_SENSOR_FOR_PH_ADDR             0x02
  #define I2C_ADDR                            0x38 //UV sensor
  #define PR_3000_ECTH_N01_V1  false  //未升级前的3合1传感器
  #define PR_3000_ECTH_N01_V2  true   //升级后的3合1传感器
  #define ST_500_Soil_PH		false//一款长的PH，第一代的PH
  #define JXBS_3001_PH      true//两根针的款式，第二代的PH
  #define USE_SHT10         false//
  #define USE_SHT20         true//
  //Integration Time
  #define IT_1_2 0x0 //1/2T
  #define IT_1   0x1 //1T
  #define IT_2   0x2 //2T
  #define IT_4   0x3 //4T
#else
  #define SOIL_SENSOR_ADDR                    0x01
  #if (TYPE06 || TYPE07)
  #define RAINFALL_SENSOR_ADDR                0x02
  #elif TYPE08
  #define RAINFALL_SENSOR_ADDR                0x03
  #endif
  #define GREENHOUSE_SENSOR_ADDR              0x08
  #define AIR_SENSOR_ADDR                     0x05
  #define WIND_RATE_SENSOR_ADDR               0x06
  #define WIND_DIRECTION_SENSOR_ADDR          0x07
#endif


#define g_Wait_Receive_Time     500
#define g_Wait_Collect_Time     500

//传感器数据变量结构体定义
typedef struct{

  struct tm curTime;                  //RTC
  float Soil_Humi;                    //土壤湿度
  unsigned int Soil_Temp;             //土壤温度
  unsigned char Soil_Temp_Flag;       //土壤温度正负标志位
  unsigned int Soil_Cond;             //土壤电导率
  unsigned int Soil_Salt;             //土壤盐度
  unsigned int Soil_PH;               //土壤PH
  unsigned char Rainfall;             //降雨降雪开关量
  unsigned char Rainfall_Buffer[8];
  unsigned int Water_Level;           //水位值
  unsigned int ADC_Value1;            //扩展的ADC电压值
  unsigned int ADC_Value2;            //扩展的ADC电压值
  float Air_Humi;                     //大气百叶箱湿度
  unsigned int Air_Temp;              //大气百叶箱温度
  unsigned char Air_Temp_Flag;        //大气百叶箱温度正负标志位
  unsigned long int Air_Lux;          //大气百叶箱光照度
  unsigned long int Air_Atmos;        //百叶箱大气压力
  float Air_Gas;                      //百叶箱大气浓度
  unsigned int Air_CO2;               //百叶箱二氧化碳浓度
  unsigned int Air_UV;                //空气紫外线
  unsigned int Air_TVOC;              //空气二氧化碳浓度
  float Air_Wind_Speed;               //风速
  unsigned int Wind_DirCode;          //风向
  unsigned long int GreenHouse_Lux;   //大棚光照度
  unsigned long int GreenHouse_Atmos; //大棚大气压力
  float GreenHouse_Temp;              //大棚传感器空气温度
  unsigned char GreenHouse_Temp_Flag; //大棚温度正负标志位
  float GreenHouse_Humi;              //大棚传感器空气湿度
  unsigned int GreenHouse_UV;         //大棚紫外线
  unsigned int GreenHouse_CO2;        //大棚二氧化碳浓度
  unsigned int GreenHouse_TVOC;       //大棚TVOC浓度

}SENSOR_DATA;

extern SENSOR_DATA Muti_Sensor_Data;

void Read_Cond_and_Salt(unsigned int *cond, unsigned int *sat, unsigned char address);
void Read_CO2_and_TVOC(unsigned int *co2, unsigned int *tvoc, unsigned char address);
void Read_Soil_Temp_and_Humi(float *hum, unsigned int *tep, unsigned char *tep_flag, unsigned char address);
void Read_Temp_and_Humi_for_Modbus(float *hum, unsigned int *tep, unsigned char *tep_flag, unsigned char address);
void Read_Temp_and_Humi_for_I2C(float *hum, float *tep, unsigned char *tep_flag);
void Read_Lux_for_Modbus(unsigned long int *lux_value, unsigned char address);
void Read_Atmos(unsigned long int *atmos, unsigned char address);
void Read_Lux_and_UV_for_I2C(unsigned long int *lux_value,unsigned int *uv);
void Read_Rainfall(unsigned char *rainfall, unsigned char address);
void Read_Photics_Rainfall(unsigned char *rainfall_buffer, unsigned char address);
void Read_UV_for_Modbus(unsigned int *uv, unsigned char address);
void Read_Wind_Speed(float *wind_speed, unsigned int address);
void Read_Wind_Direction(unsigned int *wind_direction, unsigned char address);

#if E30_DEVICE_V1_0
  // unsigned int Get_Analogy1_Value(void);
  void CJMCU6750_Init(void);
#else
  unsigned int Get_Analogy1_Value(void);
  unsigned int Get_Analogy2_Value(void);
  unsigned int Get_Analogy3_Value(void);
#endif


void Data_Acquisition(void);

#endif
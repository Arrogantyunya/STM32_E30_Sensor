#ifndef _PERIPH_H
#define _PERIPH_H

#include "board_config.h"

//DC12V电源开、关操作
#if E30_DEVICE_V1_0
#else
    #define DC12V_PWR_ON      (digitalWrite(DC12V_PWR_PIN, HIGH))
    #define DC12V_PWR_OFF     (digitalWrite(DC12V_PWR_PIN, LOW))
#endif


//RS485模块电源开、关操作
#if E30_DEVICE_V1_0
#else
#define RS485_BUS_PWR_ON    (digitalWrite(RS485_BUS_PWR_PIN, HIGH))
#define RS485_BUS_PWR_OFF   (digitalWrite(RS485_BUS_PWR_PIN, LOW))
#endif

//OLED屏幕电源开关操作
#if E30_DEVICE_V1_0
    #define OLED_PWR_ON     (digitalWrite(OLED_PWR, HIGH))
    #define OLED_PWR_OFF    (digitalWrite(OLED_PWR, LOW))  
#else
#endif

//传感器3V3电源开关操作
#if E30_DEVICE_V1_0
    #define Sensor3V3_PWR_ON     (digitalWrite(Sensor3V3_PWR, HIGH))
    #define Sensor3V3_PWR_OFF    (digitalWrite(Sensor3V3_PWR, LOW))  
#else
#endif

//GPRS模块电源开、关操作
#define GPRS_PWR_ON       (digitalWrite(GPRS_PWR_CON_PIN, HIGH))
#define GPRS_PWR_OFF      (digitalWrite(GPRS_PWR_CON_PIN, LOW))

//GPRS PWRKEY 开、关操作
#define GPRS_PWRKEY_HI    (digitalWrite(GPRS_PWRKEY_PIN, HIGH))
#define GPRS_PWRKEY_LO    (digitalWrite(GPRS_PWRKEY_PIN, LOW))

#if E30_DEVICE_V1_0
#else
    #define GPRS_RST_HI       (digitalWrite(GPRS_RST_PIN, HIGH))
    #define GPRS_RST_LO       (digitalWrite(GPRS_RST_PIN, LOW))
#endif


//GPS天线电源开、关操作
#if E30_DEVICE_V1_0
#else
    #define GPS_ANT_PWR_ON    (digitalWrite(GPS_ANT_PWR_CON_PIN, HIGH))
    #define GPS_ANT_PWR_OFF   (digitalWrite(GPS_ANT_PWR_CON_PIN, LOW))
#endif

//两个双色状态灯
#if E30_DEVICE_V1_0
    #define LED1_ON           (digitalWrite(LED1, HIGH))
    #define LED1_OFF          (digitalWrite(LED1, LOW))
    #define LED2_ON           (digitalWrite(LED2, HIGH))
    #define LED2_OFF          (digitalWrite(LED2, LOW))
#else
    #define LED1_ON           (digitalWrite(LED1, HIGH))
    #define LED1_OFF          (digitalWrite(LED1, LOW))
    #define LED2_ON           (digitalWrite(LED2, HIGH))
    #define LED2_OFF          (digitalWrite(LED2, LOW))
    #define LED3_ON           (digitalWrite(LED3, HIGH))
    #define LED3_OFF          (digitalWrite(LED3, LOW))
    #define LED4_ON           (digitalWrite(LED4, HIGH))
    #define LED4_OFF          (digitalWrite(LED4, LOW))
#endif


//USB使能和失能操作
#if E30_DEVICE_V1_0
#else
    #define USB_PORT_EN       (digitalWrite(USB_EN_PIN,LOW))
    #define USB_PORT_DIS      (digitalWrite(USB_EN_PIN,HIGH))
#endif


//GSM联网状态灯
#if E30_DEVICE_V1_0
    #define GSM_STATUS_LED_ON      LED1_ON
    #define GSM_STATUS_LED_OFF     LED1_OFF
#else
    #define GSM_STATUS_LED_ON      LED3_ON
    #define GSM_STATUS_LED_OFF     LED3_OFF
#endif


//GSM连接服务器状态灯
#if E30_DEVICE_V1_0
    #define Server_STATUS_LED_ON   LED2_ON
    #define Server_STATUS_LED_OFF  LED2_OFF
#else
    #define Server_STATUS_LED_ON   LED4_ON
    #define Server_STATUS_LED_OFF  LED4_OFF
#endif


#define DEFAULT_VOL_CHANGE_TIMES    11


#if DEVICE_V2_5
    void EP_Write_Enable(void);
    void EP_Write_Disable(void);
#elif E30_DEVICE_V1_0
    void EP_Write_Enable(void);
    void EP_Write_Disable(void);
#else
#endif

void Some_GPIO_Init(void);
unsigned int Get_Bat_Voltage(unsigned char change_times);
void SIM800_PWR_CON(void);

#endif
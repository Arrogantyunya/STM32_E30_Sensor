#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

//Hardware vertion
#define DEVICE_V2_2   0
#define DEVICE_V2_4   0
#define DEVICE_V2_5   0

#define E30_DEVICE_V1_0   1

#define TYPE06    0 //棚内+大气
#define TYPE07    0 //中科院带三路ADC
#define TYPE08    0 //第三代气象站，新增光学雨量计
#define TYPE09    0 //

#define TYPEA0    1 //GS100的gprs版本A0

#define PRINT_DEBUG   0

//EEPROM capacity
#define TABLE_SIZE 16383//128*128字节//131072 

//Serial port config.
#if DEVICE_V2_5
    #define GSM_Serial                  Serial3
    #define ModBus_Serial               Serial2
    #define LoRa_Serial                 Serial1
#elif (DEVICE_V2_2 || DEVICE_V2_4)
    #define GSM_Serial                  Serial2
    #define ModBus_Serial               Serial3
    #define LoRa_Serial                 Serial1
#elif E30_DEVICE_V1_0
    #define GSM_Serial                  Serial1//USART2 --> USART1, when use serial upload
    #define ModBus_Serial               Serial2//USART3 --> USART2, when use serial upload
    // #define LoRa_Serial                 Serial2//取消了LoRa的接口
#endif

//Button config
#if DEVICE_V2_2
    #define KEY1_INPUT                  PA0
    #define KEY2_INPUT                  PA1
#elif (DEVICE_V2_4 || DEVICE_V2_5)
    #define KEY1_INPUT                  PA4
    #define KEY2_INPUT                  PA5
#elif E30_DEVICE_V1_0
    #define KEY1_INPUT                  PB12
    #define KEY2_INPUT                  PB13
#endif

//LED config
#if DEVICE_V2_5
    #define LED1                        PC2
    #define LED2                        PC3
    #define LED3                        PC0
    #define LED4                        PC1
#elif (DEVICE_V2_2 || DEVICE_V2_4)
    #define LED1                        PC6
    #define LED2                        PC7
    #define LED3                        PB14
    #define LED4                        PB15
#elif E30_DEVICE_V1_0
    #define LED1                        PB14//GREEN
    #define LED2                        PB15//RED
#endif

//External ADC config
#define HOST_VOL_ADC_INPUT_PIN          PA6 //电压读取引脚

//外设电源、信号控制IO,输出控制
#if DEVICE_V2_5
    #define DC12V_PWR_PIN               PB12
    #define RS485_BUS_PWR_PIN           PB13
    #define LORA_PWR_PIN                PB8
    #define LORA_M0_PIN                 PD2
    #define LORA_M1_PIN                 PB3
    #define LORA_AUX_PIN                PC13
#elif (DEVICE_V2_2 || DEVICE_V2_4)
    #define DC12V_PWR_PIN               PB12
    #define RS485_BUS_PWR_PIN           PB0 
    #define LORA_PWR_PIN                PB8
    #define LORA_M0_PIN                 PC10
    #define LORA_M1_PIN                 PB5
    #define LORA_AUX_PIN                PC13
#elif E30_DEVICE_V1_0
    // #define DC12V_PWR_PIN               PB12
    // #define RS485_BUS_PWR_PIN           PA8 //485电源使能引脚 
    // #define LORA_PWR_PIN                PB8
    // #define LORA_M0_PIN                 PC10
    // #define LORA_M1_PIN                 PB5
    // #define LORA_AUX_PIN                PC13
#endif

//OLED屏幕引脚控制
#if DEVICE_V2_5

#elif (DEVICE_V2_2 || DEVICE_V2_4)

#elif E30_DEVICE_V1_0
    #define OLED_PWR    PC11    //oled电源开关
    #define OLED_SCL    PB4     //oled scl
    #define OLED_SDA    PB3     //oled sda
    #define OLED_RES    PD2     //oled reset
    #define OLED_DC     PC12    //貌似这个线悬空就行
#endif

// 是否供电检测引脚
#if DEVICE_V2_5

#elif (DEVICE_V2_2 || DEVICE_V2_4)

#elif E30_DEVICE_V1_0

#endif

//3V3传感器电源开关
#if E30_DEVICE_V1_0
    #define Sensor3V3_PWR   PA5
#endif

//EEPROM W/R enable pin.
#if DEVICE_V2_5
    #define WP_PIN                      PB5
#elif E30_DEVICE_V1_0
    #define WP_PIN                      PB5 
#endif

//USB使能脚
#if DEVICE_V2_5
    #define USB_EN_PIN                  PB15
#elif (DEVICE_V2_2 || DEVICE_V2_4)
    #define USB_EN_PIN                  PB9
#elif E30_DEVICE_V1_0 

#endif

//GPRS相关引脚
#if DEVICE_V2_5
    #define GPS_ANT_PWR_CON_PIN         PC12
    #define GPRS_RST_PIN                PC10
    #define GPRS_PWRKEY_PIN             PC11
    #define GPRS_PWR_CON_PIN            PB4
    //#define NETLIGHT_PIN                PA15
#elif (DEVICE_V2_2 || DEVICE_V2_4)
    #define GPS_ANT_PWR_CON_PIN         PC12
    #define GPRS_RST_PIN                PB4
    #define GPRS_PWRKEY_PIN             PA15
    #define GPRS_PWR_CON_PIN            PD2  
#elif E30_DEVICE_V1_0
    #define GPRS_PWRKEY_PIN             PA1     //
    #define GPRS_PWR_CON_PIN            PA4     //GPRS电源 
#endif

#if DEVICE_V2_5
    #define ANALOGY_PIN_1               PC4
    #define ANALOGY_PIN_2               PA7
    #define ANALOGY_PIN_3               PA6
#elif (DEVICE_V2_2 || DEVICE_V2_4)
    #define ANALOGY_PIN_1               PC5
    #define ANALOGY_PIN_2               PC4
    #define ANALOGY_PIN_3               PA7
#elif E30_DEVICE_V1_0  
    #define KCZJ1                       PB11
    #define KCZJ2                       PB10
#endif


//ADC定义,12位ADC，参考电压为3.3V
#define ADC_RATE                            0.8056 //3300/4096
//电池输入电压分压比
#define VBAT_DIVIDER_RATIO                  6  

#define MIN_BAT_VOL                         3300//6800

#endif
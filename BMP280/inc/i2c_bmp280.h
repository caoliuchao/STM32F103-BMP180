/***********************************************************************
*  @文件: i2c_bmp280.h
*  @作者：caochao
*  @日期：4-20-2017
*  @摘要： 这个头文件包含了各个BMP280的I2C的函数声明
*         包含初始化函数，读函数，写函数以及各个宏的定义
*  修改人：
*    日期：
*
*  修改人：
*    日期：
************************************************************************/
/*定义防止递归的包容*/
#ifndef  _I2C_BMP280_H
#define  _I2C_BMP280_H

#ifdef __cplusplus
extern "C"{
#endif
    
#include "stm32f10x.h"
  
//-----------------外界延时函数----------------------------------------//
/*BMP280时序需要延时函数 为了移植需要外界提供延时函数
  void Delay1ms(uint16_t nms) void Delay1us(uint32_t nus)*/
#define  BMP280_Delay1us       Delay1us 
#define  BMP280_Delay1ms       Delay1ms 
    
//-------------------------bmp280的I/O配置-----------------------------//
#define  BMP280_SCL_PORT          GPIOD
#define  BMP280_SCL_PIN           GPIO_Pin_2
#define  BMP280_SCL_PERIPH        RCC_APB2Periph_GPIOD   
    
#define  BMP280_SDA_PORT          GPIOD
#define  BMP280_SDA_PIN           GPIO_Pin_3
#define  BMP280_SDA_PERIPH        RCC_APB2Periph_GPIOD

#define  I2C_BMP280_SCL_HIGH      GPIO_SetBits(BMP280_SCL_PORT,BMP280_SCL_PIN)
#define  I2C_BMP280_SCL_LOW       GPIO_ResetBits(BMP280_SCL_PORT,BMP280_SCL_PIN)
#define  I2C_BMP280_SDA_HIGH      GPIO_SetBits(BMP280_SDA_PORT,BMP280_SDA_PIN)
#define  I2C_BMP280_SDA_LOW       GPIO_ResetBits(BMP280_SDA_PORT,BMP280_SDA_PIN)
#define  I2C_BMP280_READDATA      GPIO_ReadInputDataBit(BMP280_SDA_PORT,BMP280_SDA_PIN)	
		
//--------------------------bmp280接口函数----------------------------//	
void BMP280_GPIO_Init(void);      

int8_t BMP280_I2C_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint16_t cnt);
int8_t BMP280_I2C_bus_write(uint8_t device_addr, uint8_t reg_addr,
                            uint8_t *reg_data, uint16_t cnt);
void   BMP280_delay_msek(uint32_t msek);
	
#ifdef __cplusplus
}
#endif

#endif/*i2c_bmp280.k*/
/****************************文件结束*********************************/



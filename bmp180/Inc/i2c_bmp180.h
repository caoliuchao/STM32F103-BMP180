/***********************************************************************
*  @文件: i2c_bmp180.h
*  @作者：caochao
*  @日期：4-20-2017
*  @摘要： 这个头文件包含了各个BMP180的I2C的函数声明
*         包含初始化函数，读函数，写函数以及各个宏的定义
*  修改人：
*    日期：
*
*  修改人：
*    日期：
************************************************************************/
/*定义防止递归的包容*/
#ifndef  _I2C_BMP180_H
#define  _I2C_BMP180_H

#ifdef __cplusplus
extern "C"{
#endif
    
#include "stm32f10x.h"
  
//-----------------外界延时函数----------------------------------------//
/*BMP180时序需要延时函数 为了移植需要外界提供延时函数
  void Delay1ms(uint16_t nms) void Delay1us(uint32_t nus)*/
#define  BMP180_Delay1us       Delay1us 
#define  BMP180_Delay1ms       Delay1ms 
    
//-------------------------bmp180的I/O配置-----------------------------//
#define  BMP180_SCL_PORT          GPIOD
#define  BMP180_SCL_PIN           GPIO_Pin_2
#define  BMP180_SCL_PERIPH        RCC_APB2Periph_GPIOD   
    
#define  BMP180_SDA_PORT          GPIOD
#define  BMP180_SDA_PIN           GPIO_Pin_3
#define  BMP180_SDA_PERIPH        RCC_APB2Periph_GPIOD

#define  I2C_BMP180_SCL_HIGH      GPIO_SetBits(BMP180_SCL_PORT,BMP180_SCL_PIN)
#define  I2C_BMP180_SCL_LOW       GPIO_ResetBits(BMP180_SCL_PORT,BMP180_SCL_PIN)
#define  I2C_BMP180_SDA_HIGH      GPIO_SetBits(BMP180_SDA_PORT,BMP180_SDA_PIN)
#define  I2C_BMP180_SDA_LOW       GPIO_ResetBits(BMP180_SDA_PORT,BMP180_SDA_PIN)
#define  I2C_BMP180_READDATA      GPIO_ReadInputDataBit(BMP180_SDA_PORT,BMP180_SDA_PIN)	
		
//--------------------------bmp180接口函数----------------------------//	
void BMP180_GPIO_Init(void);      

char BMP180_I2C_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint8_t cnt);
char BMP180_I2C_bus_write(uint8_t device_addr, uint8_t reg_addr,
                            uint8_t *reg_data, uint8_t cnt);
void   BMP180_delay_msek(uint32_t msek);
	
#ifdef __cplusplus
}
#endif

#endif/*i2c_bmp180.k*/
/****************************文件结束*********************************/



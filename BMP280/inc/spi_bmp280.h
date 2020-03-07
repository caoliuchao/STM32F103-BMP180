/***********************************************************************
*  @文件: spi_bmp280.h
*  @作者：caochao
*  @日期：4-20-2017
*  @摘要： 这个头文件包含了各个BMP280的SPI的函数声明
*         包含初始化函数，读函数，写函数以及各个宏的定义
*  修改人：
*    日期：
*
*  修改人：
*    日期：
************************************************************************/
/*定义防止递归的包容*/
#ifndef  _SPI_BMP280_H
#define  _SPI_BMP280_H

#ifdef __cplusplus
extern "C"{
#endif
    
//-----------------外界延时函数----------------------------------------//
/*BMP280时序需要延时函数 为了移植需要外界提供延时函数
  void Delay1ms(uint16_t nms) void Delay1us(uint32_t nus)*/
#define  BMP280_Delay1us       Delay1us 
#define  BMP280_Delay1ms       Delay1ms  

//------------------BMP280与STM32相连管脚宏定义-------------------------//
#define BMP280_SPI           SPI3
#define BMP280_SPI_PERIPH    RCC_APB1Periph_SPI3
    
#define BMP280_CS_PORT       GPIOA
#define BMP280_CS_PIN        GPIO_Pin_4
#define BMP280_CS_PERIPH     RCC_APB2Periph_GPIOA

#define BMP280_CLK_PORT      GPIOA    
#define BMP280_CLK_PIN       GPIO_Pin_5
#define BMP280_CLK_PERIPH    RCC_APB2Periph_GPIOA

#define BMP280_MISO_PORT     GPIOA    
#define BMP280_MISO_PIN      GPIO_Pin_6
#define BMP280_MISO_PERIPH   RCC_APB2Periph_GPIOA

#define BMP280_MOSI_PORT     GPIOA    
#define BMP280_MOSI_PIN      GPIO_Pin_7
#define BMP280_MOSI_PERIPH   RCC_APB2Periph_GPIOA    

#define BMP280_CS_LOW()      GPIO_ResetBits(BMP280_CS_PORT,BMP280_CS_PIN)   //打开BMP280使能
#define BMP280_CS_HIGH()     GPIO_SetBits(BMP280_CS_PORT,BMP280_CS_PIN)     //关闭BMP280使能

#define BMP280_DUMMY         0xFF        //无用指令
//--------------------------bmp280接口函数----------------------------//	
void BMP280_SPI_Init(void);      

int8_t BMP280_SPI_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint16_t cnt);
int8_t BMP280_SPI_bus_write(uint8_t device_addr, uint8_t reg_addr,
                            uint8_t *reg_data, uint16_t cnt);
void   BMP280_SPI_delay_msek(uint32_t msek);
	
#ifdef __cplusplus
}
#endif

#endif/*spi_bmp280.k*/
/****************************文件结束*********************************/
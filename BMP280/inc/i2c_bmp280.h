/***********************************************************************
*  @�ļ�: i2c_bmp280.h
*  @���ߣ�caochao
*  @���ڣ�4-20-2017
*  @ժҪ�� ���ͷ�ļ������˸���BMP280��I2C�ĺ�������
*         ������ʼ����������������д�����Լ�������Ķ���
*  �޸��ˣ�
*    ���ڣ�
*
*  �޸��ˣ�
*    ���ڣ�
************************************************************************/
/*�����ֹ�ݹ�İ���*/
#ifndef  _I2C_BMP280_H
#define  _I2C_BMP280_H

#ifdef __cplusplus
extern "C"{
#endif
    
#include "stm32f10x.h"
  
//-----------------�����ʱ����----------------------------------------//
/*BMP280ʱ����Ҫ��ʱ���� Ϊ����ֲ��Ҫ����ṩ��ʱ����
  void Delay1ms(uint16_t nms) void Delay1us(uint32_t nus)*/
#define  BMP280_Delay1us       Delay1us 
#define  BMP280_Delay1ms       Delay1ms 
    
//-------------------------bmp280��I/O����-----------------------------//
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
		
//--------------------------bmp280�ӿں���----------------------------//	
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
/****************************�ļ�����*********************************/



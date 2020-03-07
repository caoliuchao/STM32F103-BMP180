/***********************************************************************
*  @�ļ�: spi_bmp280.h
*  @���ߣ�caochao
*  @���ڣ�4-20-2017
*  @ժҪ�� ���ͷ�ļ������˸���BMP280��SPI�ĺ�������
*         ������ʼ����������������д�����Լ�������Ķ���
*  �޸��ˣ�
*    ���ڣ�
*
*  �޸��ˣ�
*    ���ڣ�
************************************************************************/
/*�����ֹ�ݹ�İ���*/
#ifndef  _SPI_BMP280_H
#define  _SPI_BMP280_H

#ifdef __cplusplus
extern "C"{
#endif
    
//-----------------�����ʱ����----------------------------------------//
/*BMP280ʱ����Ҫ��ʱ���� Ϊ����ֲ��Ҫ����ṩ��ʱ����
  void Delay1ms(uint16_t nms) void Delay1us(uint32_t nus)*/
#define  BMP280_Delay1us       Delay1us 
#define  BMP280_Delay1ms       Delay1ms  

//------------------BMP280��STM32�����ܽź궨��-------------------------//
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

#define BMP280_CS_LOW()      GPIO_ResetBits(BMP280_CS_PORT,BMP280_CS_PIN)   //��BMP280ʹ��
#define BMP280_CS_HIGH()     GPIO_SetBits(BMP280_CS_PORT,BMP280_CS_PIN)     //�ر�BMP280ʹ��

#define BMP280_DUMMY         0xFF        //����ָ��
//--------------------------bmp280�ӿں���----------------------------//	
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
/****************************�ļ�����*********************************/
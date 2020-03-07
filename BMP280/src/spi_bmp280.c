/*************************************************************************
* @�ļ�  : spi_bmp280.c
* @����  : caochao
* @����  : 15-10-2017
* @ժҪ  : ���C�ļ�ʱʵ��bmp280����غ�����ʵ��
* �޸��� :
*  ����  :
*
* �޸��� :
*  ����  :
***************************************************************************/
#include "stm32f10x.h"
#include "stm32F10x_spi.h"
#include "delay.h"
#include "i2c_bmp280.h"

static uint8_t BMP280_ReadWrite(uint8_t Data);

/*************************************************************************
*    ��������BMP280_SPI_Init(void)
*    ��  �ܣ���ʼ��bmp280��spi
*    ��  �룺
*            ��
*    ��  ����
*            ��
*************************************************************************/
void BMP280_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(BMP280_CS_PERIPH,ENABLE);    //��GPIO��ʱ��
    RCC_APB2PeriphClockCmd(BMP280_CLK_PERIPH,ENABLE);
	RCC_APB2PeriphClockCmd(BMP280_MISO_PERIPH,ENABLE);    
    RCC_APB2PeriphClockCmd(BMP280_MOSI_PERIPH,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //ʹ��AFIOʱ��
    
    GPIO_InitStructure.GPIO_Pin = BMP280_CS_PIN;        //�������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(BMP280_CS_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BMP280_CLK_PIN;     //���츴��ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(BMP280_CLK_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BMP280_MISO_PIN;       //���츴��ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(BMP280_MISO_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BMP280_MOSI_PIN;      //Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(BMP280_MOSI_PORT,&GPIO_InitStructure); 		

    BMP280_CS_HIGH();  //ʧ��Ƭѡ
    
    RCC_APB2PeriphClockCmd(BMP280_SPI_PERIPH,ENABLE); //��SPI��ʱ��
	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;         //SP1������ģʽ3
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   //���ݴ�СΪ8λ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;           //SPI2��CS������I/O�ڿ���
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;       //���ȴ����λ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI��ʱ��8��Ƶ
	
	SPI_Init(BMP280_SPI,&SPI_InitStructure);
	SPI_Cmd(BMP280_SPI,ENABLE);             //��SPI����
}

/****************************************************************************
*    ��������BMP280_ReadWrite(uint8_t Data)
*    ��  �ܣ�ʵ�ֶ�BMP280�Ķ�д�ײ㺯��
*    ��  �룺
*            Data Ҫд�������
*    ��  ����
*            ���ؽ��ܵ�������
****************************************************************************/
uint8_t BMP280_ReadWrite(uint8_t Data)
{	
	uint16_t data;
	
	while (SPI_I2S_GetFlagStatus(BMP280_SPI,SPI_I2S_FLAG_TXE) == RESET) //��ⷢ�ͼĴ����Ƿ�Ϊ��
	{
        ;
	}
	SPI_I2S_SendData(BMP280_SPI,Data);    //��������      
	
	while (SPI_I2S_GetFlagStatus(BMP280_SPI,SPI_I2S_FLAG_RXNE) == RESET)  //����Ƿ���ܵ�����
	{
		;
	}
	data = (SPI_I2S_ReceiveData(BMP280_SPI));  //���ܵ�������    

	return data;	//���ؽ��ܵ�������
}

/*******************************************************************
*  ��������BMP280_SPI_bus_write(uint8_t device_addr, uint8_t reg_addr, 
*                              uint8_t *reg_data, uint16_t cnt)
*  ��  �ܣ���bmp280д������
*  ��  �룺
*         device_addr bmp280��I2C��ַ��0xEE��
*         reg_addr    bmp280�ڲ��Ĵ�����ַ
*         reg_data    Ҫд�������ָ��
*         cnt         Ҫд�������ֽ���
*  ��  ����
*         1 д��ʧ��
*         0 д��ɹ�
*********************************************************************/
int8_t BMP280_SPI_bus_write(uint8_t device_addr, uint8_t reg_addr, 
                          uint8_t *reg_data, uint16_t cnt)
{
    uint8_t  i;
    uint8_t  regAddr = reg_addr
    
	BMP280_CS_LOW(); //ʹ��Ƭѡ
    
    for (i = 0; i < cnt; i++)
    {
        BMP280_ReadWrite(regAddr&0x7F);
        BMP280_ReadWrite(reg_data[i]);
        regAddr++;
    }

    BMP280_CS_HIGH(); //ʧ��Ƭѡ
    
    return 0;
}

/********************************************************************
*  ��������BMP280_SPI_bus_read(uint8_t device_addr, uint8_t reg_addr, 
*                             uint8_t *reg_data, uint16_t cnt)
*  ��  �ܣ���bmp280������
*  ��  �룺
*         device_addr bmp280��I2C��ַ��0xEE��
*         reg_addr    bmp280�ڲ��Ĵ�����ַ
*         reg_data    Ҫ���������ݴ��ָ��
*         cnt         Ҫ���������ֽ���
*  ��  ����
*         1 ����ʧ��
*         0 �����ɹ�
**********************************************************************/
int8_t BMP280_SPI_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint16_t cnt)
{
    uint8_t  i;
    uint8_t  regAddr = reg_addr
    
	BMP280_CS_LOW(); //ʹ��Ƭѡ
    
    for (i = 0; i < cnt; i++)
    {
        BMP280_ReadWrite(regAddr|0x80);
        reg_data[i] = BMP280_ReadWrite(BMP280_DUMMY);
        regAddr++;
    }

    BMP280_CS_HIGH(); //ʧ��Ƭѡ
    
    return 0;
}	
				
/************************************************************************
*  ��������BMP280_delay_msek(uint32_t msek)
*  ��  �ܣ�bmp280��ʱ
*  ��  �룺
*         msek ��ʱ�ĺ�����
*  ��  ����
*         ��
************************************************************************/

/*bmp280��ʱ����*/
void BMP280_delay_msek(uint32_t msek)
{
	BMP280_Delay1ms(msek);
}

/*************************************�ļ�����****************************/



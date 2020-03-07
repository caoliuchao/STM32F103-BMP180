/*************************************************************************
* @文件  : spi_bmp280.c
* @作者  : caochao
* @日期  : 15-10-2017
* @摘要  : 这个C文件时实现bmp280的相关函数的实现
* 修改人 :
*  日期  :
*
* 修改人 :
*  日期  :
***************************************************************************/
#include "stm32f10x.h"
#include "stm32F10x_spi.h"
#include "delay.h"
#include "i2c_bmp280.h"

static uint8_t BMP280_ReadWrite(uint8_t Data);

/*************************************************************************
*    函数名：BMP280_SPI_Init(void)
*    功  能：初始化bmp280的spi
*    输  入：
*            无
*    输  出：
*            无
*************************************************************************/
void BMP280_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(BMP280_CS_PERIPH,ENABLE);    //打开GPIO的时钟
    RCC_APB2PeriphClockCmd(BMP280_CLK_PERIPH,ENABLE);
	RCC_APB2PeriphClockCmd(BMP280_MISO_PERIPH,ENABLE);    
    RCC_APB2PeriphClockCmd(BMP280_MOSI_PERIPH,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //使能AFIO时钟
    
    GPIO_InitStructure.GPIO_Pin = BMP280_CS_PIN;        //推挽输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(BMP280_CS_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BMP280_CLK_PIN;     //推挽复用模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(BMP280_CLK_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BMP280_MISO_PIN;       //推挽复用模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(BMP280_MISO_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BMP280_MOSI_PIN;      //为推挽输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(BMP280_MOSI_PORT,&GPIO_InitStructure); 		

    BMP280_CS_HIGH();  //失能片选
    
    RCC_APB2PeriphClockCmd(BMP280_SPI_PERIPH,ENABLE); //打开SPI的时钟
	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;         //SP1工作在模式3
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   //数据大小为8位
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;           //SPI2的CS有其他I/O口控制
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;       //首先传输高位
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI的时钟8分频
	
	SPI_Init(BMP280_SPI,&SPI_InitStructure);
	SPI_Cmd(BMP280_SPI,ENABLE);             //打开SPI功能
}

/****************************************************************************
*    函数名：BMP280_ReadWrite(uint8_t Data)
*    功  能：实现对BMP280的读写底层函数
*    输  入：
*            Data 要写入的数据
*    输  出：
*            返回接受到的数据
****************************************************************************/
uint8_t BMP280_ReadWrite(uint8_t Data)
{	
	uint16_t data;
	
	while (SPI_I2S_GetFlagStatus(BMP280_SPI,SPI_I2S_FLAG_TXE) == RESET) //检测发送寄存器是否为空
	{
        ;
	}
	SPI_I2S_SendData(BMP280_SPI,Data);    //发送数据      
	
	while (SPI_I2S_GetFlagStatus(BMP280_SPI,SPI_I2S_FLAG_RXNE) == RESET)  //检测是否接受到数据
	{
		;
	}
	data = (SPI_I2S_ReceiveData(BMP280_SPI));  //接受到的数据    

	return data;	//返回接受到的数据
}

/*******************************************************************
*  函数名：BMP280_SPI_bus_write(uint8_t device_addr, uint8_t reg_addr, 
*                              uint8_t *reg_data, uint16_t cnt)
*  功  能：向bmp280写入数据
*  输  入：
*         device_addr bmp280的I2C地址（0xEE）
*         reg_addr    bmp280内部寄存器地址
*         reg_data    要写入的数据指针
*         cnt         要写入数据字节数
*  输  出：
*         1 写入失败
*         0 写入成功
*********************************************************************/
int8_t BMP280_SPI_bus_write(uint8_t device_addr, uint8_t reg_addr, 
                          uint8_t *reg_data, uint16_t cnt)
{
    uint8_t  i;
    uint8_t  regAddr = reg_addr
    
	BMP280_CS_LOW(); //使能片选
    
    for (i = 0; i < cnt; i++)
    {
        BMP280_ReadWrite(regAddr&0x7F);
        BMP280_ReadWrite(reg_data[i]);
        regAddr++;
    }

    BMP280_CS_HIGH(); //失能片选
    
    return 0;
}

/********************************************************************
*  函数名：BMP280_SPI_bus_read(uint8_t device_addr, uint8_t reg_addr, 
*                             uint8_t *reg_data, uint16_t cnt)
*  功  能：从bmp280读数据
*  输  入：
*         device_addr bmp280的I2C地址（0xEE）
*         reg_addr    bmp280内部寄存器地址
*         reg_data    要读出的数据存放指针
*         cnt         要读出数据字节数
*  输  出：
*         1 读出失败
*         0 读出成功
**********************************************************************/
int8_t BMP280_SPI_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint16_t cnt)
{
    uint8_t  i;
    uint8_t  regAddr = reg_addr
    
	BMP280_CS_LOW(); //使能片选
    
    for (i = 0; i < cnt; i++)
    {
        BMP280_ReadWrite(regAddr|0x80);
        reg_data[i] = BMP280_ReadWrite(BMP280_DUMMY);
        regAddr++;
    }

    BMP280_CS_HIGH(); //失能片选
    
    return 0;
}	
				
/************************************************************************
*  函数名：BMP280_delay_msek(uint32_t msek)
*  功  能：bmp280延时
*  输  入：
*         msek 延时的毫秒数
*  输  出：
*         无
************************************************************************/

/*bmp280延时函数*/
void BMP280_delay_msek(uint32_t msek)
{
	BMP280_Delay1ms(msek);
}

/*************************************文件结束****************************/



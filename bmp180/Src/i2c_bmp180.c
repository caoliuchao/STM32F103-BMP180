/*************************************************************************
* @文件  : bmp180.c
* @作者  : caochao
* @日期  : 15-10-2017
* @摘要  : 这个C文件时实现bmp180的相关函数的实现
* 修改人 :
*  日期  :
*
* 修改人 :
*  日期  :
***************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "i2c_bmp180.h"

static  void     I2C_BMP180_Start(void);
static  void     I2C_BMP180_Stop(void);
	
static  void     I2C_BMP180_CPUTransAck(void);
static  void     I2C_BMP180_CPUTransNoAck(void);
static  uint8_t  I2C_BMP180_CPUWaitAck(void);
static  void     I2C_BMP180_CPUSendData(uint8_t data);
static  uint8_t  I2C_BMP180_CPUReadData(uint8_t ack);

/*************************************************************************
*    函数名：BMP180_GPIO_Init(void)
*    功  能：初始化bmp180的i2cI/O
*    输  入：
*            无
*    输  出：
*            无
*************************************************************************/
void BMP180_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(BMP180_SCL_PERIPH ,ENABLE); //打开GPIO的时钟
    RCC_APB2PeriphClockCmd(BMP180_SDA_PERIPH,ENABLE);
		
    GPIO_InitStructure.GPIO_Pin = BMP180_SCL_PIN;   //bmp180的I/O为推挽输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(BMP180_SCL_PORT,&GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = BMP180_SDA_PIN;   //bmp180的I/O为开漏输出模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(BMP180_SDA_PORT,&GPIO_InitStructure); 
	
	I2C_BMP180_SCL_HIGH;  //开始时I2C的SCL和SDA都是高电平 空闲状态           
	I2C_BMP180_SDA_HIGH;   
}

/***************************************************************************
*  函数名：I2C_BMP180_Start(void)
*  功  能：I2C协议的开始信号 调用函数后I2C的两根线都是低电平 为下面数据传输做准备
*  输  出：
*          无
*  输  入：
*          无
****************************************************************************/
void I2C_BMP180_Start(void)
{
    I2C_BMP180_SDA_HIGH;   //SDA输出高电平 
    BMP180_Delay1us (4);    //延时4us
    I2C_BMP180_SCL_HIGH;   //SCL输出高电平
    BMP180_Delay1us (4);    //延时4us
    I2C_BMP180_SDA_LOW;    //拉低SDA形成下降沿
    BMP180_Delay1us (4);   //延时4us
    I2C_BMP180_SCL_LOW;    //拉低SCL
	BMP180_Delay1us (4);    //延时4us
}

/***************************************************************************
*  函数名：I2C_BMP180_Stop(void)
*  功  能：I2C协议的结束信号 调用函数后I2C的两根线都是高电平 I2C处于空闲状态
*  输  出：
*          无
*  输  入：
*          无
****************************************************************************/
void I2C_BMP180_Stop(void)
{
    I2C_BMP180_SDA_LOW;   //SDA输出低电平
	BMP180_Delay1us (4);
    I2C_BMP180_SCL_HIGH;  //SCL输出高电平
    BMP180_Delay1us (4);
    I2C_BMP180_SDA_HIGH;  //SDA输出高电平 形成一个上升沿 
    BMP180_Delay1us (4);  	
}

/**************************************************************************
*   函数名：I2C_CPUTransAck
*   功  能：主机发射一个响应信号 
*   输  出：
*           无
*   输  入：
*           无
***************************************************************************/
void I2C_BMP180_CPUTransAck(void)
{
	I2C_BMP180_SDA_LOW;       //SDA输出低电平 低电平是响应信号
	BMP180_Delay1us (4);
	I2C_BMP180_SCL_HIGH;      //SCL输出高电平
	BMP180_Delay1us (4); 
	I2C_BMP180_SCL_LOW;      //SCL输出低电平
	BMP180_Delay1us (4);
}

/**************************************************************************
*   函数名：I2C_CPUTransNoAck
*     功能：主机发射一个无响应信号 
*     输出：
*           无
*     输入：
*           无
***************************************************************************/
void I2C_BMP180_CPUTransNoAck(void)
{
	I2C_BMP180_SDA_HIGH;       //SDA输出高电平 高电平不是响应信号
	BMP180_Delay1us (4);
	I2C_BMP180_SCL_HIGH;      //SCL输出高电平
	BMP180_Delay1us (4); 
	I2C_BMP180_SCL_LOW;      //SCL输出低电平
	BMP180_Delay1us (4);
}

/***************************************************************************
*   函数名：I2C_CPUWaitAck
*   功  能：主机接受从机的响应信号
*   输  出：
*           有 
*           返回1 表示没有从机响应 
*           返回0 表示有从机响应
*   输  入：
*           无
***************************************************************************/
uint8_t I2C_BMP180_CPUWaitAck(void)
{
    uint8_t  tempTime = 0;

    I2C_BMP180_SDA_HIGH;      //SDA输出高电平  释放总线
	BMP180_Delay1us (4);               //延时4us
    I2C_BMP180_SCL_HIGH;      //SCL输出高电平	
	BMP180_Delay1us (4);              //延时4us
    
	while (I2C_BMP180_READDATA)
    {
        tempTime++;
        if (tempTime > 100)    //判断是否在有限时间内为低电平
        {
            I2C_BMP180_Stop();
            return 1;
        }       
    }
    
	I2C_BMP180_SCL_LOW;      //SCL输出低电平
	BMP180_Delay1us (4);
	
    return 0;
}

/****************************************************************************
*    函数名：I2C_CPUSendData
*    功  能：主机发送一个字节
*    输  出： 
*            无
*    输  入：
*            有
*            data：要传送的数据
****************************************************************************/
void I2C_BMP180_CPUSendData(uint8_t data)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++)
    { 
        if (data & 0x80)
        {
            I2C_BMP180_SDA_HIGH;  //SDA输出高电平
        }
        else
        {
            I2C_BMP180_SDA_LOW;      //SDA输出低电平
        }
        
		BMP180_Delay1us (4);
        data = data << 1; 	
        I2C_BMP180_SCL_HIGH;      //SCL输出高电平
	    BMP180_Delay1us (4); 
        I2C_BMP180_SCL_LOW;      //SCL输出低电平     	
    }
	BMP180_Delay1us (4);		
}

/***************************************************************************
*	函 数 名: I2C_BMP180_CPUReadData(uint8_t ack)
*	功    能: CPU从I2C总线设备读取8bit数据
*	输    入：
*            ack 1 表示CPU向BMP180发送响应
*            ack 0 表示CPU不发送响应
*	输    出:
*            读到的数据
****************************************************************************/
uint8_t I2C_BMP180_CPUReadData(uint8_t ack)
{
	uint8_t i;
	uint8_t value = 0;    
     
    I2C_BMP180_SDA_HIGH;      //SDA输出高电平  释放总线
	BMP180_Delay1us (4);               //延时4us
    
	for (i = 0; i < 8; i++)
	{
        I2C_BMP180_SCL_HIGH;      //SCL输出高电平
		BMP180_Delay1us (4);

		value = value << 1;
		if (I2C_BMP180_READDATA)    //判断读到的是否为高电平
		{	 
			value = value + 0x01;
		}	

		I2C_BMP180_SCL_LOW;      //SCL输出低电平 
        BMP180_Delay1us (4);        
	}
	
	if (ack)
	{
		I2C_BMP180_CPUTransAck();  //CPU发送回应
	}
	else
	{
		I2C_BMP180_CPUTransNoAck(); //CPU发送不会应
	}
	
	return value;
}

/*******************************************************************
*  函数名：BMP180_I2C_bus_write(uint8_t device_addr, uint8_t reg_addr, 
*                              uint8_t *reg_data, uint8_t cnt)
*  功  能：向bmp180写入数据
*  输  入：
*         device_addr bmp180的I2C地址（0xEE）
*         reg_addr    bmp180内部寄存器地址
*         reg_data    要写入的数据指针
*         cnt         要写入数据字节数
*  输  出：
*         1 写入失败
*         0 写入成功
*********************************************************************/
char BMP180_I2C_bus_write(uint8_t device_addr, uint8_t reg_addr, 
                          uint8_t *reg_data, uint8_t cnt)
{
	I2C_BMP180_Start();          //I2C起始信号
	
	I2C_BMP180_CPUSendData(device_addr << 1);  //发送器件地址+读命令	
	if(I2C_BMP180_CPUWaitAck())  //等待回应
	{
		return 1;
	}
	
	I2C_BMP180_CPUSendData(reg_addr);  //写入寄存器地址
	if(I2C_BMP180_CPUWaitAck())        //等待应答
	{
		return 1;
	}
	
	while (cnt)
	{
		I2C_BMP180_CPUSendData(*reg_data);   //写入数据
		if(I2C_BMP180_CPUWaitAck())
	    {
			return 1;
	    }
		cnt--;
		reg_data++;
	}
	I2C_BMP180_Stop();     //I2C结束信号
	
	return 0;
}

/********************************************************************
*  函数名：BMP180_I2C_bus_read(uint8_t device_addr, uint8_t reg_addr, 
*                             uint8_t *reg_data, uint8_t cnt)
*  功  能：从bmp180读数据
*  输  入：
*         device_addr bmp180的I2C地址（0xEE）
*         reg_addr    bmp180内部寄存器地址
*         reg_data    要读出的数据存放指针
*         cnt         要读出数据字节数
*  输  出：
*         1 读出失败
*         0 读出成功
**********************************************************************/
char BMP180_I2C_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint8_t cnt)
{
	I2C_BMP180_Start();  //I2C起始信号
	
	I2C_BMP180_CPUSendData(device_addr << 1);  //发送器件地址+读命令	
	if(I2C_BMP180_CPUWaitAck())    //等待应答
	{
		return 1;
	}
	
	I2C_BMP180_CPUSendData(reg_addr);
	if(I2C_BMP180_CPUWaitAck())
	{
		return 1;
	}
	
	I2C_BMP180_Start();    //I2C协议开始信号 为读做准备
	
	I2C_BMP180_CPUSendData((device_addr << 1) | 0x01);   //发送器件地址+读命令	
	if(I2C_BMP180_CPUWaitAck())
	{
		return 1;
	}
	
	while (cnt)
	{
		if (cnt == 1)
		{
			*reg_data = I2C_BMP180_CPUReadData(0);   //读数据,发送无响应信号 为结束做准备
			
		}
		else
		{
			*reg_data = I2C_BMP180_CPUReadData(1);    //读数据,发送ACK  
		}
		cnt--;
		reg_data++;
	}
	I2C_BMP180_Stop();            //I2C结束信号
	
	return 0;
}	
				
/************************************************************************
*  函数名：BMP180_delay_msek(uint32_t msek)
*  功  能：bmp180延时
*  输  入：
*         msek 延时的毫秒数
*  输  出：
*         无
************************************************************************/

/*bmp180延时函数*/
void BMP180_delay_msek(uint32_t msek)
{
	BMP180_Delay1ms(msek);
}

/*************************************文件结束****************************/



/*************************************************************************
* @�ļ�  : bmp180.c
* @����  : caochao
* @����  : 15-10-2017
* @ժҪ  : ���C�ļ�ʱʵ��bmp180����غ�����ʵ��
* �޸��� :
*  ����  :
*
* �޸��� :
*  ����  :
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
*    ��������BMP180_GPIO_Init(void)
*    ��  �ܣ���ʼ��bmp180��i2cI/O
*    ��  �룺
*            ��
*    ��  ����
*            ��
*************************************************************************/
void BMP180_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(BMP180_SCL_PERIPH ,ENABLE); //��GPIO��ʱ��
    RCC_APB2PeriphClockCmd(BMP180_SDA_PERIPH,ENABLE);
		
    GPIO_InitStructure.GPIO_Pin = BMP180_SCL_PIN;   //bmp180��I/OΪ�������ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(BMP180_SCL_PORT,&GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = BMP180_SDA_PIN;   //bmp180��I/OΪ��©���ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_Init(BMP180_SDA_PORT,&GPIO_InitStructure); 
	
	I2C_BMP180_SCL_HIGH;  //��ʼʱI2C��SCL��SDA���Ǹߵ�ƽ ����״̬           
	I2C_BMP180_SDA_HIGH;   
}

/***************************************************************************
*  ��������I2C_BMP180_Start(void)
*  ��  �ܣ�I2CЭ��Ŀ�ʼ�ź� ���ú�����I2C�������߶��ǵ͵�ƽ Ϊ�������ݴ�����׼��
*  ��  ����
*          ��
*  ��  �룺
*          ��
****************************************************************************/
void I2C_BMP180_Start(void)
{
    I2C_BMP180_SDA_HIGH;   //SDA����ߵ�ƽ 
    BMP180_Delay1us (4);    //��ʱ4us
    I2C_BMP180_SCL_HIGH;   //SCL����ߵ�ƽ
    BMP180_Delay1us (4);    //��ʱ4us
    I2C_BMP180_SDA_LOW;    //����SDA�γ��½���
    BMP180_Delay1us (4);   //��ʱ4us
    I2C_BMP180_SCL_LOW;    //����SCL
	BMP180_Delay1us (4);    //��ʱ4us
}

/***************************************************************************
*  ��������I2C_BMP180_Stop(void)
*  ��  �ܣ�I2CЭ��Ľ����ź� ���ú�����I2C�������߶��Ǹߵ�ƽ I2C���ڿ���״̬
*  ��  ����
*          ��
*  ��  �룺
*          ��
****************************************************************************/
void I2C_BMP180_Stop(void)
{
    I2C_BMP180_SDA_LOW;   //SDA����͵�ƽ
	BMP180_Delay1us (4);
    I2C_BMP180_SCL_HIGH;  //SCL����ߵ�ƽ
    BMP180_Delay1us (4);
    I2C_BMP180_SDA_HIGH;  //SDA����ߵ�ƽ �γ�һ�������� 
    BMP180_Delay1us (4);  	
}

/**************************************************************************
*   ��������I2C_CPUTransAck
*   ��  �ܣ���������һ����Ӧ�ź� 
*   ��  ����
*           ��
*   ��  �룺
*           ��
***************************************************************************/
void I2C_BMP180_CPUTransAck(void)
{
	I2C_BMP180_SDA_LOW;       //SDA����͵�ƽ �͵�ƽ����Ӧ�ź�
	BMP180_Delay1us (4);
	I2C_BMP180_SCL_HIGH;      //SCL����ߵ�ƽ
	BMP180_Delay1us (4); 
	I2C_BMP180_SCL_LOW;      //SCL����͵�ƽ
	BMP180_Delay1us (4);
}

/**************************************************************************
*   ��������I2C_CPUTransNoAck
*     ���ܣ���������һ������Ӧ�ź� 
*     �����
*           ��
*     ���룺
*           ��
***************************************************************************/
void I2C_BMP180_CPUTransNoAck(void)
{
	I2C_BMP180_SDA_HIGH;       //SDA����ߵ�ƽ �ߵ�ƽ������Ӧ�ź�
	BMP180_Delay1us (4);
	I2C_BMP180_SCL_HIGH;      //SCL����ߵ�ƽ
	BMP180_Delay1us (4); 
	I2C_BMP180_SCL_LOW;      //SCL����͵�ƽ
	BMP180_Delay1us (4);
}

/***************************************************************************
*   ��������I2C_CPUWaitAck
*   ��  �ܣ��������ܴӻ�����Ӧ�ź�
*   ��  ����
*           �� 
*           ����1 ��ʾû�дӻ���Ӧ 
*           ����0 ��ʾ�дӻ���Ӧ
*   ��  �룺
*           ��
***************************************************************************/
uint8_t I2C_BMP180_CPUWaitAck(void)
{
    uint8_t  tempTime = 0;

    I2C_BMP180_SDA_HIGH;      //SDA����ߵ�ƽ  �ͷ�����
	BMP180_Delay1us (4);               //��ʱ4us
    I2C_BMP180_SCL_HIGH;      //SCL����ߵ�ƽ	
	BMP180_Delay1us (4);              //��ʱ4us
    
	while (I2C_BMP180_READDATA)
    {
        tempTime++;
        if (tempTime > 100)    //�ж��Ƿ�������ʱ����Ϊ�͵�ƽ
        {
            I2C_BMP180_Stop();
            return 1;
        }       
    }
    
	I2C_BMP180_SCL_LOW;      //SCL����͵�ƽ
	BMP180_Delay1us (4);
	
    return 0;
}

/****************************************************************************
*    ��������I2C_CPUSendData
*    ��  �ܣ���������һ���ֽ�
*    ��  ���� 
*            ��
*    ��  �룺
*            ��
*            data��Ҫ���͵�����
****************************************************************************/
void I2C_BMP180_CPUSendData(uint8_t data)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++)
    { 
        if (data & 0x80)
        {
            I2C_BMP180_SDA_HIGH;  //SDA����ߵ�ƽ
        }
        else
        {
            I2C_BMP180_SDA_LOW;      //SDA����͵�ƽ
        }
        
		BMP180_Delay1us (4);
        data = data << 1; 	
        I2C_BMP180_SCL_HIGH;      //SCL����ߵ�ƽ
	    BMP180_Delay1us (4); 
        I2C_BMP180_SCL_LOW;      //SCL����͵�ƽ     	
    }
	BMP180_Delay1us (4);		
}

/***************************************************************************
*	�� �� ��: I2C_BMP180_CPUReadData(uint8_t ack)
*	��    ��: CPU��I2C�����豸��ȡ8bit����
*	��    �룺
*            ack 1 ��ʾCPU��BMP180������Ӧ
*            ack 0 ��ʾCPU��������Ӧ
*	��    ��:
*            ����������
****************************************************************************/
uint8_t I2C_BMP180_CPUReadData(uint8_t ack)
{
	uint8_t i;
	uint8_t value = 0;    
     
    I2C_BMP180_SDA_HIGH;      //SDA����ߵ�ƽ  �ͷ�����
	BMP180_Delay1us (4);               //��ʱ4us
    
	for (i = 0; i < 8; i++)
	{
        I2C_BMP180_SCL_HIGH;      //SCL����ߵ�ƽ
		BMP180_Delay1us (4);

		value = value << 1;
		if (I2C_BMP180_READDATA)    //�ж϶������Ƿ�Ϊ�ߵ�ƽ
		{	 
			value = value + 0x01;
		}	

		I2C_BMP180_SCL_LOW;      //SCL����͵�ƽ 
        BMP180_Delay1us (4);        
	}
	
	if (ack)
	{
		I2C_BMP180_CPUTransAck();  //CPU���ͻ�Ӧ
	}
	else
	{
		I2C_BMP180_CPUTransNoAck(); //CPU���Ͳ���Ӧ
	}
	
	return value;
}

/*******************************************************************
*  ��������BMP180_I2C_bus_write(uint8_t device_addr, uint8_t reg_addr, 
*                              uint8_t *reg_data, uint8_t cnt)
*  ��  �ܣ���bmp180д������
*  ��  �룺
*         device_addr bmp180��I2C��ַ��0xEE��
*         reg_addr    bmp180�ڲ��Ĵ�����ַ
*         reg_data    Ҫд�������ָ��
*         cnt         Ҫд�������ֽ���
*  ��  ����
*         1 д��ʧ��
*         0 д��ɹ�
*********************************************************************/
char BMP180_I2C_bus_write(uint8_t device_addr, uint8_t reg_addr, 
                          uint8_t *reg_data, uint8_t cnt)
{
	I2C_BMP180_Start();          //I2C��ʼ�ź�
	
	I2C_BMP180_CPUSendData(device_addr << 1);  //����������ַ+������	
	if(I2C_BMP180_CPUWaitAck())  //�ȴ���Ӧ
	{
		return 1;
	}
	
	I2C_BMP180_CPUSendData(reg_addr);  //д��Ĵ�����ַ
	if(I2C_BMP180_CPUWaitAck())        //�ȴ�Ӧ��
	{
		return 1;
	}
	
	while (cnt)
	{
		I2C_BMP180_CPUSendData(*reg_data);   //д������
		if(I2C_BMP180_CPUWaitAck())
	    {
			return 1;
	    }
		cnt--;
		reg_data++;
	}
	I2C_BMP180_Stop();     //I2C�����ź�
	
	return 0;
}

/********************************************************************
*  ��������BMP180_I2C_bus_read(uint8_t device_addr, uint8_t reg_addr, 
*                             uint8_t *reg_data, uint8_t cnt)
*  ��  �ܣ���bmp180������
*  ��  �룺
*         device_addr bmp180��I2C��ַ��0xEE��
*         reg_addr    bmp180�ڲ��Ĵ�����ַ
*         reg_data    Ҫ���������ݴ��ָ��
*         cnt         Ҫ���������ֽ���
*  ��  ����
*         1 ����ʧ��
*         0 �����ɹ�
**********************************************************************/
char BMP180_I2C_bus_read(uint8_t device_addr, uint8_t reg_addr, 
                           uint8_t *reg_data, uint8_t cnt)
{
	I2C_BMP180_Start();  //I2C��ʼ�ź�
	
	I2C_BMP180_CPUSendData(device_addr << 1);  //����������ַ+������	
	if(I2C_BMP180_CPUWaitAck())    //�ȴ�Ӧ��
	{
		return 1;
	}
	
	I2C_BMP180_CPUSendData(reg_addr);
	if(I2C_BMP180_CPUWaitAck())
	{
		return 1;
	}
	
	I2C_BMP180_Start();    //I2CЭ�鿪ʼ�ź� Ϊ����׼��
	
	I2C_BMP180_CPUSendData((device_addr << 1) | 0x01);   //����������ַ+������	
	if(I2C_BMP180_CPUWaitAck())
	{
		return 1;
	}
	
	while (cnt)
	{
		if (cnt == 1)
		{
			*reg_data = I2C_BMP180_CPUReadData(0);   //������,��������Ӧ�ź� Ϊ������׼��
			
		}
		else
		{
			*reg_data = I2C_BMP180_CPUReadData(1);    //������,����ACK  
		}
		cnt--;
		reg_data++;
	}
	I2C_BMP180_Stop();            //I2C�����ź�
	
	return 0;
}	
				
/************************************************************************
*  ��������BMP180_delay_msek(uint32_t msek)
*  ��  �ܣ�bmp180��ʱ
*  ��  �룺
*         msek ��ʱ�ĺ�����
*  ��  ����
*         ��
************************************************************************/

/*bmp180��ʱ����*/
void BMP180_delay_msek(uint32_t msek)
{
	BMP180_Delay1ms(msek);
}

/*************************************�ļ�����****************************/



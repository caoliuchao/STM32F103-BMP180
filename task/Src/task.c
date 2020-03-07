/********************************************************************
* @文件  : task.c
* @作者  : caochao
* @日期  : 14-2-2017
* @摘要  : 这个c文件包含各个任务
* 修改人 :
*  日期  :
*
* 修改人 :
*  日期  :
*********************************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include "task.h"

/*-----------------开始任务堆栈-------------------------------------*/
__align(8) OS_STK START_TASK_STK[START_STK_SIZE];   //创建任务堆栈空间

/*-----------------串口打印堆栈-------------------------------------*/
__align(8) OS_STK USART1_TASK_STK[USART1_STK_SIZE]; //创建任务堆栈空间	

/*-------------------BMP180任务堆栈---------------------------------*/
__align(8) OS_STK BMP180_TASK_STK[BMP180_STK_SIZE];//创建任务堆栈空间

/*-------------------BMP280任务堆栈---------------------------------*/
__align(8) OS_STK BMP280_TASK_STK[BMP280_STK_SIZE];//创建任务堆栈空间
/*******************************************************************
*   函数名：start_task(void *pdata)
*   功  能：开始任务，建立其他任务
*   输  入：
*          pdata 指针
*   输  出：
*          无
*******************************************************************/
void start_task(void *pdata)
{
    uint8_t status;
    OS_CPU_SR cpu_sr = 0;
	
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
 	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)   
	
    OSTaskCreate(usart1_task,(void *)0,(OS_STK*)&USART1_TASK_STK[USART1_STK_SIZE-1],USART1_TASK_PRIO);  

    OSTaskCreate(bmp180_task,(void *)0,(OS_STK*)&BMP180_TASK_STK[BMP180_STK_SIZE-1],BMP180_TASK_PRIO);
    //OSTaskCreate(bmp280_task,(void *)0,(OS_STK*)&BMP280_TASK_STK[BMP280_STK_SIZE-1],BMP280_TASK_PRIO); 
    
    status = OSTaskDelReq(OS_PRIO_SELF);	//请求删除起始任务.
    if (status == OS_ERR_NONE)
    {
		status = OSTaskDel(OS_PRIO_SELF);
        if (status == OS_ERR_NONE)
        {
            printf("删除开始任务成功 \r\n");
        }
        else
        {
            printf("开始任务删除失败 \r\n");
        }
    }
    else
    {
        printf("请求开始任务删除失败 \r\n");
    }
    
    OS_EXIT_CRITICAL();	   //退出临界区(可以被中断打断)    	
}

/*********************************************************************
*   函数名：usart1_task(void *pdata)
*   功  能：usart1串口打印任务
*   输  入：
*          pdata 指针
*   输  出：
*          无
**********************************************************************/
void usart1_task(void *pdata)
{  
    while (1)
    {  	
        printf("CPU的使用率为：%d%%  \r\n",OSCPUUsage);  //打印出CPU的使用率
		Delay1ms(1000);	
    }
}
		
/*************************************************************************************
 *   函数名：bmp180_task(void *pdata)
 *   功  能：大气压任务
 *   输  入：
 *          pdata 指针
 *   输  出：
 *          无
 ************************************************************************************/
void bmp180_task(void *pdata)
{
	int16_t  temp;
	int32_t  pres; 
	float    alti;
	struct bmp180_t  bmp180;
	
    bmp180.bus_read = BMP180_I2C_bus_read;   // assign read function
    bmp180.bus_write = BMP180_I2C_bus_write; // assign write function
    bmp180.delay_msec = BMP180_delay_msek;   // assign wait function
	
	BMP180_GPIO_Init();
	if (bmp180_init(&bmp180))
	{
		printf("bmp180初始化错误! \r\n");
	}	
	while(1)
	{
        bmp180_get_cal_param();
		temp = bmp180_get_temperature((unsigned long)bmp180_get_ut());
		printf("bmp180-当前温度为：%.1f \r\n",temp*0.1);
		pres = bmp180_get_pressure((unsigned long)bmp180_get_up());
		printf("bmp180-当前大气压强为：%ld \r\n",pres);
		alti = 44330*(1 - pow(pres/101325.0,1/5.255));
		printf("当前海拔高度为 %0.1f m \r\n",alti);
        
		Delay1ms(1000);	 
	}
}

/*************************************************************************************
 *   函数名：bmp280_task(void *pdata)
 *   功  能：大气压任务
 *   输  入：
 *          pdata 指针
 *   输  出：
 *          无
 ************************************************************************************/
void bmp280_task(void *pdata)
{
    int8_t rslt;
    double temp;
    double pres;
    uint8_t meas_dur;
    int32_t temp32;
    uint32_t pres32;
    struct bmp280_dev bmp_dev;
    struct bmp280_config bmp_conf;
    struct bmp280_uncomp_data ucomp_data;
         
//    /* Sensor interface over SPI with native chip select line */
//    bmp.dev_id  =  0;
//    bmp.intf = BMP280_SPI_INTF;
//    bmp.read = BMP280_SPI_bus_read;
//    bmp.write = BMP280_SPI_bus_write;
//    bmp.delay_ms = BMP280_delay_msek;
    
    /* Sensor interface over I2C with primary slave address  */
    bmp_dev.dev_id = BMP280_I2C_ADDR_PRIM;
    bmp_dev.intf = BMP280_I2C_INTF;
    bmp_dev.read = BMP280_I2C_bus_read;
    bmp_dev.write = BMP280_I2C_bus_write;
    bmp_dev.delay_ms = BMP280_delay_msek;
    
    rslt = bmp280_init(&bmp_dev);
    if (rslt == BMP280_OK) 
    {
        /* Sensor chip ID will be printed if initialization was successful */
        printf("\n Device found with chip id 0x%x", bmp_dev.chip_id);
    }

    /* Always read the current settings before writing, especially when
    * all the configuration is not modified 
    */
    /* Check if rslt == BMP280_OK, if not, then handle accordingly */
    rslt = bmp280_get_config(&bmp_conf, &bmp_dev);
    if (rslt != BMP280_OK)
    {
        printf("if not, then handle accordingly!\r\n");
    }
    
    /* Overwrite the desired settings */
    bmp_conf.filter = BMP280_FILTER_COEFF_2;
    bmp_conf.os_pres = BMP280_OS_16X;
    bmp_conf.os_temp = BMP280_OS_4X;
    bmp_conf.odr = BMP280_ODR_1000_MS;
    
    /* Check if rslt == BMP280_OK, if not, then handle accordingly */
    rslt = bmp280_set_config(&bmp_conf, &bmp_dev);
    if (rslt != BMP280_OK)
    {
        printf("if not, then handle accordingly!\r\n");
    }

    /* Always set the power mode after setting the configuration */
    rslt = bmp280_set_power_mode(BMP280_NORMAL_MODE, &bmp_dev);
    /* Check if rslt == BMP280_OK, if not, then handle accordingly */
    if (rslt != BMP280_OK)
    {
        printf("if not, then handle accordingly!\r\n");
    }

    meas_dur = bmp280_compute_meas_time(&bmp_dev);
    
	while(1)
    {
        bmp_dev.delay_ms(meas_dur); /* Measurement time */

        rslt = bmp280_get_uncomp_data(&ucomp_data, &bmp_dev);
        /* Check if rslt == BMP280_OK, if not, then handle accordingly */

        temp32 = bmp280_comp_temp_32bit(ucomp_data.uncomp_temp, &bmp_dev);
        pres32 = bmp280_comp_pres_32bit(ucomp_data.uncomp_press, &bmp_dev);
        temp = bmp280_comp_temp_double(ucomp_data.uncomp_temp, &bmp_dev);
        pres = bmp280_comp_pres_double(ucomp_data.uncomp_press, &bmp_dev);

        printf("UT: %d, UP: %d, T32: %d, P32: %d, T: %f, P: %f\r\n", \
        ucomp_data.uncomp_temp, ucomp_data.uncomp_press, temp32, \
        pres32, temp, pres);

        bmp_dev.delay_ms(1000); /* Sleep time between measurements = BMP280_ODR_1000_MS */
	}
}
/*****************************文件结束********************************/
                



/*********************************************************************
 * @文件  : task.h
 * @作者  : caochao
 * @日期  : 22-9-2017
 * @摘要  : 这个h文件时对UCOSII的任务堆栈大小和优先级的宏定义
 *
 * 修改人 :
 *  日期  :
 *
 * 修改人 :
 *  日期  :
 *********************************************************************/
/* 防止重复编译 */
#ifndef TASK_H
#define TASK_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/*-------------------------START 任务--------------------------------*/
#define START_TASK_PRIO      			30 ////设置任务优先级 开始任务的优先级设置为最低
#define START_STK_SIZE  				128 //设置任务堆栈大小
 
/*----------------------BMP180任务---------------------------------*/
#define BMP180_TASK_PRIO                10   //设置任务优先级
#define BMP180_STK_SIZE                 256 //设置任务堆栈大小  
     
/*----------------------BMP280任务---------------------------------*/
#define BMP280_TASK_PRIO                12   //设置任务优先级
#define BMP280_STK_SIZE                 256 //设置任务堆栈大小       
     
/*-----------------------USART1-------------------------------------*/
#define USART1_TASK_PRIO                17 //设置任务优先级
#define USART1_STK_SIZE                 128 //设置任务堆栈大小  

/*-----------------开始任务堆栈--------------------------------------*/
extern OS_STK START_TASK_STK[START_STK_SIZE];   //创建任务堆栈空间

void start_task(void *pdata);  //任务函数接口
void usart1_task(void *pdata); //任务函数接口
void bmp180_task(void *pdata);  //任务函数接口
void bmp280_task(void *pdata);  //任务函数接口

#ifdef __cplusplus
 }
#endif

#endif/*task.h*/
/*******************************文件结束*****************************/
 
 
 


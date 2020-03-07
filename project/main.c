/**************************************************************************
 * @文件  : main.c
 * @作者  : caochao
 * @日期  : 10-1-2018
 * @摘要  : 这个C文件时实现了系统的初始化和ucosII的启动
 *
 * 修改人 :
 *  日期  :
 *
 * 修改人 :
 *  日期  :
 **************************************************************************/
#include "stm32f10x.h"
#include "includes.h"
#include "task.h"

/**************************************************************************
* 函数名:  main
* 功  能:  程序的主函数
* 输  入:   
*         无 
* 输  出:   
*         有
**************************************************************************/
int main(void)
{
   /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    USART1_Configuration();  //初始化usart    

    Systick_Init();          //启动UCOSII系统节拍 
    
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO);//创建起始任务
	OSStart();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {
		printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    }
}
#endif

/**
  * @}
  */

/***********************************文件结束***************************************/



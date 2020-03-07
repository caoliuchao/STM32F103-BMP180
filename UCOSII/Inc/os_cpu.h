/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu.h
* Author		: Librae
* Date			: 06/10/2010
* Description	: The C language part of the migration code for UCOS-II on STM32 
*                 includes the task stack initialization code and hook functions
*
******************************************************************************/

#ifndef	__OS_CPU_H__
#define	__OS_CPU_H__

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/******************************************************************************
*         Defines data types that are not associated with the compiler
******************************************************************************/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;			/* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;			/* Signed    8 bit quantity       */
typedef unsigned short INT16U;			/* Unsigned 16 bit quantity       */
typedef signed   short INT16S;			/* Signed   16 bit quantity       */
typedef unsigned int   INT32U;			/* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;			/* Signed   32 bit quantity       */
typedef float          FP32;			/* Single precision floating point*/
typedef double         FP64;			/* Double precision floating point*/

/*£¡<STM32 is 32 bit wide, where both OS_STK and OS_CPU_SR should be 32-bit data types>*/
typedef unsigned int   OS_STK;			/* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;		/* Define size of CPU status register*/

/* 
*******************************************************************************
*                             Cortex M3
*                     Critical Section Management
*******************************************************************************
*/

/*
*******************************************************************************
*                          ARM Miscellaneous
*******************************************************************************
*/

/******************************************************************************
*                   Define stack growth direction.
******************************************************************************/
/*!<In CM3, the stack is increased from high address to low address, 
  so OS_STK_GROWTH is set to 1>*/
#define  OS_STK_GROWTH        1u    /* Stack grows from HIGH to LOW memory on ARM */

/* !<task toggle macro, implemented by assembly.>*/
#define  OS_TASK_SW()         OSCtxSw()

/*
*******************************************************************************
*                               PROTOTYPES
*                           (see OS_CPU_A.ASM)
*******************************************************************************
*/

/* !<OS_CRITICAL_METHOD = 1: directly use the processor's switch interrupt command to 
                         implement macros>  */
/* !<OS_CRITICAL_METHOD = 2: use the stack to save and restore the state of the CPU> */
/* !<OS_CRITICAL_METHOD = 3: use the compiler extension function to get the program
                          status word and save in the local variable cpu_sr> */
						  
#define  OS_CRITICAL_METHOD   3u	   //access to the critical section

#if OS_CRITICAL_METHOD == 3u
#define  OS_ENTER_CRITICAL()      {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()       {OS_CPU_SR_Restore(cpu_sr);}
#endif

#if OS_CRITICAL_METHOD == 3                       /* See OS_CPU_A.ASM                                  */
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
#endif

void       OSCtxSw(void);
void       OSIntCtxSw(void);
void       OSStartHighRdy(void);


#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/

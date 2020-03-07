;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                               (c) Copyright 1992-2006, Micrium, Weston, FL
;                                          All Rights Reserved
;
;                                           Generic ARM Port
;
; File      : OS_CPU_A.ASM
; Version   : V2.86
; By        : Jean J. Labrosse
;
; For       : ARMv7M Cortex-M3
; Mode      : Thumb2
; Toolchain : RealView Development Suite
;             RealView Microcontroller Development Kit (MDK)
;             ARM Developer Suite (ADS)
;             Keil uVision
;********************************************************************************************************

;********************************************************************************************************
;                                           PUBLIC FUNCTIONS
;********************************************************************************************************

        IMPORT  OSRunning               ; External references
        IMPORT  OSPrioCur
        IMPORT  OSPrioHighRdy
        IMPORT  OSTCBCur
        IMPORT  OSTCBHighRdy
        IMPORT  OSIntNesting
        IMPORT  OSIntExit
        IMPORT  OSTaskSwHook
       
	   
        EXPORT  OSStartHighRdy               
        EXPORT  OSCtxSw
        EXPORT  OSIntCtxSw
		EXPORT  OS_CPU_SR_Save                                      ; Functions declared in this file
    	EXPORT  OS_CPU_SR_Restore       
        EXPORT  PendSV_Handler
        	
;********************************************************************************************************
;                                                EQUATES
;********************************************************************************************************

NVIC_INT_CTRL   	EQU     0xE000ED04  ; Interrupt control state register.
NVIC_SYSPRI12    	EQU     0xE000ED20  ; System priority register (priority 14).
NVIC_PENDSV_PRI 	EQU     0xFFFF0000  ; PendSV interrupts and system beat interrupts (both are minimum, 0xff)
NVIC_PENDSVSET  	EQU     0x10000000  ; The value that triggers the software interrup

;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************
		PRESERVE8 
		
		AREA    |.text|, CODE, READONLY
        THUMB 
    
           

;********************************************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
; Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
;                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
;
;
; Note(s)    : 1) These functions are used in general like this:
;
;                 void Task (void *p_arg)
;                 {
;                 #if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
;                     OS_CPU_SR  cpu_sr;
;                 #endif
;
;                          :
;                          :
;                     OS_ENTER_CRITICAL();             /* cpu_sr = OS_CPU_SaveSR();                */
;                          :
;                          :
;                     OS_EXIT_CRITICAL();              /* OS_CPU_RestoreSR(cpu_sr);                */
;                          :
;                          :
;                 }
;********************************************************************************************************

OS_CPU_SR_Save
    MRS     R0, PRIMASK  	;Read PRIMASK to R0,R0 to return value
    CPSID   I				;PRIMASK=1, shut off (NMI and hardware FAULT can respond)
    BX      LR			    ;return

OS_CPU_SR_Restore
    MSR     PRIMASK, R0	   	;Reading R0 to PRIMASK, R0 is the parameter
    BX      LR				;return


;/**************************************************************************************
; * function name: OSStartHighRdy
; *
; * function description: use the scheduler to run the first task
; *
; * parameter: None
; *
; * return value: None
;**************************************************************************************/  

OSStartHighRdy
    LDR     R0, =NVIC_SYSPRI12                                  ; Set the PendSV exception priority
    LDR     R1, =NVIC_PENDSV_PRI
    STR     R1, [R0]

    MOVS    R0, #0                                              ; Set the PSP to 0 for initial context switch call
    MSR     PSP, R0

    LDR     R0, =OSRunning                                      ; OSRunning = TRUE
    MOVS    R1, #1
    STRB    R1, [R0]

    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]

    CPSIE   I                                                   ; Enable interrupts at processor level

OSStartHang
    B       OSStartHang                                         ; Should never get here

;/**************************************************************************************
; * function name: OSCtxSw
; *
; * function description: task-level context switching
; *
; * parameter: None
; *
; * return value: None
;***************************************************************************************/
  
OSCtxSw
    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR
	
;/**************************************************************************************
; * function name: OSIntCtxSw
; *
; * function description: interrupt class task switching
; *
; * parameter: None
; *
; * return value: None
;***************************************************************************************/

OSIntCtxSw
    LDR     R0, =NVIC_INT_CTRL                                  ; Trigger the PendSV exception (causes context switch)
    LDR     R1, =NVIC_PENDSVSET
    STR     R1, [R0]
    BX      LR

;/**************************************************************************************
; * function name: OSPendSV
; *
; * function description: OSPendSV is used to cause a context switch.
; *
; * parameter: None
; *
; * return value: None
;***************************************************************************************/

PendSV_Handler
    CPSID   I                                                   ; Prevent interruption during context switch
    MRS     R0, PSP                                             ; PSP is process stack pointer
    CBZ     R0, PendSV_Handler_nosave                     ; Skip register save the first time

    SUBS    R0, R0, #0x20                                       ; Save remaining regs r4-11 on process stack
    STM     R0, {R4-R11}

    LDR     R1, =OSTCBCur                                       ; OSTCBCur->OSTCBStkPtr = SP;
    LDR     R1, [R1]
    STR     R0, [R1]                                            ; R0 is SP of process being switched out

                                                                ; At this point, entire context of process has been saved
PendSV_Handler_nosave
    PUSH    {R14}                                               ; Save LR exc_return value
    LDR     R0, =OSTaskSwHook                                   ; OSTaskSwHook();
    BLX     R0
    POP     {R14}

    LDR     R0, =OSPrioCur                                      ; OSPrioCur = OSPrioHighRdy;
    LDR     R1, =OSPrioHighRdy
    LDRB    R2, [R1]
    STRB    R2, [R0]

    LDR     R0, =OSTCBCur                                       ; OSTCBCur  = OSTCBHighRdy;
    LDR     R1, =OSTCBHighRdy
    LDR     R2, [R1]
    STR     R2, [R0]

    LDR     R0, [R2]                                            ; R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
    LDM     R0, {R4-R11}                                        ; Restore r4-11 from new process stack
    ADDS    R0, R0, #0x20
    MSR     PSP, R0                                             ; Load PSP with new process SP
    ORR     LR, LR, #0x04                                       ; Ensure exception return uses process stack
    CPSIE   I
    BX      LR                                                  ; Exception return will restore remaining context
    NOP
    END

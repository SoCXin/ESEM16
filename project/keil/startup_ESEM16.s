;*******************************************************************************
; *file       : startup_ESEM16.s
; *description: ESEM16 Device Startup File
; *author     : Eastsoft MCU Software Team
; *version    : V0.01
; *data       : 9/2/2016
;
; *Copyright (C) 2016 Shanghai Eastsoft Microelectronics Co., Ltd.
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;*******************************************************************************


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; HardFault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     GPIO0INT_IRQHandler          ;16, GPIO0INT IRQHandler
                DCD     GPIO1INT_IRQHandler          ;17, GPIO1INT IRQHandler
                DCD     GPIO2INT_IRQHandler          ;18, GPIO2INT IRQHandler
                DCD     GPIO3INT_IRQHandler          ;19, GPIO3INT IRQHandler
                DCD     GPIO4INT_IRQHandler          ;20, GPIO4INT IRQHandler
                DCD     GPIO5INT_IRQHandler          ;21, GPIO5INT IRQHandler
                DCD     GPIO6INT_IRQHandler          ;22, GPIO6INT IRQHandler
                DCD     GPIO7INT_IRQHandler          ;23, GPIO7INT IRQHandler
                DCD     GPIO8INT_IRQHandler          ;24, GPIO8INT IRQHandler
                DCD     GPIO9INT_IRQHandler          ;25, GPIO9INT IRQHandler
                DCD     GPIO10INT_IRQHandler          ;26, GPIO10INT IRQHandler
                DCD     GPIO11INT_IRQHandler          ;27, GPIO11INT IRQHandler
                DCD     T16N0INT_IRQHandler          ;28, T16N0INT IRQHandler
                DCD     T16N1INT_IRQHandler          ;29, T16N1INT IRQHandler
                DCD     0                              ;30, Reserved
                DCD     0                              ;31, Reserved
                DCD     T32N0INT_IRQHandler          ;32, T32N0INT IRQHandler
                DCD     0                              ;33, Reserved
                DCD     0                              ;34, Reserved
                DCD     0                              ;35, Reserved
                DCD     WDTINT_IRQHandler          ;36, WDTINT IRQHandler
                DCD     0                              ;37, Reserved
                DCD     0                              ;38, Reserved
                DCD     ADCINT_IRQHandler          ;39, ADCINT IRQHandler
                DCD     0                              ;40, Reserved
                DCD     0                              ;41, Reserved
                DCD     0                              ;42, Reserved
                DCD     UART0TINT_IRQHandler          ;43, UART0TINT IRQHandler
                DCD     UART0RINT_IRQHandler          ;44, UART0RINT IRQHandler
                DCD     0                              ;45, Reserved
                DCD     0                              ;46, Reserved
                DCD     EMINT_IRQHandler          ;47, EMINT IRQHandler


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                ;IMPORT  SystemInit
                ;LDR     R0, =SystemInit
                ;BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception IRQHandlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler	PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC



                EXPORT  GPIO0INT_IRQHandler    [WEAK]
                EXPORT  GPIO1INT_IRQHandler    [WEAK]
                EXPORT  GPIO2INT_IRQHandler    [WEAK]
                EXPORT  GPIO3INT_IRQHandler    [WEAK]
                EXPORT  GPIO4INT_IRQHandler    [WEAK]
                EXPORT  GPIO5INT_IRQHandler    [WEAK]
                EXPORT  GPIO6INT_IRQHandler    [WEAK]
                EXPORT  GPIO7INT_IRQHandler    [WEAK]
                EXPORT  GPIO8INT_IRQHandler    [WEAK]
                EXPORT  GPIO9INT_IRQHandler    [WEAK]
                EXPORT  GPIO10INT_IRQHandler    [WEAK]
                EXPORT  GPIO11INT_IRQHandler    [WEAK]
                EXPORT  T16N0INT_IRQHandler    [WEAK]
                EXPORT  T16N1INT_IRQHandler    [WEAK]
                EXPORT  T32N0INT_IRQHandler    [WEAK]
                EXPORT  WDTINT_IRQHandler    [WEAK]
                EXPORT  ADCINT_IRQHandler    [WEAK]
                EXPORT  UART0TINT_IRQHandler    [WEAK]
                EXPORT  UART0RINT_IRQHandler    [WEAK]
                EXPORT  EMINT_IRQHandler    [WEAK]



GPIO0INT_IRQHandler
GPIO1INT_IRQHandler
GPIO2INT_IRQHandler
GPIO3INT_IRQHandler
GPIO4INT_IRQHandler
GPIO5INT_IRQHandler
GPIO6INT_IRQHandler
GPIO7INT_IRQHandler
GPIO8INT_IRQHandler
GPIO9INT_IRQHandler
GPIO10INT_IRQHandler
GPIO11INT_IRQHandler
T16N0INT_IRQHandler
T16N1INT_IRQHandler
T32N0INT_IRQHandler
WDTINT_IRQHandler
ADCINT_IRQHandler
UART0TINT_IRQHandler
UART0RINT_IRQHandler
EMINT_IRQHandler


                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END



/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 全局变量定义 
*	文件名称 : typedef.h
*	版    本 : V1.0
*	描    述               
*	修改记录 : 
*		版本号   日期        作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#ifndef _VARIABLE_H
#define _VARIABLE_H

extern volatile B32_B08 g_Energy ;	
extern const B16_B08 ADJ_DEFAULT[EE_PARAM_SUM];
extern volatile uint8_t g_Tasks_Flag;
extern volatile uint16_t g_SysTick;

extern DL645_TypeDef g_dl645;
extern MeterVariable_TypeDef g_Emu_Data;
extern DeviceInfo_TypeDef g_DeviceInfo;
extern EM_GainPara_TypeDef g_EM_GainPara;
extern EP_Data_TypeDef g_EP_Data;
/*
* 其他函数调用
*/
void MCU_Init(void);
void SYS_Init(void);

#endif


/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 任务头文件
*	文件名称 : task.h
*	版    本 : V1.0
*	描    述               
*	修改记录 : 
*		版本号   日期        作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#ifndef _TASK_H
#define _TASK_H


/*
* 其他函数调用
*/
void Basic_Timer(void);
void SysTick_Task(void);
void Key_Task(void);
void Uart_Task(void);
void Sec_Task(void);	
void Minut_Task(void);

#endif

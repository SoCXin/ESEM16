/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : ����ͷ�ļ�
*	�ļ����� : task.h
*	��    �� : V1.0
*	��    ��               
*	�޸ļ�¼ : 
*		�汾��   ����        ����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************************/
#ifndef _TASK_H
#define _TASK_H


/*
* ������������
*/
void Basic_Timer(void);
void SysTick_Task(void);
void Key_Task(void);
void Uart_Task(void);
void Sec_Task(void);	
void Minut_Task(void);

#endif

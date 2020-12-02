/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : ȫ�ֱ������� 
*	�ļ����� : typedef.h
*	��    �� : V1.0
*	��    ��               
*	�޸ļ�¼ : 
*		�汾��   ����        ����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
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
* ������������
*/
void MCU_Init(void);
void SYS_Init(void);

#endif


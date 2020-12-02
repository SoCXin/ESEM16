/*
*********************************************************************************************************
*
*	ģ������ : ����ͷ�ļ�
*	�ļ����� : em.h
*	˵    �� : 
*
*********************************************************************************************************
*/
#ifndef _EMU_H
#define _EMU_H

#define VREF		1.3					//��׼��ѹ
#define Gu			2					//��ѹ��������
#define Gi			16					//������������

#define APHCAL_C	(183776.2985)		//����

#define K_V    		1081    			//��ѹͨ����ѹ�� ��������/��ѹ����
#define R_I			0.002				//�����������裨ŷ��

#define MC			1200				//�����imp/kWh
#define _PFSET_C    2359296000000000	//�й�����У׼����(P93) 

/* 
��ѹ���㹫ʽ 	��λ��(0.1V)
 u = (DATA * K_V * VREF)/(Gu*2^24)	DATAΪ��ѹ��Чֵ�Ĵ���ֵ
*/
#define _U_C		((double)10.0*VREF*K_V/Gu/(1<<24))

/* 
�������㹫ʽ 	��λ��(0.001A)
 i = (DATA * VREF)/(R_I*Gu*2^24)	DATAΪ������Чֵ�Ĵ���ֵ
*/
#define _I_C		((double)1000.0*VREF/R_I/Gi/(1<<24))

/* 
���ʼ��㹫ʽ 	��λ��(0.0001w)
 p = (DATA *K_V*VREF*VREF )/(R_I*Gi*Gu*2^31)   DATAΪ�й��ʼĴ���ֵ
*/
#define _P_C		((double)10000*VREF*VREF*K_V/R_I/Gi/Gu/(1<<21)/(1<<10))

/* 
��Ƶ���峣��	(P93)
Vu/Un = 1/K_V	Via/Ib = R_I
 PFSET =  (_PFSET_C*Gi*Gu*R_I)/(K_V*MC*VREF*VREF)
*/
#define PFSET_DATA		((double)_PFSET_C*Gi*Gu*R_I/K_V/MC/(VREF*VREF))
/* 
�������޹���
 �������� = (K_V*VREF*VREF)/(256*R_I*Gu*Gi)
*/
#define P_START			7		//��������7w
#define PSTART_DATA		((double)P_START*R_I*Gu*Gi/K_V/(VREF*VREF)*(1<<24))


/*
* ������������
*/
void PAGAIN_Adjust(uint8_t *uBuf);
void IAGAIN_Adjust(uint8_t *uBuf);
void APHCAL_Adjust(uint8_t *uBuf);
void UGAIN_Adjust(uint8_t *uBuf);
void PAOFF_Adjust(uint8_t *uBuf);
uint32_t Get_EnergyPulse(void);
void Read_UIP(void);
void Energy_Add(uint32_t pulse_cnt);
void EM_Config(void);
void Emu_Adjust_Check(void);
void Energy_Save(void);
void Energy_Reset(void);
#endif

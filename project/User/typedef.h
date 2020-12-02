/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : ȫ�����Ͷ���
*	�ļ����� : typedef.h
*	��    �� : V1.0
*	��    ��               
*	�޸ļ�¼ : 
*		�汾��   ����        ����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************************/
#ifndef _TYPEDEF_H
#define _TYPEDEF_H


//����ṹ��
typedef union  //����һ��16λ����2��8λ�ֽ�֮���ת��
{
    uint8_t  B08[2];
    uint16_t  B16;
} B16_B08 ;
typedef union
{
    uint8_t  B08[4];
    uint32_t B32;
} B32_B08;
typedef union
{
    uint16_t  B16[2];
    uint32_t B32;
} B32_B16;
typedef struct      		//������ BCD��
{
    B32_B08    U;      		//��ѹxxxxxxx.x
    B32_B08    I1;     		//����xxxxx.xxx
    B32_B08    P1;     		//�й�����xxxx.xxxx
    B32_B08    Cosa;   		//��������xxxxx.xxx
    B16_B08    Freq;   		//����Ƶ��xx.xx
} MeterVariable_TypeDef;

//6   ͨ�ŵ�ַ/�豸��(�����ڿ�д)
//32  �ʲ��������(ASCII��)     property
//6   ���ѹ(ASCII��)    rated voltage
//6   �����/��������(ASCII��)
//6   ������(ASCII��)
//10  �豸�ͺ�(ASCII��)(Ӳ���汾)
//10  ��������(ASCII��)
//16  Э��汾��(ASCII��)(��������汾)
//16  ��������汾

typedef struct      //�豸ϵͳȫ�ֱ���
{
    uint8_t Dev_ID[6] ;
    uint8_t PtyOP_ID[32] ;
    uint8_t Rated_Voltage[6] ;
    uint8_t Rated_Current[6] ;
    uint8_t Max_Current[6] ;
    uint8_t HW_Ver[10] ;
    uint8_t PD_Date[10] ;
    uint8_t EMSW_Ver[16] ;
    uint8_t WL_Ver[16] ;
} DeviceInfo_TypeDef;

/* */
typedef struct
{    
	uint8_t Flag;				/* ��־λ */
	uint8_t data_buf[220];		/* ���ͻ����� */
	uint8_t Len;				/* �������ݳ��� */
    uint8_t Cnt;				/* �������ݼ��� */
    uint8_t DataLen;            /* ���������򳤶� */  
	uint8_t Step;				/* ���ղ��� */
	uint8_t Addr;				/* ��ַ����:AA��λ��ַ��99�㲥��ַ������Ϊ������ַ  */
	uint8_t Overtimr_Timer;		/* ���ջ��ͳ�ʱ��ʱ�� */
	uint8_t TxDelay_Timer;		/* ������ʱ��ʱ�� */
} DL645_TypeDef;
/*
*********************************************************************************************************
*                                      EEPROM
*********************************************************************************************************
*/
typedef struct {
	B16_B08 U_Gain;				//v gain
	B16_B08 I_Gain;				//i gain
	B16_B08 P_Gain;				//p gain
	B16_B08 Phase_Gain; 		//phase gain
    B16_B08 Paoff_Gain; 		//paoff gain
	B16_B08 Check_Sum;
}EM_GainPara_TypeDef;

enum {
	EE_UGAIN,                   //��ѹ
	EE_IGAIN,                   //����
	EE_PGAIN,
	EE_APHCAL,
    EE_PAOFF,
    EE_CHECK_SUM,				//У���   
	EE_PARAM_SUM,				//��������	
	
};
typedef struct {
	B32_B08 energy;				//����
	uint32_t pulse;				//����
}EP_Data_TypeDef;


enum {
	TASK_FLAG_KEY,				//�����¼�
	TASK_FLAG_SEC,				//ÿ�봦���¼�
	TASK_FLAG_MINUTE,			//ÿ���Ӵ����¼�
	TASK_FLAG_UART,				//uart�¼�
};
#endif


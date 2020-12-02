/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : ȫ�ֺ궨��
*	�ļ����� : macrodef.h
*	��    �� : V1.0
*	��    ��               
*	�޸ļ�¼ : 
*		�汾��   ����        ����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************************/
#ifndef _MACRODEF_H
#define _MACRODEF_H

/*
*	ϵͳ�궨��
*/
#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif
#ifndef FAILED
	#define FAILED	0
#endif
#ifndef SUCCEED
	#define SUCCEED 1
#endif

#define mMemBitSet(c,b)			(c) |= (0x01 << (b))
#define mMemBitClr(c,b)			(c) &= ~(0x01 << (b))
#define mMemBitGet(c,b)			((c & (0x01 << (b)))>0)
#define mMemBitInv(c,b)			((c) ^= (0x01 << (b)))
#define mMemWrite (c,a)			(c)	 = (a)
#define FlagSet(c,b)			mMemBitSet(c,b)
#define FlagClr(c,b)			mMemBitClr(c,b)
#define FlagInv(c,b)			mMemBitInv(c,b)
#define FlagGet(c,b)		    mMemBitGet(c,b)		


/*
*	�û��궨��
*/
/****************************Cortex3****************************/
#define DISABLE_INTERRUPT()		__disable_irq()
#define ENABLE_INTERRUPT()		__enable_irq()

/****************************ʱ�� ****************************/
#define SYSCLK  20000000						//ϵͳʱ��,
#define PCLK	20000000						//оƬ�ڲ�����ģ��ʱ��

/****************************WDT****************************/
#define WDT_CLEAR()		((WDT->LOAD.Word = 0xffff))

/****************************KEY ****************************/
#define KEY0 	(GPIO->DATA.DATA_2)
		
/*
	�����˲�ʱ��100ms, ��λ10ms��
	ֻ��������⵽100ms״̬�������Ϊ��Ч����������Ͱ��������¼�	
*/
#define KEY_DOWN_PERIOD     10		//��λ10ms�� ����80ms����Ϊ��������
#define KEY_LONG_PERIOD     200		//��λ10ms�� ����2�룬��Ϊ�����¼�
#define KEY_REPEAT_PERIOD   5       //��λ10ms�� ������������Ϊ50ms
/****************************LED ****************************/
#define LED_On()		(GPIO->DATABCR.DATABCR_9 = 1)
#define LED_Off()		(GPIO->DATABSR.DATABSR_9 = 1)
#define LED_Toggle()	(GPIO->DATABRR.DATABRR_9 = 1)

/****************************UART****************************/
#define UART_BAUD	9600						//������
#define UART_TX_ENABLE		UART0->INT.TXIE = 1	//�����ж�ʹ��
#define UART_TX_DISABLE		UART0->INT.TXIE = 0	//�����жϽ�ֹ
#define UART_RX_ENABLE		UART0->INT.RXIE = 1	//�����ж�ʹ��
#define UART_RX_DISABLE		UART0->INT.RXIE = 0	//�����жϽ�ֹ

/**********************EEPROM ����**************************/
#define EE_ADJ_BASEADDR			0x00			//�������������ַ
#define EE_BACKUPS_OFFSET		0x20			//�����������ݵ�ַ
#define EE_DI_ADDR			    0x40			//�������ݱ����ַ

#endif


/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : iic master ����
*	�ļ����� : iicm.c
*	��    �� : V1.0
*	��    �� ��
*	��    �� ��2016\12\29             
*	�޸ļ�¼ :
*			����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************************/
#include "includes.h"
#include "iicm.h"

#define READ_SDA()		(GPIO->DATA.DATA_3)
#define SDA_INPUT()		(GPIO->DIRBSR.DIRBSR_3 = 1)			//����ģʽ
#define SDA_OUTPUT()	(GPIO->DIRBCR.DIRBCR_3 = 1)			//���ģʽ
#define SDA_H()			(GPIO->DATABSR.DATABSR_3 = 1)		//����ߵ�ƽ
#define SDA_L()			(GPIO->DATABCR.DATABCR_3 = 1)		//����͵�ƽ

#define SCK_H()			(GPIO->DATABSR.DATABSR_4 = 1)		//����ߵ�ƽ
#define SCK_L()			(GPIO->DATABCR.DATABCR_4 = 1)		//����͵�ƽ

#define ACK		0											//Ӧ��ACk
#define NACK	1											//��Ӧ��

/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣�
*	��    ��:  delayus  ÿ����ʱ1.1us����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void I2C_DelayUs(uint16_t delayus)
{
	uint8_t ucA;
	for(ucA=0;ucA<delayus;ucA++)
	{
		__asm  
		{
			nop ; nop  ;
		}
	}
}
/*
********************************************************************************************************
*	�� �� ��: I2C_START
*	����˵��: I2C���������źš�keep scl high ,sda high to low
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************************************
*/
void I2C_START(void)
{	
	SDA_OUTPUT();	
	
	SDA_H();
	I2C_DelayUs(1);
	SCK_H();
	I2C_DelayUs(2);
	SDA_L();
	I2C_DelayUs(2);
	SCK_L();
	I2C_DelayUs(2);	
}
/*
********************************************************************************************************
*	�� �� ��: I2C_STOP
*	����˵��: iic ֹͣ��keep SCK high��SDA low to high
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************************************
*/
void I2C_STOP(void)
{
	SDA_OUTPUT();
	
	SCK_L();		
	I2C_DelayUs(1);
	SDA_L();
	I2C_DelayUs(2);
	SCK_H();
	I2C_DelayUs(2);
	SDA_H();
	I2C_DelayUs(2);
}

/*
********************************************************************************************************
*	�� �� ��: I2C_WaitAck
*	����˵��: iic �ȴ�����Ӧ��
*	��    �Σ���
*	�� �� ֵ: 0����Ӧ�� 1����Ӧ��
********************************************************************************************************
*/
uint8_t I2C_WaitAck(void)
{
	uint16_t wait_time = 0, ack_state;
	
	SDA_INPUT();
	I2C_DelayUs(2);
	SCK_H();
	I2C_DelayUs(2);
	
	ack_state = ACK;
	while (READ_SDA())								//�ȴ�ACK�ź�(SDA����)
	{		
		if (wait_time >= 250)
		{
			ack_state = NACK;
			break;
		}
		wait_time++;
	}
	SCK_L();
	I2C_DelayUs(2);
	SDA_OUTPUT();
	return ack_state;								//����ֵ:ACK=0, NACK=1
}

/*
********************************************************************************************************
*	�� �� ��: I2C_SendAck
*	����˵��: �ӻ�����Ӧ��
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************************************
*/
void I2C_SendAck(void)
{
	SDA_OUTPUT();
	SDA_L();											//������Ϊ��
	I2C_DelayUs(2);
	SCK_H();
	I2C_DelayUs(2);
	SCK_L();
}
/*
********************************************************************************************************
*	�� �� ��: I2C_SendNoAck
*	����˵��: �ӻ�������Ӧ��
*	��    �Σ���
*	�� �� ֵ: ��
********************************************************************************************************
*/
void I2C_SendNoAck(void)
{
	SDA_OUTPUT();
	SDA_H();											//������Ϊ��
	I2C_DelayUs(2);
	SCK_H();
	I2C_DelayUs(2);
	SCK_L();
}
/*
*********************************************************************************************************
*	�� �� ��: I2C_Write
*	����˵��: CPU��I2C�����豸����8bit����
*	��    ��:  _ucByte �� �ȴ����͵��ֽ� 
*	�� �� ֵ: 0 д�ɹ�  1 д��ʧ��     
*********************************************************************************************************
*/
void I2C_WriteByte(uint8_t data)
{
	uint8_t i;
	
	SDA_OUTPUT();
	SCK_L();	
	for (i=0; i<8; i++)								//ͨ��������λ������д��24C02
	{
		if (data & 0x80)
			SDA_H();
		else
			SDA_L();
		data <<= 1;									//��д����������һλ
		I2C_DelayUs(2);
		SCK_H();
		I2C_DelayUs(2);
		SCK_L();
		I2C_DelayUs(2);
	}	
}
/*
*********************************************************************************************************
*	�� �� ��: I2C_ReadByte
*	����˵��: CPU��I2C�����豸��8bit����
*	��    ��: ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t I2C_ReadByte(void)
{
	uint8_t i, data = 0;
	
	SDA_INPUT();
	for (i=0; i<8; i++)
	{	
		data <<= 1;	
		SCK_L();
		I2C_DelayUs(2);
		SCK_H();		
		I2C_DelayUs(2);
		if (READ_SDA() == 1)
			data |= 0x01;	
	}
	SCK_L();
	SDA_OUTPUT();
	return data;
}
/*
*********************************************************************************************************
*	�� �� ��: EEP_WriteStr
*	����˵��: ����EEPROMָ����ַд����������
*	��    ��:  addr : ��ʼ��ַ
*			 len : ���ݳ��ȣ���λΪ�ֽ�
*			 str : ���д�����ݵĻ�����ָ��
*	�� �� ֵ:
*********************************************************************************************************
*/
uint8_t EEP_WriteStr(uint8_t addr, uint8_t *str, uint8_t len)
{
	uint8_t i;
	
	I2C_START();
	I2C_WriteByte(0xA0);
	if (I2C_WaitAck() == NACK)	
	{
		I2C_STOP();	
		return FALSE;
	}
	I2C_WriteByte(addr);
	if (I2C_WaitAck() == NACK)	
	{
		I2C_STOP();	
		return FALSE;
	}
	I2C_DelayUs(1);
	for (i=0; i<len; i++)
	{
		I2C_WriteByte(str[i]);
		if (I2C_WaitAck() == NACK)	
		{
			I2C_STOP();	
			return FALSE;
		}
	} 
	I2C_STOP();	
	DelayMs(3);
	return TRUE;
}
/*
*********************************************************************************************************
*	�� �� ��: EEP_ReadStr
*	����˵��: ������EEPROMָ����ַд����������
*	��    ��:  addr : ��ʼ��ַ
*			 	len : ���ݳ��ȣ���λΪ�ֽ�
*			 	str : ��Ŷ������ݵĻ�����ָ��
*	�� �� ֵ:
*********************************************************************************************************
*/
uint8_t EEP_ReadStr(uint8_t addr, uint8_t *str, uint8_t len)
{
	uint8_t i;
	
	I2C_START();
	I2C_WriteByte(0xA0);
	if (I2C_WaitAck() == NACK)	
	{
		I2C_STOP();	
		return FALSE;
	}

	I2C_WriteByte(addr);
	if (I2C_WaitAck() == NACK)	
	{
		I2C_STOP();	
		return FALSE;
	}
	
	I2C_START();
	I2C_WriteByte(0xA1);
	if (I2C_WaitAck() == NACK)	
	{
		I2C_STOP();	
		return FALSE;
	}
	I2C_DelayUs(1);	
	for (i=0; i<len-1; i++)
	{
		str[i] = I2C_ReadByte();
		I2C_SendAck();
	}
	str[i] = I2C_ReadByte();
	I2C_SendNoAck();
	I2C_STOP();	
	return TRUE;
}



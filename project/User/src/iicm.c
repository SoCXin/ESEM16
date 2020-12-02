/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : iic master 程序
*	文件名称 : iicm.c
*	版    本 : V1.0
*	描    述 ：
*	日    期 ：2016\12\29             
*	修改记录 :
*			作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#include "includes.h"
#include "iicm.h"

#define READ_SDA()		(GPIO->DATA.DATA_3)
#define SDA_INPUT()		(GPIO->DIRBSR.DIRBSR_3 = 1)			//输入模式
#define SDA_OUTPUT()	(GPIO->DIRBCR.DIRBCR_3 = 1)			//输出模式
#define SDA_H()			(GPIO->DATABSR.DATABSR_3 = 1)		//输出高电平
#define SDA_L()			(GPIO->DATABCR.DATABCR_3 = 1)		//输出低电平

#define SCK_H()			(GPIO->DATABSR.DATABSR_4 = 1)		//输出高电平
#define SCK_L()			(GPIO->DATABCR.DATABCR_4 = 1)		//输出低电平

#define ACK		0											//应答ACk
#define NACK	1											//无应答

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，
*	形    参:  delayus  每个延时1.1us左右
*	返 回 值: 无
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
*	函 数 名: I2C_START
*	功能说明: I2C总线启动信号。keep scl high ,sda high to low
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: I2C_STOP
*	功能说明: iic 停止。keep SCK high，SDA low to high
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: I2C_WaitAck
*	功能说明: iic 等待主机应答
*	形    参：无
*	返 回 值: 0：有应答 1：无应答
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
	while (READ_SDA())								//等待ACK信号(SDA拉低)
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
	return ack_state;								//返回值:ACK=0, NACK=1
}

/*
********************************************************************************************************
*	函 数 名: I2C_SendAck
*	功能说明: 从机发送应答
*	形    参：无
*	返 回 值: 无
********************************************************************************************************
*/
void I2C_SendAck(void)
{
	SDA_OUTPUT();
	SDA_L();											//数据线为低
	I2C_DelayUs(2);
	SCK_H();
	I2C_DelayUs(2);
	SCK_L();
}
/*
********************************************************************************************************
*	函 数 名: I2C_SendNoAck
*	功能说明: 从机发送无应答
*	形    参：无
*	返 回 值: 无
********************************************************************************************************
*/
void I2C_SendNoAck(void)
{
	SDA_OUTPUT();
	SDA_H();											//数据线为高
	I2C_DelayUs(2);
	SCK_H();
	I2C_DelayUs(2);
	SCK_L();
}
/*
*********************************************************************************************************
*	函 数 名: I2C_Write
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参:  _ucByte ： 等待发送的字节 
*	返 回 值: 0 写成功  1 写入失败     
*********************************************************************************************************
*/
void I2C_WriteByte(uint8_t data)
{
	uint8_t i;
	
	SDA_OUTPUT();
	SCK_L();	
	for (i=0; i<8; i++)								//通过向左移位将数据写入24C02
	{
		if (data & 0x80)
			SDA_H();
		else
			SDA_L();
		data <<= 1;									//待写入数据左移一位
		I2C_DelayUs(2);
		SCK_H();
		I2C_DelayUs(2);
		SCK_L();
		I2C_DelayUs(2);
	}	
}
/*
*********************************************************************************************************
*	函 数 名: I2C_ReadByte
*	功能说明: CPU从I2C总线设备读8bit数据
*	形    参: 无
*	返 回 值: 读出的数据
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
*	函 数 名: EEP_WriteStr
*	功能说明: 向串行EEPROM指定地址写入若干数据
*	形    参:  addr : 起始地址
*			 len : 数据长度，单位为字节
*			 str : 存放写入数据的缓冲区指针
*	返 回 值:
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
*	函 数 名: EEP_ReadStr
*	功能说明: 读串行EEPROM指定地址写入若干数据
*	形    参:  addr : 起始地址
*			 	len : 数据长度，单位为字节
*			 	str : 存放读出数据的缓冲区指针
*	返 回 值:
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



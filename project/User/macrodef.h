/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 全局宏定义
*	文件名称 : macrodef.h
*	版    本 : V1.0
*	描    述               
*	修改记录 : 
*		版本号   日期        作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#ifndef _MACRODEF_H
#define _MACRODEF_H

/*
*	系统宏定义
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
*	用户宏定义
*/
/****************************Cortex3****************************/
#define DISABLE_INTERRUPT()		__disable_irq()
#define ENABLE_INTERRUPT()		__enable_irq()

/****************************时钟 ****************************/
#define SYSCLK  20000000						//系统时钟,
#define PCLK	20000000						//芯片内部外设模块时钟

/****************************WDT****************************/
#define WDT_CLEAR()		((WDT->LOAD.Word = 0xffff))

/****************************KEY ****************************/
#define KEY0 	(GPIO->DATA.DATA_2)
		
/*
	按键滤波时间100ms, 单位10ms。
	只有连续检测到100ms状态不变才认为有效，包括弹起和按下两种事件	
*/
#define KEY_DOWN_PERIOD     10		//单位10ms， 持续80ms，认为按键按下
#define KEY_LONG_PERIOD     200		//单位10ms， 持续2秒，认为长按事件
#define KEY_REPEAT_PERIOD   5       //单位10ms， 连续按键周期为50ms
/****************************LED ****************************/
#define LED_On()		(GPIO->DATABCR.DATABCR_9 = 1)
#define LED_Off()		(GPIO->DATABSR.DATABSR_9 = 1)
#define LED_Toggle()	(GPIO->DATABRR.DATABRR_9 = 1)

/****************************UART****************************/
#define UART_BAUD	9600						//波特率
#define UART_TX_ENABLE		UART0->INT.TXIE = 1	//发送中断使能
#define UART_TX_DISABLE		UART0->INT.TXIE = 0	//发送中断禁止
#define UART_RX_ENABLE		UART0->INT.RXIE = 1	//接收中断使能
#define UART_RX_DISABLE		UART0->INT.RXIE = 0	//接收中断禁止

/**********************EEPROM 定义**************************/
#define EE_ADJ_BASEADDR			0x00			//计量参数保存地址
#define EE_BACKUPS_OFFSET		0x20			//计量参数备份地址
#define EE_DI_ADDR			    0x40			//电能数据保存地址

#endif


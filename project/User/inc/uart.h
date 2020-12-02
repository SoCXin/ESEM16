/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 串口头文件
*	文件名称 : uart.h
*	版    本 : V1.0
*	描    述               
*	修改记录 : 
*		版本号   日期        作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#ifndef _UART_H
#define _UART_H

/*****定义通信超时常数*****/
#define  TX_TIMEOUT_MAX		50		//500ms，串口字符间最大延时(两次发送中断产生的最大间隔时间)
#define  RX_TIMEOUT_MAX		50  	//500ms  串口字符间最大延时(两次接收中断产生的最大间隔时间)
#define  RESPONSETIME_MAX	50 		//500ms，645数据处理的最大时间，从接完成到645数据处理完成的最大时间
#define  TX_START_DELAY_MIN  3		//30ms，645数据处理完成到开始发送数据的延时时间

/* */
enum DL645_STEP{
	ST_START = 0	,	//等待接收起始码
    ST_WAIT_H1		,	//帧起始符
    ST_WAIT_ADDR	,	//地址域
    ST_WAIT_H2		,	//帧起始符
    ST_WAIT_CTRL	,	//控制码
    ST_WAIT_LEN		,	//数据域长度
    ST_WAIT_DATA	,	//数据域
    ST_WAIT_CS		,	//校验码
    ST_WAIT_END		,	//结束符
};

/*
* 其他函数调用
*/
uint8_t Uart_Tx(void);
void Uart_Rx(uint8_t rcv_data);
void Uart_Proc(void);
#endif 


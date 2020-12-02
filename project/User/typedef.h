/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 全局类型定义
*	文件名称 : typedef.h
*	版    本 : V1.0
*	描    述               
*	修改记录 : 
*		版本号   日期        作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#ifndef _TYPEDEF_H
#define _TYPEDEF_H


//定义结构体
typedef union  //用于一个16位字与2个8位字节之间的转换
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
typedef struct      		//电表变量 BCD码
{
    B32_B08    U;      		//电压xxxxxxx.x
    B32_B08    I1;     		//电流xxxxx.xxx
    B32_B08    P1;     		//有功功率xxxx.xxxx
    B32_B08    Cosa;   		//功率因数xxxxx.xxx
    B16_B08    Freq;   		//电网频率xx.xx
} MeterVariable_TypeDef;

//6   通信地址/设备号(进厂内可写)
//32  资产管理编码(ASCII码)     property
//6   额定电压(ASCII码)    rated voltage
//6   额定电流/基本电流(ASCII码)
//6   最大电流(ASCII码)
//10  设备型号(ASCII码)(硬件版本)
//10  生产日期(ASCII码)
//16  协议版本号(ASCII码)(计量软件版本)
//16  无线软件版本

typedef struct      //设备系统全局变量
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
	uint8_t Flag;				/* 标志位 */
	uint8_t data_buf[220];		/* 发送缓存区 */
	uint8_t Len;				/* 接收数据长度 */
    uint8_t Cnt;				/* 发送数据计数 */
    uint8_t DataLen;            /* 接收数据域长度 */  
	uint8_t Step;				/* 接收步骤 */
	uint8_t Addr;				/* 地址类型:AA缩位地址，99广播地址，其他为正常地址  */
	uint8_t Overtimr_Timer;		/* 接收或发送超时定时器 */
	uint8_t TxDelay_Timer;		/* 发送延时定时器 */
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
	EE_UGAIN,                   //电压
	EE_IGAIN,                   //电流
	EE_PGAIN,
	EE_APHCAL,
    EE_PAOFF,
    EE_CHECK_SUM,				//校验和   
	EE_PARAM_SUM,				//参数总数	
	
};
typedef struct {
	B32_B08 energy;				//能量
	uint32_t pulse;				//脉冲
}EP_Data_TypeDef;


enum {
	TASK_FLAG_KEY,				//按键事件
	TASK_FLAG_SEC,				//每秒处理事件
	TASK_FLAG_MINUTE,			//每分钟处理事件
	TASK_FLAG_UART,				//uart事件
};
#endif


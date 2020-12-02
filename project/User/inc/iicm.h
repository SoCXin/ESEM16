/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : IIC头文件
*	文件名称 : iicm.h
*	版    本 : V1.0
*	描    述               
*	修改记录 : 
*		版本号   日期        作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#ifndef _IICM_H
#define _IICM_H

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* 
 * AT24C01/02每页8个字节
 * AT24C04/08A/16A每页16个字节
 */
	
#define AT24C02
//#define AT24C04

#ifdef AT24C02
	#define EE_MODEL_NAME       "AT24C02"
	#define EE_DEV_ADDR         0xA0            /* 设备地址 */
	#define EE_PAGE_SIZE        8               /* 页面大小(字节) */
	#define EE_SIZE             (8*32)          /* 总容量(字节) */
	#define EE_TYPE				255				/* 最大地址 = 总容量 - 1 */
#endif

#ifdef AT24C04
	#define EE_MODEL_NAME		"AT24C04"
	#define EE_DEV_ADDR			0xA0			/* 设备地址 */
	#define EE_PAGE_SIZE		16				/* 页面大小(字节) */
	#define EE_SIZE				(16*32)			/* 总容量(字节) */
	#define EE_TYPE				512				/* 最大地址 = 总容量 - 1 */
#endif

/*
* 其他函数调用
*/
uint8_t EEP_WriteStr(uint8_t addr, uint8_t *str, uint8_t len);
uint8_t EEP_ReadStr(uint8_t addr, uint8_t *str, uint8_t len);

#endif

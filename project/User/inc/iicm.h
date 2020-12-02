/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : IICͷ�ļ�
*	�ļ����� : iicm.h
*	��    �� : V1.0
*	��    ��               
*	�޸ļ�¼ : 
*		�汾��   ����        ����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
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
 * AT24C01/02ÿҳ8���ֽ�
 * AT24C04/08A/16Aÿҳ16���ֽ�
 */
	
#define AT24C02
//#define AT24C04

#ifdef AT24C02
	#define EE_MODEL_NAME       "AT24C02"
	#define EE_DEV_ADDR         0xA0            /* �豸��ַ */
	#define EE_PAGE_SIZE        8               /* ҳ���С(�ֽ�) */
	#define EE_SIZE             (8*32)          /* ������(�ֽ�) */
	#define EE_TYPE				255				/* ����ַ = ������ - 1 */
#endif

#ifdef AT24C04
	#define EE_MODEL_NAME		"AT24C04"
	#define EE_DEV_ADDR			0xA0			/* �豸��ַ */
	#define EE_PAGE_SIZE		16				/* ҳ���С(�ֽ�) */
	#define EE_SIZE				(16*32)			/* ������(�ֽ�) */
	#define EE_TYPE				512				/* ����ַ = ������ - 1 */
#endif

/*
* ������������
*/
uint8_t EEP_WriteStr(uint8_t addr, uint8_t *str, uint8_t len);
uint8_t EEP_ReadStr(uint8_t addr, uint8_t *str, uint8_t len);

#endif

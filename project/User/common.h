/*
*********************************************************************************************************
*
*	模块名称 : _COMMON_H
*	文件名称 : common.h
*	说    明 : 
*
*********************************************************************************************************
*/
#ifndef _COMMON_H
#define _COMMON_H

/*
* 其他函数调用
*/
void    Hex2Bcd( uint8_t *Hexopt, uint8_t *BCDopt, uint8_t Len );
void    Bcd2Hex( uint8_t *BCDopt, uint8_t *HEXopt, uint8_t Len );
void    Hex2Bcd_4(uint8_t *HEXopt, uint8_t *BCDopt);
void    Bcd2Hex_4(uint8_t *BCDopt, uint8_t *HEXopt);
uint8_t  HexToBcd( uint8_t value );
uint8_t  BcdToHex( uint8_t value );
uint8_t  CheckOddEven(uint8_t Data);
void 	DelayMs(uint16_t m);
uint16_t SumCheck(uint8_t* data, uint8_t num);

#endif

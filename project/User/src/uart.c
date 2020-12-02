/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 主程序模块。
*	文件名称 : uart.c
*	版    本 : V1.0       
*	描    述 ：
*	日    期 ：2016\12\29             
*	修改记录 :
*			作者      日期		说明
*			9527	  12/29		校表时uart地址固定为0xAA AA AA AA AA AA
*								波特率9600，8,n,1.偶校验
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#include "includes.h"

DL645_TypeDef 			g_dl645;
MeterVariable_TypeDef 	g_Emu_Data;
DeviceInfo_TypeDef 		g_DeviceInfo;

uint8_t g_Outbuf[220];		//待发送数据缓存
uint8_t g_Data_C;			//控制码
uint8_t g_Data_L;			//数据域长度
uint8_t g_K_Outbuf = 0;		//返回帧数据域长度
B32_B08 gu_Data_DI;			//帧数据标识

void UartProc(uint8_t rcv_data);
void Judge_645_C ( uint8_t  Data_C );
void Uart_Proc_11 ( void );
void Uart_Proc_1F ( void );

/*
*********************************************************************************************************
*	函 数 名: Uart_Rx
*	功能说明: Uart中断接收处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_Rx(uint8_t rcv_data)
{
	uint8_t result,cs,i;
	
	result = SUCCEED;
	g_dl645.Overtimr_Timer = RX_TIMEOUT_MAX;
	switch(g_dl645.Step)
	{
		case ST_START:				//接收前导符
			if(rcv_data == 0XFE)  	//如果接收到前导符,准备接受第一个前导符
			{
				g_dl645.DataLen = 0;
				g_dl645.Len = 0;
				g_dl645.Step = ST_WAIT_H1;
			}
			else                 	//等待接收状态第一个收到的不是前导符，仍然维持等待接受状态
			{				
				result = FAILED;								
			}
			break;			
		case ST_WAIT_H1:			//接收第一个68
			if (rcv_data == 0XFE)	//如果接收到前导符保持状态不变
			{
			
			}
			else if (rcv_data == 0X68) //接收第一个68
			{
				g_dl645.data_buf[0] = 0x68;
				g_dl645.DataLen = 0;
				g_dl645.Len = 0;
				g_dl645.Step = ST_WAIT_ADDR;				
			}
			else
			{
				result = FAILED;
			}
			break;
		case ST_WAIT_ADDR:				//接收地址	
			g_dl645.Len++;		
			g_dl645.data_buf[g_dl645.Len] = rcv_data;
			if (g_dl645.Len >= 6)
			{
				for (i=0; i<6; i++)     //查看地址是否为0xAA
				{
//                    if ( ( g_dl645.data_buf[1 + i] != g_DeviceInfo.Dev_ID[i] )
//                            && ( g_dl645.data_buf[1 + i] != 0XAA ) )
					if (g_dl645.data_buf[1 + i] != 0XAA )
						break;
				}
				if ( i != 6)
                {
                    result = FAILED;
                }
                else
                {
                    g_dl645.Step = ST_WAIT_H2;
                }				
			}
			break;
		case ST_WAIT_H2:				//接收第二个68
            g_dl645.Len++;
            if (rcv_data == 0x68)
            {
                g_dl645.data_buf[g_dl645.Len] = 0x68;
                g_dl645.Step = ST_WAIT_CTRL;
                g_dl645.DataLen = 0;
            }
            else
            {
                result = FAILED;
            }
			break;
		case ST_WAIT_CTRL:              //控制码
            g_dl645.Len++;
            g_dl645.data_buf[g_dl645.Len] = rcv_data;
            g_dl645.Step = ST_WAIT_LEN;
            g_dl645.DataLen = 0;            
			break;
		case ST_WAIT_LEN:               //接收数据长度
            g_dl645.Len++;
            g_dl645.data_buf[g_dl645.Len] = rcv_data;
            if (rcv_data == 0)
            {
                g_dl645.Step = ST_WAIT_CS;
                g_dl645.DataLen = 0;     
            }
            else if (rcv_data <= 200)
            {
                g_dl645.Step = ST_WAIT_DATA;
                g_dl645.DataLen = 0;             
            }
            else
            {
                result = FAILED;
            }            
			break;
		case ST_WAIT_DATA:              //接收数据域
            g_dl645.Len++;
            g_dl645.data_buf[g_dl645.Len] = rcv_data;
            g_dl645.DataLen++;   
            if (g_dl645.DataLen >= g_dl645.data_buf[9])
            {
                g_dl645.Step = ST_WAIT_CS;
            }            
			break;
		case ST_WAIT_CS:
            cs = 0;
            for (i=0; i<= g_dl645.Len; i++)
            {
                cs = cs + g_dl645.data_buf[i];
            }
            if (cs == rcv_data)
            {
 				g_dl645.Len++;
				g_dl645.data_buf[g_dl645.Len] = rcv_data;
				g_dl645.Step = ST_WAIT_END;               
            }
            else
            {
                result = FAILED;
            }
			break;
		case ST_WAIT_END:
            if (rcv_data == 0x16)
            {
                g_dl645.Len++;
                g_dl645.data_buf[g_dl645.Len] = 0x16;
                g_dl645.Step = ST_START;  
                g_dl645.Overtimr_Timer = RESPONSETIME_MAX;
				FlagSet(g_Tasks_Flag, TASK_FLAG_UART);			//产生事件标志，给主函数处理
                UART_TX_DISABLE;								//发送中断禁止
				UART_RX_DISABLE;								//接收中断禁止
            }
            else
            {
                result = FAILED;
            }           
			break;
		default:
            result = FAILED;
			break;		
	}
    if (result != SUCCEED)
    {
        g_dl645.Overtimr_Timer = 1;     		//重新进入等待接收状态
        g_dl645.Step = ST_START;  
    }
}
/*
*********************************************************************************************************
*	函 数 名: Uart_Proc
*	功能说明: Uart处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_Proc(void)
{
	uint8_t i,buf08;
	
	g_Data_C = g_dl645.data_buf[8];
	g_Data_L = g_dl645.data_buf[9];
	
	//余3码处理
	for (i=0; i<g_Data_L; i++)
	{
        g_dl645.data_buf[10 + i] -= 0x33;    
	}
	switch (g_Data_C)
	{
		case 0x11:		//读数据
		case 0x1F:		//扩展命令，校表
			gu_Data_DI.B08[0] = g_dl645.data_buf[10];			
			gu_Data_DI.B08[1] = g_dl645.data_buf[11];	
			gu_Data_DI.B08[2] = g_dl645.data_buf[12];	
			gu_Data_DI.B08[3] = g_dl645.data_buf[13];	
			break;
		default:
			break;
	}
	Judge_645_C(g_Data_C);
	g_Data_C |= 0x80 ;
	/*******发送数据处理*********/
	g_dl645.data_buf[0] = 0xFE;
	g_dl645.data_buf[1] = 0xFE;
	g_dl645.data_buf[2] = 0xFE;
	g_dl645.data_buf[3] = 0x68;
// 	memcpy( (g_dl645.data_buf+4),g_DeviceInfo.Dev_ID , 6); //copy
	g_dl645.data_buf[4] = 0;		//地址未用
	g_dl645.data_buf[5] = 0;
	g_dl645.data_buf[6] = 0;
	g_dl645.data_buf[7] = 0;
	g_dl645.data_buf[8] = 0;
	g_dl645.data_buf[9] = 0;
	g_dl645.data_buf[10] = 0x68;
	g_dl645.data_buf[11] = g_Data_C;
	switch (g_Data_C)
	{
		case 0x91:
			g_dl645.data_buf[12] = g_K_Outbuf + 4;			//增加4位数据标识
			memcpy ((g_dl645.data_buf+13),gu_Data_DI.B08, 4);
			memcpy ((g_dl645.data_buf+17),g_Outbuf, g_K_Outbuf);
			break;
		case 0x9F:
			g_dl645.data_buf[12] = g_K_Outbuf;
			memcpy ((g_dl645.data_buf+3),g_Outbuf, g_K_Outbuf);
			break;
		default:
			break;
	}	
    //余3码处理
    for (i=0; i<g_dl645.data_buf[12]; i++)
    {
        g_dl645.data_buf[13+i] += 0x33;
    }
    //校验和
   	buf08 = 0;
	for (i=3; i<13+g_dl645.data_buf[12]; i++)
	{
		buf08 = buf08 + g_dl645.data_buf[i];
	}
	g_dl645.data_buf[13+g_dl645.data_buf[12]] = buf08;		//cs
	g_dl645.data_buf[14+g_dl645.data_buf[12]] = 0x16;
	g_dl645.Len = 15+g_dl645.data_buf[12];					//发送数据长度

	g_dl645.TxDelay_Timer = TX_START_DELAY_MIN;				//30ms后发送第一个前导码
	g_dl645.Overtimr_Timer = TX_START_DELAY_MIN*2;			//如果长时间没有进入发送状态，则强制进入接收态
	
}
/*
*********************************************************************************************************
*	函 数 名: Judge_645_C
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Judge_645_C ( uint8_t  Data_C )
{
    switch ( Data_C ) //判断控制码
    {
        case 0X11:    //读数据
            Uart_Proc_11();
            break;
            /************************************************/
        case 0X1F:    //校表
            Uart_Proc_1F(); 
            break;
            /************************************************/
        default:      //无请求数据
            break;
            /************************************************/
    }
}
/*
*********************************************************************************************************
*	函 数 名: Uart_Proc_11
*	功能说明: 645-11命令， 读数据处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Uart_Proc_11 ( void )
{
	B32_B08 temp;
	
	switch (gu_Data_DI.B32)
	{
        case 0x02010100 :  // 读取A相电压值
            g_Outbuf[0] = g_Emu_Data.U.B08[0];
            g_Outbuf[1] = g_Emu_Data.U.B08[1];
            g_K_Outbuf = 2 ;
            break ;
        case 0x02020100 :  // 读取A相电流值
            g_Outbuf[0] = g_Emu_Data.I1.B08[0];
            g_Outbuf[1] = g_Emu_Data.I1.B08[1];
            g_Outbuf[2] = g_Emu_Data.I1.B08[2];
            g_K_Outbuf = 3 ;
            break ;
        case 0x02030000 :  //瞬时总有功功率
            temp.B32 = g_Emu_Data.P1.B32;
            temp.B32 = temp.B32 >> 12;//转换数据格式
            g_Outbuf[0] = temp.B08[0] ;
            g_Outbuf[1] = temp.B08[1] ;
            g_Outbuf[2] = temp.B08[2] ;
            g_K_Outbuf = 3 ;
            break ;
        case 0x02060000 :  //总功率因数
            g_Outbuf[0] = g_Emu_Data.Cosa.B08[0];
            g_Outbuf[1] = g_Emu_Data.Cosa.B08[1];
            g_K_Outbuf = 2 ;
            break ;
        case 0x02800001 :  //N线电流
            g_Outbuf[0] = 0 ; //g_Emu_Data.I2.B08[0];
            g_Outbuf[1] = 0 ; //g_Emu_Data.I2.B08[1];
            g_Outbuf[2] = 0 ; //g_Emu_Data.I2.B08[2];
            g_K_Outbuf = 3 ;
            break ;
        case 0x02800002 :  //电网频率
            g_Outbuf[0] = g_Emu_Data.Freq.B08[0];
            g_Outbuf[1] = g_Emu_Data.Freq.B08[1];
            g_K_Outbuf = 2 ;
            break ;
        default:
            g_K_Outbuf = 1 ;
            g_Outbuf[0] = 2 ; // 错误信息字
            break ;
	}
}
/*
*********************************************************************************************************
*	函 数 名: Uart_Proc_1F
*	功能说明: 645-1F命令， 校表处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void  Uart_Proc_1F ( void )
{
    B32_B08 data;
    data.B08[0] = g_dl645.data_buf[14] ;
    data.B08[1] = g_dl645.data_buf[15] ;
    data.B08[2] = g_dl645.data_buf[16] ;
    data.B08[3] = g_dl645.data_buf[17] ;
    switch ( gu_Data_DI.B32 )
    {
        case 0x01000000 :  			//清除校准寄存器命令,初始化
            if ( data.B32 == 0 )
            {
				memcpy(g_EM_GainPara.U_Gain.B08, ADJ_DEFAULT, EE_PARAM_SUM);
				EEP_WriteStr(EE_ADJ_BASEADDR, (uint8_t *)ADJ_DEFAULT[0].B08 ,EE_PARAM_SUM*2);	
				EEP_WriteStr(EE_ADJ_BASEADDR+EE_BACKUPS_OFFSET, (uint8_t *)ADJ_DEFAULT[0].B08 ,EE_PARAM_SUM*2);	  
				Emu_Adjust_Check();
				g_K_Outbuf = 0 ;
            }
            else
            {
                g_Outbuf[0] = 5 ;
                g_K_Outbuf = 1 ;
            }
            break ;
        case 0x01000001 :           //校UGAIN
			UGAIN_Adjust( &data.B08[0] ) ;	
			g_K_Outbuf = 0 ;
            break ;
        case 0x01000002 :           //校IAGAIN
			IAGAIN_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000003 :   		//校PAGAIN
			PAGAIN_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000004 :  			//校APHCAL
			APHCAL_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000005 :  			//校PAOFF
			PAOFF_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000006 :  			//ESEM16没有B线电流校准         
			g_Outbuf[0] = 5 ;	
			g_K_Outbuf = 1 ;
            break ;
        case 0x010000FF :  			//清除电量
            if ( data.B32 == 0 )
            {
                // add clear power		
				Energy_Reset();				
                g_K_Outbuf = 0 ;				
            }
            else
            {
                g_Outbuf[0] = 5 ;
                g_K_Outbuf = 1 ;
            }
            break ;
        default:
            g_K_Outbuf = 1 ;
            g_Outbuf[0] = 6 ;
            break ;
    }
}
/*
*********************************************************************************************************
*	函 数 名: Uart_Tx
*	功能说明: uart 发送处理
*	形    参：无
*	返 回 值: 0:发送完成 1：发送未完成
*********************************************************************************************************
*/
uint8_t Uart_Tx(void)
{
	uint8_t senddata = 0 ;
	
	if (g_dl645.Cnt >= g_dl645.Len)
	{
		return 0;
	}
	else
	{
		g_dl645.Overtimr_Timer = TX_TIMEOUT_MAX;
		senddata = g_dl645.data_buf[g_dl645.Cnt++];
		UART0->TXBUF0.TXBUF0 = senddata;
		return 1;
	}
}

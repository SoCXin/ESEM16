/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : ������ģ�顣
*	�ļ����� : uart.c
*	��    �� : V1.0       
*	��    �� ��
*	��    �� ��2016\12\29             
*	�޸ļ�¼ :
*			����      ����		˵��
*			9527	  12/29		У��ʱuart��ַ�̶�Ϊ0xAA AA AA AA AA AA
*								������9600��8,n,1.żУ��
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************************/
#include "includes.h"

DL645_TypeDef 			g_dl645;
MeterVariable_TypeDef 	g_Emu_Data;
DeviceInfo_TypeDef 		g_DeviceInfo;

uint8_t g_Outbuf[220];		//���������ݻ���
uint8_t g_Data_C;			//������
uint8_t g_Data_L;			//�����򳤶�
uint8_t g_K_Outbuf = 0;		//����֡�����򳤶�
B32_B08 gu_Data_DI;			//֡���ݱ�ʶ

void UartProc(uint8_t rcv_data);
void Judge_645_C ( uint8_t  Data_C );
void Uart_Proc_11 ( void );
void Uart_Proc_1F ( void );

/*
*********************************************************************************************************
*	�� �� ��: Uart_Rx
*	����˵��: Uart�жϽ��մ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_Rx(uint8_t rcv_data)
{
	uint8_t result,cs,i;
	
	result = SUCCEED;
	g_dl645.Overtimr_Timer = RX_TIMEOUT_MAX;
	switch(g_dl645.Step)
	{
		case ST_START:				//����ǰ����
			if(rcv_data == 0XFE)  	//������յ�ǰ����,׼�����ܵ�һ��ǰ����
			{
				g_dl645.DataLen = 0;
				g_dl645.Len = 0;
				g_dl645.Step = ST_WAIT_H1;
			}
			else                 	//�ȴ�����״̬��һ���յ��Ĳ���ǰ��������Ȼά�ֵȴ�����״̬
			{				
				result = FAILED;								
			}
			break;			
		case ST_WAIT_H1:			//���յ�һ��68
			if (rcv_data == 0XFE)	//������յ�ǰ��������״̬����
			{
			
			}
			else if (rcv_data == 0X68) //���յ�һ��68
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
		case ST_WAIT_ADDR:				//���յ�ַ	
			g_dl645.Len++;		
			g_dl645.data_buf[g_dl645.Len] = rcv_data;
			if (g_dl645.Len >= 6)
			{
				for (i=0; i<6; i++)     //�鿴��ַ�Ƿ�Ϊ0xAA
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
		case ST_WAIT_H2:				//���յڶ���68
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
		case ST_WAIT_CTRL:              //������
            g_dl645.Len++;
            g_dl645.data_buf[g_dl645.Len] = rcv_data;
            g_dl645.Step = ST_WAIT_LEN;
            g_dl645.DataLen = 0;            
			break;
		case ST_WAIT_LEN:               //�������ݳ���
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
		case ST_WAIT_DATA:              //����������
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
				FlagSet(g_Tasks_Flag, TASK_FLAG_UART);			//�����¼���־��������������
                UART_TX_DISABLE;								//�����жϽ�ֹ
				UART_RX_DISABLE;								//�����жϽ�ֹ
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
        g_dl645.Overtimr_Timer = 1;     		//���½���ȴ�����״̬
        g_dl645.Step = ST_START;  
    }
}
/*
*********************************************************************************************************
*	�� �� ��: Uart_Proc
*	����˵��: Uart����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_Proc(void)
{
	uint8_t i,buf08;
	
	g_Data_C = g_dl645.data_buf[8];
	g_Data_L = g_dl645.data_buf[9];
	
	//��3�봦��
	for (i=0; i<g_Data_L; i++)
	{
        g_dl645.data_buf[10 + i] -= 0x33;    
	}
	switch (g_Data_C)
	{
		case 0x11:		//������
		case 0x1F:		//��չ���У��
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
	/*******�������ݴ���*********/
	g_dl645.data_buf[0] = 0xFE;
	g_dl645.data_buf[1] = 0xFE;
	g_dl645.data_buf[2] = 0xFE;
	g_dl645.data_buf[3] = 0x68;
// 	memcpy( (g_dl645.data_buf+4),g_DeviceInfo.Dev_ID , 6); //copy
	g_dl645.data_buf[4] = 0;		//��ַδ��
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
			g_dl645.data_buf[12] = g_K_Outbuf + 4;			//����4λ���ݱ�ʶ
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
    //��3�봦��
    for (i=0; i<g_dl645.data_buf[12]; i++)
    {
        g_dl645.data_buf[13+i] += 0x33;
    }
    //У���
   	buf08 = 0;
	for (i=3; i<13+g_dl645.data_buf[12]; i++)
	{
		buf08 = buf08 + g_dl645.data_buf[i];
	}
	g_dl645.data_buf[13+g_dl645.data_buf[12]] = buf08;		//cs
	g_dl645.data_buf[14+g_dl645.data_buf[12]] = 0x16;
	g_dl645.Len = 15+g_dl645.data_buf[12];					//�������ݳ���

	g_dl645.TxDelay_Timer = TX_START_DELAY_MIN;				//30ms���͵�һ��ǰ����
	g_dl645.Overtimr_Timer = TX_START_DELAY_MIN*2;			//�����ʱ��û�н��뷢��״̬����ǿ�ƽ������̬
	
}
/*
*********************************************************************************************************
*	�� �� ��: Judge_645_C
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Judge_645_C ( uint8_t  Data_C )
{
    switch ( Data_C ) //�жϿ�����
    {
        case 0X11:    //������
            Uart_Proc_11();
            break;
            /************************************************/
        case 0X1F:    //У��
            Uart_Proc_1F(); 
            break;
            /************************************************/
        default:      //����������
            break;
            /************************************************/
    }
}
/*
*********************************************************************************************************
*	�� �� ��: Uart_Proc_11
*	����˵��: 645-11��� �����ݴ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Uart_Proc_11 ( void )
{
	B32_B08 temp;
	
	switch (gu_Data_DI.B32)
	{
        case 0x02010100 :  // ��ȡA���ѹֵ
            g_Outbuf[0] = g_Emu_Data.U.B08[0];
            g_Outbuf[1] = g_Emu_Data.U.B08[1];
            g_K_Outbuf = 2 ;
            break ;
        case 0x02020100 :  // ��ȡA�����ֵ
            g_Outbuf[0] = g_Emu_Data.I1.B08[0];
            g_Outbuf[1] = g_Emu_Data.I1.B08[1];
            g_Outbuf[2] = g_Emu_Data.I1.B08[2];
            g_K_Outbuf = 3 ;
            break ;
        case 0x02030000 :  //˲ʱ���й�����
            temp.B32 = g_Emu_Data.P1.B32;
            temp.B32 = temp.B32 >> 12;//ת�����ݸ�ʽ
            g_Outbuf[0] = temp.B08[0] ;
            g_Outbuf[1] = temp.B08[1] ;
            g_Outbuf[2] = temp.B08[2] ;
            g_K_Outbuf = 3 ;
            break ;
        case 0x02060000 :  //�ܹ�������
            g_Outbuf[0] = g_Emu_Data.Cosa.B08[0];
            g_Outbuf[1] = g_Emu_Data.Cosa.B08[1];
            g_K_Outbuf = 2 ;
            break ;
        case 0x02800001 :  //N�ߵ���
            g_Outbuf[0] = 0 ; //g_Emu_Data.I2.B08[0];
            g_Outbuf[1] = 0 ; //g_Emu_Data.I2.B08[1];
            g_Outbuf[2] = 0 ; //g_Emu_Data.I2.B08[2];
            g_K_Outbuf = 3 ;
            break ;
        case 0x02800002 :  //����Ƶ��
            g_Outbuf[0] = g_Emu_Data.Freq.B08[0];
            g_Outbuf[1] = g_Emu_Data.Freq.B08[1];
            g_K_Outbuf = 2 ;
            break ;
        default:
            g_K_Outbuf = 1 ;
            g_Outbuf[0] = 2 ; // ������Ϣ��
            break ;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: Uart_Proc_1F
*	����˵��: 645-1F��� У����
*	��    �Σ���
*	�� �� ֵ: ��
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
        case 0x01000000 :  			//���У׼�Ĵ�������,��ʼ��
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
        case 0x01000001 :           //УUGAIN
			UGAIN_Adjust( &data.B08[0] ) ;	
			g_K_Outbuf = 0 ;
            break ;
        case 0x01000002 :           //УIAGAIN
			IAGAIN_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000003 :   		//УPAGAIN
			PAGAIN_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000004 :  			//УAPHCAL
			APHCAL_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000005 :  			//УPAOFF
			PAOFF_Adjust ( &data.B08[0] ) ;
			g_K_Outbuf = 0;
            break ;
        case 0x01000006 :  			//ESEM16û��B�ߵ���У׼         
			g_Outbuf[0] = 5 ;	
			g_K_Outbuf = 1 ;
            break ;
        case 0x010000FF :  			//�������
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
*	�� �� ��: Uart_Tx
*	����˵��: uart ���ʹ���
*	��    �Σ���
*	�� �� ֵ: 0:������� 1������δ���
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

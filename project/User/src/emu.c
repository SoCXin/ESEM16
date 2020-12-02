/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	ģ������ : ����ģ�顣
*	�ļ����� : emu.c
*	��    �� : V1.0
*	��    �� ��
*	��    �� ��2016\12\29             
*	�޸ļ�¼ :
*			����      ˵��
*
* ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************************************************************/
#include "includes.h"
 
EM_GainPara_TypeDef g_EM_GainPara;
EP_Data_TypeDef g_EP_Data ;

void Emu_Adjust_Save(void);
/*
*********************************************************************************************************
*	�� �� ��: Set_GAINPara
*	����˵��: ����У׼����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Set_GAINPara(EM_GainPara_TypeDef *para)
{
	EM->UGAIN.Word = para->U_Gain.B16;
	EM->IAGAIN.Word = para->I_Gain.B16;
	EM->PAGAIN.Word = para->P_Gain.B16;
	EM->APHCAL.Word = para->Phase_Gain.B16;
	EM->PAOFF.Word  = para->Paoff_Gain.B16;
}
/*
*********************************************************************************************************
*	�� �� ��: Set_SatrtGate
*	����˵��: �����������޹���
*	��    ��: д��Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Set_SatrtGate(double value)
{		
	EM->PSTART.Word = (uint16_t)value;
}
/*
*********************************************************************************************************
*	�� �� ��: Set_HighFreqPulse
*	����˵��: ���ø�Ƶ���峣���Ĵ��� �����ݵ���������Ƶ���峣��ֵ��
*	��    ��: д��Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Set_HighFreqPulse(double value)
{
	EM->PFSET.Word = (uint32_t)value;
}
/*
*********************************************************************************************************
*	�� �� ��: EM_Config
*	����˵��: EM��ʼ������
*	��    ��: д��Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void EM_Config(void)
{
	Set_SatrtGate(PSTART_DATA);
	Set_HighFreqPulse(PFSET_DATA);
	Set_GAINPara(&g_EM_GainPara);
}

/*
*********************************************************************************************************
*	�� �� ��: Get_URMS
*	����˵��: �õ�������ѹֵ
*	��    ��: ��
*	�� �� ֵ: ʵ�ʵ�ѹֵ(����10��)
*********************************************************************************************************
*/
uint32_t Get_URMS(void)
{
	uint32_t urms;

	urms = EM->URMS.Word;
	urms = urms *_U_C;
	return urms;
}
/*
*********************************************************************************************************
*	�� �� ��: Get_IRMS
*	����˵��: �õ�ʵ�ʵ���ֵ
*	��    ��: ��
*	�� �� ֵ: ʵ�ʵ���ֵ(����1000��)
*********************************************************************************************************
*/
uint32_t Get_IRMS(void)
{
	uint32_t irms;

	irms = EM->IARMS.Word;
	irms = irms * _I_C;
	return irms;
}
/*
*********************************************************************************************************
*	�� �� ��: Get_FRQ
*	����˵��: �õ�ʵ��Ƶ��
*	��    ��: ��
*	�� �� ֵ: ʵ��Ƶ��
*********************************************************************************************************
*/
uint16_t Get_FRQ(void)
{
	uint16_t hz;

	hz = EM->FRQ.Word;
	hz = hz * 100 / 256;
	return (uint16_t)hz;
}
/*
*********************************************************************************************************
*	�� �� ��: Get_PHASE
*	����˵��: �õ�ʵ�����
*	��    ��: ��
*	�� �� ֵ: ʵ�����
*********************************************************************************************************
*/
uint16_t Get_PHASE(void)
{
	uint16_t ph;
	
	ph = EM->APHASE.Word;
	ph = ph / (1<<7);
	
	return ph;
}
/*
*********************************************************************************************************
*	�� �� ��: Get_PRMS
*	����˵��: �õ�ʵ�ʹ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ,��λ��(0.0001w)
*********************************************************************************************************
*/
uint32_t Get_PRMS(void)
{
	uint32_t prms;

	prms = EM->PA.Word;			//���з���32λ��
	if (prms & 0x80000000)	    //����
	{
		prms = ~prms + 1;
	}
	prms = prms * _P_C ;
	return prms;
}
/*
*********************************************************************************************************
*	�� �� ��: Get_EnergyPulse
*	����˵��: ��ȡ���ܼ������塣��ȡ��Ĵ����Զ����㣬�Ĵ�������Ϊ0.1���������塣ÿ����á�
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint32_t Get_EnergyPulse(void)
{
	return (EM->AE.Word);
}
/*
*********************************************************************************************************
*	�� �� ��: Get_Factor
*	����˵��: �õ���������
*	��    ��: ��
*	�� �� ֵ: ��������* 1000
*********************************************************************************************************
*/
uint16_t Get_Factor(void)
{
	uint16_t frms;
	
	frms = EM->AFAC.Word;
	if (frms & 0x8000)
	{
		frms = ~frms + 1;
	}
	frms = (frms*1000)/32768;	
	return frms;
}
/*
*********************************************************************************************************
*	�� �� ��: GetErrData
*	����˵��: ����оƬ���� (ʵ�ʲ�������-̨����ʵ����)/(̨����ʵ����)	
*	��    ��: ��
*	�� �� ֵ: 
*********************************************************************************************************
*/		
double GetErrData(uint8_t *uBuf)
{
	B32_B08  temp32_p;  					//̨�幦��
	uint32_t power;
	double   err_data;   					//�������
	
	Bcd2Hex_4(uBuf, temp32_p.B08);  		//��ǰ����ת����HEX	
	power = Get_PRMS();
	err_data = (double)power / temp32_p.B32 - 1;	//������ (ʵ�ʲ�������-̨����ʵ����)/(̨����ʵ����)	
	return err_data;
}
/*
*********************************************************************************************************
*	�� �� ��: UGAIN_Adjust
*	����˵��: ��ѹУ׼
*	��    ��: ̨��ʵ�ʵ�ѹֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/    
void UGAIN_Adjust(uint8_t *uBuf)
{
	uint16_t u_gain;
	uint32_t v_data;
	double err_data;
	B32_B08 temp32_Un;                  				//̨���ѹ
    
    Bcd2Hex_4(uBuf, temp32_Un.B08);     
    //У׼ǰ������Ĵ���
    EM->UGAIN.Word = 0;    	
	DelayMs(1000);										//Ҫ���Ӽ��ʱ�䡣
	v_data = EM->URMS.Word;                  			//����ѹ��Чֵ�Ĵ���

	err_data = temp32_Un.B32 / _U_C / v_data - 1; 		//��� = (Ų�� - U����)/U����
	if (err_data >= 0)									//ת���ɲ�����ʽ ��������ֲ�/Ӧ�ñʼ�						
	{	
		u_gain = (uint16_t)(err_data * 32768);
	}
	else
	{
		u_gain = (uint16_t)(err_data * 32768 + 65536);
	}
	//����У����
	EM->UGAIN.Word = u_gain;         
    g_EM_GainPara.U_Gain.B16 = u_gain;
	Emu_Adjust_Save();      								//EEPROM����
}
/*
*********************************************************************************************************
*	�� �� ��: IAGAIN_Adjust
*	����˵��: ����У׼
*	��    ��: ̨��ʵ�ʵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IAGAIN_Adjust(uint8_t *uBuf)
{
	uint16_t i_gain;
	uint32_t i_data;
	double err_data;
	B32_B08    temp32_Ia;  								//̨�����
	
	Bcd2Hex_4(uBuf, temp32_Ia.B08); 
	//У׼ǰ������Ĵ���
    EM->IAGAIN.Word = 0;    
	
	DelayMs(1000);
	i_data = EM->IARMS.Word;							//��������Чֵ�Ĵ���
	err_data = temp32_Ia.B32 / _I_C / i_data - 1;		//��� = (Į��-I����)/I����
	if (err_data >= 0)
	{
		i_gain = (uint16_t)(err_data * 32768);
	}
	else
	{
		i_gain = (uint16_t)(err_data * 32768 + 65536);
	}
	//����У����
	EM->IAGAIN.Word = i_gain;
    g_EM_GainPara.I_Gain.B16 = i_gain;
	Emu_Adjust_Save();
}
/*
*********************************************************************************************************
*	�� �� ��: PAGAIN_Adjust
*	����˵��: �й�����У׼
*	��    ��: ̨��ʵ�ʹ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PAGAIN_Adjust(uint8_t *uBuf)
{
	double err_data;
	uint32_t gain;	

	//У׼ǰ������Ĵ���
	EM->PAGAIN.Word = 0;  	

	err_data = GetErrData(uBuf);				//�������
	err_data = -err_data / (1 + err_data);		//������ΪG-1 = (1/(1+err_data))-1
	//��(G-1)����Ϊ*2^15�Ĳ�����ʽ  ��������ֲ�/Ӧ�ñʼ�
	if (err_data >= 0)
		gain = (uint16_t)(err_data*32768);
	else
		gain = (uint16_t)(err_data*32768 + 65536);
	//����У����
	EM->PAGAIN.Word = gain;
    g_EM_GainPara.P_Gain.B16 = gain;
	Emu_Adjust_Save();  	
}
/*
*********************************************************************************************************
*	�� �� ��: APHCAL_Adjust
*	����˵��: �ǲ�У׼
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void APHCAL_Adjust(uint8_t *uBuf)
{
	uint8_t aphcal;
	double   err_data;

	//У׼ǰ������Ĵ���
    EM->APHCAL.Word = 0;    
	DelayMs(1000);
	
	err_data = GetErrData(uBuf);					//�������
	err_data = err_data * APHCAL_C /50;				//��������Ϊ3675.52�Ĳ�����ʽ
	if (err_data >= 0)
		aphcal = (uint8_t)err_data;
	else
		aphcal = (uint8_t)(256 + err_data);
	//����У����	
	EM->APHCAL.Word = aphcal; 
	g_EM_GainPara.Phase_Gain.B16 = aphcal;
	Emu_Adjust_Save();
}
/*
*********************************************************************************************************
*	�� �� ��: PAOFF_Adjust
*	����˵��: ����ʧ��У׼
*	��    ��: ̨�幦��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void  PAOFF_Adjust(uint8_t *uBuf)
{
	double err_data;
	uint16_t gain;
	B32_B08 temp32_p;  
	B32_B08 temp32;
    
    Bcd2Hex_4(uBuf, temp32_p.B08);   				//̨�幦��
	//У׼ǰ������Ĵ���
    EM->PAOFF.Word = 0;    

	err_data = GetErrData(uBuf);					//�������
	if (err_data >= 0)
	{
		temp32.B32 = (double)temp32_p.B32 * err_data / 32;  
        temp32.B32 = (~temp32.B32) + 1;
	}
	else
	{
		temp32.B32 = (double)temp32_p.B32 * (-err_data) / 16;
	}
	gain = (uint16_t)temp32.B32;
	//����У����
	EM->PAOFF.Word = gain;
    g_EM_GainPara.Paoff_Gain.B16 = gain;
	Emu_Adjust_Save(); 	
}
/*
*********************************************************************************************************
*	�� �� ��: Energy_Save()
*	����˵��: �������������ݱ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Energy_Save(void)
{	
	EEP_WriteStr(EE_DI_ADDR, (uint8_t*)g_EP_Data.energy.B08 ,sizeof(g_EP_Data));	
}
/*
*********************************************************************************************************
*	�� �� ��: Energy_Reset()
*	����˵��: �����������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Energy_Reset(void)
{
	g_EP_Data.energy.B32 = 0;
	g_EP_Data.pulse = 0;
	EEP_WriteStr(EE_DI_ADDR, (uint8_t*)g_EP_Data.energy.B08 ,sizeof(g_EP_Data));	
}
/*
*********************************************************************************************************
*	�� �� ��: Energy_Add()
*	����˵��: �����ۼӣ���С����0.01KWh.��1s�������е��á�
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#define MINOR_PULSE_CNT		(MC/10)				//��С����0.01KWh
void Energy_Add(uint32_t pulse_cnt)
{
	g_EP_Data.pulse += pulse_cnt;
	if (g_EP_Data.pulse < MINOR_PULSE_CNT)
		return;
	g_EP_Data.pulse -= MINOR_PULSE_CNT;
	g_EP_Data.energy.B32 ++;
	if (g_EP_Data.energy.B32 > 99999999)		//���ܴ���999999.99
	{
		Energy_Reset();		
	}	
	else
	{
		Energy_Save();
	}

}
/*
*********************************************************************************************************
*	�� �� ��: Emu_Adjust_Save()
*	����˵��: ��У�����ݱ��浽EEPROM
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Emu_Adjust_Save(void)
{
	g_EM_GainPara.Check_Sum.B16 = SumCheck(g_EM_GainPara.U_Gain.B08, EE_CHECK_SUM*2);
	EEP_WriteStr(EE_ADJ_BASEADDR, g_EM_GainPara.U_Gain.B08 ,EE_PARAM_SUM*2);	
	EEP_WriteStr(EE_ADJ_BASEADDR+EE_BACKUPS_OFFSET, g_EM_GainPara.U_Gain.B08 ,EE_PARAM_SUM*2);	   
}
/*
*********************************************************************************************************
*	�� �� ��: ReadUIP()
*	����˵��: ������оƬ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Read_UIP(void)
{
    B32_B08 temp32;
    B16_B08 temp16;
	
    /***********************Ƶ��**************************/
	temp16.B16 = Get_FRQ();
    Hex2Bcd ( temp16.B08, g_Emu_Data.Freq.B08, 2 );	
	/******************* ��ѹ��Чֵ	*********************/
	temp32.B32 = Get_URMS();
    Hex2Bcd ( temp32.B08, g_Emu_Data.U.B08, 4 );
   
    if(EM->ESR.NO_LOAD == 1)     //����Ǳ�������������ʣ�����������ʾΪ0
    {
        g_Emu_Data.I1.B32 = 0;
        g_Emu_Data.P1.B32 = 0;
        g_Emu_Data.Cosa.B32 = 0;
    }
    else
    {
        /******************* ����  ********************/
        temp32.B32 = Get_IRMS();	
        Hex2Bcd ( temp32.B08, g_Emu_Data.I1.B08, 4 ); 			//3λС��
        /**********************����********************/
        temp32.B32 = Get_PRMS();	
        Hex2Bcd ( temp32.B08, g_Emu_Data.P1.B08, 4 );			//4λС��
        /******************* ��������******************/
        temp32.B32 = Get_Factor();
        Hex2Bcd ( temp32.B08, g_Emu_Data.Cosa.B08, 4 );	
    }
}
/*
********************************************************************************************************
*	�� �� ��: Emu_Adjust_Check
*	����˵��: ��֤����У׼�Ĵ�����ֵ�Ƿ���У׼�����Ƿ�һ�£�����һ������д
*	��    ��:  ��
*	�� �� ֵ:  ��
********************************************************************************************************
*/
void Emu_Adjust_Check(void)
{
	B16_B08 check;
	EM_GainPara_TypeDef *ptr;
	uint16_t temp;
	
	check.B16 = SumCheck(g_EM_GainPara.U_Gain.B08, EE_CHECK_SUM*2);
	temp = g_EM_GainPara.Check_Sum.B16;	
	if (temp != check.B16)				//ram����
	{
		/**************** EEPROM 1У��**********************/
		EEP_ReadStr(EE_ADJ_BASEADDR, ptr->U_Gain.B08, EE_PARAM_SUM*2);
		check.B16 = SumCheck(ptr->U_Gain.B08, EE_CHECK_SUM*2);
		temp = g_EM_GainPara.Check_Sum.B16;	
		if (temp != check.B16)			//eeprom1����
		{
			EEP_ReadStr(EE_ADJ_BASEADDR+EE_BACKUPS_OFFSET, ptr->U_Gain.B08, EE_PARAM_SUM*2);
			check.B16 = SumCheck(ptr->U_Gain.B08, EE_CHECK_SUM*2);
			temp = g_EM_GainPara.Check_Sum.B16;	
			if (temp != check.B16)		//eeprom2����
			{
				memcpy(&g_EM_GainPara.U_Gain.B08[0], ADJ_DEFAULT, EE_PARAM_SUM);
				EEP_WriteStr(EE_ADJ_BASEADDR, (uint8_t *)ADJ_DEFAULT[0].B08 ,EE_PARAM_SUM*2);	
				EEP_WriteStr(EE_ADJ_BASEADDR+EE_BACKUPS_OFFSET, (uint8_t *)ADJ_DEFAULT[0].B08 ,EE_PARAM_SUM*2);	
			}	
			else						//eeprom2=>eeprom1=>ram
			{
				memcpy (g_EM_GainPara.U_Gain.B08,ptr->U_Gain.B08 ,EE_PARAM_SUM*2);
				EEP_WriteStr(EE_ADJ_BASEADDR, ptr->U_Gain.B08 ,EE_PARAM_SUM*2);
				
			}
		}
		else
		{
			memcpy (&g_EM_GainPara.U_Gain.B08[0],ptr ,EE_PARAM_SUM*2);
		}
		
	}
	if (g_EM_GainPara.P_Gain.B16 != EM->PAGAIN.Word)
	{
		EM->PAGAIN.Word = g_EM_GainPara.P_Gain.B16;
	}
	if (g_EM_GainPara.U_Gain.B16 != EM->UGAIN.Word)
	{
		EM->UGAIN.Word = g_EM_GainPara.U_Gain.B16;
	}
	if (g_EM_GainPara.I_Gain.B16 != EM->IAGAIN.Word)
	{
		EM->IAGAIN.Word = g_EM_GainPara.I_Gain.B16;
	}
	if (g_EM_GainPara.Phase_Gain.B16 != EM->APHCAL.Word)
	{
		EM->APHCAL.Word = g_EM_GainPara.Phase_Gain.B16;
	}
}


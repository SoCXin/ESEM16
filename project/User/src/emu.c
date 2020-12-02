/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 计量模块。
*	文件名称 : emu.c
*	版    本 : V1.0
*	描    述 ：
*	日    期 ：2016\12\29             
*	修改记录 :
*			作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#include "includes.h"
 
EM_GainPara_TypeDef g_EM_GainPara;
EP_Data_TypeDef g_EP_Data ;

void Emu_Adjust_Save(void);
/*
*********************************************************************************************************
*	函 数 名: Set_GAINPara
*	功能说明: 设置校准增益
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: Set_SatrtGate
*	功能说明: 设置启动门限功率
*	形    参: 写入寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
void Set_SatrtGate(double value)
{		
	EM->PSTART.Word = (uint16_t)value;
}
/*
*********************************************************************************************************
*	函 数 名: Set_HighFreqPulse
*	功能说明: 设置高频脉冲常数寄存器 。根据电表常数计算高频脉冲常数值。
*	形    参: 写入寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
void Set_HighFreqPulse(double value)
{
	EM->PFSET.Word = (uint32_t)value;
}
/*
*********************************************************************************************************
*	函 数 名: EM_Config
*	功能说明: EM初始化设置
*	形    参: 写入寄存器值
*	返 回 值: 无
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
*	函 数 名: Get_URMS
*	功能说明: 得到测量电压值
*	形    参: 无
*	返 回 值: 实际电压值(扩大10倍)
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
*	函 数 名: Get_IRMS
*	功能说明: 得到实际电流值
*	形    参: 无
*	返 回 值: 实际电流值(扩大1000倍)
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
*	函 数 名: Get_FRQ
*	功能说明: 得到实际频率
*	形    参: 无
*	返 回 值: 实际频率
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
*	函 数 名: Get_PHASE
*	功能说明: 得到实际相角
*	形    参: 无
*	返 回 值: 实际相角
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
*	函 数 名: Get_PRMS
*	功能说明: 得到实际功率
*	形    参: 无
*	返 回 值: 功率值,单位：(0.0001w)
*********************************************************************************************************
*/
uint32_t Get_PRMS(void)
{
	uint32_t prms;

	prms = EM->PA.Word;			//（有符号32位）
	if (prms & 0x80000000)	    //负数
	{
		prms = ~prms + 1;
	}
	prms = prms * _P_C ;
	return prms;
}
/*
*********************************************************************************************************
*	函 数 名: Get_EnergyPulse
*	功能说明: 获取电能计量脉冲。读取后寄存器自动清零，寄存器精度为0.1个电能脉冲。每秒调用。
*	形    参: 无
*	返 回 值: 电能脉冲
*********************************************************************************************************
*/
uint32_t Get_EnergyPulse(void)
{
	return (EM->AE.Word);
}
/*
*********************************************************************************************************
*	函 数 名: Get_Factor
*	功能说明: 得到功率因数
*	形    参: 无
*	返 回 值: 功率因数* 1000
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
*	函 数 名: GetErrData
*	功能说明: 计量芯片常数 (实际测量功率-台体真实功率)/(台体真实功率)	
*	形    参: 无
*	返 回 值: 
*********************************************************************************************************
*/		
double GetErrData(uint8_t *uBuf)
{
	B32_B08  temp32_p;  					//台体功率
	uint32_t power;
	double   err_data;   					//功率误差
	
	Bcd2Hex_4(uBuf, temp32_p.B08);  		//当前功率转换成HEX	
	power = Get_PRMS();
	err_data = (double)power / temp32_p.B32 - 1;	//误差计算 (实际测量功率-台体真实功率)/(台体真实功率)	
	return err_data;
}
/*
*********************************************************************************************************
*	函 数 名: UGAIN_Adjust
*	功能说明: 电压校准
*	形    参: 台体实际电压值
*	返 回 值: 无
*********************************************************************************************************
*/    
void UGAIN_Adjust(uint8_t *uBuf)
{
	uint16_t u_gain;
	uint32_t v_data;
	double err_data;
	B32_B08 temp32_Un;                  				//台体电压
    
    Bcd2Hex_4(uBuf, temp32_Un.B08);     
    //校准前，清除寄存器
    EM->UGAIN.Word = 0;    	
	DelayMs(1000);										//要增加间隔时间。
	v_data = EM->URMS.Word;                  			//读电压有效值寄存器

	err_data = temp32_Un.B32 / _U_C / v_data - 1; 		//误差 = (U台体 - U测量)/U测量
	if (err_data >= 0)									//转换成补码形式 详见数据手册/应用笔记						
	{	
		u_gain = (uint16_t)(err_data * 32768);
	}
	else
	{
		u_gain = (uint16_t)(err_data * 32768 + 65536);
	}
	//保存校表结果
	EM->UGAIN.Word = u_gain;         
    g_EM_GainPara.U_Gain.B16 = u_gain;
	Emu_Adjust_Save();      								//EEPROM保存
}
/*
*********************************************************************************************************
*	函 数 名: IAGAIN_Adjust
*	功能说明: 电流校准
*	形    参: 台体实际电流值
*	返 回 值: 无
*********************************************************************************************************
*/
void IAGAIN_Adjust(uint8_t *uBuf)
{
	uint16_t i_gain;
	uint32_t i_data;
	double err_data;
	B32_B08    temp32_Ia;  								//台体电流
	
	Bcd2Hex_4(uBuf, temp32_Ia.B08); 
	//校准前，清除寄存器
    EM->IAGAIN.Word = 0;    
	
	DelayMs(1000);
	i_data = EM->IARMS.Word;							//读电流有效值寄存器
	err_data = temp32_Ia.B32 / _I_C / i_data - 1;		//误差 = (I台体-I测量)/I测量
	if (err_data >= 0)
	{
		i_gain = (uint16_t)(err_data * 32768);
	}
	else
	{
		i_gain = (uint16_t)(err_data * 32768 + 65536);
	}
	//保存校表结果
	EM->IAGAIN.Word = i_gain;
    g_EM_GainPara.I_Gain.B16 = i_gain;
	Emu_Adjust_Save();
}
/*
*********************************************************************************************************
*	函 数 名: PAGAIN_Adjust
*	功能说明: 有功功率校准
*	形    参: 台体实际功率值
*	返 回 值: 无
*********************************************************************************************************
*/
void PAGAIN_Adjust(uint8_t *uBuf)
{
	double err_data;
	uint32_t gain;	

	//校准前，清除寄存器
	EM->PAGAIN.Word = 0;  	

	err_data = GetErrData(uBuf);				//计算误差
	err_data = -err_data / (1 + err_data);		//误差调整为G-1 = (1/(1+err_data))-1
	//将(G-1)调整为*2^15的补码形式  详见数据手册/应用笔记
	if (err_data >= 0)
		gain = (uint16_t)(err_data*32768);
	else
		gain = (uint16_t)(err_data*32768 + 65536);
	//保存校表结果
	EM->PAGAIN.Word = gain;
    g_EM_GainPara.P_Gain.B16 = gain;
	Emu_Adjust_Save();  	
}
/*
*********************************************************************************************************
*	函 数 名: APHCAL_Adjust
*	功能说明: 角差校准
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void APHCAL_Adjust(uint8_t *uBuf)
{
	uint8_t aphcal;
	double   err_data;

	//校准前，清除寄存器
    EM->APHCAL.Word = 0;    
	DelayMs(1000);
	
	err_data = GetErrData(uBuf);					//计算误差
	err_data = err_data * APHCAL_C /50;				//将误差调整为3675.52的补码形式
	if (err_data >= 0)
		aphcal = (uint8_t)err_data;
	else
		aphcal = (uint8_t)(256 + err_data);
	//保存校表结果	
	EM->APHCAL.Word = aphcal; 
	g_EM_GainPara.Phase_Gain.B16 = aphcal;
	Emu_Adjust_Save();
}
/*
*********************************************************************************************************
*	函 数 名: PAOFF_Adjust
*	功能说明: 功率失调校准
*	形    参: 台体功率
*	返 回 值: 无
*********************************************************************************************************
*/
void  PAOFF_Adjust(uint8_t *uBuf)
{
	double err_data;
	uint16_t gain;
	B32_B08 temp32_p;  
	B32_B08 temp32;
    
    Bcd2Hex_4(uBuf, temp32_p.B08);   				//台体功率
	//校准前，清除寄存器
    EM->PAOFF.Word = 0;    

	err_data = GetErrData(uBuf);					//计算误差
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
	//保存校表结果
	EM->PAOFF.Word = gain;
    g_EM_GainPara.Paoff_Gain.B16 = gain;
	Emu_Adjust_Save(); 	
}
/*
*********************************************************************************************************
*	函 数 名: Energy_Save()
*	功能说明: 电量、脉冲数据保存
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Energy_Save(void)
{	
	EEP_WriteStr(EE_DI_ADDR, (uint8_t*)g_EP_Data.energy.B08 ,sizeof(g_EP_Data));	
}
/*
*********************************************************************************************************
*	函 数 名: Energy_Reset()
*	功能说明: 清除电量、脉冲数据
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: Energy_Add()
*	功能说明: 电量累加，最小精度0.01KWh.在1s的任务中调用。
*	形    参: 
*	返 回 值: 无
*********************************************************************************************************
*/
#define MINOR_PULSE_CNT		(MC/10)				//最小精度0.01KWh
void Energy_Add(uint32_t pulse_cnt)
{
	g_EP_Data.pulse += pulse_cnt;
	if (g_EP_Data.pulse < MINOR_PULSE_CNT)
		return;
	g_EP_Data.pulse -= MINOR_PULSE_CNT;
	g_EP_Data.energy.B32 ++;
	if (g_EP_Data.energy.B32 > 99999999)		//电能大于999999.99
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
*	函 数 名: Emu_Adjust_Save()
*	功能说明: 将校表数据保存到EEPROM
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: ReadUIP()
*	功能说明: 读计量芯片数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Read_UIP(void)
{
    B32_B08 temp32;
    B16_B08 temp16;
	
    /***********************频率**************************/
	temp16.B16 = Get_FRQ();
    Hex2Bcd ( temp16.B08, g_Emu_Data.Freq.B08, 2 );	
	/******************* 电压有效值	*********************/
	temp32.B32 = Get_URMS();
    Hex2Bcd ( temp32.B08, g_Emu_Data.U.B08, 4 );
   
    if(EM->ESR.NO_LOAD == 1)     //发生潜动，电流，功率，功率因数显示为0
    {
        g_Emu_Data.I1.B32 = 0;
        g_Emu_Data.P1.B32 = 0;
        g_Emu_Data.Cosa.B32 = 0;
    }
    else
    {
        /******************* 电流  ********************/
        temp32.B32 = Get_IRMS();	
        Hex2Bcd ( temp32.B08, g_Emu_Data.I1.B08, 4 ); 			//3位小数
        /**********************功率********************/
        temp32.B32 = Get_PRMS();	
        Hex2Bcd ( temp32.B08, g_Emu_Data.P1.B08, 4 );			//4位小数
        /******************* 功率因数******************/
        temp32.B32 = Get_Factor();
        Hex2Bcd ( temp32.B08, g_Emu_Data.Cosa.B08, 4 );	
    }
}
/*
********************************************************************************************************
*	函 数 名: Emu_Adjust_Check
*	功能说明: 验证计量校准寄存器的值是否与校准参数是否一致，若不一致则重写
*	形    参:  无
*	返 回 值:  无
********************************************************************************************************
*/
void Emu_Adjust_Check(void)
{
	B16_B08 check;
	EM_GainPara_TypeDef *ptr;
	uint16_t temp;
	
	check.B16 = SumCheck(g_EM_GainPara.U_Gain.B08, EE_CHECK_SUM*2);
	temp = g_EM_GainPara.Check_Sum.B16;	
	if (temp != check.B16)				//ram出错
	{
		/**************** EEPROM 1校验**********************/
		EEP_ReadStr(EE_ADJ_BASEADDR, ptr->U_Gain.B08, EE_PARAM_SUM*2);
		check.B16 = SumCheck(ptr->U_Gain.B08, EE_CHECK_SUM*2);
		temp = g_EM_GainPara.Check_Sum.B16;	
		if (temp != check.B16)			//eeprom1出错
		{
			EEP_ReadStr(EE_ADJ_BASEADDR+EE_BACKUPS_OFFSET, ptr->U_Gain.B08, EE_PARAM_SUM*2);
			check.B16 = SumCheck(ptr->U_Gain.B08, EE_CHECK_SUM*2);
			temp = g_EM_GainPara.Check_Sum.B16;	
			if (temp != check.B16)		//eeprom2出错
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


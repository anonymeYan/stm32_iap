#include "ads1110.h"
#include "24cxx.h"
#include "calculate.h"
#include "get_data.h"
#include "delay.h"

#include "ntc.h"
#include "ltc6803-2.h"
#include "sys.h"
#include "wdg.h"



extern u16 sys_data[40];
extern float current_soc;
extern union number
{
	float f;
	uint8_t c[4];
}my_num;
extern u8 LTC6803_Ini_OK;
extern u8 LTC6803_Ini_OK1;
extern u8 LTC6803Err_Counter;
u8 LTC6803Err_Counter1=0;
extern u8 timeT;
extern u16 CellV[8];
extern u16 Total_CellV;    //总电压
extern u16 CellV_Buff[8]; //电池电压缓存区
extern u16 CellT_Buff[2];          //温度存储区
extern u8 LTC6803Err_Flag;
extern u16 TempT[2];     //温度寄存器组
extern float T2;
void get_data()
{ 

	u16 buff;                                                                                                       
u16 ibuff;
	u16 i;//,adcx;
	float i_buff,res,buff1;
	int res1;





	u8 cbuff[4]; 

	//电流
	buff=gather_i()-60;                    //地址一加43；地址二加153；
	res1=(int)buff;
res1=trans_data(res1);
	i_buff=(float)res1*0.00305176*3;      //正常加0.3  100A

	ibuff=i_buff*10.0;
	res=(float)i_buff/7200.0;                  //AH
	
	if(buff&0x8000)
	{
	sys_data[1]=ibuff-2*ibuff;
	current_soc-=res;
	
	
	}
	else
	{
		if(ibuff<30 && ibuff>0)
		{ibuff=0;
		res=0;}
		sys_data[1]=ibuff;
	
	current_soc+=res*0.95;}
	if(current_soc<0)
		current_soc=0;
	if(current_soc>=sys_data[31])
		current_soc=sys_data[31];
	my_num.f=current_soc;
		for(i=0;i<4;i++)
	{
		cbuff[i]=my_num.c[i];
	}
//buff1=cbuff[0]*256+cbuff[1];
	//buff2=cbuff[2]*256+cbuff[3];
	AT24CXX_Write(16,(u8*)&cbuff,4);
//	AT24CXX_Write(18,(u8*)&buff2,2);
	
	res1=current_soc/(float)sys_data[31]*1000;     //SOC
	sys_data[2]=(u16)res1;

buff=gather_u();
	buff1=((float)buff)*2.048/32768.0;
	sys_data[0]=buff1*300;
	//6803的数据

	if(!LTC6803_Ini_OK)		
	{
	  LTC6803Init(0x80,0x49);			              //重新初始化LTC6803
		if(LTC6803_Ini_OK==0)
		{
			LTC6803Err_Counter++;	
			if(LTC6803Err_Counter>=5)
				 for(i=0;i<8;i++)
			CellV_Buff[i]=0;      
		}
		else
			LTC6803Err_Counter=0;
	}
	else
	{
		ReadCellVol(0x80,0x49);                 //读单体电池电压	
	for(i=0;i<8;i++)
			CellV_Buff[i]=CellV[i]/10;                    //计算后的电压值放入缓冲单元
		//sys_data[0]=Total_CellV;
	}
						delay_us(2);
	
/*	
	
	if(!LTC6803_Ini_OK1)
	{
	LTC6803Init(0x81,0x4e);
		if(LTC6803_Ini_OK1==0)
		{
			LTC6803Err_Counter1++;	
			if(LTC6803Err_Counter1>=5)
				 for(i=0;i<8;i++)
			CellV_Buff[i+8]=0;      
		}
		else
			LTC6803Err_Counter=0;
	}
	else
	{ReadCellVol(0x81,0x4e);                 //读单体电池电压
		//	sys_data[0]=(sys_data[0]+Total_CellV)/100;         	  //电压
						for(i=0;i<8;i++)
				CellV_Buff[i+8]=CellV[i]/10;                    //计算后的电压值放入缓冲单元
	}
*/
if(Total_CellV<40000)

IWDG_Feed();

	delay_ms(2);



}

void Para_Init(void)
{
	u8 x;
		u8 datatemp[50];
	u8 datatemp1[4];
		 u16 datax[50];
	
				AT24CXX_Read(16,datatemp1,4);
	
				my_num.c[0]=datatemp1[0];
	my_num.c[1]=datatemp1[1];
	my_num.c[2]=datatemp1[2];
	my_num.c[3]=datatemp1[3];
	current_soc=my_num.f;     
	sys_data[2]=current_soc/(float)sys_data[31]*1000;
				AT24CXX_Read(20,datatemp,48);
	for(x=0;x<48;x=x+2)
	 {datax[x]=datatemp[x+1]<<8;
	 sys_data[x/2+10]=datax[x]|datatemp[x];
	 }




}

























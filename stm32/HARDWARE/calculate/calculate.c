#include "calculate.h"
#include "led.h"



extern u8 uoutalarm_on,uoutalarm_stat,suoutalarm_on,suoutalarm_stat;       //过压标志
extern u8 uoutcut_on,uoutcut_stat,suoutcut_on,suoutcut_stat;              //过压

extern u8 ulowalarm_on,ulowalarm_stat,sulowalarm_on,sulowalarm_stat;  //欠压报警            
extern u8 ulowcut_on,ulowcut_stat,sulowcut_on,sulowcut_stat;		//欠压切断
extern u8 temalarm_on,temalarm_stat;		//过温报警
extern u8 temcut_on,temcut_stat;        //过温切断
extern u8 soc_low,soc_num;
extern u8 soc_on;		//soc复位
extern u8 I_on,Icut_stat;
extern u8 Ialarm_stat;
extern u8 Noise;


extern u16 CellV_Buff[]; 
extern u16 I;
extern u16 sys_data[40];
extern float current_soc;

int trans_data(u16 data)
{
	if(data & 0x8000)   //负数
	{	uint16_t buff;
//		int buff1;
		if(data==0x8000)
			return 0xffff;
	

		buff=0xffff-data+1;
	//	buff1=(int)(buff + 0x8000);
		return buff;

	}
	else
		return data;

}

u16 trans_u(u16 data)
{
	float buff;
	u16 buff1;
	buff=((float)data)*2.048/32768.0*10; //放大了10倍
	buff1=buff*20;
	return buff1;
}
	float trans_i(u16 data)   
{ 
	float buff;
	int buff1;
	buff1=trans_data(data);
	buff=(float)buff1*0.00763;     //A
	return buff;
}
	

	
void action(void)
	{
		u8 i,a,and1,and2,and3;
		u16 min,max;
		u16 T[4];
		for(i=0;i<2;i++)     //for(i=0;i<4;i++)
		{if(sys_data[i+3]&0x8000)
		T[i]=0;
			else
				T[i]=sys_data[i+3];
		}
		I=trans_data(sys_data[1]);
		
			 min=CellV_Buff[0];
		for(a=0;a<8;a++)                   //找出在线电池中最小电压的电池for(a=0;a<16;a++)
		  {
				if(CellV_Buff[a]<min) 
				{
				 min=CellV_Buff[a];
					
				}
		  }
		max=CellV_Buff[0];
					for(a=0;a<8;a++)                   //找出在线电池中最大电压的电池	for(a=0;a<16;a++)   
		  {
				if(CellV_Buff[a]>max) 
				{
				 max=CellV_Buff[a];
				}
		  }

	     //电压      过压切断
	if(sys_data[0]>=sys_data[10])				//总过压
	{
		if(uoutalarm_on>=6)
		{uoutalarm_on=6;
			uoutalarm_stat=1;
		}
	else
		uoutalarm_on++;
	}
	if(sys_data[0]<=sys_data[12])
	{
		uoutalarm_on=0;
		uoutalarm_stat=0;
	}

	if(max>=sys_data[11])            //单只过压报警值
	{
		if(suoutalarm_on>=6)
		{
			suoutalarm_on=6;
			suoutalarm_stat=1;
	
		}
		else
			suoutalarm_on++;

	}
	
	if(max<=sys_data[13])
	{
		suoutalarm_on=0;
		suoutalarm_stat=0;
	
	}
	
	if(sys_data[0]>=sys_data[14])			//过压切断
	{
		if(uoutcut_on>=6)
		{
			uoutcut_on=6;
			uoutcut_stat=1;
		}
		else
			uoutcut_on++;
	}
	if(sys_data[0]<=sys_data[16])
	{
		uoutcut_on=0;
		uoutcut_stat=0;
	}
	
	if(max>=sys_data[15])				//单只过压切断
	{
		if(suoutcut_on>=6)
		{
			suoutcut_on=6;
			suoutcut_stat=1;
		}
		else
			suoutcut_on++;
	}
	if(max<=sys_data[17])
	{
		suoutcut_on=0;
		suoutcut_stat=0;
	}
	
//欠压
		if(sys_data[0]<=sys_data[18])		//欠压报警
		{
			if(ulowalarm_on>=6)
			{
				ulowalarm_on=6;
				ulowalarm_stat=1;

			}
			else
						ulowalarm_on++;
		}
		
		if(sys_data[0]>=sys_data[20])			//
		{
			ulowalarm_on=0;
			ulowalarm_stat=0;

		}
		//单只欠压报警
		if(min<=sys_data[19])
		{
			if(sulowalarm_on>=6)
			{
			sulowalarm_on=6;
				sulowalarm_stat=1;
		
			}
			else
				sulowalarm_on++;
		}	
		if(min>=sys_data[21])
		{
			sulowalarm_on=0;
			sulowalarm_stat=0;
			
		}
		//欠压切断
		if(sys_data[0]<=sys_data[22])
		{
			if(ulowcut_on>=6)
			{
				ulowcut_on=6;
				ulowcut_stat=1;
			}
			else
				ulowcut_on++;
		}
		if(sys_data[0]>=sys_data[24])
		{
			ulowcut_on=0;
			ulowcut_stat=0;
		}
		if(min<=sys_data[23])			//单只欠压
		{
			if(sulowcut_on>=6)
			{
				sulowcut_on=6;
				sulowcut_stat=1;
			}
			else
				sulowcut_on++;
		}
		if(min>=sys_data[25])
		{
			sulowcut_on=0;
			sulowcut_stat=0;
		}
	//开关复位
		


    //SOC报警

	

		

		//过温报警
		if(T[0]>=sys_data[26]||T[1]>=sys_data[26]||T[2]>=sys_data[26]||T[3]>=sys_data[26])
		{
			if(temalarm_on>=6)
			{
			temalarm_on=6;
				temalarm_stat=1;
			}
			else
				temalarm_on++;
		
		}
		//if(T[0]<=sys_data[27]&&T[1]<=sys_data[27]&&T[2]<=sys_data[27]&&T[3]<=sys_data[27])
		if(T[0]<=sys_data[27]&&T[1]<=sys_data[27])
		
		{
		temalarm_on=0;
			temalarm_stat=0;
		}
	
		//if(T[0]>=sys_data[28]||T[1]>=sys_data[28]||T[2]>=sys_data[28]||T[3]>=sys_data[28])
		if(T[0]>=sys_data[28]||T[1]>=sys_data[28])
		{
			if(temcut_on>=6)
			{
				temcut_on=6;
				temcut_stat=1;
			}
			else
				temcut_on++;
		}
		//if(T[0]<=sys_data[29]&&T[1]<=sys_data[29]&&T[2]<=sys_data[29]&&T[3]<=sys_data[29])
		if(T[0]<=sys_data[29]&&T[1]<=sys_data[29])
		
		{
			temcut_on=0;
			temcut_stat=0;
		}
		//电流保护
//		if(I>=3000)
//		{
//			sys_data[36]=1;
//			Icut_stat=1;
//		}
//		else
//		{
//			sys_data[36]=0;
//			Icut_stat=0;
//		}

		
		
		
		

		//报警
			and1=uoutalarm_stat+ulowalarm_stat+suoutalarm_stat+sulowalarm_stat+uoutcut_stat+ulowcut_stat+suoutcut_stat+sulowcut_stat;
		if(and1)
		{
		if(uoutalarm_stat==1)
		sys_data[34]=1;	                      //电压高


		if(ulowalarm_stat==1)                      //电压低
		sys_data[34]=2;                      
		if(suoutalarm_stat==1)                 //单只高
		sys_data[34]=3;                       
		if(sulowalarm_stat==1)                    //单只低
		sys_data[34]=4;                      
		if(uoutcut_stat==1)                  //高切
			sys_data[34]=5;
		if(ulowcut_stat)                           //低切
			sys_data[34]=6; 
		if(suoutcut_stat)
		sys_data[34]=7;
		if(sulowcut_stat)
			sys_data[34]=8;

		}
		else
		{

			sys_data[34]=0;
		}
		if(temalarm_stat||temcut_stat)
		{if(temalarm_stat)
			sys_data[35]=1;
		if(temcut_stat)
			sys_data[35]=2;
		}
		else
			sys_data[35]=0;

		
		and2=uoutcut_stat+suoutcut_stat+ulowcut_stat+sulowcut_stat+temcut_stat;
		if(and2)
		{
			CHR=0;
			Cut=1;
		}
		else
		{
			CHR=1;
			Cut=0;
		}
		if(uoutcut_stat||suoutcut_stat||temcut_stat)
			DCHR=1;//DCHR=0;
		else
			DCHR=0;//DCHR=1;
		and3=uoutalarm_stat+suoutalarm_stat+ulowalarm_stat+sulowalarm_stat+temalarm_stat;
		if(and3)
			Alarm=1;
		else
			Alarm=0;
			if(sys_data[0]>=sys_data[32]||sys_data[2]==1000||sys_data[37]==1)
			{
				sys_data[2]=1000;
				Over_V=1;
				current_soc=sys_data[31];
			}
			else 
			{
				Over_V=0;
			}
			if(sys_data[0]<=sys_data[33]||sys_data[2]<sys_data[30])
			Need=1;
			else
				Need=0;
		
}

	




	
	
	
	
	
	




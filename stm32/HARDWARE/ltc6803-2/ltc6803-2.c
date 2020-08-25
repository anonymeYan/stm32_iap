
#include "ltc6803-2.h"		 					//调用LTC6803寄存器定义头文件
#include "ntc.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
u8 CFGR[6];    //配置寄存器组

u8 CVR[19];    //电池电压寄存器组

u8 TMPR[5];     //温度寄存器组

u16 MCxI_CellNumb;
u8 LTC6803_Ini_OK;
u8 LTC6803_Ini_OK1;
struct flagreg{
u8 byte;

}Readflag[3];

u8 LTC6803Err_Counter=0;      //错误数目
u8 LTC6803Err_Flag;         //错误标志位
extern u16 CellT[2];         //
extern u16 CellV[8];
extern u16 Total_CellV;    //总电压
extern u16 CellV_Buff[8]; //电池电压缓存区
 u8 Check[6]={0};
extern u16 CellT_Buff[2];          //温度存储区
 extern	u16 TempT[2];
 extern float T1,T2;
 
 
 

void LTC_Init(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );//PORTB时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // PB12 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
 
        LTC6803CS=1;				//SPI FLASH不选中
	SPI2_Init();		   	//初始化SPI
	SPI2_SetSpeed(SPI_BaudRatePrescaler_32);//设置为18M时钟,高速模式

	//	LTC6803_ReadID();//读取FLASH ID.  

}  


void  LTC6803Init(u8 adder,u8 pec)		 					//对LTC6803的初始化设置
{
	u8 i;
	u8 Err,CRC_Temp;
	u16 MCxI_Temp;  

	CFGR[0]=0xe9;				//关看门狗
	CFGR[1]=0x00;      		//放电开关0x00
	CFGR[2]=0x00;		//使能中断对于所有的电池(低四位)	

	CFGR[3]=0xf0;          //5-12电池的标志位
	CFGR[4]=0;          //低压比较电压
	CFGR[5]=0xff;            //过压比较电压
	
	
	//根据配置计算有效电池节数
	MCxI_Temp=CFGR[3];      //MCxI高8位
	MCxI_Temp<<=4;
	MCxI_Temp|=(CFGR[2]&0xF0)>>4;  //MCxI低4位
	MCxI_CellNumb=0;
	for(i=0;i<12;i++)
	{
	 if((MCxI_Temp&0x0001)==0) 
	 {
	  MCxI_CellNumb++;      //有效电池节数计数
		MCxI_Temp>>=1;        //MCxI缓存单元右移位
	 }		
	}
	//配置寄存器的初始化
	
	LTC6803CS=0;     								//LTC6803片选信号置低
  SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	delay_us(2);	
	SPI2_ReadWriteByte(pec); 
		delay_us(2);
	SPI2_ReadWriteByte(WRCFG);   								//Write configuration
	delay_us(2);                    	//延时2us子程序     
	SPI2_ReadWriteByte(WRCFG_CRC);   						//Write CRC 
	delay_us(2);                    	//延时2us子程序   	
	for(i=0;i<6;i++)
	{
		SPI2_ReadWriteByte(CFGR[i]); 
		
	}		
  CRC_Temp=CRC8(CFGR,6);            //计算CFGR单元的CRC码
	SPI2_ReadWriteByte(CRC_Temp);               //写CRC校验字节
	delay_us(2);                    	//延时2us子程序     
	LTC6803CS=1;        			    	//LTC6803片选信号置高
	delay_us(50);                    //延时0.1ms子程序         
	//读取LTC6803寄存器配置数据是否正确
	LTC6803CS=0;     					    	//LTC6803片选信号置低	
	SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	delay_us(2);	
	SPI2_ReadWriteByte(pec); 
		delay_us(2);
	
	SPI2_ReadWriteByte(RDCFG);
	delay_us(2);                    	//延时5us子程序     
	SPI2_ReadWriteByte(RDCFG_CRC);              //Write CRC  
	delay_us(2);                    	//延时5us子程序   
	for(Err=0;Err<6;Err++)			    	//读取写入的寄存器值
	{
		Check[Err]=SPI2_ReadWriteByte(0xff);

		if((Check[Err]!=CFGR[Err]))  //读的不对就重新读
		{
			Err=0;
			LTC6803Err_Counter++;
			break;
		}
	}	LTC6803CS=1;        			    	//LTC6803片选信号置高
	//SPI2_ReadWriteByte(0xff);


//	if(CFGR[1]||(CFGR[2]&0x0F)!=0)    //有放电开关接通
//		LPC_GPIO2->DATA &= ~(1ul<<8);   //p2.8置低放电开关指示灯亮 
if(adder==0x80)	
{if(Err!=0)
		LTC6803_Ini_OK=1;			    			//LTC6803初始化成功
	else
		LTC6803_Ini_OK=0;			    			//LTC6803初始化失败
}
	if(adder==0x81)
	{if(Err!=0)
			LTC6803_Ini_OK1=1;
		else
			LTC6803_Ini_OK1=0;

	}
}


 
//================================================================

//================================================================
//函数功能:读取LTC6803电池电压寄存器
//filename:ReadCellVol(void)
//================================================================
void ReadCellVol(u8 adder,u8 pec)          //12位电池电压
{
	u8 a,b,c,CRC_Cal;
/*	if(!(LTC6803_Ini_OK||LTC6803_Ini_OK1))		
	{
	  LTC6803Init(adder,pec);			              //重新初始化LTC6803
		if(LTC6803_Ini_OK==0)
		{
			LTC6803Err_Counter++;	
			if(LTC6803Err_Counter>=5)
				 LTC6803Err_Flag=1;           //连续初始化5次依旧没成功则报错
		}
		else    //5次之内重新初始化成功则说明LTC6803无问题
		{
			LTC6803Err_Counter=0;
			LTC6803Err_Flag=0;              //LTC6803初始化错误标志清除
		}
	}		
	else	 //LTC6803已经正常初始化,当前允许读取电压值
	{*/
		LTC6803CS=0;     								//LTC6803片选信号置低
SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	delay_us(2);	
		SPI2_ReadWriteByte(pec); 
			delay_us(2);
		SPI2_ReadWriteByte(STCVAD);    					//start cell conversion for all cells
			delay_us(2);
		SPI2_ReadWriteByte(STCVAD_CRC);    
		LTC6803CS=1;        			    	//LTC6803片选信号置高
		delay_us(20);                     //延时20ms
	  LTC6803CS=0;     								//LTC6803片选信号置低
			SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	
	delay_us(2);
		SPI2_ReadWriteByte(pec); 
			delay_us(2);
		SPI2_ReadWriteByte(RDCV); 									//read cell voltages command
			delay_us(2);
		SPI2_ReadWriteByte(RDCV_CRC); 
		for(a=0;a<19;a++)									//将读出的数值赋给电池电压寄存器组
		{
			CVR[a]=SPI2_ReadWriteByte(0xff);
	
		}
		LTC6803CS=1;        			    	//LTC6803片选信号置高
		//将电池电压寄存器组转化成实际的电池电压值3个寄存器对应两个电池电压值
		CRC_Cal=CRC8(CVR,18);							//计算校验  
		if(CRC_Cal==CVR[18])							//CRC校验正确
		{
			for(b=0;b<MCxI_CellNumb/2;b++)	//电池电压数据重组(MCxI_CellNumb/2)
			{
				CellV[b*2]=(u16)CVR[b*3+1];
				CellV[b*2] <<=8;
				CellV[b*2] &=0x0F00;
				CellV[b*2]+=(CVR[b*3]);
				CellV[b*2+1]=CVR[b*3+2];
				CellV[b*2+1] <<=4;
				CellV[b*2+1] +=(CVR[b*3+1]&0xF0)>>4;				
			}
			Total_CellV=0;
			for(c=0;c<MCxI_CellNumb;c++)
			{
				CellV[c]=(CellV[c]-512)*15/10;	 		   //电压计算结果保留小数点后3位精确到mV

			  Total_CellV+=CellV[c];                     //计算LCU总电池电压值并存放
		
			}
		}		

	}	




//================================================================
//函数功能:读取LTC6803电池温度
//filename:ReadTemp(void)
//================================================================
void ReadTemp(u8 adder,u8 pec)
{
	u8 j;


	if(!LTC6803_Ini_OK)		
	{
	  LTC6803Init(adder,pec);			              //重新初始化LTC6803
		if(LTC6803_Ini_OK==0)
		{
			LTC6803Err_Counter++;	
			if(LTC6803Err_Counter>=5)
				 LTC6803Err_Flag=1;           //连续初始化5次依旧没成功则报错
		}
		else    //5次之内重新初始化成功则说明LTC6803无问题
		{
			LTC6803Err_Counter=0;
			LTC6803Err_Flag=0;              //LTC6803初始化错误标志清除
		}
	}		
	else	  //LTC6803已经正常初始化,当前允许读取温度值
	{
		LTC6803CS=0;     						//LTC6803片选信号置低
	SPI2_ReadWriteByte(adder);  			//
	  SPI2_ReadWriteByte(pec);
		SPI2_ReadWriteByte(STTMPAD);  			//start cell conversion for all cells  开始测量温度
	  SPI2_ReadWriteByte(STTMPAD_CRC);
		LTC6803CS=1;        			  //LTC6803片选信号置高		
		delay_us(20);            		  //延时20ms子程序   		
		LTC6803CS=0;     						//LTC6803片选信号置低
SPI2_ReadWriteByte(adder);  			//
	  SPI2_ReadWriteByte(pec);	

		SPI2_ReadWriteByte(RDTMP); 							//read temprature command
		SPI2_ReadWriteByte(RDTMP_CRC); 	
		for(j=0;j<5;j++) 							//将读出的数值赋给温度寄存器组
		{
			TMPR[j]=SPI2_ReadWriteByte(0xff);
		}
		LTC6803CS=1;        			  //LTC6803片选信号置高
		CellT[0]=TMPR[1];
		CellT[0]<<=8;
		CellT[0]&=0x0F00;
		CellT[0]+=(TMPR[0]);
	
		CellT[1]=TMPR[2];
		CellT[1]<<=4;
		CellT[1]+=TMPR[1]>>4;
		for(j=0;j<2;j++)
		{
			CellT[j]=(CellT[j]-512)*15/10;	          //当前温度采集到的电压值mV
		}
		
		for(j=0;j<2;j++)	 //计算出温度传感器电压对应的电阻值R=Vtemp*100/(Vref-Vtemp)
		{
			if(CellT[j]>=3065)    		//Vtemp>3V说明NTC采样无效
				    TempT[j] = 0x7FFF; 		//无效
			else  TempT[j] = CellT[j]*100*10/(3065-CellT[j]);	 //电阻值精确到R*10				
		}
		//计算出温度值 由电阻值转变为温度值(温度系数查表内容还需确认)

	T1=GetTempreture(TempT[0]);
		CellT_Buff[0]=(u16)T1-300; 
	  T2=GetTempreture(TempT[1]);	
   	CellT_Buff[1]=(u16)T2-300; 		
	}
}
//================================================================


//  计算CRC校验码函数;
//	输  入:	要计算数据缓冲区的起始指针pBuff，缓冲区的字节数Len;
//	输  出:	CRC校验码;
//	功  能:	计算CRC校验码。
//------------------------------------------------------------------------------------------------------
//CRC算法说明:
/*校验多项式为G(X)=X8+X2+X+1，对每个控制字或信息字的前5个字节进行CRC校验，生成一个8位的余数，该余数取反
后即可作为校验码尾随前5字节发送。计算方法如下：将每个控制字或信息字中的前5个字节按从低到高的顺序排列，
每个字节的位从高到低排列，构成一个位流，在这个位流的后面加上8个0，从而得到一个48位的位流。例如，对某个信息
字有B0-B5共6个字节，每个字节8位分别为bi7-bi0(i为字节序号,取值范围0~4),则按上面的方法得到的位序列为
b07b06...b00，b17b16..b10，...，b47b47...b40，00000000,用校验多项式107H (X^8+X^2+X+1)作为除数去除这个码流，
相除时不做减法而做异或，最后得到一个8位的余数*/
//===============================================================================
u8 CRC8(u8 *buf, int8_t len)   
{   
  u16   i,j=0;   
  u8  crc = 0x41;       //包错误代码（pec）预置为0x41 
  
  while(len--!=0)  
	{   
    for(i= 0x80;i!=0;i/=2)         //移8次
		{   
      if ((crc & 0x80)!=0)  
			{   
        crc *= 2;   
        crc ^= 0x07;           //X8+x2+x+1
      }  
			else  crc *= 2;      
      if ((*(buf+j) & i)!=0)   
        crc^=0x07;   
    }    
    j++;
  }   
  return   crc;   
} 

//******************************************************************
//  The  	End 	of 	this   File
//******************************************************************





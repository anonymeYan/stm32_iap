#include "modbus.h"
#include "crc16.h"
#include "sys.h"
#include "24cxx.h"
#include "delay.h"

extern u32 RS485_Baudrate;                       //通讯波特率
extern u8 RS485_Parity;                              //0无校验；1奇校验；2偶校验
extern const u8 RS485_Addr;                                //从机地址
extern u16 RS485_Frame_Distance;                     //数据帧最小间隔（ms),超过此时间则认为是下一帧

extern u8 RS485_RX_BUFF[512];                         //接收缓冲区2048字节
extern u16 RS485_RX_CNT;                             //接收计数器
extern u8 RS485_FrameFlag;                           //帧结束标记
extern u8 RS485_TX_BUFF[512];                         //发送缓冲区
extern u16 RS485_TX_CNT;                             //发送计数器

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Modbus寄存器和单片机寄存器的映射关系
vu32 *Modbus_InputIO[20];                      //输入开关量寄存器指针(这里使用的是位带操作)
vu32 *Modbus_OutputIO[20];                     //输出开关量寄存器指针(这里使用的是位带操作)
extern u16 *Modbus_HoldReg[512];                      //保持寄存器指针
extern u16 sys_data[40];
extern u16 CellV_Buff[12];
extern u16 Total_CellV;
extern u16 MCxI_CellNumb;
extern u16 CellT_Buff[2];
extern u16 LTC6803Err_Flag;
extern u16 LTC6803_Ini_OK;
extern u16 Dcc_Voltage;                //开启均衡电压
extern u16 Dcc_Value;
extern u8 Noise;



void Modbus_RegMap(void)
{
	u8 i;
        //输入开关量寄存器指针指向
/*	Modbus_InputIO[0]=(vu32*)&PEin(4);//KEY0
Modbus_InputIO[1]=(vu32*)&PEin(3);//KEY1
	Modbus_InputIO[2]=(vu32*)&PEin(2);//KEY2
	Modbus_InputIO[3]=(vu32*)&PAin(0);//KEY3
	
	//输出开关量寄存器指针指向
	Modbus_OutputIO[0]=(vu32*)&PBout(5);//LED0
	Modbus_OutputIO[1]=(vu32*)&PEout(5);//LED1
	
	*///保持寄存器指针指向
Modbus_HoldReg[0]=(u16*) &sys_data[0];//总电压
	Modbus_HoldReg[1]=(u16*)&sys_data[34];	//电压信息
	
	Modbus_HoldReg[2]=(u16*) &sys_data[1];//电流
	Modbus_HoldReg[3]=(u16*)&sys_data[36];	//电流信息
	
	Modbus_HoldReg[4]=(u16*) &sys_data[2];//SOC
	Modbus_HoldReg[5]=(u16*)&sys_data[37];	//复位SOC
	Modbus_HoldReg[8]=(u16*) &sys_data[3];//温度1
	Modbus_HoldReg[10]=(u16*) &sys_data[4];//温度2
		Modbus_HoldReg[12]=(u16*) &sys_data[5];//温度3
	Modbus_HoldReg[14]=(u16*) &sys_data[6];//温度4
	Modbus_HoldReg[15]=(u16*)&sys_data[35];//温度信息
	
	Modbus_HoldReg[16]=(u16*) &sys_data[7];//soc
	Modbus_HoldReg[18]=(u16*) &sys_data[8];//soc


//设定值
Modbus_HoldReg[20]=(u16*) &sys_data[10];//过压报警
Modbus_HoldReg[22]=(u16*) &sys_data[11];//单只过压报警
Modbus_HoldReg[24]=(u16*) &sys_data[12];//过压报警恢复值
Modbus_HoldReg[26]=(u16*) &sys_data[13];//单只过压报警恢复值

Modbus_HoldReg[28]=(u16*) &sys_data[14];//过压切断
Modbus_HoldReg[30]=(u16*) &sys_data[15];//单只过压切断
Modbus_HoldReg[32]=(u16*) &sys_data[16];//过压切断恢复值
Modbus_HoldReg[34]=(u16*) &sys_data[17];//单只过压切断恢复值

Modbus_HoldReg[36]=(u16*) &sys_data[18];//欠压报警
Modbus_HoldReg[38]=(u16*) &sys_data[19];//单只欠压报警
Modbus_HoldReg[40]=(u16*) &sys_data[20];//欠压报警恢复值
Modbus_HoldReg[42]=(u16*) &sys_data[21];//单只欠压报警恢复值

Modbus_HoldReg[44]=(u16*) &sys_data[22];//欠压切断
Modbus_HoldReg[46]=(u16*) &sys_data[23];//单只欠压切断
Modbus_HoldReg[48]=(u16*) &sys_data[24];//欠压切断恢复值
Modbus_HoldReg[50]=(u16*) &sys_data[25];//单只欠压切断恢复值

Modbus_HoldReg[52]=(u16*) &sys_data[26];//过温报警
Modbus_HoldReg[54]=(u16*) &sys_data[27];//过温报警恢复值
Modbus_HoldReg[56]=(u16*) &sys_data[28];//过温切断
Modbus_HoldReg[58]=(u16*) &sys_data[29];//过温切断恢复值

Modbus_HoldReg[60]=(u16*) &sys_data[30];       //SOC下限

Modbus_HoldReg[62]=(u16*) &sys_data[31];       //额定容量

Modbus_HoldReg[64]=(u16*) &sys_data[32];       //充电电压上限

Modbus_HoldReg[66]=(u16*) &sys_data[33];       //放电电压下限





for(i=0;i<8;i++)                                //for(i=0;i<16;i++)
Modbus_HoldReg[i+70]=(u16*)&CellV_Buff[i];       //单体电压


//
/*



Modbus_HoldReg
Modbus_HoldReg
Modbus_HoldReg
Modbus_HoldReg
Modbus_HoldReg*/
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//初始化USART2
void RS485_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA,D时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA4端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//默认接收状态
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位
	
	USART_InitStructure.USART_BaudRate=RS485_Baudrate;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//收发模式
	switch(RS485_Parity)
	{
		case 0:USART_InitStructure.USART_Parity=USART_Parity_No;break;//无校验
		case 1:USART_InitStructure.USART_Parity=USART_Parity_Odd;break;//奇校验
		case 2:USART_InitStructure.USART_Parity=USART_Parity_Even;break;//偶校验
	}
	USART_Init(USART2,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能串口2接收中断 
    
	USART_Cmd(USART2,ENABLE);       //使能串口2
    
	RS485_TX_EN=0;                  //默认为接收模式
	Timer7_Init();                  //定时器7初始化，用于监视空闲时间
	Modbus_RegMap();                //Modbus寄存器映射
    
}

//定时器7初始化
void Timer7_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //TIM7时钟使能 

	//TIM7初始化设置
	TIM_TimeBaseStructure.TIM_Period = RS485_Frame_Distance*10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =7200-1; //设置用来作为TIMx时钟频率除数的预分频值 设置计数频率为10kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_ITConfig( TIM7, TIM_IT_Update, ENABLE );//TIM7 允许更新中断
//    TIM_Cmd(TIM7, ENABLE);
	//TIM7中断分组配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //配置NVIC中断优先级编组
	NVIC_InitStructure.NVIC_IRQChannel =TIM7_IRQn;              //TIM7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;          //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                             //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器                                                                  
}

//////////////////////////////////////////////////////////////////////////////
//发送n个字节数据
//buff:发送区首地址
//len：发送的字节数
void RS485_SendData(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
	delay_ms(5);
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	 
delay_us(5);		
		USART_SendData(USART2,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
	//RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//设置为接收模式	
}

/////////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void)//串口2中断服务程序
{
	u8 res;
	u8 err;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	{
		if(USART_GetFlagStatus(USART2,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE)) 
            err=1;                                      //检测到噪音、帧错误或校验错误
		else 
            err=0;
		res=USART_ReceiveData(USART2);                  //读接收到的字节，同时相关标志自动清除
		
		if((RS485_RX_CNT<2047)&&(err==0))
		{
			RS485_RX_BUFF[RS485_RX_CNT]=res;
			RS485_RX_CNT++;
			
			TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除定时器溢出中断
			TIM_SetCounter(TIM7,0);                     //当接收到一个新的字节，将定时器7复位为0，重新计时（相当于喂狗）
			TIM_Cmd(TIM7,ENABLE);                       //开始计时
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//用定时器7判断接收空闲时间，当空闲时间大于指定时间，认为一帧结束
//定时器7中断服务程序         
void TIM7_IRQHandler(void)
{                                                                   
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志
		TIM_Cmd(TIM7,DISABLE);                      //停止定时器
		RS485_TX_EN=1;                              //停止接收，切换为发送状态
		RS485_FrameFlag=1;                          //置位帧结束标记
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//RS485服务程序，用于处理接收到的数据(请在主函数中循环调用)
 u16 startRegAddr;
u16 RegNum;
u16 calCRC;
void RS485_Service(void)
{
	u16 recCRC;
	if(RS485_FrameFlag==1)
	{
        if(RS485_RX_BUFF[0]==RS485_Addr)//地址正确
		{
			if((RS485_RX_BUFF[1]==03)||(RS485_RX_BUFF[1]==06))//功能码正确
			{
				startRegAddr=(((u16)RS485_RX_BUFF[2])<<8)|RS485_RX_BUFF[3];//获取寄存器起始地址
				if(startRegAddr<1000)//寄存器地址在范围内
				{
					calCRC=CRC_Compute(RS485_RX_BUFF,RS485_RX_CNT-2);//计算所接收数据的CRC
					recCRC=RS485_RX_BUFF[RS485_RX_CNT-1]|(((u16)RS485_RX_BUFF[RS485_RX_CNT-2])<<8);//接收到的CRC(低字节在前，高字节在后)
					if(calCRC==recCRC)//CRC校验正确
					{
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						switch(RS485_RX_BUFF[1])//根据不同的功能码进行处理
						{	
							case 03: //读多个寄存器
							{
								Modbus_03_Solve();
								break;
							}
									
							case 06: //写单个寄存器
							{
								Modbus_06_Solve();
								break;
							}
							default:	
                                break;
						}
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					}
					else//CRC校验错误
					{
						RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
						RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
						RS485_TX_BUFF[2]=0x04; //异常码
						RS485_SendData(RS485_TX_BUFF,3);
					}        
				}
				else//寄存器地址超出范围
				{
					RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
					RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
					RS485_TX_BUFF[2]=0x02; //异常码
					RS485_SendData(RS485_TX_BUFF,3);
				}                                                
			}
			else//功能码错误
			{
				RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
				RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
				RS485_TX_BUFF[2]=0x01; //异常码
				RS485_SendData(RS485_TX_BUFF,3);
			}
        }						
        RS485_FrameFlag=0;//复位帧结束标志	
        RS485_RX_CNT=0;//接收计数器清零
		RS485_TX_EN=0;//开启接收模式                
	}                
}



//Modbus功能码03处理程序
//读保持寄存器
void Modbus_03_Solve(void)
{
	u8 i;
	RegNum = (((u16)RS485_RX_BUFF[4])<<8)|RS485_RX_BUFF[5];//获取寄存器数量
	if((startRegAddr+RegNum)<1000)//寄存器地址+数量在范围内
	{
		RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
		RS485_TX_BUFF[1]=RS485_RX_BUFF[1];
		RS485_TX_BUFF[2]=RegNum*2;
		for(i=0;i<RegNum;i++)
		{
			RS485_TX_BUFF[4+i*2]=*Modbus_HoldReg[startRegAddr+i]&0xFF;//后发送低字节
			RS485_TX_BUFF[3+i*2]=(*Modbus_HoldReg[startRegAddr+i]>>8)&0xFF;//先发送高字节 
		}
		calCRC=CRC_Compute(RS485_TX_BUFF,RegNum*2+3);
		RS485_TX_BUFF[RegNum*2+3]=(calCRC>>8)&0xFF;
		RS485_TX_BUFF[RegNum*2+4]=calCRC&0xFF;
		RS485_SendData(RS485_TX_BUFF,RegNum*2+5);

	}
	else//寄存器地址+数量超出范围
	{
		RS485_TX_BUFF[0]=RS485_RX_BUFF[0];
		RS485_TX_BUFF[1]=RS485_RX_BUFF[1]|0x80;
		RS485_TX_BUFF[2]=0x02; //异常码
		RS485_SendData(RS485_TX_BUFF,3);
	}
}


//Modbus功能码06处理程序
//写单个保持寄存器
void Modbus_06_Solve(void)
{
	*Modbus_HoldReg[startRegAddr] = ((u16)RS485_RX_BUFF[4])<<8;//高字节在后
	*Modbus_HoldReg[startRegAddr]|= RS485_RX_BUFF[5];//低字节在前
	
	RS485_TX_BUFF[0]=RS485_Addr;
	RS485_TX_BUFF[1]=0x06;
	RS485_TX_BUFF[2]=RS485_RX_BUFF[2];
	RS485_TX_BUFF[3]=RS485_RX_BUFF[3];
	RS485_TX_BUFF[4]=RS485_RX_BUFF[4];
	RS485_TX_BUFF[5]=RS485_RX_BUFF[5];
	
	calCRC=CRC_Compute(RS485_TX_BUFF,6);
	RS485_TX_BUFF[7]=calCRC&0xFF;
	RS485_TX_BUFF[6]=(calCRC>>8)&0xFF;
	RS485_SendData(RS485_TX_BUFF,8);
			//	LCD_Fill(0,170,239,319,WHITE);//清除半屏    
 			//LCD_ShowString(30,170,200,16,16,"Start Write 24C02....");
			AT24CXX_Write(startRegAddr,(u8*)Modbus_HoldReg[startRegAddr],2);
			//LCD_ShowString(30,170,200,16,16,"24C02 Write Finished!");//提示传送完成
		delay_ms(10);
	
}


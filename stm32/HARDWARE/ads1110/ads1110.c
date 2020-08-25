#include "ads1110.h"
#include "delay.h"
#include "sys.h"

/******************************************************************************/
// 写:ADS1110 功能函数
// 输入参数:u8 dat
// 返回:none
/*******************************************************************************/
/*struct ADS_DATA
{
u8 TMdata;
  u8 TMTEMP;


}ADS_DATA;
*/

void ADS1110_Init(void)
{
	IIC2_Init();
}

//读电压
u16 gather_u()
{u16 volt;

	Config_ADS1110(0x92);
volt=READ_ADS1110(0x93);
return volt;
}


//读电流
u16 gather_i()
{
int an;
	Config_ADS1110(0x90);
an=READ_ADS1110(0x91);
	return an;

}

//配置ads1110
void Config_ADS1110(u8 daddr)
{
    IIC_Start1();
    IIC_Send_Byte1(daddr) ;//写地址
    IIC_Wait_Ack1() ; 
    IIC_Send_Byte1(0x9c) ;//数据
    IIC_Wait_Ack1() ;
    IIC_Stop1();
}
/******************************************************************************/
// 读:ADS1110 功能函数
// 输入参数:none
// 返回:none
 
/*******************************************************************************/
 
u16 READ_ADS1110(u8 daddr)
{
   u16 buff;


	//volatile u8 TMTEMP;
    IIC_Start1();
    IIC_Send_Byte1(daddr) ;//读地址
    IIC_Wait_Ack1() ; 
    buff = IIC_Read_Byte1(1)<<8;
        IIC_Ack1(); //
  buff+= IIC_Read_Byte1(1);//读出的转换数据存入16位变量中
    IIC_Wait_Ack1(); //

   
    IIC_Stop1();

return buff;
}
/*void Get_AdsData(void)
{ 
    //IIC_Init();
    WRITE_ADS1110(0x8c);//配置寄存器 默认状态值
    READ_ADS1110();
}*/
//配置寄存器为0X8C，所以是连续转换，16位数据结果，增益为1；
//所以最后的电压 V=ADS_DATA.TMdata*2.048/32768

void IIC2_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 输出高
}




void IIC_Start1(void)
{
	SDA_OUT1();     //sda线输出
	IIC_SDA1=1;	  	  
	IIC_SCL1=1;
	delay_us(4);
 	IIC_SDA1=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL1=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop1(void)
{
	SDA_OUT1();//sda线输出
	IIC_SCL1=0;
	IIC_SDA1=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL1=1; 
	IIC_SDA1=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack1(void)
{
	u8 ucErrTime=0;
	SDA_IN1();      //SDA设置为输入  
	IIC_SDA1=1;delay_us(1);	   
	IIC_SCL1=1;delay_us(1);	 
	while(READ_SDA1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop1();
			return 1;
		}
	}
	IIC_SCL1=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=0;
	delay_us(2);
	IIC_SCL1=1;
	delay_us(2);
	IIC_SCL1=0;
}
//不产生ACK应答		    
void IIC_NAck1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=1;
	delay_us(2);
	IIC_SCL1=1;
	delay_us(2);
	IIC_SCL1=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte1(u8 txd)
{                        
    u8 t;   
	SDA_OUT1(); 	    
    IIC_SCL1=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA1=1;
		else
			IIC_SDA1=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL1=1;
		delay_us(2); 
		IIC_SCL1=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte1(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN1();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL1=0; 
        delay_us(2);
		IIC_SCL1=1;
        receive<<=1;
        if(READ_SDA1)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck1();//发送nACK
    else
        IIC_Ack1(); //发送ACK   
    return receive;
}






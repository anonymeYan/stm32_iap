#include "ads1110.h"
#include "delay.h"
#include "sys.h"

/******************************************************************************/
// д:ADS1110 ���ܺ���
// �������:u8 dat
// ����:none
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

//����ѹ
u16 gather_u()
{u16 volt;

	Config_ADS1110(0x92);
volt=READ_ADS1110(0x93);
return volt;
}


//������
u16 gather_i()
{
int an;
	Config_ADS1110(0x90);
an=READ_ADS1110(0x91);
	return an;

}

//����ads1110
void Config_ADS1110(u8 daddr)
{
    IIC_Start1();
    IIC_Send_Byte1(daddr) ;//д��ַ
    IIC_Wait_Ack1() ; 
    IIC_Send_Byte1(0x9c) ;//����
    IIC_Wait_Ack1() ;
    IIC_Stop1();
}
/******************************************************************************/
// ��:ADS1110 ���ܺ���
// �������:none
// ����:none
 
/*******************************************************************************/
 
u16 READ_ADS1110(u8 daddr)
{
   u16 buff;


	//volatile u8 TMTEMP;
    IIC_Start1();
    IIC_Send_Byte1(daddr) ;//����ַ
    IIC_Wait_Ack1() ; 
    buff = IIC_Read_Byte1(1)<<8;
        IIC_Ack1(); //
  buff+= IIC_Read_Byte1(1);//������ת�����ݴ���16λ������
    IIC_Wait_Ack1(); //

   
    IIC_Stop1();

return buff;
}
/*void Get_AdsData(void)
{ 
    //IIC_Init();
    WRITE_ADS1110(0x8c);//���üĴ��� Ĭ��״ֵ̬
    READ_ADS1110();
}*/
//���üĴ���Ϊ0X8C������������ת����16λ���ݽ��������Ϊ1��
//�������ĵ�ѹ V=ADS_DATA.TMdata*2.048/32768

void IIC2_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 �����
}




void IIC_Start1(void)
{
	SDA_OUT1();     //sda�����
	IIC_SDA1=1;	  	  
	IIC_SCL1=1;
	delay_us(4);
 	IIC_SDA1=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL1=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop1(void)
{
	SDA_OUT1();//sda�����
	IIC_SCL1=0;
	IIC_SDA1=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL1=1; 
	IIC_SDA1=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack1(void)
{
	u8 ucErrTime=0;
	SDA_IN1();      //SDA����Ϊ����  
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
	IIC_SCL1=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte1(u8 txd)
{                        
    u8 t;   
	SDA_OUT1(); 	    
    IIC_SCL1=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA1=1;
		else
			IIC_SDA1=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL1=1;
		delay_us(2); 
		IIC_SCL1=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte1(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN1();//SDA����Ϊ����
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
        IIC_NAck1();//����nACK
    else
        IIC_Ack1(); //����ACK   
    return receive;
}






//ͷ�ļ�DO.H
#include "sys.h"

 
#define SDA_IN1()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<12;}
#define SDA_OUT1() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)3<<12;}  
 
//����ߵ͵�ƽҲ�������·���.
// #define SCL_H()   GPIO_SetBits(GPIOB, GPIO_Pin_6)
// #define SCL_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_6)
// #define SDA_H()    GPIO_SetBits(GPIOB, GPIO_Pin_7)
// #define SDA_L()    GPIO_ResetBits(GPIOB, GPIO_Pin_7)
//#define READ_SDA() GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
 #define IIC_SCL1    PBout(10) //SCL
#define IIC_SDA1    PBout(11) //SDA	 
#define READ_SDA1   PBin(11)  //����SDA 
//��Ȼ����ĺ궨��ͬ������.
void IIC2_Init(void);
void ADS1110_Init(void);                             
void IIC_Start1(void);               
void IIC_Stop1(void);                
void IIC_Send_Byte1(u8 txd);         
u8 IIC_Read_Byte1(unsigned char ack);
u8 IIC_Wait_Ack1(void);              
void IIC_Ack1(void);                 
void IIC_NAck1(void);                
void IIC_Write_One_Byte1(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte1(u8 daddr,u8 addr);   
void Get_AdsData1(void);
void Config_ADS1110(u8 daddr);
u16 READ_ADS1110(u8 daddr);
	 u16 gather_u(void);
 u16 gather_i(void);
 
//�°벿��ΪADS1110��غ���,IIC��Э�鶼�ǳ��õ�,������ֱ����ԭ�Ӹ�Ĵ���,�ڴ�����һ��.
//���õ�ADS�ͺ�ΪEDO,�ο��ֲ�ʶ�д��ַ Ϊ1001 000 X,XΪ��дλ,��1д0; 


 
















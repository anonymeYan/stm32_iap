
#include "ltc6803-2.h"		 					//����LTC6803�Ĵ�������ͷ�ļ�
#include "ntc.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
u8 CFGR[6];    //���üĴ�����

u8 CVR[19];    //��ص�ѹ�Ĵ�����

u8 TMPR[5];     //�¶ȼĴ�����

u16 MCxI_CellNumb;
u8 LTC6803_Ini_OK;
u8 LTC6803_Ini_OK1;
struct flagreg{
u8 byte;

}Readflag[3];

u8 LTC6803Err_Counter=0;      //������Ŀ
u8 LTC6803Err_Flag;         //�����־λ
extern u16 CellT[2];         //
extern u16 CellV[8];
extern u16 Total_CellV;    //�ܵ�ѹ
extern u16 CellV_Buff[8]; //��ص�ѹ������
 u8 Check[6]={0};
extern u16 CellT_Buff[2];          //�¶ȴ洢��
 extern	u16 TempT[2];
 extern float T1,T2;
 
 
 

void LTC_Init(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );//PORTBʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // PB12 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
 
        LTC6803CS=1;				//SPI FLASH��ѡ��
	SPI2_Init();		   	//��ʼ��SPI
	SPI2_SetSpeed(SPI_BaudRatePrescaler_32);//����Ϊ18Mʱ��,����ģʽ

	//	LTC6803_ReadID();//��ȡFLASH ID.  

}  


void  LTC6803Init(u8 adder,u8 pec)		 					//��LTC6803�ĳ�ʼ������
{
	u8 i;
	u8 Err,CRC_Temp;
	u16 MCxI_Temp;  

	CFGR[0]=0xe9;				//�ؿ��Ź�
	CFGR[1]=0x00;      		//�ŵ翪��0x00
	CFGR[2]=0x00;		//ʹ���ж϶������еĵ��(����λ)	

	CFGR[3]=0xf0;          //5-12��صı�־λ
	CFGR[4]=0;          //��ѹ�Ƚϵ�ѹ
	CFGR[5]=0xff;            //��ѹ�Ƚϵ�ѹ
	
	
	//�������ü�����Ч��ؽ���
	MCxI_Temp=CFGR[3];      //MCxI��8λ
	MCxI_Temp<<=4;
	MCxI_Temp|=(CFGR[2]&0xF0)>>4;  //MCxI��4λ
	MCxI_CellNumb=0;
	for(i=0;i<12;i++)
	{
	 if((MCxI_Temp&0x0001)==0) 
	 {
	  MCxI_CellNumb++;      //��Ч��ؽ�������
		MCxI_Temp>>=1;        //MCxI���浥Ԫ����λ
	 }		
	}
	//���üĴ����ĳ�ʼ��
	
	LTC6803CS=0;     								//LTC6803Ƭѡ�ź��õ�
  SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	delay_us(2);	
	SPI2_ReadWriteByte(pec); 
		delay_us(2);
	SPI2_ReadWriteByte(WRCFG);   								//Write configuration
	delay_us(2);                    	//��ʱ2us�ӳ���     
	SPI2_ReadWriteByte(WRCFG_CRC);   						//Write CRC 
	delay_us(2);                    	//��ʱ2us�ӳ���   	
	for(i=0;i<6;i++)
	{
		SPI2_ReadWriteByte(CFGR[i]); 
		
	}		
  CRC_Temp=CRC8(CFGR,6);            //����CFGR��Ԫ��CRC��
	SPI2_ReadWriteByte(CRC_Temp);               //дCRCУ���ֽ�
	delay_us(2);                    	//��ʱ2us�ӳ���     
	LTC6803CS=1;        			    	//LTC6803Ƭѡ�ź��ø�
	delay_us(50);                    //��ʱ0.1ms�ӳ���         
	//��ȡLTC6803�Ĵ������������Ƿ���ȷ
	LTC6803CS=0;     					    	//LTC6803Ƭѡ�ź��õ�	
	SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	delay_us(2);	
	SPI2_ReadWriteByte(pec); 
		delay_us(2);
	
	SPI2_ReadWriteByte(RDCFG);
	delay_us(2);                    	//��ʱ5us�ӳ���     
	SPI2_ReadWriteByte(RDCFG_CRC);              //Write CRC  
	delay_us(2);                    	//��ʱ5us�ӳ���   
	for(Err=0;Err<6;Err++)			    	//��ȡд��ļĴ���ֵ
	{
		Check[Err]=SPI2_ReadWriteByte(0xff);

		if((Check[Err]!=CFGR[Err]))  //���Ĳ��Ծ����¶�
		{
			Err=0;
			LTC6803Err_Counter++;
			break;
		}
	}	LTC6803CS=1;        			    	//LTC6803Ƭѡ�ź��ø�
	//SPI2_ReadWriteByte(0xff);


//	if(CFGR[1]||(CFGR[2]&0x0F)!=0)    //�зŵ翪�ؽ�ͨ
//		LPC_GPIO2->DATA &= ~(1ul<<8);   //p2.8�õͷŵ翪��ָʾ���� 
if(adder==0x80)	
{if(Err!=0)
		LTC6803_Ini_OK=1;			    			//LTC6803��ʼ���ɹ�
	else
		LTC6803_Ini_OK=0;			    			//LTC6803��ʼ��ʧ��
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
//��������:��ȡLTC6803��ص�ѹ�Ĵ���
//filename:ReadCellVol(void)
//================================================================
void ReadCellVol(u8 adder,u8 pec)          //12λ��ص�ѹ
{
	u8 a,b,c,CRC_Cal;
/*	if(!(LTC6803_Ini_OK||LTC6803_Ini_OK1))		
	{
	  LTC6803Init(adder,pec);			              //���³�ʼ��LTC6803
		if(LTC6803_Ini_OK==0)
		{
			LTC6803Err_Counter++;	
			if(LTC6803Err_Counter>=5)
				 LTC6803Err_Flag=1;           //������ʼ��5������û�ɹ��򱨴�
		}
		else    //5��֮�����³�ʼ���ɹ���˵��LTC6803������
		{
			LTC6803Err_Counter=0;
			LTC6803Err_Flag=0;              //LTC6803��ʼ�������־���
		}
	}		
	else	 //LTC6803�Ѿ�������ʼ��,��ǰ�����ȡ��ѹֵ
	{*/
		LTC6803CS=0;     								//LTC6803Ƭѡ�ź��õ�
SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	delay_us(2);	
		SPI2_ReadWriteByte(pec); 
			delay_us(2);
		SPI2_ReadWriteByte(STCVAD);    					//start cell conversion for all cells
			delay_us(2);
		SPI2_ReadWriteByte(STCVAD_CRC);    
		LTC6803CS=1;        			    	//LTC6803Ƭѡ�ź��ø�
		delay_us(20);                     //��ʱ20ms
	  LTC6803CS=0;     								//LTC6803Ƭѡ�ź��õ�
			SPI2_ReadWriteByte(adder);    					//start cell conversion for all cells
	
	delay_us(2);
		SPI2_ReadWriteByte(pec); 
			delay_us(2);
		SPI2_ReadWriteByte(RDCV); 									//read cell voltages command
			delay_us(2);
		SPI2_ReadWriteByte(RDCV_CRC); 
		for(a=0;a<19;a++)									//����������ֵ������ص�ѹ�Ĵ�����
		{
			CVR[a]=SPI2_ReadWriteByte(0xff);
	
		}
		LTC6803CS=1;        			    	//LTC6803Ƭѡ�ź��ø�
		//����ص�ѹ�Ĵ�����ת����ʵ�ʵĵ�ص�ѹֵ3���Ĵ�����Ӧ������ص�ѹֵ
		CRC_Cal=CRC8(CVR,18);							//����У��  
		if(CRC_Cal==CVR[18])							//CRCУ����ȷ
		{
			for(b=0;b<MCxI_CellNumb/2;b++)	//��ص�ѹ��������(MCxI_CellNumb/2)
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
				CellV[c]=(CellV[c]-512)*15/10;	 		   //��ѹ����������С�����3λ��ȷ��mV

			  Total_CellV+=CellV[c];                     //����LCU�ܵ�ص�ѹֵ�����
		
			}
		}		

	}	




//================================================================
//��������:��ȡLTC6803����¶�
//filename:ReadTemp(void)
//================================================================
void ReadTemp(u8 adder,u8 pec)
{
	u8 j;


	if(!LTC6803_Ini_OK)		
	{
	  LTC6803Init(adder,pec);			              //���³�ʼ��LTC6803
		if(LTC6803_Ini_OK==0)
		{
			LTC6803Err_Counter++;	
			if(LTC6803Err_Counter>=5)
				 LTC6803Err_Flag=1;           //������ʼ��5������û�ɹ��򱨴�
		}
		else    //5��֮�����³�ʼ���ɹ���˵��LTC6803������
		{
			LTC6803Err_Counter=0;
			LTC6803Err_Flag=0;              //LTC6803��ʼ�������־���
		}
	}		
	else	  //LTC6803�Ѿ�������ʼ��,��ǰ�����ȡ�¶�ֵ
	{
		LTC6803CS=0;     						//LTC6803Ƭѡ�ź��õ�
	SPI2_ReadWriteByte(adder);  			//
	  SPI2_ReadWriteByte(pec);
		SPI2_ReadWriteByte(STTMPAD);  			//start cell conversion for all cells  ��ʼ�����¶�
	  SPI2_ReadWriteByte(STTMPAD_CRC);
		LTC6803CS=1;        			  //LTC6803Ƭѡ�ź��ø�		
		delay_us(20);            		  //��ʱ20ms�ӳ���   		
		LTC6803CS=0;     						//LTC6803Ƭѡ�ź��õ�
SPI2_ReadWriteByte(adder);  			//
	  SPI2_ReadWriteByte(pec);	

		SPI2_ReadWriteByte(RDTMP); 							//read temprature command
		SPI2_ReadWriteByte(RDTMP_CRC); 	
		for(j=0;j<5;j++) 							//����������ֵ�����¶ȼĴ�����
		{
			TMPR[j]=SPI2_ReadWriteByte(0xff);
		}
		LTC6803CS=1;        			  //LTC6803Ƭѡ�ź��ø�
		CellT[0]=TMPR[1];
		CellT[0]<<=8;
		CellT[0]&=0x0F00;
		CellT[0]+=(TMPR[0]);
	
		CellT[1]=TMPR[2];
		CellT[1]<<=4;
		CellT[1]+=TMPR[1]>>4;
		for(j=0;j<2;j++)
		{
			CellT[j]=(CellT[j]-512)*15/10;	          //��ǰ�¶Ȳɼ����ĵ�ѹֵmV
		}
		
		for(j=0;j<2;j++)	 //������¶ȴ�������ѹ��Ӧ�ĵ���ֵR=Vtemp*100/(Vref-Vtemp)
		{
			if(CellT[j]>=3065)    		//Vtemp>3V˵��NTC������Ч
				    TempT[j] = 0x7FFF; 		//��Ч
			else  TempT[j] = CellT[j]*100*10/(3065-CellT[j]);	 //����ֵ��ȷ��R*10				
		}
		//������¶�ֵ �ɵ���ֵת��Ϊ�¶�ֵ(�¶�ϵ��������ݻ���ȷ��)

	T1=GetTempreture(TempT[0]);
		CellT_Buff[0]=(u16)T1-300; 
	  T2=GetTempreture(TempT[1]);	
   	CellT_Buff[1]=(u16)T2-300; 		
	}
}
//================================================================


//  ����CRCУ���뺯��;
//	��  ��:	Ҫ�������ݻ���������ʼָ��pBuff�����������ֽ���Len;
//	��  ��:	CRCУ����;
//	��  ��:	����CRCУ���롣
//------------------------------------------------------------------------------------------------------
//CRC�㷨˵��:
/*У�����ʽΪG(X)=X8+X2+X+1����ÿ�������ֻ���Ϣ�ֵ�ǰ5���ֽڽ���CRCУ�飬����һ��8λ��������������ȡ��
�󼴿���ΪУ����β��ǰ5�ֽڷ��͡����㷽�����£���ÿ�������ֻ���Ϣ���е�ǰ5���ֽڰ��ӵ͵��ߵ�˳�����У�
ÿ���ֽڵ�λ�Ӹߵ������У�����һ��λ���������λ���ĺ������8��0���Ӷ��õ�һ��48λ��λ�������磬��ĳ����Ϣ
����B0-B5��6���ֽڣ�ÿ���ֽ�8λ�ֱ�Ϊbi7-bi0(iΪ�ֽ����,ȡֵ��Χ0~4),������ķ����õ���λ����Ϊ
b07b06...b00��b17b16..b10��...��b47b47...b40��00000000,��У�����ʽ107H (X^8+X^2+X+1)��Ϊ����ȥ�����������
���ʱ������������������õ�һ��8λ������*/
//===============================================================================
u8 CRC8(u8 *buf, int8_t len)   
{   
  u16   i,j=0;   
  u8  crc = 0x41;       //��������루pec��Ԥ��Ϊ0x41 
  
  while(len--!=0)  
	{   
    for(i= 0x80;i!=0;i/=2)         //��8��
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





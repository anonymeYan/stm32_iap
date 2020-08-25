#include "yan.h"
#include "stdio.h"
#include "common.h"
#include "delay.h"

void RS485_Send_D1ata(int ch)
{   
    RS485_TX_EN=1;			//����Ϊ����ģʽ
    delay_ms(5);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    delay_us(5);
    USART_SendData(USART2,(unsigned char)ch);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
    RS485_TX_EN=0;				//����Ϊ����ģʽ
}

void Iap_RS485_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PA1�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ

    USART_InitStructure.USART_BaudRate = bound;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;///У��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure); ; //��ʼ������

//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //ʹ�ܴ���2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�

    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
    RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
}


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;

};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
int fputc(int ch,FILE *f)
{
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
    RS485_Send_D1ata(ch);
    while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);
    return (ch);
}
#endif





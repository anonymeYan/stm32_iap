#ifndef __modbus_h
#define __modbus_h
#include "sys.h"

#define RS485_TX_EN  PAout(1)

void Modbus_RegMap(void);
void RS485_Init(void);
void Timer7_Init(void);
void RS485_SendData(u8 *buff,u8 len);
void USART2_IRQHandler(void);
void TIM7_IRQHandler(void);
void RS485_Service(void);
void Modbus_02_Solve(void);
void Modbus_01_Solve(void);
void Modbus_05_Solve(void);
void Modbus_15_Solve(void);
void Modbus_03_Solve(void);
void Modbus_06_Solve(void);
void Modbus_16_Solve(void);

#endif

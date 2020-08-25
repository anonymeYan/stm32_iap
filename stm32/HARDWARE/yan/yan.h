#ifndef yan_H
#define yan_H
#include "sys.h"
#include "delay.h"

#define RS485_TX_EN		PAout(1)	//485模式控制.0,接收;1,发送.
void Iap_RS485_Init(u32 bound);




#endif


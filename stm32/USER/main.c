#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "wdg.h"
#include "common.h"
#include "iap.h"
#include "yan.h"

u8 updataMark=0;

int main(void)
{
    Start_Init();
    TIM3_Int_Init(9999,7199);         //定时器1s
    delay_init();	    	 //延时函数初始化
    Iap_RS485_Init(9600);
//    IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
    while(1)
    {
        switch(IAP_FLASH_ReadFlag())
        {
        case APPRUN_FLAG_DATA://jump to app
            if( IAP_RunApp())
            {
                if(IAP_Recovery())
                    IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
            }
            break;
        case INIT_FLAG_DATA://initialze state (blank mcu)
            IAP_Main_Menu();
            break;
        case UPDATE_FLAG_DATA:// download app state
            if( !IAP_Update())
                IAP_FLASH_WriteFlag(APPRUN_FLAG_DATA);
            else
                IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
            break;
        case ERASE_FLAG_DATA:// erase app state
            IAP_Erase();
            IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
            break;
        case COPY_FLAG_DATA:// copy app state
            if( !IAP_Copy())
                IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
            else
                IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
            break;
        default:
            IAP_FLASH_WriteFlag(INIT_FLAG_DATA);
            break;
        }
    }
}




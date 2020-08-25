#ifndef LTC6803_H
#define LTC6803_H
#include "sys.h"



//÷∏¡Ó±Ì
#define WRCFG      0x01          //Write Configuration Register Group
#define RDCFG      0x02          //Read Configuration Register Group  
#define RDCV       0x04          // Read All Cell Voltage Group 
#define RDCVA      0x06           //Read Cell Voltages 1-4
#define RDCVB      0x08                //   
#define RDCVC      0x0A                //  
#define RDFLG      0X0C          //Read Flag Register Group
#define RDTMP      0X0E          //Read Temperature Register Group 
#define STCVAD     0X10          //Start Cell Voltage ADC Conversions and Poll Status   
#define STOWAD     0X20          //Start Open-Wire ADC Conversions and Poll Status
#define STTMPAD    0X30          //Start Temperature ADC Conversions and Poll Status
#define PLADC      0X40          //Poll ADC Converter Status 
#define PLINT      0X50          //Poll Interrupt Status º‡≤‚  
#define DAGN       0X52          //Start Diagnose and Poll Status 
#define RDDGNR     0X54          //Read Diagnostic Register
#define STCVDC     0X60          //Start Cell Voltage ADC Conversions and Poll Status,with Discharge Permitted 
#define STOWDC     0X70          //Start Open-Wire ADC Conversions and Poll Status,with Discharge Permitted
#define	LTC6803CS 		PBout(12)   //ltc6803∆¨—°

#define WRCFG_CRC  0xc7                      //
#define RDCFG_CRC  0xce               //
#define RDCV_CRC   0xdc                    //   
#define RDFLG_CRC  0xe4                    //    
#define RDTMP_CRC  0xea                    //
#define STCVAD_CRC 0xb0                     //   
#define STOWAD_CRC  0x20                    // 
#define STTMPAD_CRC  0x50                    //
#define PLADC_CRC  0x07
#define PLINT_CRC  0x77
#define DAGN_CRC   0x79
#define RDDGNR_CRC 0x6b
#define STCVDC_CRC 0xe7
#define STOWDC_CRC 0x97
#define LTC6803CLK PBout(13)
#define LPC1114_MOSI PBout(15)
#define LPC1114_MISO_PIN PBout(14)


void LTC6803Init(u8 adder,u8 pec);

void ReadFlagReg(void);
void ReadCellVol(u8 adder,u8 pec);

void ReadTemp(u8 adder,u8 pec);
void Check_Status(void);
u8 CRC8(u8 *buf, int8_t len) ;
void LTC_Init(void);




#endif






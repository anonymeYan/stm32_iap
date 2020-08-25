#ifndef __IAP_CONFIG_H__
#define __IAP_CONFIG_H__

/* Define the APP start address -------------------------------*/
#define ApplicationAddress    0x8010000
#define BufAddress    0x8028000
/* Output printer switch --------------------------------------*/
#define ENABLE_PUTSTR         1

/* Bootloader command -----------------------------------------*/
#define CMD_UPDATE_STR        "update"
#define CMD_UPLOAD_STR        "upload"
#define CMD_ERASE_STR		      "erase"
#define CMD_MENU_STR          "menu"
#define CMD_RUNAPP_STR        "runapp"
#define CMD_COPYAPP_STR        "copyapp"
#define CMD_ERROR_STR         "error"
#define CMD_DISWP_STR         "diswp"//��ֹд����

/* IAP command------------------------------------------------ */
#define IAP_FLASH_FLAG_ADDR   (uint32_t)(ApplicationAddress - 1024 * 2)//App�����Bootloader��������Ϣ�ĵ�ַ(�ݶ���СΪ2K)
//#define IAP_FLASH_FLAG_ADDR   0x8002800
#define APPRUN_FLAG_DATA      0x0000   //APP����Ҫ���κδ���ֱ������״̬
#define INIT_FLAG_DATA        0xFFFF   //Ĭ�ϱ�־������(��Ƭ�ӵ����)
#define UPDATE_FLAG_DATA      0xEEEE   //���ر�־������
#define UPLOAD_FLAG_DATA      0xDDDD   //�ϴ���־������
#define ERASE_FLAG_DATA       0xCCCC   //������־������
#define COPY_FLAG_DATA        0xC0C0   //������־������
/* Define the Flsah area size ---------------------------------*/  
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
 #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
 #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#elif defined STM32F10X_CL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_HD || defined (STM32F10X_HD_VL)
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
 #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_XL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x100000) /* 1 MByte */
#else 
 #error "Please select first the STM32 device to be used (in stm32f10x.h)"    
#endif

/* Compute the FLASH upload image size --------------------------*/  
#define FLASH_IMAGE_SIZE                   (uint32_t) (FLASH_SIZE - (ApplicationAddress - 0x08000000))

/* The maximum length of the command string -------------------*/
#define CMD_STRING_SIZE       128

#endif

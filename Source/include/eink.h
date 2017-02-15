#ifndef X_EINK_H
#define X_EINK_H

#include <FreeRTOS.h>
#include <task.h>

void RCCInit(void);
void GPIOInit(void);

#define SDA_H  GPIO_SetBits(GPIOD, GPIO_Pin_10)	          //PD.10
#define SDA_L  GPIO_ResetBits(GPIOD, GPIO_Pin_10)

#define SCLK_H GPIO_SetBits(GPIOD, GPIO_Pin_9)	          //PD.9
#define SCLK_L GPIO_ResetBits(GPIOD, GPIO_Pin_9)

#define nCS_H  GPIO_SetBits(GPIOD, GPIO_Pin_8)            //PD.8
#define nCS_L  GPIO_ResetBits(GPIOD, GPIO_Pin_8)

#define nDC_H  GPIO_SetBits(GPIOE, GPIO_Pin_15)           //PE.15
#define nDC_L  GPIO_ResetBits(GPIOE, GPIO_Pin_15)

#define nRST_H GPIO_SetBits(GPIOE, GPIO_Pin_14)           //PE.14
#define nRST_L GPIO_ResetBits(GPIOE, GPIO_Pin_14)

#define nBUSY  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13)

#define nBS_H  GPIO_SetBits(GPIOE, GPIO_Pin_11);	 	  //PE.11
#define nBS_L  GPIO_ResetBits(GPIOE, GPIO_Pin_11);

#define VPP_H  GPIO_SetBits(GPIOA, GPIO_Pin_4);			  //PA4
#define VPP_L  GPIO_ResetBits(GPIOA, GPIO_Pin_4);         //PA4

void SPI4W_WRITECOM(unsigned char INIT_COM);
void SPI4W_WRITEDATA(unsigned char INIT_DATA);
void SPI4W_WRITE(unsigned char INIT_COM,unsigned char INIT_DATA);
void MYRESET(void);
void DELAY_100nS(size_t t);
#define DELAY_mS(t) vTaskDelay(t/portTICK_RATE_MS)
#define DELAY_S(t) DELAY_mS(1000*t)
#define DELAY_M(t) DELAY_S(60*t)

void lcd_chkstatus(void);
void EPD_W21_Init(void);
void SPI4W_WRITECOM(unsigned char INIT_COM);
void SPI4W_WRITEDATA(unsigned char INIT_DATA);
void MCU_write_flash(unsigned char command);
void displayImage(const unsigned char *Image);
void transferImage(const unsigned char *Image);

#endif

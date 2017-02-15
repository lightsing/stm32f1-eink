#ifndef X_LED_H
#define X_LED_H

#include <stm32f10x.h>

#define LED_GPIO     GPIOC

#define LED_1        GPIO_Pin_13
#define LED_2        GPIO_Pin_1
#define LED_3        GPIO_Pin_2
#define LED_4        GPIO_Pin_3

#define LEDInit      AllLEDoff
#define LEDoff(xled) GPIO_SetBits(LED_GPIO, xled)
#define LEDon(xled)  GPIO_ResetBits(LED_GPIO, xled)

void AllLEDoff(void);
void AllLEDon(void);
#endif

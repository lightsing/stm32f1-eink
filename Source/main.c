//STM32F103 Define
#include <stm32f10x.h>
//FreeRTOS Define
#include <FreeRTOS.h>
#include <task.h>
//User Define
#include <eink.h>
#include <img0.h>

void vTaskDisplayImage(void *p);
void vTaskBlinkLED(void *p);

int main(void)
{
	//Init
    RCCInit();
	GPIOInit();

    xTaskCreate(vTaskDisplayImage, (const char*)"Display Image", 128, NULL, 2, NULL);
    xTaskCreate(vTaskBlinkLED, (const char*)"IDLE", 128, NULL, 1, NULL);
    
	vTaskStartScheduler();
	return 0;
}

void vTaskDisplayImage(void *p){
    for(;;){
        displayImage(img0);
    }
}

void vTaskBlinkLED(void *p){
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    GPIO_SetBits(GPIOC, GPIO_Pin_1);
    GPIO_SetBits(GPIOC, GPIO_Pin_2);
    GPIO_SetBits(GPIOC, GPIO_Pin_3);
    for(;;){
        GPIOC->ODR ^= GPIO_Pin_13;
        vTaskDelay(100/portTICK_RATE_MS);
    }
}

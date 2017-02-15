#include <led.h>

void AllLEDoff(void){
    LEDoff(LED_1);
    LEDoff(LED_2);
    LEDoff(LED_3);
    LEDoff(LED_4);
}

void AllLEDon(void){
    LEDon(LED_1);
    LEDon(LED_2);
    LEDon(LED_3);
    LEDon(LED_4);
}

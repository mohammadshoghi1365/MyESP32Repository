#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"


#define LED_PIN 2 


void led_blink_task(void *pvParameters)
{
    
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
       
        gpio_set_level(LED_PIN, 1);
        printf("LED ON\n");
        vTaskDelay(300 / portTICK_PERIOD_MS); 
        // vTaskDelay(pdMS_TO_TICKS(300));

        gpio_set_level(LED_PIN, 0);
        printf("LED OFF\n");
        vTaskDelay(300 / portTICK_PERIOD_MS); 
        // vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void app_main(void)
{
    
    // xTaskCreate(&blink_task, "blink_task", 2048, NULL, 5, NULL);

    xTaskCreate(
        led_blink_task,      // Task function
        "LED_Blink_Task",    // Task name for debugging & log 
        2048,                // Stack size based on BYTE
        NULL,                // Task input parameter 
        5,                   // Task priority (high priority = high number --> 0 is lower priority)
        NULL                 // Task Handle
    );

    printf("LED Blink Task started !\n");

    while(1)
    {
        // do nothing ...
    }
}

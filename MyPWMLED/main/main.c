#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LED_GPIO        2                 // Pin number
#define PWM_FREQ_HZ     5000              // PWM frequency (5kHz)
#define PWM_CHANNEL     LEDC_CHANNEL_0    // PWM channel
#define PWM_TIMER       LEDC_TIMER_0      // PWM Timer
#define PWM_RESOLUTION  LEDC_TIMER_10_BIT // PWM resolution (10 Bit - it means 0 ~ 1023) 

void pwm_task(void *pvParameters) 
{
    // LEDC timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE, // High speed mode
        .timer_num        = PWM_TIMER,
        .duty_resolution  = PWM_RESOLUTION,       // 10 Bit resolution
        .freq_hz          = PWM_FREQ_HZ,          // Frequency
        .clk_cfg          = LEDC_AUTO_CLK         // Auto select clock
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // LEDC channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_HIGH_SPEED_MODE,
        .channel        = PWM_CHANNEL,
        .timer_sel      = PWM_TIMER,
        .intr_type      = LEDC_INTR_DISABLE, // Interrupt Disable
        .gpio_num       = LED_GPIO,
        .duty           = 0,                 // Init value of duty cycle (OFF).
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

 
    int duty = 0;
    int step = 10; // Duty Cycle change step
    while (1) 
    {
        // Increase light
        for (duty = 0; duty <= 1023; duty += step) 
        {
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL));
            vTaskDelay(25 / portTICK_PERIOD_MS); 
        }
        // Decrease light
        for (duty = 1023; duty >= 0; duty -= step) 
        {
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL));
            vTaskDelay(25 / portTICK_PERIOD_MS); 
        }
    }
}

void app_main(void) 
{
    xTaskCreate(pwm_task, "pwm_task", 2048, NULL, 5, NULL);
}

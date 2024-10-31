#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

//#define PWM_PIN 0 // OK
#define PWM_PIN 1 // OK
//#define PWM_PIN 2 // NOT OK
//#define PWM_PIN 4 // NOT OK
//#define PWM_PIN 8 // NOT OK
//#define PWM_PIN 14
//#define PWM_PIN 27
// ADC GPIO
//   0   26
//   1   27
//   2   28 
//   3   29 
#define ADC_INPUT 3
#define ADC_GPIO_PIN 29

float convert_to_temperature(uint16_t adc_value) {
    /*
    float voltage = adc_value * (3300.0f / 4096);
    return 25.0f + (voltage - 750.0f) / 10.0f;
    */
    float voltage = adc_value * (3.3f / 4096);
    return voltage * 100.0f - 5.0f; // adjust by calibration
}

int main() {
    stdio_init_all();
    printf("pwmfan start\n");

    // Initialize PWM
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_wrap(slice_num, 65535); // PWM range
    pwm_set_chan_level(slice_num, PWM_PIN, 32767); // 50% duty cycle
    pwm_set_enabled(slice_num, true);

    // Initialize ADC
    adc_init();
    adc_gpio_init(ADC_GPIO_PIN);
    adc_select_input(ADC_INPUT);

    while (1) {
        // Read temperature from TMP36 sensor
        uint16_t adc_value = adc_read();
        float temperature = convert_to_temperature(adc_value);
        printf("Temperature: %.2f°C adc %f V\n", temperature, adc_value/4096.0f*3.3f);

        float fan = 0;
        if (temperature > 23) fan = 0.1f;
        if (temperature > 25) fan = 0.3f;
        if (temperature > 30) fan = 0.5f;
        if (temperature > 40) fan = 0.7f;
        if (temperature > 50) fan = 1.0f;
        // Change PWM duty cycle based on temperature
        //uint16_t duty_cycle = adc_value / 4; // Scale ADC value to PWM range

        pwm_set_chan_level(slice_num, PWM_PIN, (int)(65535 * fan));

        sleep_ms(1000); // Delay for 1 second
    }

    return 0;
}


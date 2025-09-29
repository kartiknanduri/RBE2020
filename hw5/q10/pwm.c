#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// ---------------------- Display Setup ---------------------- //
const int pin_map[7] = {19,18,17,16,15,14,13};   // segment pins a–g
const bool digits[10][7] = {                      // segment patterns
    {0,0,0,0,0,0,1}, //0
    {1,0,0,1,1,1,1}, //1
    {0,0,1,0,0,1,0}, //2
    {0,0,0,0,1,1,0}, //3
    {1,0,0,1,1,0,0}, //4
    {0,1,0,0,1,0,0}, //5
    {0,1,0,0,0,0,0}, //6
    {0,0,0,1,1,1,1}, //7
    {0,0,0,0,0,0,0}, //8
    {0,0,0,1,1,0,0}, //9
};
const int segment[4] = {9,10,11,12}; // digit enable pins
const int dec_pin = 20;              // decimal point
const bool decimal[4] = {1,1,0,1};   // only third digit has decimal ON

// ---------------------- ADC & PWM Setup ---------------------- //
const int ADC_PIN = 26;  // potentiometer on ADC0 (GPIO26)
const int PWM_A = 0;     // H-bridge input A (GPIO0)
const int PWM_B = 1;     // H-bridge input B (GPIO1)

const int BUTTON = 2;    // button with pull-up on GPIO2

// ---------------------- Helper Functions ---------------------- //
void display_num(int num) {  // write one digit
    for (int i = 0; i < 7; i++) {
        gpio_put(pin_map[i], digits[num][i]);
    }
}

void display_numbers(int digs[4]) {  // multiplex digits
    for (int i = 0; i < 4; i++) {
        display_num(digs[i]);
        gpio_put(dec_pin, decimal[i]);   // set decimal
        gpio_put(segment[i], 1);         // turn digit on
        sleep_ms(5);                     // small on-time
        gpio_put(segment[i], 0);         // turn digit off
    }
}

// ---------------------- Main ---------------------- //
int main() {
    stdio_init_all();

    // ---- Initialize 7-seg display pins ---- //
    for (int i = 0; i < 7; i++) {
        gpio_init(pin_map[i]);
        gpio_set_dir(pin_map[i], GPIO_OUT);
    }
    gpio_init(dec_pin);
    gpio_set_dir(dec_pin, GPIO_OUT);
    for (int i = 0; i < 4; i++) {
        gpio_init(segment[i]);
        gpio_set_dir(segment[i], GPIO_OUT);
    }

    // ---- Initialize ADC ---- //
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);   // ADC0 → GPIO26

    // ---- Initialize PWM on GPIO0 & GPIO1 ---- //
    gpio_set_function(PWM_A, GPIO_FUNC_PWM);  // GPIO0
    gpio_set_function(PWM_B, GPIO_FUNC_PWM);  // GPIO1

    uint slice = pwm_gpio_to_slice_num(PWM_A);  // both GPIO0 & GPIO1 use slice 0

    float clkdiv = 1.0f;
    uint16_t wrap = 14999; // 10 kHz
    pwm_set_clkdiv(slice, clkdiv);
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);


    pwm_set_clkdiv(slice, clkdiv);
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);

    // ---- Initialize Button ---- //
    gpio_init(BUTTON);
    gpio_pull_up(BUTTON);  // enable internal pull-up
    gpio_set_dir(BUTTON, GPIO_IN);

    bool direction = true;  // motor starts forward
    bool last_button = true;

    const float VREF = 3.3f;

    while (true) {
        // ---- Read potentiometer ---- //
        uint16_t raw = adc_read();
        float voltage = (raw * VREF) / 4095.0f;
        float duty_pct = (voltage / VREF) * 100.0f;
        if (duty_pct > 100.0f) duty_pct = 100.0f;

        // ---- Set PWM level ---- //
        uint16_t level = (uint16_t)((duty_pct / 100.0f) * (wrap + 1));

        // ---- Handle button press (toggle direction) ---- //
        bool button_state = gpio_get(BUTTON); // HIGH = not pressed
        if (!button_state && last_button) {   // detect falling edge
            direction = !direction;           // toggle direction
            sleep_ms(200);                    // debounce
        }
        last_button = button_state;

        // ---- Drive H-bridge with PWM ---- //
        if (direction) {
            // Forward: PWM on GPIO0 (channel A), LOW on GPIO1
            pwm_set_chan_level(slice, PWM_CHAN_A, level);  // PWM on A
            pwm_set_chan_level(slice, PWM_CHAN_B, 0);      // B off
        } else {
            // Reverse: PWM on GPIO1 (channel B), LOW on GPIO0
            pwm_set_chan_level(slice, PWM_CHAN_A, 0);      // A off
            pwm_set_chan_level(slice, PWM_CHAN_B, level);  // PWM on B
        }

        // ---- Prepare duty cycle for display ---- //
        int d = (int)(duty_pct * 10 + 0.5);  // show one decimal place
        if (d > 9999) d = 9999;

        int digs[4];
        digs[0] = d / 1000;
        digs[1] = (d % 1000) / 100;
        digs[2] = (d % 100) / 10;
        digs[3] = d % 10;

        // ---- Update 7-seg display ---- //
        for (int rep = 0; rep < 5; rep++) {
            display_numbers(digs);
        }
    }
    return 0;
}

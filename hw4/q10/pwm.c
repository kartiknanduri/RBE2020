#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

const int pin_map[7] = {19,18,17,16,15,14,13}; // allows me to call each pin sequentially by mapping them to numbers 0-6
const bool digits[10][7] = { // the patterns of LED to display each number in an array
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

const int segment[4] = {9,10,11,12}; //maps digit pin numbers to an array thats easily called
const int dec_pin = 20; // assigns decimal point pin to pin 20
const bool decimal[4] = {1,1,0,1}; // assigns the decimal point to the correct pin
const int ADC_PIN = 26; // assigns adc pin to pin 26
const int PWM_PIN = 0; // assigns pwm pin to pin 0

void display_num(int num) { //function to display each number on a 7 segment display
    for (int i = 0; i < 7; i++) {
        gpio_put(pin_map[i], digits[num][i]);
    }
}
void display_numbers(int digs[4]) {    // multiplexes through the numbers rapidly
    for (int i = 0; i < 4; i++) {
        display_num(digs[i]);
        gpio_put(dec_pin, decimal[i]);   // turn decimal on/off for this digit
        gpio_put(segment[i], 1);         // choose the digit being addressed
        sleep_ms(5);                     // keep it lit for 5 ms
        gpio_put(segment[i], 0);         // turn digit off
    }
}




int main() {
    stdio_init_all();
    for (int i = 0; i < 7; i++) {     // initialize LED GPIO pins 
        gpio_init(pin_map[i]);
        gpio_set_dir(pin_map[i], GPIO_OUT);
    }
    gpio_init(dec_pin);              // initialize decimal pin
    gpio_set_dir(dec_pin, GPIO_OUT);
    for (int i = 0; i < 4; i++) {   // initialize digit GPIO pins
        gpio_init(segment[i]);
        gpio_set_dir(segment[i], GPIO_OUT);
    }

    adc_init();              // initalize adc
    adc_gpio_init(ADC_PIN);  // tells pin 26 to be an adc pin
    adc_select_input(0);     // select ADC0

    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);    //initialize pwm on pwm pin
    uint slice = pwm_gpio_to_slice_num(PWM_PIN);
  
    // Configure PWM for 10 kHz
    // f_pwm = f_sys / (clkdiv * (wrap + 1))
    // f_sys = 150 MHz (default)
    
    const uint PWM_GPIO = 0;  // set pin 0 to pwm output

    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM);
    
    float clkdiv = 1.0f;
    uint16_t wrap = 14999; // gives 10 kHz
    pwm_set_clkdiv(slice, clkdiv);
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);

    const float VREF = 3.3f;

    while (true) {
        
        uint16_t raw = adc_read();              //reads voltage
        float voltage = (raw * VREF) / 4095.0f;

        
        float duty_pct = (voltage / VREF) * 100.0f;  // calculate duty cycle
        if (duty_pct > 100.0f) duty_pct = 100.0f;


        uint16_t level = (uint16_t)((duty_pct / 100.0f) * (wrap + 1));  // sets pwm 
        pwm_set_chan_level(slice, PWM_CHAN_A, level);

        int d = (int)(duty_pct * 10 + 0.5); // converts duty cycle to 4 digits
        if (d > 9999) d = 9999;

        int digs[4];    // creates an array for digits and assigns them based on the number
        digs[0] = d / 1000;           
        digs[1] = (d % 1000) / 100;   
        digs[2] = (d % 100) / 10;     
        digs[3] = d % 10;         

        for (int rep = 0; rep < 5; rep++) {  // repeat to reduce flicker
            display_numbers(digs);            // display the 4 digit number on the display
        }
    }
    return 0;
}

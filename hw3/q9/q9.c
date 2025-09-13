#include "pico/stdlib.h"
#include "pico/time.h"   // needed for repeating timer

const int pin_map[7] = {19,18,17,16,15,14,13};   // map GPIO pins to 7-seg segments

const bool digits[10][7] = {   // segment patterns for 0–9
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

volatile int currnum = 0;  // must be volatile (used in callback)


void display_num(int num){       // display a digit on the 7-segment
    for (int i = 0; i < 7; i++){
        gpio_put(pin_map[i], digits[num % 10][i]);
    }
}


bool num_update_callback(struct repeating_timer *t) {           // timer callback function (runs every interval)
    currnum = (currnum + 1) % 10;   // increment, wrap at 9
    display_num(currnum);
    return true;   // keep repeating
}

int main() {
    for(int i=0; i<7 ; i++){          // initialize segment pins
        gpio_init(pin_map[i]);
        gpio_set_dir(pin_map[i], GPIO_OUT);
        gpio_put(pin_map[i], 1);   // all off at start 
    }
    
    
    struct repeating_timer timer;                  // create repeating timer (every 1000 ms)
    add_repeating_timer_ms(1000, num_update_callback, NULL, &timer);
    
    
    while(true){              // main loop handles button   
    }

    return 0;
}
  

  



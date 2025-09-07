#include "pico/stdlib.h"
//#include "pico/cyw43_arch.h"





int main() {
//    const bool pin_map[7] = {25,24,22,21,20,19,17};     // allows me to call each pin sequentially by mapping them to numbers 0-6

//    const bool digits[10][7] = {        // the patterns of LED to display each number in an array
//      {1,1,1,1,1,1,0}, //0
//      {0,1,1,0,0,0,0}, //1
//      {1,1,0,1,1,0,1}, //2
//      {1,1,1,1,0,0,1}, //3
//      {0,1,1,0,0,1,1}, //4
//      {1,0,1,1,0,1,1}, //5
//      {1,0,1,1,1,1,1}, //6
//      {1,1,1,0,0,0,0}, //7
//      {1,1,1,1,1,1,1}, //8
//      {1,1,1,0,0,1,1}, //9
//      };
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
//    for(int i=0; i<7 ; i++){         //initialize GPIO pins
//        gpio_init(pin_map[i]);
//        gpio_set_dir(pin_map[i], GPIO_OUT);
//        gpio_put(pin_map[i], 1)   
//        }



    while(true) {                      // displays numbers sequentially with 1 second spacing
        //for (int num = 0;num <= 9; num++){
        //  display_num(num);
        //    sleep_ms(1000);
        gpio_put(25,1);
        sleep_ms(500);
        gpio_put(25,0);
        sleep_ms(500);
        //}
    }
}


//void display_num(int num){       //function to display each number 
//    for (int i = 0; i < 7; i++){
//        gpio_put(pin_map[i],digits[num][i]);
//   }}

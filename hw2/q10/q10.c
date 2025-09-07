#include "pico/stdlib.h"

  const int pin_map[7] = {19,18,17,16,15,14,13};     // allows me to call each pin sequentially by mapping them to numbers 0-6
  const bool digits[6][7] = {        // the patterns of LED to display each number in an array
    {0,1,1,1,1,1,1}, //0
    {0,0,1,1,1,1,1}, //1
    {0,0,0,1,1,1,1}, //2
    {0,0,0,0,1,1,1}, //3
    {0,0,0,0,0,1,1}, //4
    {0,0,0,0,0,0,1}, //5
    };
        
void display_num(int num){       //function to display each number 
    for (int i = 0; i < 7; i++){
        gpio_put(pin_map[i],digits[num][i]);
    }
}
int main(){
  for(int i=0; i<7 ; i++){         //initialize GPIO pins
    gpio_init(pin_map[i]);
    gpio_set_dir(pin_map[i], GPIO_OUT);
    gpio_put(pin_map[i], 1);
    }

  while(true){                     // loops through the array and display each line of the array on the display
    for (int num = 0;num < 7; num++){
      for (int num = 0;num <= 5; num++){
        display_num(num);
        sleep_ms(1000/6);
        }
      }  
    }
  return(0);
} 

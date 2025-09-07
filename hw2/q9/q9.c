#include "pico/stdlib.h"

  const int pin_map[7] = {19,18,17,16,15,14,13};     // allows me to call each pin sequentially by mapping them to numbers 0-6
  const bool digits[10][7] = {        // the patterns of LED to display each number in an array
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

  while(true){                     // loops through each line and assigns the LED values to each LED 
    for (int num = 0;num < 7; num++){
      for (int num = 0;num <= 9; num++){
        display_num(num);
        sleep_ms(1000);
        }
      }  
    }
  return(0);
} 
  

  



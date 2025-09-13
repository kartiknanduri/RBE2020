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
  const int btn_pin = 0;
        
void display_num(int num){       //function to display each number 
    for (int i = 0; i < 7; i++){
        gpio_put(pin_map[i],digits[num][i]);
    }
}
int main(){
  for(int i=0; i<7 ; i++){         //initialize LED GPIO pins
    gpio_init(pin_map[i]);
    gpio_set_dir(pin_map[i], GPIO_OUT);
    gpio_put(pin_map[i], 1);      // turn all LEDs off
    }
  gpio_init(btn_pin);             //initialize Button GPIO pin
  gpio_set_dir(btn_pin, GPIO_IN);
  gpio_pull_up(btn_pin);          //set button pin to pullup
  int currnum = 0;                //initializes numbers
  display_num(0);                 //initializes display
  
  while(true){
    bool pressed = !gpio_get(btn_pin);  // inverts logic for the button
    if (pressed){                       // if the button is pressed, it displays the next number
      currnum += 1;
      display_num(currnum%10);
      //sleep_ms(100);
    }

  return(0);    //return does nothing but for standard practice
} 
  

  



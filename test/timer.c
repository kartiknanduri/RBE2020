#include "pico/stdlib.h"
        

int main(){
  gpio_init(19);
  gpio_set_dir(19, GPIO_OUT);
  gpio_put(19, 1);
  

  while(true){ 
    gpio_put(19, 0
    );
    }
  return(0);
} 

#include "pic24_all.h"
#include "revF14.h"


void configure(){
	
	HARDWARE_CONFIG()		
	
	
}

 

int main(void){
	
	
	
  LED1 = 0;
  LED2 = 1;
  LED3 = 0
  while (1) {
    // Delay long enough to see LED blink.
    DELAY_MS(250);
    // Toggle the LED.
    LED1 = !LED1;
    LED2 = !LED2;
    LED3 = !LED3;
    
  } // end while (1)
	
}

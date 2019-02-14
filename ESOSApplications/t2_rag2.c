// Created by Benjamin and Mehedi
// 2/7/2019

#include "esos.h"
#include "esos_pic24.h"
#include "revF14.h"
#include "stdbool.h"

// global variable for every light
bool b_NS_red, b_NS_amber, b_NS_green, b_EW_red, b_EW_amber, b_EW_green;
	
ESOS_USER_TASK(calculate_light_cycle){
	ESOS_TASK_BEGIN(){
		// red is LED1, amber is LED2, green is LED3
		while(TRUE){

			//// state 1
			b_NS_red = true;		
			b_NS_amber = false;		
			b_NS_green = false;		
			
			b_EW_red = false;
			b_EW_amber = false;
			b_EW_green = true;

			if (SW1_PRESSED()) { // rush hour!
				ESOS_TASK_WAIT_TICKS(30000); // delay 30 seconds
			}
			else { // not rush hour!
				ESOS_TASK_WAIT_TICKS(10000); // delay 10 seconds
			}
			
			// state 2
			b_NS_red = true;		
			b_NS_amber = false;		
			b_NS_green = false;		

			b_EW_red = false;
			b_EW_amber = true;
			b_EW_green = false;
			
			ESOS_TASK_WAIT_TICKS(3000); // delay 3 seconds
			
			// state 3 - rush hour only!
			if (SW1_PRESSED()) {
				b_NS_red = true;		b_EW_red = true;
				b_NS_amber = false;		b_EW_amber = false;
				b_NS_green = false;		b_EW_green = false;

				ESOS_TASK_WAIT_TICKS(1000); // delay 1 seconds
			}
			
			// state 4
			b_NS_red = false;		b_EW_red = true;
			b_NS_amber = false;		b_EW_amber = false;
			b_NS_green = true;		b_EW_green = false;
			
			if (SW1_PRESSED()) { // rush hour!
				ESOS_TASK_WAIT_TICKS(30000); // delay 30 seconds
			}
			else { // not rush hour!
				ESOS_TASK_WAIT_TICKS(10000); // delay 10 seconds
			}

			// state 5
			b_NS_red = false;		b_EW_red = true;
			b_NS_amber = true;		b_EW_amber = false;
			b_NS_green = false;		b_EW_green = false;

			ESOS_TASK_WAIT_TICKS(3000); // delay 3 seconds

			// state 6 - rush hour only
			if (SW1_PRESSED()) { // rush hour!
				b_NS_red = true;		b_EW_red = true;
				b_NS_amber = false;		b_EW_amber = false;
				b_NS_green = false;		b_EW_green = false;

				ESOS_TASK_WAIT_TICKS(1000); // delay 1 seconds
			}
			//repeat the cycle
		}
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(display_lights){
	ESOS_TASK_BEGIN(){
		while(true){
			if(SW3_PRESSED()){
				// display EW lights
				/*LED1 = b_EW_red;
				LED2 = b_EW_amber;
				LED3 = b_EW_green;*/

				// Should LED1 be on?
				if (b_EW_red == true) {
					LED1_ON();
				}
				else {
					LED1_OFF();
				}
				// Should LED2 be on?
				if (b_EW_amber == true) {
					LED2_ON();
				}
				else {
					LED2_OFF();
				}
				// should LED3 be on?
				if (b_EW_green == true) {
					LED3_ON();
				}
				else {
					LED3_OFF();
				}
			}
			else{ 
				// display NS lights
				//LED1 = b_NS_red;
				//LED2 = b_NS_amber;
				//LED3 = b_NS_green;
				// Should LED1 be on?
				if (b_NS_red == true) {
					LED1_ON();
				}
				else {
					LED1_OFF();
				}
				// Should LED2 be on?
				if (b_NS_amber == true) {
					LED2_ON();
				}
				else {
					LED2_OFF();
				}
				// should LED3 be on?
				if (b_NS_green == true) {
					LED3_ON();
				}
				else {
					LED3_OFF();
				}
			}
			ESOS_TASK_YIELD();
		}
	}
	ESOS_TASK_END();
} // constantly update the lights in hardware

void user_init(void) {
	
	__esos_unsafe_PutString( HELLO_MSG );
	
	HARDWARE_CONFIG();
	esos_RegisterTask(calculate_light_cycle);
	esos_RegisterTask(display_lights);
}
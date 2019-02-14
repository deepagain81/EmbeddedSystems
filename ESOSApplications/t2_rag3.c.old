// Created by Benjamin and Marcos
// 2/7/2018

#include "esos.h"
#include "esos_pic24.h"
#include "revF14.h"
#include "stdbool.h"

// global variable for every light
bool b_NS_red = false, b_NS_amber = false, b_NS_green = false, b_NS_left = false;
bool b_EW_red = false, b_EW_amber = false, b_EW_green = false, b_EW_left = false;

void blinkLeftTurn() { // function called every 75 milliseconds by a timer
	// note that the task which usually manages the lights now has clauses that cede control if blinking
	// otherwise, it will constantly set the green led to off, defeating the point of blinking
	if (SW3_PRESSED() && b_EW_left){ // display EW left turn
		LED3_TOGGLE();
	}
	if (SW3_RELEASED() && b_NS_left){ // display NS left turn
		LED3_TOGGLE();
	}
}
	
ESOS_USER_TASK(calculate_light_cycle){
	ESOS_TASK_BEGIN(){
		// red is LED1, amber is LED2, green is LED3
		while(TRUE){

			if (SW2_PRESSED()) { // turn signal only if SW2 is pressed
				//// state 1
				b_NS_red = true;		b_EW_red = false;
				b_NS_amber = false;		b_EW_amber = false;
				b_NS_green = false;		b_EW_green = false;
				b_NS_left = false;		b_EW_left = true;

				ESOS_TASK_WAIT_TICKS(10000); // delay 10 seconds
			}
			
			// state 2
			b_NS_red = true;		b_EW_red = false;
			b_NS_amber = false;		b_EW_amber = false;
			b_NS_green = false;		b_EW_green = true;
			b_NS_left = false;		b_EW_left = false;
			
			if (SW1_PRESSED()) { // rush hour!
				ESOS_TASK_WAIT_TICKS(30000); // delay 30 seconds
			}
			else { // not rush hour!
				ESOS_TASK_WAIT_TICKS(10000); // delay 10 seconds
			}
			
			// state 3
			b_NS_red = true;		b_EW_red = false;
			b_NS_amber = false;		b_EW_amber = true;
			b_NS_green = false;		b_EW_green = false;
			b_NS_left = false;		b_EW_left = false;

			ESOS_TASK_WAIT_TICKS(3000); // delay 3 seconds

			// state 4 - rush hour only!
			if (SW1_PRESSED()) {
				b_NS_red = true;		b_EW_red = true;
				b_NS_amber = false;		b_EW_amber = false;
				b_NS_green = false;		b_EW_green = false;
				b_NS_left = false;		b_EW_left = false;

				ESOS_TASK_WAIT_TICKS(1000); // delay 1 seconds
			}
			
			// state 5
			if (SW2_PRESSED()) { // turn signal only if SW2 is pressed
				b_NS_red = false;		b_EW_red = true;
				b_NS_amber = false;		b_EW_amber = false;
				b_NS_green = false;		b_EW_green = false;
				b_NS_left = true;		b_EW_left = false;

				ESOS_TASK_WAIT_TICKS(10000); // delay 10 seconds
			}

			// state 6
			b_NS_red = false;		b_EW_red = true;
			b_NS_amber = false;		b_EW_amber = false;
			b_NS_green = true;		b_EW_green = false;
			b_NS_left = false;		b_EW_left = false;
			
			if (SW1_PRESSED()) { // rush hour!
				ESOS_TASK_WAIT_TICKS(30000); // delay 30 seconds
			}
			else { // not rush hour!
				ESOS_TASK_WAIT_TICKS(10000); // delay 10 seconds
			}

			// state 7
			b_NS_red = false;		b_EW_red = true;
			b_NS_amber = true;		b_EW_amber = false;
			b_NS_green = false;		b_EW_green = false;
			b_NS_left = false;		b_EW_left = false;

			ESOS_TASK_WAIT_TICKS(3000); // delay 3 seconds

			// state 8 - rush hour only
			if (SW1_PRESSED()) { // rush hour!
				b_NS_red = true;		b_EW_red = true;
				b_NS_amber = false;		b_EW_amber = false;
				b_NS_green = false;		b_EW_green = false;
				b_NS_left = false;		b_EW_left = false;

				ESOS_TASK_WAIT_TICKS(1000); // delay 1 seconds
			}
			//repeat the cycle
		}
	} // END ESOS_TASK_BEGIN(){}
	ESOS_TASK_END();
}

ESOS_USER_TASK(display_lights){
	ESOS_TASK_BEGIN(){
		while(true){
			if(SW3_PRESSED()){ // display EW lights

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
				else if(b_EW_left == false){ // only turn off the green led if it isn't supposed to be blinking
					LED3_OFF();
				}
			}
			else{ // display NS lights

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
				else if (b_NS_left == false) { // only turn off the green led if it isn't supposed to be blinking
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
	esos_RegisterTimer(blinkLeftTurn, 250 / 2);
}
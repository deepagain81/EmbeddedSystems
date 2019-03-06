// Created by Benjamin and Deepak
// on 3/5/2019

#include "esos_f14ui.h"
#include "esos_lcd44780.h"

ESOS_USER_TASK(get_temperature);

void user_init(void) {
	config_esos_uiF14();
	esos_RegisterTask(get_temperature);
}

// USER TASKS
ESOS_USER_TASK(get_temperature) {
	ESOS_TASK_BEGIN();
	esos_lcd44780_init(); // initialize LCD
	esos_lcd44780_configDisplay();
	esos_uiF14_flashLED3(250); // flash the led with a period of 250ms
	
	while(true){
		ESOS_TASK_YIELD();
	}

	ESOS_TASK_END();
}
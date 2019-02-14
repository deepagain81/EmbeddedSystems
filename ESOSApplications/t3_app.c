// Created by Benjamin, Marcos, and Mehedi
// 2/12/2019

#include "esos_f14ui.h"

ESOS_USER_TASK (test_LED1);

void user_init(void) {
	config_esos_uiF14();
	esos_RegisterTask(test_LED1);
	//__esos_unsafe_PutString(HELLO_MSG);

	//HARDWARE_CONFIG();
	//esos_RegisterTask(calculate_light_cycle);
	//esos_RegisterTask(display_lights);
	//esos_RegisterTimer(blinkLeftTurn, 250 / 2);
}

ESOS_USER_TASK(test_LED1) {
	ESOS_TASK_BEGIN();
	while (true) {
		esos_uiF14_turnLED1On();
		ESOS_TASK_WAIT_TICKS(5000); // delay 5 seconds
		esos_uiF14_turnLED1Off();
		ESOS_TASK_WAIT_TICKS(5000); // delay 5 seconds
		esos_uiF14_flashLED1(1000);
		while (true) {
			ESOS_TASK_YIELD();
		}
	}
	ESOS_TASK_END();
}
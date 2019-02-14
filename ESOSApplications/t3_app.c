// Created by Benjamin, Marcos, and Mehedi
// 2/12/2019

//#include "revF14.h"
#include "esos_f14ui.h"

ESOS_USER_TASK (test_LED1);
ESOS_USER_TASK(test_SW1);

void user_init(void) {
	config_esos_uiF14();
	//esos_RegisterTask(test_LED1);
	esos_RegisterTask(test_SW1);
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

ESOS_USER_TASK(test_SW1) {
	ESOS_TASK_BEGIN();
	while (true) {
		if (esos_uiF14_isSW1Pressed()) {
			esos_uiF14_turnLED1On();
		}
		else if (esos_uiF14_isSW1DoublePressed()) {
			esos_uiF14_flashLED1(1000);
		}
		else if (esos_uiF14_isSW1Released()) {
			esos_uiF14_turnLED1Off();
		}
		else {
			//LED3_ON();
			esos_uiF14_flashLED1(200);
		}
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}
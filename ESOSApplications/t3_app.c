// Created by Benjamin, Marcos, and Mehedi
// 2/12/2019

//#include "revF14.h"
#include "esos_f14ui.h"

ESOS_USER_TASK (test_LED3); // using find and replace
ESOS_USER_TASK(test_SW3);   // using find and replace
ESOS_USER_TASK(test_RPG);
ESOS_USER_TASK(t3_demo_program);

void user_init(void) {
	config_esos_uiF14();
	//esos_RegisterTask(test_LED3);
	//esos_RegisterTask(test_SW3);
	//esos_RegisterTask(test_RPG);
	esos_RegisterTask(t3_demo_program);
}

ESOS_USER_TASK(t3_demo_program) {
	ESOS_TASK_BEGIN();
	esos_uiF14_flashLED3(500/2); // flash LED1 constantly with 500ms period
	while (true) {
		// print the menu to screen
		ESOS_TASK_WAIT_ON_SEND_STRING("#####Running t3_demo_program#####\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("SW1 is ");	if (esos_uiF14_isSW1Pressed()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("pressed.\n"); } 
													else if (esos_uiF14_isSW1DoublePressed()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("double pressed.\n"); }
													else if (esos_uiF14_isSW1Released()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("released\n"); }
													else 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("???\n"); }
		ESOS_TASK_WAIT_ON_SEND_STRING("SW2 is ");	if (esos_uiF14_isSW2Pressed()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("pressed.\n"); }
													else if (esos_uiF14_isSW2DoublePressed()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("double pressed.\n"); }
													else if (esos_uiF14_isSW2Released()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("released\n"); }
													else 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("???\n"); }
		ESOS_TASK_WAIT_ON_SEND_STRING("SW3 is ");	if (esos_uiF14_isSW3Pressed()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("pressed.\n"); }
													else if (esos_uiF14_isSW3DoublePressed()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("double pressed.\n"); }
													else if (esos_uiF14_isSW3Released()) 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("released\n"); }
													else 
														{ ESOS_TASK_WAIT_ON_SEND_STRING("???\n"); }
		ESOS_TASK_WAIT_ON_SEND_STRING("RPG \n");	
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		

		// set state of LED2 based on RPG
		if (esos_uiF14_isRPGTurningSlow()) {
			esos_uiF14_turnLED2On();
		}
		else if (esos_uiF14_isRPGTurningMedium()) {
			esos_uiF14_flashLED2(500/2);  // period of 500 ms
		}
		else if (esos_uiF14_isRPGTurningFast()) {
			esos_uiF14_flashLED2(100/2);  // period of 500 ms
		}
		else {
			esos_uiF14_isLED2Off();
		}
		// set state of LED1
		if (esos_uiF14_isSW3Pressed() || esos_uiF14_isSW3DoublePressed()) { // SW3 controls whether it shows state of SW1 or SW2
			// SW1
			if (esos_uiF14_isSW1Pressed()) {
				esos_uiF14_turnLED1On();
			}
			else if (esos_uiF14_isSW1DoublePressed()) {
				// 1
				esos_uiF14_turnLED1On();
				ESOS_TASK_WAIT_TICKS(100);
				esos_uiF14_turnLED1Off();
				ESOS_TASK_WAIT_TICKS(150);
				// 2
				esos_uiF14_turnLED1On();
				ESOS_TASK_WAIT_TICKS(100);
				esos_uiF14_turnLED1Off();
				ESOS_TASK_WAIT_TICKS(150);
				// 3
				esos_uiF14_turnLED1On();
				ESOS_TASK_WAIT_TICKS(100);
				esos_uiF14_turnLED1Off();
			}
			else {
				esos_uiF14_turnLED1Off();
			}
		}
		else { // SW2
			   // SW2
			if (esos_uiF14_isSW2Pressed()) {
				esos_uiF14_turnLED1On();
			}
			else if (esos_uiF14_isSW2DoublePressed()) {
				// 1
				esos_uiF14_turnLED1On();
				ESOS_TASK_WAIT_TICKS(100);
				esos_uiF14_turnLED1Off();
				ESOS_TASK_WAIT_TICKS(150);
				// 2
				esos_uiF14_turnLED1On();
				ESOS_TASK_WAIT_TICKS(100);
				esos_uiF14_turnLED1Off();
				ESOS_TASK_WAIT_TICKS(150);
				// 3
				esos_uiF14_turnLED1On();
				ESOS_TASK_WAIT_TICKS(100);
				esos_uiF14_turnLED1Off();
			}
			else {
				esos_uiF14_turnLED1Off();
			}
		}
	ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(test_LED3) {
	ESOS_TASK_BEGIN();
	while (true) {
		esos_uiF14_turnLED3On();
		ESOS_TASK_WAIT_TICKS(5000); // delay 5 seconds
		esos_uiF14_turnLED3Off();
		ESOS_TASK_WAIT_TICKS(5000); // delay 5 seconds
		esos_uiF14_flashLED3(1000);
		while (true) {
			ESOS_TASK_YIELD();
		}
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(test_SW3) {
	ESOS_TASK_BEGIN();
	ESOS_TASK_WAIT_ON_SEND_STRING("Beginning test_SW3...\n");
	while(true) {
		if (esos_uiF14_isSW3Pressed()) {
			esos_uiF14_turnLED1On();
		}
		else if (esos_uiF14_isSW3DoublePressed()) {
			esos_uiF14_flashLED1(1000);
		}
		else if (esos_uiF14_isSW3Released()) {
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

ESOS_USER_TASK(test_RPG) {
	ESOS_TASK_BEGIN();
	ESOS_TASK_WAIT_ON_SEND_STRING("Beginning test_RPG1...\n");
	while (true) {
		// test different speeds using LED1
		if (esos_uiF14_isRPGTurningSlow()) {
			esos_uiF14_turnLED1On();
		}
		else if (esos_uiF14_isRPGTurningMedium()) {
			esos_uiF14_flashLED1(500);
		}
		else if (esos_uiF14_isRPGTurningFast()) {
			esos_uiF14_flashLED1(200);
		}
		else {
			esos_uiF14_turnLED1Off();
		}
		// output message if turning
		if (esos_uiF14_isRPGTurning()) {
			ESOS_TASK_WAIT_ON_SEND_STRING("TURNING!!!");
		}
		// test CW and CCW with LED 2 and 3
		if (esos_uiF14_isRPGTurningCW()) {
			ESOS_TASK_WAIT_ON_SEND_STRING(" CW\n");
		}
		if (esos_uiF14_isRPGTurningCCW()) {
			ESOS_TASK_WAIT_ON_SEND_STRING(" CCW\n");
		}
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}
//inline uint16_t esos_uiF14_getRPGValue_u16(void);
//inline bool esos_uiF14_isRPGTurning(void);
//inline bool esos_uiF14_isRPGTurningSlow(void);
//inline bool esos_uiF14_isRPGTurningMedium(void);
//inline bool esos_uiF14_isRPGTurningFast(void);
//inline bool esos_uiF14_isRPGTurningCW(void);
//inline bool esos_uiF14_isRPGTurningCCW(void);
//
//void config_esos_uiF14();
//int16_t esos_uiF14_getRPGVelocity_i16(void);
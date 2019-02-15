// Author: Benjamin, Marcos, Mehedi, Jorden, and Deepak
// Date:   02/12/2019

//#include "revF14.h"
#include "esos_f14ui.h"

// List all user task
ESOS_USER_TASK (test_LED3); // using find and replace
ESOS_USER_TASK(test_SW3);   // using find and replace
ESOS_USER_TASK(test_RPG);
ESOS_USER_TASK(t3_demo_program);

// User provided functions to config HW, create/initialize SW structures
// register atleat one task
void user_init(void) {
	config_esos_uiF14();
	//esos_RegisterTask(test_LED3);
	//esos_RegisterTask(test_SW3);
	//esos_RegisterTask(test_RPG);
	esos_RegisterTask(t3_demo_program);
}

// menu state variables
int menu_SW1_state;        // 0 - Off, 1 - On, 2 - Flashing
int menu_SW2_state;        // 0 - Off, 1 - On, 2 - Flashing
int menu_SW3_state;        // 0 - Off, 1 - On, 2 - Flashing
int menu_RPG_speed_state;  // 0 - Stopped, 1 - Low, 2 - Medium, 3 - High
int menu_RPG_direction;    // 0 - Stopped, 1 - Clockwise, 2 - Counter-Clockwise

// All user-provided task (must include wait and yield periodically)
ESOS_USER_TASK(t3_demo_program) {
	// Books says they go here. ('static' preserved across yeild and wait)
	// menu state variables
	/*static int menu_SW1_state;        // 0 - Off, 1 - On, 2 - Flashing
	static int menu_SW2_state;        // 0 - Off, 1 - On, 2 - Flashing
	static int menu_SW3_state;        // 0 - Off, 1 - On, 2 - Flashing
	static int menu_RPG_speed_state;  // 0 - Stopped, 1 - Low, 2 - Medium, 3 - High
	static int menu_RPG_direction;    // 0 - Stopped, 1 - Clockwise, 2 - Counter-Clockwise*/

	ESOS_TASK_BEGIN();
	// initialize menu variables
	menu_SW1_state        = 0;
	menu_SW2_state        = 0;
	menu_SW3_state        = 0;
	menu_RPG_speed_state  = 0;
	menu_RPG_direction    = 0;
	// initialize menu text
	ESOS_TASK_WAIT_ON_SEND_STRING("#####Running t3_demo_program#####\n");
	ESOS_TASK_WAIT_ON_SEND_STRING("TODO: Add instructions for changing variables\n");

	#warning Add menu input and instructions

	// initilize LED3
	esos_uiF14_flashLED3(500); // flash LED1 constantly with 500ms period

	// main loop
	while (true) {
		ESOS_TASK_WAIT_ON_SEND_STRING("DEBUG - RPG Counter: ");
		ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)esos_uiF14_getRPGValue_u16());
		ESOS_TASK_WAIT_ON_SEND_STRING("  RPG Velocity: ");
		ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)esos_uiF14_getRPGVelocity_i16());
		ESOS_TASK_WAIT_ON_SEND_STRING("  RPG Menu State: ");
		ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)menu_RPG_speed_state);
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("DEBUG - RPGA: ");
		//ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)RPGA_OUT());
		//ESOS_TASK_WAIT_ON_SEND_STRING("  RPGB: ");
		//ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)RPGB_OUT());
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//// print the menu to screen
		//ESOS_TASK_WAIT_ON_SEND_STRING("#####Running t3_demo_program#####\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("SW1 is ");	if (esos_uiF14_isSW1Pressed()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("pressed.\n"); } 
		//											else if (esos_uiF14_isSW1DoublePressed()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("double pressed.\n"); }
		//											else if (esos_uiF14_isSW1Released()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("released\n"); }
		//											else 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("???\n"); }
		//ESOS_TASK_WAIT_ON_SEND_STRING("SW2 is ");	if (esos_uiF14_isSW2Pressed()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("pressed.\n"); }
		//											else if (esos_uiF14_isSW2DoublePressed()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("double pressed.\n"); }
		//											else if (esos_uiF14_isSW2Released()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("released\n"); }
		//											else 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("???\n"); }
		//ESOS_TASK_WAIT_ON_SEND_STRING("SW3 is ");	if (esos_uiF14_isSW3Pressed()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("pressed.\n"); }
		//											else if (esos_uiF14_isSW3DoublePressed()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("double pressed.\n"); }
		//											else if (esos_uiF14_isSW3Released()) 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("released\n"); }
		//											else 
		//												{ ESOS_TASK_WAIT_ON_SEND_STRING("???\n"); }
		//ESOS_TASK_WAIT_ON_SEND_STRING("RPG \n");	
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		//ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		

		// set state of LED2 based on RPG
		if (esos_uiF14_isRPGTurningSlow()) {
			esos_uiF14_turnLED2On();
			menu_RPG_speed_state = 1;
		}
		else if (esos_uiF14_isRPGTurningMedium()) {
			esos_uiF14_flashLED2(500);  // period of 500 ms
			menu_RPG_speed_state = 2;
		}
		else if (esos_uiF14_isRPGTurningFast()) {
			esos_uiF14_flashLED2(100);  // period of 100 ms
			menu_RPG_speed_state = 3;
		}
		else {
			esos_uiF14_turnLED2Off();
			menu_RPG_speed_state = 0;
		}
		// set state of LED1
		if (esos_uiF14_isSW3Pressed() || esos_uiF14_isSW3DoublePressed()) { // SW3 controls whether it shows state of SW1 or SW2
			// SW2 controls LED1 if SW3 is not pressed
			if (esos_uiF14_isSW1Pressed()) {
				esos_uiF14_turnLED1On();
			}
			else if (esos_uiF14_isSW1DoublePressed()) { // short flashing sequence for a double press; the check for the double press variable resets the double press state
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
		else { // SW2 controls LED1 if SW3 is not pressed
			if (esos_uiF14_isSW2Pressed()) {
				esos_uiF14_turnLED1On();
			}
			else if (esos_uiF14_isSW2DoublePressed()) { // short flashing sequence for a double press; the check for the double press variable resets the double press state
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
		} // END setting state for LED1
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
// Author: Benjamin, Marcos, Mehedi, Jorden, and Deepak
// Date:   02/12/2019

//#include "revF14.h"
#include "esos_f14ui.h"

// List all user task
ESOS_USER_TASK (hearbeat); // using find and replace
ESOS_USER_TASK(display_ADC);   // using find and replace
ESOS_USER_TASK(set_sample_state);
ESOS_USER_TASK(sample_R5);

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
bool sample_state;        // False - Off, True - On

// ESOS In handling variables
uint8_t ESOS_digit_in;
uint8_t buffer_in[11]; //a buffer length of 10 + \n
int next_position; // ptr for the next place to put a number
uint32_t new_threshold_value;
int error_value;

// get a number out of an array
uint32_t get_number_from_array(uint8_t *uint8_buf, int next, int *error) {
	int i;
	int place = 1;
	int value = 0;
	*error = 0;
	for (i = next - 2; i >= 1 && uint8_buf[i] != ' '; i--) { // evaluate numbers from last character(next - 2) to first that is not a command or space (index 1)
		if (uint8_buf[i] > '9' || uint8_buf[i] < '0') { // character is within ASCII number range
			*error = 1;
			return 0;
		}
		// convert to values I can use in math
		value += (uint8_buf[i] - (uint8_t)'0') * place; // I want integer values to do math with, so I subtract the lowest ASCII code of the numbers (0)
		place *= 10; // increase place by power of 10 each time
	}
	return value;
}

// All user-provided task (must include wait and yield periodically)
ESOS_USER_TASK(heartbeat) {

	ESOS_TASK_BEGIN();
		
		while (true){
			esos_uiF14_turnLED1On();
			ESOS_TASK_WAIT_TICKS(250); // may be better as 50 for heartbeat
			esos_uiF14_turnLED1Off();
			ESOS_TASK_WAIT_TICKS(250); // may be better as 200 for heartbeat
		}
		
	ESOS_TASK_END();
}

ESOS_USER_TASK(test_LED3) {
	ESOS_TASK_BEGIN();
		while (true) {
			ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
			
			ESOS_TASK_WAIT_ON_SEND_STRING("ADC result: ");
			ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_TASK_WAIT_SENSOR_QUICK_READ()); // echo ADC Value
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			
			ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(set_sample_state) {
	ESOS_TASK_BEGIN();
		while(true) {
			if (esos_uiF14_isSW2Pressed() && sample_state == False){
				sample_state = True;
			}else if ((esos_uiF14_isSW1Pressed() || esos_uiF14_isSW2Pressed()) && sample_state == True){
				sample_state = False;
			}
			ESOS_TASK_YIELD();
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(sample_R5) {
	ESOS_TASK_BEGIN();
		while(true) {
			if (sample_state == True){
				ESOS_TASK_WAIT_ON_SEND_STRING("ADC result: ");
				//ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_TASK_WAIT_SENSOR_QUICK_READ());//print R5 to bootloader
				ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			}
			ESOS_TASK_WAIT_TICKS(1000);
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
		// OUTPUT VELOCITY
		ESOS_TASK_WAIT_ON_SEND_STRING("RPG Position: ");
		ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING((uint8_t)esos_uiF14_getRPGValue_u16());
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");
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
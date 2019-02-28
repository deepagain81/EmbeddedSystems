// Author: Benjamin, Marcos, Mehedi, Jorden, and Deepak
// Date:   02/12/2019

//#include "revF14.h"
#include "esos_f14ui.h"
//#include "esos_sensor.h"

// List all user task
ESOS_USER_TASK(heartbeat_LED3);
ESOS_USER_TASK(display_ADC);
ESOS_USER_TASK(set_sample_state);
ESOS_USER_TASK(sample_R5);

// User provided functions to config HW, create/initialize SW structures
// register atleat one task
void user_init(void) {
	config_esos_uiF14();
	esos_RegisterTask(heartbeat_LED3);
	esos_RegisterTask(display_ADC);
	esos_RegisterTask(set_sample_state);
	esos_RegisterTask(sample_R5);
}

// menu state variables
bool sample_state;        // 0 - Off, 1 - On

// ESOS In handling variables
uint8_t ESOS_digit_in;
uint8_t buffer_in[11]; //a buffer length of 10 + \n
int next_position; // ptr for the next place to put a number
uint32_t new_threshold_value;
int error_value;
uint16_t sensor_value;
bool SW2_HELD;

// All user-provided task (must include wait and yield periodically)
ESOS_USER_TASK(heartbeat_LED3) {

	ESOS_TASK_BEGIN();
		
		while (1){
			esos_uiF14_turnLED3On();
			ESOS_TASK_WAIT_TICKS(250); // may be better as 50 for heartbeat
			esos_uiF14_turnLED3Off();
			ESOS_TASK_WAIT_TICKS(250); // may be better as 200 for heartbeat
		}
		
	ESOS_TASK_END();
}

ESOS_USER_TASK(display_ADC) {
	ESOS_TASK_BEGIN();
		while (1) {
			ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
			ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED(); // trigger read when SW1 depressed
			
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02,ESOS_SENSOR_VREF_5V0); // activate the ADC - read from AN2(VPOT) - VREF 5V (currently the default)
			ESOS_TASK_WAIT_SENSOR_QUICK_READ(sensor_value); // get sensor_value

			ESOS_TASK_WAIT_ON_SEND_STRING("ADC result: ");
			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(sensor_value); // echo ADC Value
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			
			ESOS_SENSOR_CLOSE(); // turn off the ADC
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(set_sample_state) {
	ESOS_TASK_BEGIN();
		while(1) {
			if (esos_uiF14_isSW2Pressed() && sample_state == 0 && !SW2_HELD){
				sample_state = 1;
				SW2_HELD = 1;
			}else if ((esos_uiF14_isSW1Pressed() || esos_uiF14_isSW2Pressed()) && sample_state == 1 && !SW2_HELD){
				sample_state = 0;
				SW2_HELD = 1;
			}
			else {
				SW2_HELD = 0;
			}
			ESOS_TASK_YIELD();
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(sample_R5) {
	ESOS_TASK_BEGIN();
		while(1) {
			if (sample_state == 1){
				ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_5V0); // activate the ADC - read from AN2(VPOT) - VREF 5V (currently the default)
				ESOS_TASK_WAIT_SENSOR_QUICK_READ(sensor_value); // get sensor_value

				ESOS_TASK_WAIT_ON_SEND_STRING("ADC result: ");
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(sensor_value); // echo ADC Value
				ESOS_TASK_WAIT_ON_SEND_STRING("\n");

				ESOS_SENSOR_CLOSE(); // turn off the ADC
			}
			ESOS_TASK_WAIT_TICKS(1000);
		}
	ESOS_TASK_END();
}

// test
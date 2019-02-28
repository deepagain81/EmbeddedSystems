// Author: Benjamin and Deepak
// Date:   02/27/2019

#include "esos_f14ui.h"

// constants
const esos_sensor_ch_t PERIFERAL_TO_USE = ESOS_SENSOR_CH02; // potentiometer

// user functions
bool isInCharRange(char input, char a, char b);

// List all user task
ESOS_USER_TASK(sample_ADC);
ESOS_USER_TASK(user_menu);
ESOS_USER_TASK(debug);
//ESOS_USER_TASK(monitor_switches);

void user_init(void) {
	config_esos_uiF14();
	esos_RegisterTask(sample_ADC);
	esos_RegisterTask(user_menu);
	//esos_RegisterTask(debug);
	//esos_RegisterTask(monitor_switches);
}

// menu state variables
uint8_t sampling_mode; // 0 - off,  1 - once,  2 - repeat
esos_sensor_process_t processing_Mode = ESOS_SENSOR_ONE_SHOT;
bool sw1_last_state, sw2_last_state;
uint16_t sensor_value;

ESOS_USER_TASK(user_menu) {
	ESOS_TASK_BEGIN();
	// initialize variables
	char input_char = 'x';
	sw1_last_state = false;
	sw2_last_state = false;
	esos_uiF14_flashLED3(250); // flash the led with a period of 250ms

	// print instructions
	ESOS_TASK_WAIT_ON_SEND_STRING("t4_sensor2.c\n");
	ESOS_TASK_WAIT_ON_SEND_STRING("Get an immediate sample in hex with SW1. Get repeated sampling in Volts with SW2. Configure repeating sampling using SW3.\n");
	// start user loop
	while (1) {
		FLUSH_ESOS_COMM_IN_DATA();

		// Handle switch interface first

		// output once when SW1 is depressed
		if (esos_uiF14_isSW1Pressed() && sw1_last_state == false) {
			sampling_mode = 0; // stop sampling
			sw1_last_state = true;
		}
		else if(esos_uiF14_isSW1Released() && sw1_last_state == true){
			sampling_mode = 1; // sample and output once
			sw1_last_state = false;
		}

		// SW2 starts repeated sampling
		if (esos_uiF14_isSW2Pressed() && sw2_last_state == false && sampling_mode != 2) {
			sampling_mode = 2; // starts repeated sampling
			sw2_last_state = true;
		}
		else if (esos_uiF14_isSW2Pressed() && sw2_last_state == false && sampling_mode == 2) { // toggle off with a second press
			sampling_mode = 0;
			sw2_last_state = true;
		}
		else if (esos_uiF14_isSW2Released()) {
			sw2_last_state = false;
		}

		if (esos_uiF14_isSW3Pressed()) { //configure for looping output
			sampling_mode = 0;
			// output instructions
			ESOS_TASK_WAIT_ON_SEND_STRING("##### Repeated Sampling Configuration #####\n");
			ESOS_TASK_WAIT_ON_SEND_STRING("Processing Modes: 1. One-shot    2. Average    3. Minimum    4. Maximum    5. Median\n");
			ESOS_TASK_WAIT_ON_SEND_STRING("Sampling   Modes: 1. two         2. four       3. eight      4. sixteen    5. thirty-two    6. sixty-four\n");
			ESOS_TASK_WAIT_ON_SEND_STRING("Enter a processing mode, then a sampling mode:\n");
			while (true) { // wait until the user inputs a number
				sampling_mode = 0; // turn off sampling
				if (GET_ESOS_COMM_IN_DATA_LEN() > 0) { // keyboard input!!!
					ESOS_TASK_WAIT_ON_GET_UINT8(input_char); // get a character
					if (isInCharRange(input_char, '1', '5')) { // character is in the appropriate range
						// echo the character
						ESOS_TASK_WAIT_ON_SEND_UINT8(input_char);
						// set property
						if (input_char == '1') {
							processing_Mode = ESOS_SENSOR_ONE_SHOT;
						}
						else if (input_char == '2') {
							processing_Mode = ESOS_SENSOR_AVG2;
						}
						else if (input_char == '3') {
							processing_Mode = ESOS_SENSOR_MIN2;
						}
						else if (input_char == '4') {
							processing_Mode = ESOS_SENSOR_MAX2;
						}
						else if (input_char == '5') {
							processing_Mode = ESOS_SENSOR_MEDIAN2;
						}
						else {
							ESOS_TASK_WAIT_ON_SEND_STRING("\nERROR 35\n");
						}
						break; // break first selection loop
					} // end isInCharRange condition
				} // end Keyboard input condition
				ESOS_TASK_YIELD(); // don't let this config loop hog the processor
			} // end processing mode selection loop

			// skip next step if one-shot was selected
			if (input_char == '1') {
				processing_Mode = ESOS_SENSOR_ONE_SHOT;
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
				//sampling_mode = 2; // repeat sampling
				continue; // skip second selection
			}

			// select sampling mode
			while (true) { // wait until the user inputs a number
				sampling_mode = 0; // turn off sampling
				if (GET_ESOS_COMM_IN_DATA_LEN() > 0) { // keyboard input!!!
					ESOS_TASK_WAIT_ON_GET_UINT8(input_char); // get a character
					if (isInCharRange(input_char, '1', '6')) { // character is in the appropriate range
						// echo the character
						ESOS_TASK_WAIT_ON_SEND_UINT8(input_char);
						ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
						// set property
						processing_Mode += (input_char - '0' - 1); // -1 because all the modes begin at 1, and we don't want to add that on again!
						//sampling_mode = 2; // repeat sampling
						break; // break second selection loop
					} // end isInCharRange loop
				} // end Keyboard input loop
				ESOS_TASK_YIELD(); // don't let this config loop hog the processor
			} // end sampling mode selection loop
		} // end SW2_is_Pressed
		/*ESOS_TASK_WAIT_ON_SEND_STRING("Value of processing_Mode:");
		ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(processing_Mode);
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");*/
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(sample_ADC) {
	ESOS_TASK_BEGIN();
	// initialize variables
	sensor_value = 0;

	while (1) {
		/*ESOS_TASK_WAIT_ON_SEND_STRING("sampling_mode = ");
		ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(sampling_mode);
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");*/
		if (sampling_mode == 0) {
			// do nothing - make sure ADC is off
			ESOS_SENSOR_CLOSE();
		}
		else if (sampling_mode == 1) { // sample once
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(PERIFERAL_TO_USE, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_QUICK_READ(sensor_value);
			ESOS_SENSOR_CLOSE();
			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(sensor_value); // echo ADC Value
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			sampling_mode = 0; // remember, only sample once!
		}
		else if (sampling_mode == 2) { // repeatedly sample every 1 second
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(PERIFERAL_TO_USE, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_READ(sensor_value, processing_Mode, ESOS_SENSOR_FORMAT_VOLTAGE);
			sensor_value /= 3; // into the 3 v range
			ESOS_TASK_WAIT_ON_SEND_UINT8(sensor_value/1000 + '0'); // echo ADC Value - add ASCII offset up to numbers with '0'
			ESOS_TASK_WAIT_ON_SEND_STRING(".");
			ESOS_TASK_WAIT_ON_SEND_UINT8(sensor_value % 1000 / 100 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(sensor_value % 100  / 10  + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(sensor_value % 10   / 1   + '0');
			ESOS_TASK_WAIT_ON_SEND_STRING(" Volts\n");
			ESOS_SENSOR_CLOSE();
			ESOS_TASK_WAIT_TICKS(1000);
		}
		ESOS_TASK_YIELD();
	} // end task loop
	ESOS_TASK_END();
}

ESOS_USER_TASK(debug) {
	ESOS_TASK_BEGIN();
	while (1) {
		ESOS_TASK_WAIT_TICKS(1000);
		if (sampling_mode != 0) {
			ESOS_TASK_WAIT_ON_SEND_STRING("DEBUG: sampling_mode:    ");
			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(sampling_mode); // echo ADC Value
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		}
	}
	ESOS_TASK_END();
}

// functions
bool isInCharRange(char input, char lowChar, char highChar) {
	return ( input >= lowChar && input <= highChar );
}
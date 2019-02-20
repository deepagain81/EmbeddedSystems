// Author: Benjamin, Marcos, Mehedi, Jorden, and Deepak
// Date:   02/12/2019

//#include "revF14.h"
#include "esos_f14ui.h"

// List all user task
ESOS_USER_TASK(heartbeat_LED3);
ESOS_USER_TASK(display_ADC);
ESOS_USER_TASK(set_sample_state);
ESOS_USER_TASK(sample_R5);
ESOS_USER_TASK(change_process);

// User provided functions to config HW, create/initialize SW structures
// register atleat one task
void user_init(void) {
	config_esos_uiF14();
	esos_RegisterTask(heartbeat_LED3);
	esos_RegisterTask(display_ADC);
	esos_RegisterTask(set_sample_state);
	esos_RegisterTask(sample_R5);
	esos_RegisterTask(change_process);
}

// menu state variables
bool sample_State;        // 0 - Off, 1 - On
int processing_Mode = 0;
int num_Samples = 0;

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

ESOS_USER_TASK(test_LED3) {
	ESOS_TASK_BEGIN();
		while (1) {
			ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
			
			ESOS_TASK_WAIT_ON_SEND_STRING("ADC result: ");
			//ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_TASK_WAIT_SENSOR_QUICK_READ()); // echo ADC Value
			
			switch(processing_Mode) {
				case 2 :
					switch(num_Samples) {
						case 2 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_AVG4());// echo ADC Value
							break;
						case 3 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_AVG8());// echo ADC Value
							break;
						case 4 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_AVG16());// echo ADC Value
							break;
						case 5 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_AVG32());// echo ADC Value
							break;
						case 6 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_AVG64());// echo ADC Value
							break;
						default :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_AVG2());// echo ADC Value
					}
					break;
				case 3 :
					switch(num_Samples) {
						case 2 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MIN4());// echo ADC Value
							break;
						case 3 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MIN8());// echo ADC Value
							break;
						case 4 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MIN16());// echo ADC Value
							break;
						case 5 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MIN32());// echo ADC Value
							break;
						case 6 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MIN64());// echo ADC Value
							break;
						default :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MIN2());// echo ADC Value
					}
					break;
				case 4 :
					switch(num_Samples) {
						case 2 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MAX4());// echo ADC Value
							break;
						case 3 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MAX8());// echo ADC Value
							break;
						case 4 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MAX16());// echo ADC Value
							break;
						case 5 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MAX32());// echo ADC Value
							break;
						case 6 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MAX64());// echo ADC Value
							break;
						default :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MAX2());// echo ADC Value
					}
					break;
				case 5 :
					switch(num_Samples) {
						case 2 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MEDIAN4());// echo ADC Value
							break;
						case 3 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MEDIAN8());// echo ADC Value
							break;
						case 4 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MEDIAN16());// echo ADC Value
							break;
						case 5 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MEDIAN32());// echo ADC Value
							break;
						case 6 :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MEDIAN64());// echo ADC Value
							break;
						default :
							ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_MEDIAN2());// echo ADC Value
					}
					break;
				default :
					ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_SENSOR_ONE_SHOT());// echo ADC Value
			}
			
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			
			ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(set_sample_state) {
	ESOS_TASK_BEGIN();
		while(1) {
			if (esos_uiF14_isSW2Pressed() && sample_State == 0){
				sample_State = 1;
			}else if ((esos_uiF14_isSW1Pressed() || esos_uiF14_isSW2Pressed()) && sample_State == 1){
				sample_State = 0;
			}
			ESOS_TASK_YIELD();
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(sample_R5) {
	ESOS_TASK_BEGIN();
		while(1) {
			if (sample_State == 1){
				ESOS_TASK_WAIT_ON_SEND_STRING("ADC result: ");
				//ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(ESOS_TASK_WAIT_SENSOR_QUICK_READ());//print R5 to bootloader
				ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			}
			ESOS_TASK_WAIT_TICKS(1000);
		}
	ESOS_TASK_END();
}

ESOS_USER_TASK(change_process) {
	ESOS_TASK_BEGIN();
	while (1) {
		if (esos_uiF14_isSW3Pressed()){
			ESOS_TASK_WAIT_ON_SEND_STRING("Change the Processing mode and set the Sample Size: \n");
			//Tell User current settings
			ESOS_TASK_WAIT_ON_SEND_STRING("Current Settings are\n");
			ESOS_TASK_WAIT_ON_SEND_STRING("Processing Mode: ");
			switch(processing_Mode) {
				case 2 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Average\n" );
					break;
				case 3 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Minimum\n" );
					break;
				case 4 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Maximum\n" );
					break;
				case 5 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Median\n" );
					break;
				default :
					ESOS_TASK_WAIT_ON_SEND_STRING("One Shot\n" );
			}
			ESOS_TASK_WAIT_ON_SEND_STRING("Sample Rate: ");
			switch(num_Samples) {
				case 2 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Four\n" );
					break;
				case 3 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Eight\n" );
					break;
				case 4 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Sixteen\n" );
					break;
				case 5 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Thirty-Two\n" );
					break;
				case 6 :
					ESOS_TASK_WAIT_ON_SEND_STRING("Sixty-Four\n" );
					break;
				default :
					ESOS_TASK_WAIT_ON_SEND_STRING("Two\n" );
			}
			//Allow user to select new settings
			ESOS_TASK_WAIT_ON_SEND_STRING("Processing Options (select the number of the option):\n");
			ESOS_TASK_WAIT_ON_SEND_STRING("1. one-shot \n2. average \n3. minimum \n4. maximum \n5. median \n0. Default\n\n");
			//get processing_Mode from user
			ESOS_TASK_WAIT_ON_GET_UINT8(ESOS_digit_in);
			if (next_position != 11 || ESOS_digit_in != (uint8_t)'\r' || ESOS_digit_in != (uint8_t)'\n') {
				buffer_in[next_position] = ESOS_digit_in;
				next_position++;
			}
			else if(next_position != 11){
				ESOS_TASK_WAIT_ON_SEND_STRING("\n###next position is 11. Error in buffer handling.###\n");
			}
			if (next_position == 11 || (ESOS_digit_in == (uint8_t)'\r')) { // \r\n is newline on windows
				// evaluate character string for a command
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n'); // results on next line
				processing_Mode = get_number_from_array(buffer_in, next_position, &error_value);
				// clear the buffer
				for (i = 0; i < 11; i++) {
					buffer_in[i] = 0;
				}
				next_position = 0;
			}
			if (processing_Mode != 1){
				ESOS_TASK_WAIT_ON_SEND_STRING("Sample Size (select the number of the option):\n");
				ESOS_TASK_WAIT_ON_SEND_STRING("1. two \n2. four \n3. eight \n4. sixteen \n5. thirty-two \n6. sixty-four \n\n");
				//get num_Samples from user
				ESOS_TASK_WAIT_ON_GET_UINT8(ESOS_digit_in);
				if (next_position != 11 || ESOS_digit_in != (uint8_t)'\r' || ESOS_digit_in != (uint8_t)'\n') {
					buffer_in[next_position] = ESOS_digit_in;
					next_position++;
				}
				else if(next_position != 11){
					ESOS_TASK_WAIT_ON_SEND_STRING("\n###next position is 11. Error in buffer handling.###\n");
				}
				if (next_position == 11 || (ESOS_digit_in == (uint8_t)'\r')) { // \r\n is newline on windows
					// evaluate character string for a command
					ESOS_TASK_WAIT_ON_SEND_UINT8('\n'); // results on next line
					num_Samples = get_number_from_array(buffer_in, next_position, &error_value);
					// clear the buffer
					for (i = 0; i < 11; i++) {
						buffer_in[i] = 0;
					}
					next_position = 0;
				}
			}
		}
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}
// Created by Benjamin and Deepak
// on 3/5/2019

#include "esos_f14ui.h"
#include "esos_lcd44780.h"

static bool b_read_temp = true;
static uint16_t u16_potentiometer_reading; // in integer percentage
static uint16_t u16_temperature_reading;   // in millivolts
static int temperature; // calculated Celcius temperature
static SW3_last_state = false;
ESOS_USER_TASK(get_temperature);
ESOS_USER_TASK(flash_screen);
ESOS_USER_TASK(change_state);

void user_init(void) {
	config_esos_uiF14();
	esos_lcd44780_init(); // initialize LCD
	esos_RegisterTask(get_temperature);
	esos_RegisterTask(flash_screen);
	esos_RegisterTask(change_state);
}

// USER TASKS
ESOS_USER_TASK(get_temperature) {
	ESOS_TASK_BEGIN();
	int i;
	esos_uiF14_flashLED3(250); // flash the led with a period of 250ms
	ESOS_TASK_WAIT_TICKS(300);
	esos_lcd44780_configDisplay();
	esos_lcd44780_clearScreen();
	esos_lcd44780_setCursorHome();
	for(i = 0; i < 5; i++){
		esos_lcd44780_writeChar( 0, i, 'A' );
	}
	while(true){
		if(!b_read_temp){
			// read the potentiometer
			u16_temperature_reading = 0;
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_READ(u16_potentiometer_reading, ESOS_SENSOR_ONE_SHOT, ESOS_SENSOR_FORMAT_PERCENT);
			ESOS_SENSOR_CLOSE();
			ESOS_TASK_WAIT_ON_SEND_STRING("Potentiometer:");
			ESOS_TASK_WAIT_ON_SEND_UINT8(u16_potentiometer_reading / 100 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(u16_potentiometer_reading % 100 / 10 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(u16_potentiometer_reading % 10 / 1 + '0');
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			ESOS_TASK_WAIT_TICKS(1000);
		}
		else{
			// read the temperature sensor
			u16_potentiometer_reading = 0;
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_READ(u16_temperature_reading, ESOS_SENSOR_ONE_SHOT, ESOS_SENSOR_FORMAT_VOLTAGE);
			ESOS_SENSOR_CLOSE();
			u16_temperature_reading /= 3; // into the 3V range
			temperature = 100000 * ((int32_t)u16_temperature_reading - 424) / 625; // should produce Celsius degrees; 6.25mV per Celsius degree - multiply by 100 to avoid floats - extra 3 places for decimal conversion; offset of 424mV at 0 Celsius
			if (temperature < 0) {
				ESOS_TASK_WAIT_ON_SEND_UINT8('-');
				temperature *= -1; // let the normal logic work 
			}
			ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 1000000 / 100000 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 100000 / 10000 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 10000 / 1000 + '0'); // echo ADC Value - add ASCII offset up to numbers with '0'
			ESOS_TASK_WAIT_ON_SEND_STRING(".");
			ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 1000 / 100 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 100 / 10 + '0');
			ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 10 / 1 + '0');
			ESOS_TASK_WAIT_ON_SEND_STRING(" Degrees Celsius\n");
			ESOS_TASK_WAIT_TICKS(1000);
		}
		ESOS_TASK_YIELD();
		ESOS_TASK_WAIT_TICKS(50);
	}

	ESOS_TASK_END();
}

ESOS_USER_TASK(flash_screen) {
	ESOS_TASK_BEGIN();
	static bool visible = true;
	while(1){
		esos_lcd44780_setDisplayVisible(visible);
		ESOS_TASK_WAIT_TICKS(1000);
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(change_state){
	ESOS_TASK_BEGIN();
	while(1){
		if (esos_uiF14_isSW3Released() && SW3_last_state == true) {
			b_read_temp = !b_read_temp; // toggle state upon SW2 release
			SW3_last_state = false;
		}
		else if (esos_uiF14_isSW3Pressed()) {
			//b_read_temp = 0; // stop sampling
			SW3_last_state = true;
		}
		else if(esos_uiF14_isSW3Released()){
			SW3_last_state = false;
		}
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}
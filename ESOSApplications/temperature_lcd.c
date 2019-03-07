// Created by Benjamin and Deepak
// on 3/5/2019

#include "esos_f14ui.h"
#include "esos_lcd44780.h"

static bool b_read_temp = false; // show potentiometer by default
static uint16_t u16_potentiometer_reading; // in integer percentage
static uint16_t u16_temperature_reading;   // in millivolts
static int32_t temperature; // calculated Celcius temperature
static SW3_last_state = false;
ESOS_USER_TASK(get_temperature);
ESOS_USER_TASK(flash_screen);
ESOS_USER_TASK(change_state);

char int_to_hex(int n);

// custom chars
uint8_t pu8_custombar_vertical1[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111
};
uint8_t pu8_custombar_vertical2[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111
};
uint8_t pu8_custombar_vertical3[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111
};
uint8_t pu8_custombar_vertical4[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
uint8_t pu8_custombar_vertical5[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
uint8_t pu8_custombar_vertical6[8] = {
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
uint8_t pu8_custombar_vertical7[8] = {
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};
uint8_t pu8_custombar_vertical8[8] = {
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

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
	esos_uiF14_flashLED3(1000); // flash the led with a period of 250ms
	esos_lcd44780_setCustomChar(0,pu8_custombar_vertical1);
	esos_lcd44780_setCustomChar(1,pu8_custombar_vertical2);
	esos_lcd44780_setCustomChar(2,pu8_custombar_vertical3);
	esos_lcd44780_setCustomChar(3,pu8_custombar_vertical4);
	esos_lcd44780_setCustomChar(4,pu8_custombar_vertical5);
	esos_lcd44780_setCustomChar(5,pu8_custombar_vertical6);
	esos_lcd44780_setCustomChar(6,pu8_custombar_vertical7);
	esos_lcd44780_setCustomChar(7,pu8_custombar_vertical8);
	ESOS_TASK_WAIT_TICKS(300);
	esos_lcd44780_configDisplay();
	esos_lcd44780_clearScreen();
	esos_lcd44780_setCursorHome();
	// test screen by writing 5 letters to both lines
	// for(i = 0; i < 5; i++){
	// 	esos_lcd44780_writeChar( 0, i, 'A'+ i );
	// }
	// for(i=0; i<5; i++){
	// 	esos_lcd44780_writeChar(1,i, 'A'+ i);
	// }
	while(true){
		if(!b_read_temp){
			// read the potentiometer
			u16_temperature_reading = 0;
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_READ(u16_potentiometer_reading, ESOS_SENSOR_AVG32, ESOS_SENSOR_FORMAT_PERCENT);
			ESOS_SENSOR_CLOSE();
			// ESOS_TASK_WAIT_ON_SEND_STRING("Potentiometer:");
			// ESOS_TASK_WAIT_ON_SEND_UINT8(u16_potentiometer_reading / 100 + '0');
			// ESOS_TASK_WAIT_ON_SEND_UINT8(u16_potentiometer_reading % 100 / 10 + '0');
			// ESOS_TASK_WAIT_ON_SEND_UINT8(u16_potentiometer_reading % 10 / 1 + '0');
			// ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			//ESOS_TASK_WAIT_TICKS(100);
		}
		else{
			// read the temperature sensor
			u16_potentiometer_reading = 0;
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_READ(u16_temperature_reading, ESOS_SENSOR_AVG32, ESOS_SENSOR_FORMAT_VOLTAGE);
			ESOS_SENSOR_CLOSE();
			u16_temperature_reading /= 3; // into the 3V range
			temperature = 100000 * ((int32_t)u16_temperature_reading - 424) / 625; // should produce Celsius degrees; 6.25mV per Celsius degree - multiply by 100 to avoid floats - extra 3 places for decimal conversion; offset of 424mV at 0 Celsius
			// if (temperature < 0) {
			// 	ESOS_TASK_WAIT_ON_SEND_UINT8('-');
			// 	temperature *= -1; // let the normal logic work 
			// }
			// ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 1000000 / 100000 + '0');
			// ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 100000 / 10000 + '0');
			// ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 10000 / 1000 + '0'); // echo ADC Value - add ASCII offset up to numbers with '0'
			// ESOS_TASK_WAIT_ON_SEND_STRING(".");
			// ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 1000 / 100 + '0');
			// ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 100 / 10 + '0');
			// ESOS_TASK_WAIT_ON_SEND_UINT8(temperature % 10 / 1 + '0');
			// ESOS_TASK_WAIT_ON_SEND_STRING(" Degrees Celsius\n");
			//ESOS_TASK_WAIT_TICKS(100);
		}
		ESOS_TASK_YIELD();
		//ESOS_TASK_WAIT_TICKS(50);
	}

	ESOS_TASK_END();
}

ESOS_USER_TASK(flash_screen) {
	ESOS_TASK_BEGIN();
	
	while(1){
		if(!b_read_temp){
			// show potentiometer meter
			// should read pot hexval
			esos_lcd44780_writeChar(0,0, 'P');
			esos_lcd44780_writeChar(0,1, 'o');
			esos_lcd44780_writeChar(0,2, 't');
			esos_lcd44780_writeChar(0,3, ' ');
			esos_lcd44780_writeChar(0,4, '0');
			esos_lcd44780_writeChar(0,5, 'x');
			esos_lcd44780_writeChar(0,6, int_to_hex(u16_potentiometer_reading/16));
			esos_lcd44780_writeChar(0,7, int_to_hex(u16_potentiometer_reading%16));
			// print hex values
			esos_lcd44780_writeChar(1,0, ((u16_potentiometer_reading *10 / 125 == 0) ? '+' : '-')); // 100/8 = 12.5
			esos_lcd44780_writeChar(1,1, ((u16_potentiometer_reading *10 / 125 == 1) ? '+' : '-')); // so 1000/125 = 8 divisions
			esos_lcd44780_writeChar(1,2, ((u16_potentiometer_reading *10 / 125 == 2) ? '+' : '-'));
			esos_lcd44780_writeChar(1,3, ((u16_potentiometer_reading *10 / 125 == 3) ? '+' : '-')); // replace - and + with custom chars later
			esos_lcd44780_writeChar(1,4, ((u16_potentiometer_reading *10 / 125 == 4) ? '+' : '-'));
			esos_lcd44780_writeChar(1,5, ((u16_potentiometer_reading *10 / 125 == 5) ? '+' : '-'));
			esos_lcd44780_writeChar(1,6, ((u16_potentiometer_reading *10 / 125 == 6) ? '+' : '-'));
			esos_lcd44780_writeChar(1,7, ((u16_potentiometer_reading *10 / 125 >= 7) ? '+' : '-')); // accounts for 100% as well
			// print bottom row
		}
		else{ // output temperature display
			esos_lcd44780_writeChar(0,0, 'L');
			esos_lcd44780_writeChar(0,1, 'M');
			esos_lcd44780_writeChar(0,2, '6');
			esos_lcd44780_writeChar(0,3, '0');
			esos_lcd44780_writeChar(0,4, ' ');
			esos_lcd44780_writeChar(0,5, ' ');
			esos_lcd44780_writeChar(0,6, ' ');
			esos_lcd44780_writeChar(0,7, (temperature/1000 - 20 > 8) ? ((temperature/1000 - 20) - 9) : ' ');
			// second row
			esos_lcd44780_writeChar(1,0, (temperature/1000 /10 + '0')); // divide by 1000 to remove decimal digits from consideration
			esos_lcd44780_writeChar(1,1, (temperature/1000 %10 + '0'));
			esos_lcd44780_writeChar(1,2, 'C');
			esos_lcd44780_writeChar(1,3, ' ');
			esos_lcd44780_writeChar(1,4, ' ');
			esos_lcd44780_writeChar(1,5, ' ');
			esos_lcd44780_writeChar(1,6, ' ');
			esos_lcd44780_writeChar(1,7, (temperature/1000 - 20 == 0) ? 0x00 :((temperature/1000 - 20 > 8) ? 0x07 : (temperature/1000 - 20) - 1)); // custom character 8 if greater than 8
		}
		ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(change_state){
	ESOS_TASK_BEGIN();
	while(1){
		if (esos_uiF14_isSW3Released() && SW3_last_state == true) {
			b_read_temp = !b_read_temp; // toggle state upon SW3 release
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

char int_to_hex(int n){
	if(n >= 0 && n<=9){ // n is in normal char range
		return n + '0';
	}
	else if(n >= 10 && n <= 16){
		return n - 10 + 'A';
	}
	else{
		return '?';
	}
}
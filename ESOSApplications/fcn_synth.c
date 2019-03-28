// Author : Deepak Chapagain
// Date   : 03/19/2019
// ESOS user application that allow user to enter desired parameter 
// to control the analog function synthesizr output from the DAC.


#include "revF14.h"
#include "fall17lib.h"
#include "esos_f14ui.h"
#include "esos_menu.h"
#include "esos_lcd44780.h"
#include "DAC_comms.h"
#include "esos_sensor.h"
#include "I2C_comms.h"

#include <stdint.h> // for uint64_t
// Array for waveforms
#include "stdio.h"

//void configDAC(void);
//void writeDAC(uint16_t u16_x, uint16_t u16_y);

//volatile uint8_t u8_per, u8_amp;	// variables
//volatile uint16_t u16_per;

const uint8_t au8_sinetbl[] = {127,133,139,146,152,158,164,170,176,181,187,192,198,203,208,212,217,221,225,229,233,236,239,242,244,247,249,250,252,253,253,254,254,254,253,253,252,250,249,247,244,242,239,236,233,229,225,221,217,212,208,203,198,192,187,181,176,170,164,158,152,146,139,133,127,121,115,108,102,96,90,84,78,73,67,62,56,51,46,42,37,33,29,25,21,18,15,12,10,7,5,4,2,254,254,0,0,0,254,254,2,4,5,7,10,12,15,18,21,25,29,33,37,42,46,51,56,62,67,73,78,84,90,96,102,108,115,121};
const uint8_t au8_sqrtbl[] = {254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t au8_tritbl[] = {0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,254,252,248,244,240,236,232,228,224,220,216,212,208,204,200,196,192,188,184,180,176,172,168,164,160,156,152,148,144,140,136,132,128,124,120,116,112,108,104,100,96,92,88,84,80,76,72,68,64,60,56,52,48,44,40,36,32,28,24,20,16,12,8,4,0};
const uint8_t au8_usr1tbl[] = {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint16_t u16_sensor_millivolts;
uint32_t u32_scaled_DAC_value;
uint16_t u16_I2C_temp;
uint8_t  u8_wav_val; // temp storage for the current step of the wave
uint16_t u16_volt_scale; // temp storage for current setting of wave amplitude
uint16_t u16_DAC_wave_out_index; // initilized in fcn_synth task
uint64_t sixty_four_test;

// Config pin for output (here is where DAC comes into play)
// #define WAVEOUT(_LATB2)
// void config_waveout(){
// 	CONFIG_RB2_AS_DIG_OUTPUT();
// }

ESOS_USER_TASK( read_LM60_task );
ESOS_USER_TASK( read_1631_task );
ESOS_USER_TASK( set_LEDs_task );

uint16_t usToTicks(uint16_t u16_us, uint16_t u16_pre);
void configTimer2(void);

// Timer2 configuration
// 8 =  variable
#define ISP_PERIOD	(8)	// actually 7.8 ms


/*TODO: Ctrl+F replace all instances of mm with a name that abides
		by the coding standards.
*/
static esos_menu_longmenu_t my_menu = {
	.u8_numitems = 8,
	.u8_choice = 0, //Default
	.ast_items = {
		{ "Set",
		"wvform"},
		{ "Set",
		"freq"},
		{ "Set",
		"ampltd"},
		{ "Set",
		"duty"},
		{ "Read",
		"LM60"},
		{ "Read",
		"1631"},
		{ "Set",
		"LEDs"},
		{ "",
		"About..."},
	},
};

static esos_menu_longmenu_t my_menu_no_duty_cycle = {
	.u8_numitems = 7,
	.u8_choice = 0, //Default
	.ast_items = {
		{ "Set",
		"wvform"},
		{ "Set",
		"freq"},
		{ "Set",
		"ampltd"},
		//{ "Set",
		//"duty"},
		{ "Read",
		"LM60"},
		{ "Read",
		"1631"},
		{ "Set",
		"LEDs"},
		{ "",
		"Back..."},
	},
};

// To display error msg
static esos_menu_staticmenu_t err = {
	.u8_numlines = 2,
	.u8_currentline = 0,
	.lines = {{"404"}, {"NotFnd"}},
};

// menu for selecting waveform type
static esos_menu_longmenu_t menu_setWvform = {
	.u8_numitems = 4,
	.u8_choice = 0, //Default
	.ast_items = {
		{ "tri",
		"wvform"},
		{ "sine",
		"wvform"},
		{ "square",
		"wvform"},
		{ "user1",
		"wvform"},
	},
};

// Selecting frequency
static esos_menu_entry_t freq = {
	.entries[0].label = "Freq= ",
	.entries[0].value = 64, //Default
	.entries[0].min = 64,
	.entries[0].max = 2047,
};

// Selecting amplitude
static esos_menu_entry_t ampltd = {
	.entries[0].label = "amp(dV)=",
	.entries[0].value = 16, //Default - unit is deciVolts
	.entries[0].min = 0,
	.entries[0].max = 33, // make this deciVolts
};

// Selecting duty cycle - should only appear when squarewave 
static esos_menu_entry_t duty = {
	.entries[0].label = "duty= ",
	.entries[0].value = 50, //Default
	.entries[0].min = 0,
	.entries[0].max = 100,
};

// typedef struct {
// 	char label[8];
// 	int32_t dynamic_data;
// 	uint8_t format;
// 	bool editable;
// } esos_menu_datadisplaymenu_t;

static esos_menu_datadisplaymenu_t read_LM60 = {
	.label = "LM60(C)",
	.dynamic_data = -1,
	.format = 0,
};

static esos_menu_datadisplaymenu_t read_1631 = {
	.label = "1631(C)",
	.dynamic_data = -1,
	.format = 0,
};

// Selecting LEDS
static esos_menu_entry_t LEDs = {
	.entries[0].label = "LEDs= ",
	.entries[0].value = 1, //Default
	.entries[0].min = 0,
	.entries[0].max = 7,
};

static esos_menu_staticmenu_t about_menu = {
	.u8_numlines = 11,
	.u8_currentline = 0,
	.lines = {"ESOS by",
			 "Jones,",
			 "Bruce, &",
			 "Reese.",
			 "Program",
			 "by",
			 "Benjamin",
			 "Deepak",
			 "Jordan",
			 "Marcos",
			 "Mehedi"}
};

// typedef struct {
// 	uint8_t u8_numlines;
// 	uint8_t u8_currentline;
// 	char lines[][8];
// } esos_menu_staticmenu_t;

/*// Gives option to select from
static esos_menu_entry_t ledfp = {
	.entries[0].label = "Per = ",
	.entries[0].value = 1000, //Default, per = 1s
	.entries[0].min = 250,
	.entries[0].max = 2000,
};*/

// heartbeat
// ESOS_USER_TIMER( heartbeat_LED ) {
// 	esos_uiF14_toggleLED3();
// }

// LCD 
ESOS_USER_TASK( fcn_synth ) {
	ESOS_TASK_BEGIN();
	u16_DAC_wave_out_index = 0; // initilize to the first index of the wave
	//setDACA(0x3FF);
	shutdownDACA();
	shutdownDACB();
	setDACA( 0x513 ); // proves that DACA works
	setDACB( 0x0FF ); // proves that DACB works
	startI2C();
	write1_I2C(0b10010000, 0x51); // initiate conversion
	stopI2C();
	ESOS_TASK_WAIT_TICKS(1000);
	startI2C();
	write1_I2C(0b10010000, 0xAA); // read temperature
	rstartI2C();
	u16_I2C_temp =  (uint16_t)readI2C(true) << 8;
	u16_I2C_temp += (uint16_t)readI2C(false);
	stopI2C();
	printf("I2C read from temperature: %d\n",u16_I2C_temp);
	//configTimer2(); // output to DAC
	while(TRUE){
		printf("Beginning of fcn_synth loop...\n");
		// Display main menu until sw3 pressed
		ESOS_TASK_WAIT_ESOS_MENU_LONGMENU(my_menu);
		printf("LONG Menu exited...\n");

		if (my_menu.u8_choice < 0) // must have atleast one option in menu
			ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(err);
		else if(my_menu.u8_choice == 0){

			//Display waveform menu until sw3 pressed
			esos_lcd44780_clearScreen();
			ESOS_TASK_WAIT_ESOS_MENU_LONGMENU(menu_setWvform);

			// Inside the wvform menu, has another menu
			if (menu_setWvform.u8_choice < 0)// at least one option
				ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(err);
			else if (menu_setWvform.u8_choice > 0){
				// Display trig wave
				// test screen by writing 5 letters to both lines
				
				continue;
			}
			else if (menu_setWvform.u8_choice == 0){
				// Display sine wave
				continue;
			}
			else if (menu_setWvform.u8_choice == 1){
				// Display sine wave
				continue;
			}

			else if (menu_setWvform.u8_choice == 2){
				// Display sqaure wave
				// make duty cycle visible
				// reconfigure the timer appropriately
				// reconfigure the timer interrupt to not use array, and reconfigure timer between interrupts
				//configTimer2();
				continue;
			}

			else if (menu_setWvform.u8_choice == 3){
				// Display trig wave
				continue;
			}
			else if (menu_setWvform.u8_choice == 4){
				// Display user1 wave
				continue;
			}

			else if (menu_setWvform.u8_choice == 5){
				// Display user2 wave
				continue;
			}

			else{
				ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(err);
			}
		}//end if else choice==8
		else if (my_menu.u8_choice == 1){
			ESOS_TASK_WAIT_ESOS_MENU_ENTRY(freq);
			configTimer2();
			printf("Back in ESOS!\n");
		} else if (my_menu.u8_choice == 2){
		 	ESOS_TASK_WAIT_ESOS_MENU_ENTRY(ampltd);
		} else if (my_menu.u8_choice == 3){
		 	ESOS_TASK_WAIT_ESOS_MENU_ENTRY(duty); // should only appear when square wave is selected
		 	//configTimer2();
		} else if (my_menu.u8_choice == 4){
			//readLM60();
			ESOS_TASK_WAIT_ESOS_MENU_DATADISPLAYMENU(read_LM60);
		} else if (my_menu.u8_choice == 5){
			//read1631();
			ESOS_TASK_WAIT_ESOS_MENU_DATADISPLAYMENU(read_1631);
		} else if (my_menu.u8_choice == 6){
			//setLEDS();
			ESOS_TASK_WAIT_ESOS_MENU_ENTRY(LEDs);
		} else if (my_menu.u8_choice == 7){
			// about menu
			about_menu.u8_currentline = 0;
			ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(about_menu);
		}
	}// end while
	ESOS_TASK_END();
}// end user task

// ESOS_USER_TASK(flash_led) {
// 	ESOS_TASK_BEGIN();
// 	while(TRUE){
// 		ESOS_TASK_WAIT_TICKS(ledfp.entries[0].value);
// 		LED1 = !LED1;
// 	}
// 	ESOS_TASK_END();
// }


ESOS_USER_TASK( read_LM60_task ) {
	ESOS_TASK_BEGIN();
	while(true){
		if(my_menu.u8_choice != 4){
			// do nothing
		} else {
			// read the sensor
			ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_3V3);
			ESOS_TASK_WAIT_SENSOR_READ(u16_sensor_millivolts, ESOS_SENSOR_AVG32, ESOS_SENSOR_FORMAT_VOLTAGE);
			ESOS_SENSOR_CLOSE();
			// convert to temperature
			u16_sensor_millivolts /= 3;
			read_LM60.dynamic_data = 100000 * ((int32_t)u16_sensor_millivolts - 424) / 625 / 1000; // removed decimal places
		}
		ESOS_TASK_WAIT_TICKS(10);
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK( read_1631_task ) {
	ESOS_TASK_BEGIN();
	while(true){
		if(my_menu.u8_choice != 5){
			// do nothing
		} else {
			// read the sensor
			read_1631.dynamic_data = -1;
		}
		ESOS_TASK_WAIT_TICKS(10);
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK( set_LEDs_task ) {
	ESOS_TASK_BEGIN();
	while(true){
		// LED1 MSB
		if(LEDs.entries[0].value & 0b0000000000000100){
			esos_uiF14_turnLED1On();
		} else {
			esos_uiF14_turnLED1Off();
		}
		// LED2
		if(LEDs.entries[0].value & 0b0000000000000010){ // 16 bits
			esos_uiF14_turnLED2On();
		} else {
			esos_uiF14_turnLED2Off();
		}
		// ESOS_TASK_WAIT_ON_SEND_STRING(" LED's settings: ");
		// ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(LEDs.entries[0].value);
		// ESOS_TASK_WAIT_ON_SEND_STRING("\n");
		// LED3
		if(LEDs.entries[0].value & 0b0000000000000001){
			esos_uiF14_turnLED3On();
		} else {
			esos_uiF14_turnLED3Off();
		}
		ESOS_TASK_WAIT_TICKS(10);
	}
	ESOS_TASK_END();
}

// for timer use; avoid floats
uint16_t usToTicks(uint16_t u16_us, uint16_t u16_pre){
	uint64_t internal_freq;
	uint64_t timerTicks;
	internal_freq = 60000000; // 60 MHz
	timerTicks = ( FCY * (uint64_t)u16_us) / (uint64_t)u16_pre / (uint64_t)1E6;
	printf("u16_us: %d \nticks: %d \n",u16_us,(int)timerTicks);
	return (uint16_t)timerTicks;
}

// shouldn't implement hardware timers! Use ESOS!

void configTimer2(void){
	//set like thing for understanding could be set to register value 0x0020
	T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
			| T2_32BIT_MODE_OFF
			| T2_SOURCE_INT
			| T2_PS_1_64;

	//PR2 = 5849;//msToU16Ticks(ISP_PERIOD, getTimerPrescale(T2CONbits))-1; 	// results to 5849;
	PR2 = usToTicks(((uint32_t)1E6/freq.entries[0].value), 64) - 1; // -1 since timout is actually PR2 - 1; get desired microsec by 1 mega / freq
	printf("PR2: %d\n", PR2);
	TMR2 = 0;		// clear T2 value
	_T2IF = 0;		// Clear interrupt flag
	_T2IP = 3; 		//priority
	_T2IE = 1; 		// Enable
	T2CONbits.TON = 1;		// turn on the timer
	printf("End of configTimer2(). Returning to ESOS...");
}//end timer config

//Interrupt service ( DAC comes here, i guess)
void _ISR_T2Interrupt(void){
	// get variables
	LED1_ON();
	printf("Hi\n");
	u16_volt_scale = ampltd.entries[0].value;
	if(menu_setWvform.u8_choice == 0){ //tri, sine, square, usr1
		u8_wav_val = au8_tritbl[u16_DAC_wave_out_index];
	} else if(menu_setWvform.u8_choice == 1) { // sine
		u8_wav_val = au8_sinetbl[u16_DAC_wave_out_index];
	} else if(menu_setWvform.u8_choice == 2) { // square
		u8_wav_val = au8_sqrtbl[u16_DAC_wave_out_index];
	}  else if(menu_setWvform.u8_choice == 3) { // usr1
		u8_wav_val = au8_usr1tbl[u16_DAC_wave_out_index];
	} else {
		u8_wav_val = 0x7F; // default to DC of half the max if there is a problem
	}
	//write to DAC
		// amplitude scaled value of the array - double length at least
		u32_scaled_DAC_value = ((uint32_t)u8_wav_val * (uint32_t)u16_volt_scale * 16 / 33) + 
		                       ((uint32_t)u8_wav_val * (uint32_t)u16_volt_scale * 15/33 / 255); // max of 0xFFF, min of 0x000
		// send value to DACA
		printf("Preparing to setDACA inside interrupt...\n");
		setDACA((uint16_t)u32_scaled_DAC_value);
		printf("scaled DAC: %d\n", u32_scaled_DAC_value);
	//increment index of array
	u16_DAC_wave_out_index++;
	//WAVEOUT = !WAVEOUT;
	_T2IF = 0;	// clear interrupt flag
}

void user_init(){
	config_esos_uiF14();
	esos_menu_init();
	esos_RegisterTask(fcn_synth);
	esos_RegisterTask(read_LM60_task); // not completed yet
	esos_RegisterTask(read_1631_task); // not completed yet
	esos_RegisterTask(set_LEDs_task);
	//configTimer2();
	//esos_RegisterTask(flash_led);
	//esos_RegisterTimer(heartbeat_LED, 500);
	//esos_RegisterTask(get_temperature);
}
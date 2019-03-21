// Author : Deepak Chapagain
// Date   : 03/19.2019
// ESOS user application that allow user to enter desired parameter 
// to control the analog function synthesizr output from the DAC.


#include "revF14.h"
#include "fall17lib.h"
#include "esos_menu.h"
#include "esos_lcd44780.h"

// Array for waveforms
#include "stdio.h"

void configDAC(void);
void writeDAC(uint16_t u16_x, uint16_t u16_y);

volatile uint8_t u8_per, u8_amp;	// variables
volatile uint16_t u16_per;

const uint8_t au8_sinetbl[] = {127,133,139,146,152,158,164,170,176,181,187,192,198,203,208,212,217,221,225,229,233,236,239,242,244,247,249,250,252,253,253,254,254,254,253,253,252,250,249,247,244,242,239,236,233,229,225,221,217,212,208,203,198,192,187,181,176,170,164,158,152,146,139,133,127,121,115,108,102,96,90,84,78,73,67,62,56,51,46,42,37,33,29,25,21,18,15,12,10,7,5,4,2,1,1,0,0,0,1,1,2,4,5,7,10,12,15,18,21,25,29,33,37,42,46,51,56,62,67,73,78,84,90,96,102,108,115,121};

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
		"Back..."},
	},
};


// menu for selecting waveform type
static esos_menu_longmenu_t menu_setWvform = {
	.u8_numitems = 6,
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
		{ "user2",
		"wvform"},
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

// Gives option to select from
static esos_menu_entry_t ledfp = {
	.entries[0].label = "Per = ",
	.entries[0].value = 1000, //Default, per = 1s
	.entries[0].min = 250,
	.entries[0].max = 2000,
};

// Selecting frequency
static esos_menu_entry_t freq = {
	.entries[0].label = "Freq= ",
	.entries[0].value = 1, //Default, per = 1s
	.entries[0].min = 0,
	.entries[0].max = 100,
};
	
// heartbeat
ESOS_USER_TIMER( heartbeat_LED ) {
	esos_uiF14_toggleLED3();
}

// LCD 
ESOS_USER_TASK( fcn_synth ) {
	ESOS_TASK_BEGIN();
	while(TRUE){

		// Display main menu until sw3 pressed
		ESOS_TASK_WAIT_ESOS_MENU_LONGMENU(my_menu);

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
			else if (menu_setWvform.u8_choice == 1){
				// Display sine wave
				continue;
			}

			else if (menu_setWvform.u8_choice == 2){
				// Display sqaure wave
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
		else if (my_menu.u8_choice == 1)
			ESOS_TASK_WAIT_ESOS_MENU_ENTRY(freq);
		// else if (my_menu.u8_choice == 2)
		// 	ESOS_TASK_WAIT_ESOS_MENU_ENTRY(ampltd);
		// else if (my_menu.u8_choice == 3)
		// 	ESOS_TASK_WAIT_ESOS_MENU_ENTRY(duty);
		else if (my_menu.u8_choice == 4)
			readLM60();
		else if (my_menu.u8_choice == 5)
			read1631();
		else if (my_menu.u8_choice == 6)
			setLEDS();
		else if (my_menu.u8_choice == 7){
		}
		
	}// end while
	ESOS_TASK_END();
}// end user task

ESOS_USER_TASK(flash_led) {
	ESOS_TASK_BEGIN();
	while(TRUE){
		ESOS_TASK_WAIT_TICKS(ledfp.entries[0].value);
		LED1 = !LED1;
	}
	ESOS_TASK_END();
}




void user_init(){
	
	//config_esos_uiF14();
	esos_menu_init();
	esos_RegisterTask(fcn_synth);
	esos_RegisterTask(flash_led);
	esos_RegisterTimer(heartbeat_LED, 500);

	//esos_RegisterTask(get_temperature);
}

void readLM60(){
	// read temp from LM60. We did it on last lab (t5)
	return;
}

void read1631(){
	// read temp
	return;
}

void setLEDS(){
	// light up 7 LEDs (external leds i guess)
	return;
}

//************************************************************************************//

// to test A A A written on screen
// int i = 0;
// 				for(i = 0; i < 5; i++){
// 					esos_lcd44780_clearScreen();
// 					esos_lcd44780_writeChar( 0, i, 'A'+ i );

// 				ESOS_TASK_WAIT_TICKS(500);
// 				}
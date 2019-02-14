/*******************************************************************
 * 
 * C code framework for ESOS user-interface (UI) service
 * 
 *    requires the EMBEDDED SYSTEMS target rev. F14
 * 
 * ****************************************************************/
 // modified 5pm 2/12
#include <stdbool.h>
#include <stdint.h>
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "revF14.h"



// PRIVATE FUNCTIONS
inline void _esos_uiF14_setRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_RPGCounter = newValue;
    return;
}

inline void _esos_uiF14_setLastRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_lastRPGCounter = newValue;
    return;
}

// PUBLIC SWITCH FUNCTIONS
inline bool esos_uiF14_getSW1Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==true);
}

inline bool esos_uiF14_getSW1Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==false);
}

inline bool esos_uiF14_getSW1DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==true);
}

/***** SW2 and SW3 need need similar  *****/

// PUBLIC LED FUNCTIONS

inline bool esos_uiF14_isLED1On (void) {
    return (_st_esos_uiF14Data.b_LED1On==true);
}

inline bool esos_uiF14_isLED1Off (void) {
    return (_st_esos_uiF14Data.b_LED1On==false);
}

inline void esos_uiF14_turnLED1On (void) {
    _st_esos_uiF14Data.b_LED1On = true;
    return;
}

inline void esos_uiF14_turnLED1Off (void) {
    _st_esos_uiF14Data.b_LED1On = false;
    return;
}

inline void esos_uiF14_toggleLED1 (void) {
    _st_esos_uiF14Data.b_LED1On ^= 1;
    return;
}

inline void esos_uiF14_flashLED1( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
	return;
}

/****** LED2 and LED3 will need similar.  ********/
/****** RED, GREEN, and YELLOW functions need to be created *******/

// PUBLIC RPG FUNCTIONS

inline uint16_t esos_uiF14_getRpgValue_u16 ( void ) {
    return _st_esos_uiF14Data.u16_RPGCounter;
}

inline bool esos_uiF14_isRpgTurning ( void ) {
    return (esos_uiF14_getRPGVelocity_i16() != 0);
}

inline bool esos_uiF14_isRpgTurningSlow( void ) {
  // not yet implemented
	return 0;
}

inline bool esos_uiF14_isRpgTurningMedium( void ) {
  // not yet implemented
	#warning not yet implemented
	return 0;
}

inline bool esos_uiF14_isRpgTurningFast( void ) {
  // not yet implemented
	#warning not yet implemented
		return 0;
}

inline bool esos_uiF14_isRpgTurningCW( void ) {
  // not yet implemented
	#warning not yet implemented
		return 0;
}

inline bool esos_uiF14_isRpgTurningCCW( void ) {
  // not yet implemented
	#warning not yet implemented
		return 0;
}

int16_t esos_uiF14_getRPGVelocity_i16( void ) {
  // not yet implemented
	#warning not yet implemented
		return 0;
}

// UIF14 INITIALIZATION FUNCTION

void config_esos_uiF14() {
  HARDWARE_CONFIG(); // from revF14.h

  // setup your UI implementation
  esos_RegisterTask(__esos_uiF14_task);
}

uint32_t u32_LED1_Timer_Base = 0;
uint32_t u32_LED2_Timer_Base = 0;
uint32_t u32_LED3_Timer_Base = 0;

uint32_t u32_SW1_debounce_Timer_Base = 0;	uint32_t u32_SW1_double_press_Timer_Base = 0;
uint32_t u32_SW2_debounce_Timer_Base = 0;	uint32_t u32_SW2_double_press_Timer_Base = 0;
uint32_t u32_SW3_debounce_Timer_Base = 0;	uint32_t u32_SW3_double_press_Timer_Base = 0;
// UIF14 task to manage user-interface
ESOS_USER_TASK( __esos_uiF14_task ){
  
  ESOS_TASK_BEGIN();
  while(true) {
	// do your UI stuff here
	// LED1 manager
	  if (_st_esos_uiF14Data.u16_LED1FlashPeriod != 0) { // use the timer to toggle the state of LED1
		  if (u32_LED1_Timer_Base + _st_esos_uiF14Data.u16_LED1FlashPeriod <= esos_GetSystemTick()) { // has the timer expired?
			  LED1_TOGGLE();
			  u32_LED1_Timer_Base = esos_GetSystemTick(); // restart the timer
		  }
	  }
	  else if (_st_esos_uiF14Data.b_LED1On) { // LED1 should be ON
		  LED1_ON();
	  }
	  else { // LED1 should be OFF
		  LED1_OFF();
	  }
	// LED2 manager
	// LED3 manager
	// SW1 - debouncer
	  if (SW1_PRESSED()) {
		  if ( ( u32_SW1_debounce_Timer_Base + __SWITCH_DEBOUNCING_INTERVAL ) < esos_GetSystemTick()) {// check for debouncing
			  // SW1 was debounced
			  // was SW1 pressed or double-pressed?
			  if ((u32_SW1_double_press_Timer_Base - u32_SW1_debounce_Timer_Base) > 100) { // was this a double press?
				  // this is a double press!
				  _st_esos_uiF14Data.b_SW1DoublePressed = true; // set SW1 not double pressed
				  if (u32_SW1_double_press_Timer_Base <= __SWITCH_DOUBLE_PRESS_INTERVAL) {
					  u32_SW1_double_press_Timer_Base += 500; // make sure the switch double press timer immediately resets upon release
				  }
			  }
			  else {
				  // still the first press
				  _st_esos_uiF14Data.b_SW1Pressed = true; // set SW1 pressed
			  }
		  }
		  else {
		  }// SW1 is not yet debounced; do nothing!
	  }
	  else { // SW1 is not pressed
		  _st_esos_uiF14Data.b_SW1Pressed = false; // set SW1 not pressed
		  _st_esos_uiF14Data.b_SW1DoublePressed = false; // set SW1 not double pressed
		  u32_SW1_debounce_Timer_Base = esos_GetSystemTick(); // reset debounce timer

		  if ( (( esos_GetSystemTick() - u32_SW1_double_press_Timer_Base) < __SWITCH_DEBOUNCING_INTERVAL) || (u32_SW1_double_press_Timer_Base + __SWITCH_DOUBLE_PRESS_INTERVAL) < esos_GetSystemTick()) { // switch was not double pressed
			  u32_SW1_double_press_Timer_Base = esos_GetSystemTick(); // reset double pressing timer if released before debounce or timer is past the interval
		  }
	  }
	// SW2 - debouncer
	// SW3 - debouncer
	// RPG counter - update previous state


    ESOS_TASK_WAIT_TICKS( __ESOS_UIF14_UI_PERIOD_MS ); // call task every 10 ms
  }
  ESOS_TASK_END();
}

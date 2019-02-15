/*******************************************************************
 * 
 * C code framework for ESOS user-interface (UI) service
 * 
 *    requires the EMBEDDED SYSTEMS target rev. F14
 * 
 * ****************************************************************/
 // modified 5pm 2/12
//#include <stdbool.h>
//#include <stdint.h>
//#include "esos.h"
//#include "esos_pic24.h"
//#include "revF14.h"
#include <stdlib.h> // for the function abs()
//#include <stdbool.h>
//#include <stdint.h>
//#include "esos.h"
//#include "esos_pic24.h"

// user libraries
#include "revF14.h"
#include "esos_f14ui.h"


_st_esos_uiF14Data_t _st_esos_uiF14Data; // should this be in the .c file?


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
// SW1
inline bool esos_uiF14_isSW1Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==true);
}

inline bool esos_uiF14_isSW1Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==false);
}

inline bool esos_uiF14_isSW1DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==true);
}
// SW2
inline bool esos_uiF14_isSW2Pressed(void) {
	return (_st_esos_uiF14Data.b_SW2Pressed == true);
}

inline bool esos_uiF14_isSW2Released(void) {
	return (_st_esos_uiF14Data.b_SW2Pressed == false);
}

inline bool esos_uiF14_isSW2DoublePressed(void) {
	return (_st_esos_uiF14Data.b_SW2DoublePressed == true);
}
// SW3
inline bool esos_uiF14_isSW3Pressed(void) {
	return (_st_esos_uiF14Data.b_SW3Pressed == true);
}

inline bool esos_uiF14_isSW3Released(void) {
	return (_st_esos_uiF14Data.b_SW3Pressed == false);
}

inline bool esos_uiF14_isSW3DoublePressed(void) {
	return (_st_esos_uiF14Data.b_SW3DoublePressed == true);
}

// PUBLIC LED FUNCTIONS

// LED1
inline bool esos_uiF14_isLED1On (void) {
    return (_st_esos_uiF14Data.b_LED1On==true);
}

inline bool esos_uiF14_isLED1Off (void) {
    return (_st_esos_uiF14Data.b_LED1On==false);
}

inline void esos_uiF14_turnLED1On (void) {
    _st_esos_uiF14Data.b_LED1On = true;
	esos_uiF14_flashLED1(0); // stop flashing behavior
    return;
}

inline void esos_uiF14_turnLED1Off (void) {
    _st_esos_uiF14Data.b_LED1On = false;
	esos_uiF14_flashLED1(0); // stop flashing behavior
    return;
}

inline void esos_uiF14_toggleLED1 (void) {
    _st_esos_uiF14Data.b_LED1On ^= 1;
	esos_uiF14_flashLED1(0); // stop flashing behavior
    return;
}

inline void esos_uiF14_flashLED1( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
	return;
}

/****** LED2 and LED3 will need similar.  ********/
// LED2
inline bool esos_uiF14_isLED2On(void) {
	return (_st_esos_uiF14Data.b_LED2On == true);
}

inline bool esos_uiF14_isLED2Off(void) {
	return (_st_esos_uiF14Data.b_LED2On == false);
}

inline void esos_uiF14_turnLED2On(void) {
	_st_esos_uiF14Data.b_LED2On = true;
	esos_uiF14_flashLED2(0); // stop flashing behavior
	return;
}

inline void esos_uiF14_turnLED2Off(void) {
	_st_esos_uiF14Data.b_LED2On = false;
	esos_uiF14_flashLED2(0); // stop flashing behavior
	return;
}

inline void esos_uiF14_toggleLED2(void) {
	_st_esos_uiF14Data.b_LED2On ^= 1;
	esos_uiF14_flashLED2(0); // stop flashing behavior
	return;
}

inline void esos_uiF14_flashLED2(uint16_t u16_period) {
	_st_esos_uiF14Data.u16_LED2FlashPeriod = u16_period;
	return;
}
// LED3
inline bool esos_uiF14_isLED3On(void) {
	return (_st_esos_uiF14Data.b_LED3On == true);
}

inline bool esos_uiF14_isLED3Off(void) {
	return (_st_esos_uiF14Data.b_LED3On == false);
}

inline void esos_uiF14_turnLED3On(void) {
	_st_esos_uiF14Data.b_LED3On = true;
	esos_uiF14_flashLED3(0); // stop flashing behavior
	return;
}

inline void esos_uiF14_turnLED3Off(void) {
	_st_esos_uiF14Data.b_LED3On = false;
	esos_uiF14_flashLED3(0); // stop flashing behavior
	return;
}

inline void esos_uiF14_toggleLED3(void) {
	_st_esos_uiF14Data.b_LED3On ^= 1;
	esos_uiF14_flashLED3(0); // stop flashing behavior
	return;
}

inline void esos_uiF14_flashLED3(uint16_t u16_period) {
	_st_esos_uiF14Data.u16_LED3FlashPeriod = u16_period;
	return;
}

// RED LED - LED1
inline void esos_uiF14_turnRedLEDOn(void) {
	esos_uiF14_turnLED1On();
	return;
}
inline void esos_uiF14_turnRedLEDOff(void) {
	esos_uiF14_turnLED1Off();
	return;
}
// GREEN LED - LED3
inline void esos_uiF14_turnGreenLEDOn(void) {
	esos_uiF14_turnLED3On();
	return;
}
inline void esos_uiF14_turnGreenLEDOff(void) {
	esos_uiF14_turnLED3Off();
	return;
}
// YELLOW LED - LED2
inline void esos_uiF14_turnYellowLEDOn(void) {
	esos_uiF14_turnLED2On();
	return;
}
inline void esos_uiF14_turnYellowLEDOff(void) {
	esos_uiF14_turnLED2Off();
	return;
}

// PUBLIC RPG FUNCTIONS

inline uint16_t esos_uiF14_getRPGValue_u16 ( void ) { //esos_uiF14_getRPGValue_u16
    return _st_esos_uiF14Data.u16_RPGCounter;
}

inline bool esos_uiF14_isRPGTurning ( void ) {
    return (esos_uiF14_getRPGVelocity_i16() != 0);
}

inline bool esos_uiF14_isRPGTurningSlow( void ) {
	return ( abs(esos_uiF14_getRPGVelocity_i16()) >= (__RPG_SLOW_SPEED_THRESHOLD * 10) && (abs(esos_uiF14_getRPGVelocity_i16()) < (__RPG_MEDIUM_SPEED_THRESHOLD * 10)));
}

inline bool esos_uiF14_isRPGTurningMedium( void ) {
	return ( (abs(esos_uiF14_getRPGVelocity_i16()) >= (__RPG_MEDIUM_SPEED_THRESHOLD * 10)) && (abs(esos_uiF14_getRPGVelocity_i16()) < (__RPG_HIGH_SPEED_THRESHOLD * 10)));
}

inline bool esos_uiF14_isRPGTurningFast( void ) {
		return (abs(esos_uiF14_getRPGVelocity_i16()) >= (__RPG_HIGH_SPEED_THRESHOLD * 10));
}

inline bool esos_uiF14_isRPGTurningCW( void ) {
	#warning not tested
		return ((_st_esos_uiF14Data.b_RPGAHigh != _st_esos_uiF14Data.b_RPGBHigh) && (_st_esos_uiF14Data.u16_lastRPGCounter != _st_esos_uiF14Data.u16_RPGCounter));
}

inline bool esos_uiF14_isRPGTurningCCW( void ) {
	#warning not tested
		return ((_st_esos_uiF14Data.b_RPGAHigh == _st_esos_uiF14Data.b_RPGBHigh) && (_st_esos_uiF14Data.u16_lastRPGCounter != _st_esos_uiF14Data.u16_RPGCounter));
}

int16_t esos_uiF14_getRPGVelocity_i16( void ) {
	#warning should getRPGVelocity be calculating? Or should code calculate every 100ms and store the state?
		return (_st_esos_uiF14Data.u16_lastRPGCounter * 10); // returns rotations per second
}

// UIF14 INITIALIZATION FUNCTION

void config_esos_uiF14() {
  HARDWARE_CONFIG(); // from revF14.h

  // setup your UI implementation
  esos_RegisterTask(__esos_uiF14_task);
}

uint32_t u32_LED1_Timer_Base;
uint32_t u32_LED2_Timer_Base;
uint32_t u32_LED3_Timer_Base;

uint32_t u32_SW1_debounce_Timer_Base;	uint32_t u32_SW1_double_press_Timer_Base;
uint32_t u32_SW2_debounce_Timer_Base;	uint32_t u32_SW2_double_press_Timer_Base;
uint32_t u32_SW3_debounce_Timer_Base;	uint32_t u32_SW3_double_press_Timer_Base;

uint32_t u32_RPG_Velocity_Timer_Base;

//int previousRPGVelocity; // currently, this is being stored in the "last RPG counter" variable


// UIF14 task to manage user-interface
ESOS_USER_TASK( __esos_uiF14_task ){
  
	ESOS_TASK_BEGIN() {
		// intitialize data variables

		// INIT SW1, SW2, SW3
		_st_esos_uiF14Data.b_SW1Pressed = 0;	_st_esos_uiF14Data.b_SW1DoublePressed = 0;
		_st_esos_uiF14Data.b_SW2Pressed = 0;	_st_esos_uiF14Data.b_SW2DoublePressed = 0;
		_st_esos_uiF14Data.b_SW3Pressed = 0;	_st_esos_uiF14Data.b_SW3DoublePressed = 0;

		// INIT LED1, LED2, LED3
		_st_esos_uiF14Data.b_LED1On = 0;		_st_esos_uiF14Data.u16_LED1FlashPeriod = 0;
		_st_esos_uiF14Data.b_LED2On = 0;		_st_esos_uiF14Data.u16_LED2FlashPeriod = 0;
		_st_esos_uiF14Data.b_LED3On = 0;		_st_esos_uiF14Data.u16_LED3FlashPeriod = 0;
		
		// INIT RPGA
		_st_esos_uiF14Data.b_RPGAHigh = RPGA_IS_HIGH();		_st_esos_uiF14Data.u16_RPGCounter = 0;
		_st_esos_uiF14Data.b_RPGBHigh = RPGB_IS_HIGH();		_st_esos_uiF14Data.u16_lastRPGCounter = 0;

		// inititialize timers
		u32_LED1_Timer_Base = 0;
		u32_LED2_Timer_Base = 0;
		u32_LED3_Timer_Base = 0;

		u32_SW1_debounce_Timer_Base = 0;	u32_SW1_double_press_Timer_Base = 0;
		u32_SW2_debounce_Timer_Base = 0;	u32_SW2_double_press_Timer_Base = 0;
		u32_SW3_debounce_Timer_Base = 0;	u32_SW3_double_press_Timer_Base = 0;

		u32_RPG_Velocity_Timer_Base = 0;
		// initialize RPG
		/*previousRPGVelocity = 0;*/

		// LED1 manager
		while (true) {
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
			if (_st_esos_uiF14Data.u16_LED2FlashPeriod != 0) { // use the timer to toggle the state of LED2
				if (u32_LED2_Timer_Base + _st_esos_uiF14Data.u16_LED2FlashPeriod <= esos_GetSystemTick()) { // has the timer expired?
					LED2_TOGGLE();
					u32_LED2_Timer_Base = esos_GetSystemTick(); // restart the timer
				}
			}
			else if (_st_esos_uiF14Data.b_LED2On) { // LED2 should be ON
				LED2_ON();
			}
			else { // LED2 should be OFF
				LED2_OFF();
			}
			// LED3 manager
			if (_st_esos_uiF14Data.u16_LED3FlashPeriod != 0) { // use the timer to toggle the state of LED3
				if (u32_LED3_Timer_Base + _st_esos_uiF14Data.u16_LED3FlashPeriod <= esos_GetSystemTick()) { // has the timer expired?
					LED3_TOGGLE();
					u32_LED3_Timer_Base = esos_GetSystemTick(); // restart the timer
				}
			}
			else if (_st_esos_uiF14Data.b_LED3On) { // LED3 should be ON
				LED3_ON();
			}
			else { // LED3 should be OFF
				LED3_OFF();
			}
			// SW1 - debouncer
			if (SW1_PRESSED()) {
				if ((u32_SW1_debounce_Timer_Base + __SWITCH_DEBOUNCING_INTERVAL) <= esos_GetSystemTick()) {// check for debouncing
					// SW1 was debounced
					// was SW1 pressed or double-pressed?
					if ((u32_SW1_double_press_Timer_Base - u32_SW1_debounce_Timer_Base) > 100) { // was this a double press?
						_st_esos_uiF14Data.b_SW1DoublePressed = true; // set SW1 double pressed
						if ( ((esos_GetSystemTick() - u32_SW1_double_press_Timer_Base) <= __SWITCH_DOUBLE_PRESS_INTERVAL) && (u32_SW1_double_press_Timer_Base >= 500)) {
							u32_SW1_double_press_Timer_Base -= 500; // make sure the switch double press timer immediately resets upon release
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

				if (((esos_GetSystemTick() - u32_SW1_double_press_Timer_Base) <= __SWITCH_DEBOUNCING_INTERVAL) || (u32_SW1_double_press_Timer_Base + __SWITCH_DOUBLE_PRESS_INTERVAL) <= esos_GetSystemTick()) { // switch was not double pressed
					u32_SW1_double_press_Timer_Base = esos_GetSystemTick(); // reset double pressing timer if released before debounce or timer is past the interval
				}
			}
			// SW2 - debouncer
			if (SW2_PRESSED()) {
				if ((u32_SW2_debounce_Timer_Base + __SWITCH_DEBOUNCING_INTERVAL) <= esos_GetSystemTick()) {// check for debouncing
																										   // SW2 was debounced
																										   // was SW2 pressed or double-pressed?
					if ((u32_SW2_double_press_Timer_Base - u32_SW2_debounce_Timer_Base) > 100) { // was this a double press?
						_st_esos_uiF14Data.b_SW2DoublePressed = true; // set SW2 double pressed
						if (((esos_GetSystemTick() - u32_SW2_double_press_Timer_Base) <= __SWITCH_DOUBLE_PRESS_INTERVAL) && (u32_SW2_double_press_Timer_Base >= 500)) {
							u32_SW2_double_press_Timer_Base -= 500; // make sure the switch double press timer immediately resets upon release
						}
					}
					else {
						// still the first press
						_st_esos_uiF14Data.b_SW2Pressed = true; // set SW2 pressed
					}
				}
				else {
				}// SW2 is not yet debounced; do nothing!
			}
			else { // SW2 is not pressed
				_st_esos_uiF14Data.b_SW2Pressed = false; // set SW2 not pressed
				_st_esos_uiF14Data.b_SW2DoublePressed = false; // set SW2 not double pressed
				u32_SW2_debounce_Timer_Base = esos_GetSystemTick(); // reset debounce timer

				if (((esos_GetSystemTick() - u32_SW2_double_press_Timer_Base) <= __SWITCH_DEBOUNCING_INTERVAL) || (u32_SW2_double_press_Timer_Base + __SWITCH_DOUBLE_PRESS_INTERVAL) <= esos_GetSystemTick()) { // switch was not double pressed
					u32_SW2_double_press_Timer_Base = esos_GetSystemTick(); // reset double pressing timer if released before debounce or timer is past the interval
				}
			}
			// SW3 - debouncer
			if (SW3_PRESSED()) {
				if ((u32_SW3_debounce_Timer_Base + __SWITCH_DEBOUNCING_INTERVAL) <= esos_GetSystemTick()) {// check for debouncing
																										   // SW3 was debounced
																										   // was SW3 pressed or double-pressed?
					if ((u32_SW3_double_press_Timer_Base - u32_SW3_debounce_Timer_Base) > 100) { // was this a double press?
						_st_esos_uiF14Data.b_SW3DoublePressed = true; // set SW3 double pressed
						if (((esos_GetSystemTick() - u32_SW3_double_press_Timer_Base) <= __SWITCH_DOUBLE_PRESS_INTERVAL) && (u32_SW3_double_press_Timer_Base >= 500)) {
							u32_SW3_double_press_Timer_Base -= 500; // make sure the switch double press timer immediately resets upon release
						}
					}
					else {
						// still the first press
						_st_esos_uiF14Data.b_SW3Pressed = true; // set SW3 pressed
					}
				}
				else {
				}// SW3 is not yet debounced; do nothing!
			}
			else { // SW3 is not pressed
				_st_esos_uiF14Data.b_SW3Pressed = false; // set SW3 not pressed
				_st_esos_uiF14Data.b_SW3DoublePressed = false; // set SW3 not double pressed
				u32_SW3_debounce_Timer_Base = esos_GetSystemTick(); // reset debounce timer

				if (((esos_GetSystemTick() - u32_SW3_double_press_Timer_Base) <= __SWITCH_DEBOUNCING_INTERVAL) || (u32_SW3_double_press_Timer_Base + __SWITCH_DOUBLE_PRESS_INTERVAL) <= esos_GetSystemTick()) { // switch was not double pressed
					u32_SW3_double_press_Timer_Base = esos_GetSystemTick(); // reset double pressing timer if released before debounce or timer is past the interval
				}
			}

			// RPG counter - update previous state
			if ((_st_esos_uiF14Data.b_RPGAHigh != RPGA_IS_HIGH())) { // RPG has been moved!
				if (RPGA_IS_HIGH() != RPGB_IS_HIGH()) { // if current state of both pins are not equal, then they are going clockwise
					_esos_uiF14_setRPGCounter(_st_esos_uiF14Data.u16_RPGCounter + 1); // increment counter
				}
				else { // must be going counter clockwise
					_esos_uiF14_setRPGCounter(_st_esos_uiF14Data.u16_RPGCounter - 1); // decrement the counter
				}
			}
			else { // RPG not moving

			}
			// move value of RPG counter to last timer for velocity calculations
			if (u32_RPG_Velocity_Timer_Base + __RPG_VELOCITY_CALC_PERIOD <= esos_GetSystemTick()) {
				_st_esos_uiF14Data.u16_lastRPGCounter = _st_esos_uiF14Data.u16_RPGCounter; // record the amount the counter moved in 100ms
				_st_esos_uiF14Data.u16_RPGCounter = 0; // reset the counter
				u32_RPG_Velocity_Timer_Base = esos_GetSystemTick(); // restart timer
			}
			// assign new values of RPG pins to variables
			_st_esos_uiF14Data.b_RPGAHigh = (RPGA_IS_HIGH()) ? true : false;
			_st_esos_uiF14Data.b_RPGBHigh = (RPGB_IS_HIGH()) ? true : false;

			ESOS_TASK_WAIT_TICKS(__ESOS_UIF14_UI_PERIOD_MS); // call task every 10 ms
		}
	}
  ESOS_TASK_END();
}

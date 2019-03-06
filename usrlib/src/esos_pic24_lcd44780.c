/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

/**
 * \addtogroup ESOS_Task_LCD_Service
 * @{
 */
 
/*** I N C L U D E S *************************************************/
#include "esos_pic24_lcd44780.h"
#include "revF14.h"

/*** T H E   C O D E *************************************************/
void __esos_lcd44780_pic24_config ( void )
{
    // Set up the hardware aspects of the pic24 interface of the LCD module service
    //    direction, thresholds, etc beyond what is already done in esos_lcd44780_config()


}

void __esos_lcd44780_pic24_setDataPins( uint8_t u8_data) {
	// write the hardware-specific code to take the u8_data passed in
	// and place it on the appropriate data pins
	if (u8_data & 0b00000001 != 0) { __ESOS_LCD44780_HW_SET_D0(); }
	else { __ESOS_LCD44780_HW_CLEAR_D0(); }
	if (u8_data & 0b00000010 != 0) { __ESOS_LCD44780_HW_SET_D1(); }
	else { __ESOS_LCD44780_HW_CLEAR_D1(); }
	if (u8_data & 0b00000100 != 0) { __ESOS_LCD44780_HW_SET_D2(); }
	else { __ESOS_LCD44780_HW_CLEAR_D2(); }
	if (u8_data & 0b00001000 != 0) { __ESOS_LCD44780_HW_SET_D3(); }
	else { __ESOS_LCD44780_HW_CLEAR_D3(); }
	if (u8_data & 0b00010000 != 0) { __ESOS_LCD44780_HW_SET_D4(); }
	else { __ESOS_LCD44780_HW_CLEAR_D4(); }
	if (u8_data & 0b00100000 != 0) { __ESOS_LCD44780_HW_SET_D5(); }
	else { __ESOS_LCD44780_HW_CLEAR_D5(); }
	if (u8_data & 0b01000000 != 0) { __ESOS_LCD44780_HW_SET_D6(); }
	else { __ESOS_LCD44780_HW_CLEAR_D6(); }
	if (u8_data & 0b10000000 != 0) { __ESOS_LCD44780_HW_SET_D7(); }
	else { __ESOS_LCD44780_HW_CLEAR_D7(); }
}

uint8_t __esos_lcd44780_pic24_getDataPins( void ) {
	// write the hardware-specific code to read the appropriate data pins
	// and create the uint8 data to return to the caller
	uint8_t value = 0;
	if (__ESOS_LCD44780_HW_GET_D0()) { value | 0b00000001; }
	if (__ESOS_LCD44780_HW_GET_D1()) { value | 0b00000010; }
	if (__ESOS_LCD44780_HW_GET_D2()) { value | 0b00000100; }
	if (__ESOS_LCD44780_HW_GET_D3()) { value | 0b00001000; }
	if (__ESOS_LCD44780_HW_GET_D4()) { value | 0b00010000; }
	if (__ESOS_LCD44780_HW_GET_D5()) { value | 0b00100000; }
	if (__ESOS_LCD44780_HW_GET_D6()) { value | 0b01000000; }
	if (__ESOS_LCD44780_HW_GET_D7()) { value | 0b10000000; }
	return value;
}

void __esos_lcd44780_pic24_configDataPinsAsInput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "inputs" to prepare for a read of the LCD module
	CONFIG_LCD_DATA_PINS_READ_MODE();
}

void __esos_lcd44780_pic24_configDataPinsAsOutput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "outputs" to prepare for a write to the LCD module
	CONFIG_LCD_DATA_PINS_WRITE_MODE();
}

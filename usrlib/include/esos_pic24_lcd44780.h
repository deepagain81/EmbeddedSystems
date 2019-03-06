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

#ifndef ESOS_PIC24_LCD_H
#define ESOS_PIC24_LCD_H

/**
 * \addtogroup ESOS_Task_LCD_Service
 * @{
 */
 
 /** \file
 *  \brief This file contains routines which configure and
 *  use an LCD interface on the Microchip PIC24 MCUs.
 */

/* I N C L U D E S **********************************************************/
#include <esos_lcd.h>

/* HW-SPECIFIC MACROS *******************************************************/
#define  __esos_lcd44780_hw_config()		__esos_lcd44780_hwxxx_config

#define  __esos_lcd44780_hw_setDataPins() \
    __esos_lcd44780_hwxxx_setDataPins
#define  __esos_lcd44780_hw_getDataPins() \
    __esos_lcd44780_hwxxx_getDataPins
#define  __esos_lcd44780_hw_configDataPinsAsInput() \
    __esos_lcd44780_hwxxx_configDataPinsAsInput
#define  __esos_lcd44780_hw_configDataPinsAsOutput() \
    __esos_lcd44780_hwxxx_configDataPinsAsOutput

#define  __ESOS_LCD44780_HW_SET_E_HIGH()          LCDE_SET()        // E=1
#define  __ESOS_LCD44780_HW_SET_E_LOW()           LCDE_RESET()      // E=0
#define  __ESOS_LCD44780_HW_SET_RW_READ()         LCDRW_SET()       // RW=1
#define  __ESOS_LCD44780_HW_SET_RW_WRITE()        LCDRW_RESET()     // RW=0
#define  __ESOS_LCD44780_HW_SET_RS_REGISTERS()    LCDRS_SET() 	    // RS=0
#define  __ESOS_LCD44780_HW_SET_RS_DATA()         LCDRS_RESET()     // RS=1

#define  __ESOS_LCD44780_HW_SET_D0()            LCDD0_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D0()          LCDD0_RESET()
#define  __ESOS_LCD44780_HW_GET_D0()            IS_LCDD0_SET()
#define  __ESOS_LCD44780_HW_SET_D1()            LCDD1_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D1()          LCDD1_RESET()
#define  __ESOS_LCD44780_HW_GET_D1()            IS_LCDD1_SET()
#define  __ESOS_LCD44780_HW_SET_D2()            LCDD2_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D2()          LCDD2_RESET()
#define  __ESOS_LCD44780_HW_GET_D2()            IS_LCDD2_SET()
#define  __ESOS_LCD44780_HW_SET_D3()            LCDD3_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D3()          LCDD3_RESET()
#define  __ESOS_LCD44780_HW_GET_D3()            IS_LCDD3_SET()
#define  __ESOS_LCD44780_HW_SET_D4()            LCDD4_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D4()          LCDD4_RESET()
#define  __ESOS_LCD44780_HW_GET_D4()            IS_LCDD4_SET()
#define  __ESOS_LCD44780_HW_SET_D5()            LCDD5_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D5()          LCDD5_RESET()
#define  __ESOS_LCD44780_HW_GET_D5()            IS_LCDD5_SET()
#define  __ESOS_LCD44780_HW_SET_D6()            LCDD6_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D6()          LCDD6_RESET()
#define  __ESOS_LCD44780_HW_GET_D6()            IS_LCDD6_SET()
#define  __ESOS_LCD44780_HW_SET_D7()            LCDD7_SET()
#define  __ESOS_LCD44780_HW_CLEAR_D7()          LCDD7_RESET()
#define  __ESOS_LCD44780_HW_GET_D7()            IS_LCDD7_SET()

/* P R I V A T E   P R O T O T Y P E S *****************************************/
void __esos_lcd44780_pic24_config ( void );
void __esos_lcd44780_pic24_setDataPins( uint8_t u8_data);
uint8_t __esos_lcd44780_pic24_getDataPins( void );
void __esos_lcd44780_pic24_configDataPinsAsInput( void );
void __esos_lcd44780_pic24_configDataPinsAsOutput( void );

#endif

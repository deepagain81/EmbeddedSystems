// .. "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
//    All rights reserved.
//    (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
//    (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
//    (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
//
//    Permission to use, copy, modify, and distribute this software and its
//    documentation for any purpose, without fee, and without written agreement is
//    hereby granted, provided that the above copyright notice, the following
//    two paragraphs and the authors appear in all copies of this software.
//
//    IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
//    DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
//    OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
//    HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//    THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
//    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
//    AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
//    ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
//    PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
//
//    Please maintain this header in its entirety when copying/modifying
//    these files.
//
// *****************************************************************************
// reset.c - Demonstrates software reset, idle, sleep, watchdog timer operation.
// *****************************************************************************
// Program that tests some of the ways that a PIC24 can be reset, as well as sleep mode, idle mode, and the watchdog timer. Intended to be used in a laboratory exercise in which the current draw of the processor is monitored before/after the power down modes.

#include "pic24_all.h"


// This function puts the PIC24 in low-power mode, following the directions given in DS70615C section 9.2.2.1 by:
//  - Configuring all digital I/O pins as inputs and enabling pullups on them.
//  - Configuring all analog I/O pins shared with digital I/O pins to be digital only.
//  - Disabling the internal voltage regulators when in sleep mode.
//  - Disabling all peripherals, except UART 1.
//
// WARNING: if pullups are enabled on pins used by the oscillator, the clock typically stops running. Therefore, this code only works with the FRC oscillator.
//
// Note: this function is processor specific.
#if (defined(__dsPIC33EP128GP502__))
void configPinsForLowPower(void) {
  // Configure all digital I/O pins for input.
  TRISB = 0xFFFF;
  TRISA = 0xFFFF;
  ANSELA = 0x0000;
  ANSELB = 0x0000;
  // The primary oscillator is not used, so
  // turn on all the pull-ups.
  CNPUA = 0xFFFF;
  CNPUB = 0xFFFF;
  // Turn off the internal voltage regulators when in sleep.
  _VREGS = 0;
  _VREGSF = 0;
  // Power off all peripherals except UART 1.
  PMD1 = 0xFFDF;
  PMD2 = 0xFFFF;
  PMD3 = 0xFFFF;
  PMD4 = 0xFFFF;
  PMD6 = 0xFFFF;
  PMD7 = 0xFFFF;
}
#else
# warning "Using dummy function for configPinsForLowPower()."
void configPinsForLowPower(void) {
}
#endif


uint8_t printMenuGetChoice() {
  uint8_t u8_c;
  outString("'1' enable watchdog timer\n");
  outString("'2' enter sleep mode\n");
  outString("'3' enter idle mode\n");
  outString("'4' enable watchdog timer and enter sleep mode\n");
  outString("'5' doze = divide by 2\n");
  outString("'6' doze = divide by 128\n");
  outString("'7' execute reset instruction\n");
  outString("Choice: ");
  u8_c = inChar();
  outChar(u8_c);      //echo character
  outString("\n");      //newline
  return(u8_c);
}

//persistent variables are not touched at reset
_PERSISTENT uint8_t u8_resetCount;


int main(void) {
  configClock();                //clock configuration
  configPinsForLowPower();      //config pins for low power since we are measuring current
  configHeartbeat();            //heartbeat LED
  configDefaultUART(DEFAULT_BAUDRATE); //serial port config
  outString(HELLO_MSG);         //say Hello!

  if (_POR) {
    u8_resetCount = 0;      // if power on reset, init the reset count variable
  } else {
    u8_resetCount++;        //keep track of the number of non-power on resets
  }
  if (_WDTO) {
    _SWDTEN = 0;            //If Watchdog timeout, disable WDT.
  }
  printResetCause();        //print statement about what caused reset
  //print the reset count
  outString("The reset count is ");
  outUint8(u8_resetCount);
  outString("\n");

  while (1) {
    uint8_t u8_c;
    u8_c = printMenuGetChoice();
    DELAY_MS(1);  //let characters clear the UART before executing choice
    switch (u8_c) {
      case '1':         //enable watchdog timer
        _SWDTEN = 1;    //WDT ENable bit = 1
        break;
      case '2':         //sleep mode
        asm("pwrsav #0");  //sleep
        outString("after sleep\n");   //never executed.
        break;
      case '3':         //idle mode
        asm("pwrsav #1"); //idle
        outString("after idle\n");   //never executed.
        break;
      case '4':
        _SWDTEN = 1;     //WDT ENable bit = 1
        asm("pwrsav #0"); //sleep
        outString("after WDT enable, sleep.\n"); //executed on wakeup
        break;
      case '5':
        _DOZE = 1; //chose divide by 2
        _DOZEN = 1; //enable doze mode
        break;
      case '6':
        _DOZE = 7; //chose divide by 128
        _DOZEN = 1; //enable doze mode
        break;
      case '7':
        asm("reset"); //reset myself
        break;

      default:
        break;  //ignore
    }

  } // end while (1)
  return 0;
}

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
// ***********************************************************************
// change_wakeup.c - wake from sleep using a change notification interrupt
// ***********************************************************************
// Demonstrates the use of change notification interrupt
// to wake from Sleep mode. A pushbutton on a change notfication pin will be used
// to wake the from Sleep mode.

#include "pic24_all.h"


// Interrupt Service Routine for Change Notification
void _ISRFAST _CNInterrupt(void) {
  // Clear the change notification interrupt bit.
  _CNIF = 0;
}

// Pushbutton configuration: assumes PB is on RB13.
void config_pb(void)  {
  CONFIG_RB13_AS_DIG_INPUT();
  ENABLE_RB13_PULLUP();
  ENABLE_RB13_CN_INTERRUPT();
  // Wait for pull-up to take effect.
  DELAY_US(1);
}

int main(void) {
  configBasic(HELLO_MSG);
  /** Configure the switch ***********/
  config_pb();  //enables individual CN interrupt also
  /** Configure Change Notification general interrupt  */
  _CNIF = 0;         //Clear the interrupt flag
  _CNIP = 2;         //Choose a priority > 0
  _CNIE = 1;         //enable the Change Notification general interrupt
  while (1) {
    outString("Entering Sleep mode. Press button to wake.\n");
    // Finish sending characters before sleeping
    WAIT_UNTIL_TRANSMIT_COMPLETE_UART1();
    SLEEP();         //macro for asm("pwrsav #0")
  }
}

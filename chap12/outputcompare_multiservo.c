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
// ********************************************************************************************************************************************************
// outcompare_multiservo.c - Demonstrates pulse width modulation using four digital outputs and the OC1 module to create four PWM outputs for hobby servos.
// ********************************************************************************************************************************************************
// Demonstrates pulse width modulation using four digital outputs
// and the OC1 module to create four PWM outputs for hobby servos.
// A table is used to control the pulse widths of the four servos.
#include "pic24_all.h"
#include <stdio.h>

#define PWM_PERIOD 20000   //in microseconds

void  configTimer2(void) {
  T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
          | T2_32BIT_MODE_OFF
          | T2_SOURCE_INT
          | T2_PS_1_256 ;  //1 tick = 1.6 us at FCY=40 MHz
  PR2 = usToU16Ticks(PWM_PERIOD, getTimerPrescale(T2CONbits)) - 1;
  TMR2  = 0;       //clear timer2 value
}

//just pick four digital outputs
#define NUM_SERVOS 4
#define SERVO0  _LATB2
#define SERVO1  _LATB3
#define SERVO2  _LATB13
#define SERVO3  _LATB14

#define MIN_PW  600            //minimum pulse width, in us
#define MAX_PW 2400            //minimum pulse width, in us
#define SLOT_WIDTH 2800        //slot width, in us

volatile uint16_t au16_servoPWidths[NUM_SERVOS];
volatile uint8_t u8_currentServo =0;
volatile uint8_t u8_servoEdge = 1;  //1 = RISING, 0 = FALLING
volatile uint16_t u16_slotWidthTicks = 0;

void initServos(void) {
  uint8_t u8_i;
  uint16_t u16_initPW;

  u8_currentServo = 0;
  CONFIG_RB2_AS_DIG_OUTPUT();
  CONFIG_RB3_AS_DIG_OUTPUT();
  CONFIG_RB13_AS_DIG_OUTPUT();
  CONFIG_RB14_AS_DIG_OUTPUT();
  u16_initPW = usToU16Ticks(MIN_PW + (MAX_PW-MIN_PW)/2, getTimerPrescale(T2CONbits));

  //config all servos for half maximum pulse width
  for (u8_i=0; u8_i<NUM_SERVOS; u8_i++) au16_servoPWidths[u8_i]=u16_initPW;
  SERVO0 = 0; //all servo outputs low initially
  SERVO1 = 0;
  SERVO2 = 0;
  SERVO3 = 0;  //outputs initially low
  u16_slotWidthTicks = usToU16Ticks(SLOT_WIDTH, getTimerPrescale(T2CONbits));
}



void setServoOutput (uint8_t u8_servo, uint8_t u8_val) {
  switch (u8_servo) {
    case 0:
      SERVO0 = u8_val;
      break;
    case 1:
      SERVO1 = u8_val;
      break;
    case 2:
      SERVO2 = u8_val;
      break;
    case 3:
      SERVO3 = u8_val;
      break;
    default:
      break;
  }
}

void _ISR _OC1Interrupt(void) {
  _OC1IF = 0;
//change the servo's value
  setServoOutput(u8_currentServo, u8_servoEdge);
//schedule next interrupt
  if (u8_servoEdge == 1) {  //rising edge
    //next interrupt occurs after pulse width has elapsed
    OC1R = OC1R + au16_servoPWidths[u8_currentServo];
    u8_servoEdge = 0;     //change to falling edge
  } else { //falling edge
    //next interrupt occurs at beginning of next slot
    if (u8_currentServo != NUM_SERVOS -1)
      OC1R = u16_slotWidthTicks*(u8_currentServo+1);
    else //last servo!
      OC1R = 0;
    u8_servoEdge = 1;     //change to rising edge
    u8_currentServo++;
    if (u8_currentServo == NUM_SERVOS) u8_currentServo = 0;
  }
}


void configOutputCapture1(void) {
  T2CONbits.TON = 0;       //disable Timer when configuring Output compare
  OC1R  =  0;  //initialize to 0
//turn on the compare toggle mode using Timer2
#ifdef OC1CON1
  OC1CON1 = OC_TIMER2_SRC |      //Timer2 source
            OC_TOGGLE_PULSE;     //single compare toggle, just care about compare event
  OC1CON2 = OC_SYNCSEL_TIMER2;   //synchronize to timer2
#else
  OC1CON = OC_TIMER2_SRC |      //Timer2 source
           OC_TOGGLE_PULSE;     //single compare toggle, just care about compare event
#endif
  _OC1IF = 0;
  _OC1IP = 1;
  _OC1IE = 1;    //enable the OC1 interrupt
}


char sz_buf[32];

void getServoValue(void) {
  int16_t u16_servo;
  int16_t u16_pw;
  printf("Choose servo (1,2,3,4): ");
  inStringEcho(sz_buf,31);
  sscanf(sz_buf,"%d",(int *) &u16_servo);
  if ((u16_servo > 4) || (u16_servo < 1)) {
    printf("Invalid servo..\n");
    return;
  }
  printf("Enter pulse width (min 600, max 2400): ");
  inStringEcho(sz_buf,31);
  sscanf(sz_buf,"%d",(int *) &u16_pw);
  if ((u16_pw > 2400) || (u16_pw < 600)) {
    printf("Invalid pulse width..\n");
    return;
  }
  //set the pulse width
  _OC1IE = 0; //disable the interrupt while changing
  au16_servoPWidths[u16_servo-1]=usToU16Ticks(u16_pw, getTimerPrescale(T2CONbits));
  _OC1IE = 1;
}

int main(void) {
  configBasic(HELLO_MSG);
  configTimer2();
  initServos();
  configOutputCapture1();
  T2CONbits.TON = 1;       //turn on Timer2 to start PWM

  while (1) {
    getServoValue();       //prints menu, gets new servo value from console.
  }
}

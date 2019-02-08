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
// ************************************************************************************
// adc4simul_dma.c - Simultaneous sampling of 4 channels (only for PIC24 CPUs with DMA)
// ************************************************************************************
// Simultaneously samples four channels and uses a timer used to trigger conversions.
// Four channels are
// Ping-pong buffering is used.
// Conversion results are printed to screen.
// (HEX values and voltages are printed.)
// This is only for PIC24 CPUs without DMA.

#include "pic24_all.h"
#include "stdio.h"


#if !defined(_DMA0IF) || defined(__PIC24E__) || defined(__dsPIC33E__)
# warning "This processor selection does not have a DMA module; this code example is incompatible with a PIC24 CPU that does not have DMA."
# warning "OR this is a E family processor, which this example does not support."
int main(void) {
  return 0;
}
#else

// setup an output to help us measure ADC IRQ responses
#define CONFIG_LED2()       CONFIG_RB5_AS_DIG_OUTPUT()
#define LED2                _LATB5

#define   ADC_LEN           10
#define   ADC_NSTEPS        1024
#define   ADC_12BIT_FLAG    0

// set this to one of the values of
// 1, 2, 4, 8, 16, 32, 64, 128
#define CONVERSIONS_PER_INPUT  1 //for this example, assumed always to be '1'
#define MAX_CHANNELS   16
//DMA transfer size is in words.
#define MAX_TRANSFER (CONVERSIONS_PER_INPUT*MAX_CHANNELS)   //make power of two for alignment to work

//DMA buffers, alignment is based on number of bytes
/// \cond nodoxygen
// These attributes confuse Doxygen.
uint16_t au16_bufferA[MAX_TRANSFER] __attribute__((space(dma),aligned(MAX_TRANSFER*2)));
uint16_t au16_bufferB[MAX_TRANSFER] __attribute__((space(dma),aligned(MAX_TRANSFER*2)));
/// \endcond


//For this example, we will just use ordered conversion mode,
//and one conversion per ADC input
void configDMA_ADC(uint8_t    u8_ch0Select, \
                   uint16_t   u16_ch123SelectMask, \
                   uint16_t   u16_numTcyMask) {


  AD1CON1bits.ADON = 0;   // turn off ADC (changing setting while ADON is not allowed)
  /** Configure the internal ADC **/
  AD1CON1 = ADC_CLK_TMR | ADC_SAMPLE_SIMULTANEOUS | ADC_ADDMABM_ORDER;
  AD1CON3 = (u16_numTcyMask & 0x00FF);
  AD1CON2 = ADC_VREF_AVDD_AVSS | ADC_CONVERT_CH0123;
#if (defined(__PIC24H__)|| defined(__dsPIC33F__))
  AD1CHS0 = ADC_CH0_NEG_SAMPLEA_VREFN | (u8_ch0Select & 0x1F);
  AD1CHS123 = u16_ch123SelectMask;
#else
  AD1CHS = ADC_CH0_NEG_SAMPLEA_VREFN | (u8_ch0Select & 0x1F);
#endif
  AD1CON4 = ADC_1_WORD_PER_INPUT;
  AD1CSSL = 0;

  //configure the DMA channel 0 interrupt
  DMA0PAD = (unsigned int) &ADC1BUF0;
  DMA0REQ = DMA_IRQ_ADC1;
  DMA0STA = __builtin_dmaoffset(au16_bufferA);
  DMA0STB = __builtin_dmaoffset(au16_bufferB);
  DMA0CNT = 4 - 1; //converting four inputs, so DMA0CNT = 3
  DMA0CON =   //configure and enable the module Module
    (DMA_MODULE_ON |
     DMA_SIZE_WORD |
     DMA_DIR_READ_PERIPHERAL |
     DMA_INTERRUPT_FULL |
     DMA_NULLW_OFF |
     DMA_AMODE_REGISTER_POSTINC |
     DMA_MODE_CONTINUOUS_PING_PONG);

  _DMA0IF = 0;
  _DMA0IP = 6;
  _DMA0IE = 1;


  AD1CON1bits.ADON = 1;   // turn on the ADC
}

uint16_t              au16_buffer[MAX_TRANSFER];
volatile  uint16_t    au16_sum[MAX_TRANSFER];
volatile  uint8_t     u8_gotData;
volatile  uint8_t     u8_activeBuffer;

void _ISRFAST _DMA0Interrupt(void) {
  static uint8_t      u8_adcCount=64;
  uint8_t       u8_i;
  uint16_t*     au16_adcHWBuff = (uint16_t*) &au16_bufferA;
  _DMA0IF = 0;

  if (u8_activeBuffer) {
    au16_adcHWBuff = (uint16_t*) &au16_bufferB;
    u8_activeBuffer = 0;
  } else {
    au16_adcHWBuff = (uint16_t*) &au16_bufferA;
    u8_activeBuffer = 1;
  }

  //accumulate the sum
  for ( u8_i=0; u8_i<MAX_TRANSFER; u8_i++) {
    au16_buffer[u8_i] += au16_adcHWBuff[u8_i];
  } //end for()

  // we got the data, so start the sampling process again
  SET_SAMP_BIT_ADC1();
  u8_adcCount--;
  if (u8_adcCount==0) {
    u8_adcCount = 64;
    u8_gotData = 1;
    for ( u8_i=0; u8_i<MAX_TRANSFER; u8_i++) {
      au16_sum[u8_i] = au16_buffer[u8_i];
      au16_buffer[u8_i] = 0;
    } //end for()
  }
  // toggle a port pin so that we can measure how often DMA IRQs are coming in
  LED2 = !LED2;
}


int main (void) {
  uint8_t   u8_i;
  uint16_t  u16_pot;
  uint32_t  u32_ticks;
  float   f_pot;

  configBasic(HELLO_MSG);

  // make RA0/AN0/VREF+ a digital input to kill the pullup and
  // set the TRISA bit, then make it ANALOG so the ADC will work
  CONFIG_RA0_AS_ANALOG();
  CONFIG_RA1_AS_ANALOG();
  CONFIG_RB0_AS_ANALOG();
  CONFIG_RB12_AS_ANALOG();

  CONFIG_LED2();

  u8_gotData = 0;

  // configure T2/T3 as 32-bit timer to trigger every 1/64 second
  T3CONbits.TON = 0;
  T2CONbits.TON = 0;
  T2CON = T2_32BIT_MODE_ON | T2_PS_1_1 | T2_SOURCE_INT;
  TMR3 = 0;
  TMR2 = 0;
  u32_ticks = usToU32Ticks(15625, getTimerPrescale(T2CONbits)) - 1;     // # of ticks for 1/64 seconds
  PR3 = u32_ticks>>16;
  PR2 = u32_ticks & 0xFFFF;
  T2CONbits.TON = 1;

  configDMA_ADC(12, ADC_CH123_POS_SAMPLEA_AN0AN1AN2, ADC_CONV_CLK_10Tcy );
  SET_SAMP_BIT_ADC1();

  while (1) {
    while (!u8_gotData) {
      doHeartbeat();
    }
    u8_gotData = 0;
    for ( u8_i=0; u8_i<4; u8_i++) {
      u16_pot = au16_sum[u8_i];
      f_pot = (3.3 / 1023 / 64 ) * u16_pot;
      printf("r");
      outChar( '0'+u8_i );
      printf(":0x%04X=%1.3fV  ",  u16_pot, (double) f_pot );
    } //end for()
    printf("\n");
  } //endof while()
} // endof main()
#endif

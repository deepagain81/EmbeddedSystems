// Benjamin Wilkinson and Deepak Chapagain
// 4/10/2019

#include "usr_spi_lib.h"
#include <stdint.h>
#include "pic24_all.h"
#include "pic24_ports.h"

#define FALSE (0)
#define TRUE !(FALSE)

void spiInteraction(uint16_t *pu16_out, uint16_t *pu16_in, uint16_t u16_cnt) {
  static uint16_t*      pu16_tempPtrIn;
  static uint16_t*      pu16_tempPtrOut;
  static uint16_t       u16_tempCnt, u16_i;
  static uint8_t        u8_isReading, u8_isWriting;
  uint16_t              u16_scratch;

  pu16_tempPtrOut=pu16_out;
  pu16_tempPtrIn=pu16_in;
  u16_tempCnt=u16_cnt;
  if (pu16_tempPtrOut == NULLPTR)
    u8_isWriting = FALSE;
  else
    u8_isWriting = TRUE;

  if (pu16_tempPtrIn == NULLPTR)
    u8_isReading = FALSE;
  else
    u8_isReading = TRUE;

  // clear the overflow flag, just in case it is set
  if (SPI1STATbits.SPIROV) SPI1STATbits.SPIROV = 0;
  //clear SPI interrupt flag since we are about to write new value to SPI
  _SPI1IF = 0;
   // read SPI1BUF to clear SPI_RX_BUFFER_FULL bit just in case previous
   //   SPI use did not read the SPI1BUF that last time!
  
  u16_scratch = SPI1BUF;
  for (u16_i=0; u16_i < u16_tempCnt; u16_i++) {
    if (u8_isWriting) {
      SPI1BUF = *pu16_tempPtrOut;
      pu16_tempPtrOut++;
    } else {
      SPI1BUF = 0;
    } // end isWriting

    /* Seen some strange behavior checking _SPI1IF like the
     * hardware support library.  The following method is valid
     * and appears to work in all cases.
     */
    // wait for TX word to be copied to SPI1SR
    while( SPI1STAT & SPI_TX_BUFFER_FULL );
    // wait for RX word to be copied from SPI1SR
    while( !(SPI1STAT & SPI_RX_BUFFER_FULL) ); // was a wait until
    // read the word from SPI (clears SPI_RX_BUFFER_FULL bit)
    u16_scratch = SPI1BUF;
    if (u8_isReading) {
      *pu16_tempPtrIn = u16_scratch;
      pu16_tempPtrIn++;
    } // end isReading
  } // end for()
} // end spiInteraction


void spi_write1( uint16_t u16_d1 ){
	spiInteraction(&u16_d1, NULLPTR, 1);
}
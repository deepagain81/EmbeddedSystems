// Created by Benjamin Wilkinson
// April 2, 2019
// task1, part1
// based on "Microcontrollers 2nd edition" by Jones, Reese, and Bruce code from pg 562 to 570

#include "pic24_ecan.h" // defines ECANMSG
#include "pic24_dma.h"  // for DMA marcros
#include "pic24_util.h" // defines HELLO_MSG and doHeartbeat()
#include "pic24_delay.h"// DELAY_MS
//#include "pic24_all.h"
#include <stdio.h> // for printf

#define NUM_TX_BUFS (0) // reserve one buffer for TX
#define NUM_BUFS    (8) // total number of buffers
//ECANMSG msgBuf[NUM_BUFS] __attribute__((space(dma),eds,aligned(NUM_BUFS*16))); // eds added; allows allocation in the extended data space (including dma memory)

#define MSG_ID (0x7A0) // Arbitrary choice for 11-bit message ID
#define RX_BUFFER_ID (1) //if 15, use FIFO - which RX buffer should be used as input?

// config DMA code called inside configECAN1

// from ecan_example_fifo.c
__eds__ ECANMSG msgBuf[NUM_BUFS] __attribute__((space(dma),aligned(NUM_BUFS*16),eds));

//configure DMA transmit buffer
// void configDMA0(void) {
//   DMAPWC = 0; // Reset the DMA Peripheral Write Collision Status Register
//   _DMA0IF = 0;
//   DMA0PAD = (unsigned int) &C1TXD;
//   DMA0REQ = DMA_IRQ_ECAN1TX;
//   DMA0STAL = (unsigned long int) &msgBuf;
//   DMA0STAH = 0;
//   DMA0CNT =   sizeof(ECANMSG)/2 -1;  // == 7
//   DMA0CON =   //configure and enable the module Module
//     (DMA_MODULE_ON |
//      DMA_SIZE_WORD |
//      DMA_DIR_WRITE_PERIPHERAL |
//      DMA_INTERRUPT_FULL |
//      DMA_NULLW_OFF |
//      DMA_AMODE_PERIPHERAL_INDIRECT |
//      DMA_MODE_CONTINUOUS);
// }

//configure DMA receive buffer
void configDMA1(void) {
  _DMA1IF = 0;
  DMA1PAD = (unsigned int) &C1RXD;
  DMA1REQ = DMA_IRQ_ECAN1RX;
  DMA1STAL = (unsigned long int) &msgBuf;
  DMA1STAH = 0;
  DMA1CNT =  sizeof(ECANMSG)/2 -1;  // == 7
  DMA1CON =   //configure and enable the module Module
    (DMA_MODULE_ON |
     DMA_SIZE_WORD |
     DMA_DIR_READ_PERIPHERAL |
     DMA_INTERRUPT_FULL |
     DMA_NULLW_OFF |
     DMA_AMODE_PERIPHERAL_INDIRECT |
     DMA_MODE_CONTINUOUS);
}

// from book
// void configDMA0() {
// 	_DMA0IF = 0;
// 	DMA0PAD = (unsigned int) &C1TXD;       // periferal address register - set to TX of ECAN
// 	DMA0REQ = DMA_IRQ_ECAN1TX;             // DMA interrupt request (indicates which interrupt to service)
// 	//DMA0STA = __builtin_dmaoffset(msgBuf); // DMA address registers - book says STA, divided into high and low in reality
// 	DMA0STAH = 0x0000;
// 	DMA0STAL = __builtin_dmaoffset(msgBuf);
// 	DMA0CNT = 7; // 8 words in ECANMSG     // indicates how many transfers are needed each interrupt
// 	DMA0CON =                              // DMA module init
// 	(   DMA_MODULE_ON |
// 		DMA_SIZE_WORD |
// 		DMA_DIR_WRITE_PERIPHERAL |
// 		DMA_INTERRUPT_FULL |
// 		DMA_NULLW_OFF |
// 		DMA_AMODE_PERIPHERAL_INDIRECT |
// 		DMA_MODE_CONTINUOUS
// 	);
// }

// void configDMA1() {
// 	_DMA1IF = 0;
// 	DMA1PAD = (unsigned int) &C1RXD;
// 	DMA1REQ = DMA_IRQ_ECAN1RX;
// 	//DMA1STA = __builtin_dmaoffset(msgBuf); // book combines 2 registers STAH and STAL (for high and low)
// 	DMA1STAH = 0x0000;
// 	DMA1STAL = __builtin_dmaoffset(msgBuf);
// 	DMA1CNT = 7; // 8 words in ECANMSG
// 	DMA1CON = // DMA Module Init
// 	(   DMA_MODULE_ON |
// 		DMA_SIZE_WORD |
// 		DMA_DIR_READ_PERIPHERAL |
// 		DMA_INTERRUPT_FULL |
// 		DMA_NULLW_OFF |
// 		DMA_AMODE_PERIPHERAL_INDIRECT |
// 		DMA_MODE_CONTINUOUS
// 	);
// }

void configECAN1() {
	uint8_t u8_i;
	CHANGE_MODE_ECAN1(ECAN_MODE_CONFIGURE);
	configBaudECAN1();
	C1FCTRL = ECAN_FIFO_START_AREA_0 | ECAN_DMA_BUF_SIZE_8;
	configRxFilterECAN1(0, MSG_ID, ECAN_MATCH_SID, RX_BUFFER_ID, 0);
	configRxMaskECAN1(0, 0x000, 0, 0); // formerly 0x7FC (filter all but last 2 bits) - last bit used to be a 1 for extended address space?

	// configRxFilterECAN1( 0, 0x7a0, 0, 1, 0 ); // 2nd to last argument used to be 15 - for FIFO operation
    // configRxMaskECAN1( 0, 0x000, 0, 0 );

	clrRxFullOvfFlagsECAN1(); // clear all RX full, overflow flags
	// First 8 buffs must be configured as either TX or RX
	for ( u8_i = 0; u8_i<NUM_BUFS; u8_i++ ) {
		if(u8_i < NUM_TX_BUFS){
			configTxRxBufferECAN1(u8_i, ECAN_TX_BUFF, 3); // config buffer u8_i as TX
		}
		else {
			configTxRxBufferECAN1(u8_i, ECAN_RX_BUFF, 3);
		}
	}
	// DMA configuration after initilizing ECAN
	//configDMA0();
	configDMA1();
	CHANGE_MODE_ECAN1(ECAN_MODE_NORMAL);
}

// // user defined
// uint32_t rrot32(uint32_t x){
// 	uint32_t u32_lower_bits = 0;
// 	uint32_t result = 0;
// 	u32_lower_bits = x & 0x0000000F; // get the last 4 bits
// 	result = x >> 4;
// 	result = result | (u32_lower_bits << 28);
// 	printf("rrot32 result: 0x%X\n",result);
// 	return result;
// }

uint32_t rrot32(uint32_t u32_x) { // from example code
    if (u32_x & 0x1) {
      u32_x = u32_x >> 1;
      u32_x = u32_x | 0x80000000;
    } else u32_x = u32_x >> 1;
    return u32_x;
}

int main(void){
	uint32_t u32_out0, u32_out1, u32_in0, u32_in1;
	uint8_t  rx_buff_id, u8_cnt;

	configBasic(HELLO_MSG);
	configECAN1();
	CHANGE_MODE_ECAN1(ECAN_MODE_LISTEN_ONLY); // don't want to reply - ECAN_MODE_LISTEN_ONLY not ECAN_LISTEN_ALL_MESSAGES
	u32_out0 = 0xFEDCBA98; u32_out1 = 0x76543210;
	u8_cnt = 0;
	while (1) {
		doHeartbeat();
		if(_DMA1IF){ // message received
			_DMA1IF = 0; // RX message accepted
			rx_buff_id = GET_FIFO_READBUFFER_ECAN1();
			//printf("rx_buff_id = %d\n", rx_buff_id);
			u32_in0 = msgBuf[rx_buff_id].data.u32[0];
			u32_in1 = msgBuf[rx_buff_id].data.u32[1];
			printf("Rx Buff: %d. Msg ID: 0x%X, Received Message: 0x%08lX%08lX\n",
				rx_buff_id, msgBuf[rx_buff_id].w0.SID, u32_in0, u32_in1);
			clrRxFullFlagECAN1(rx_buff_id);
		}
		// DELAY_MS(500);
		// msgBuf[0].data.u32[0] = u32_out0; // output to Buffer 0 (TX) in 32 bit increments
		// msgBuf[0].data.u32[1] = u32_out1;
		// //printf("Before packing: %x, %x\n",msgBuf[0].data.u32[0], msgBuf[0].data.u32[1]);
		// // Format Buffer 0 for TX with SID=MSG_ID, data length = 8 bytes
		// formatStandardDataFrameECAN(&msgBuf[0], MSG_ID+u8_cnt, 8);
		// ////printf("After packing: %x, %x\n",msgBuf[0].data.u32[0], msgBuf[0].data.u32[1]);
		// startTxECAN1(0); // start transmission of buffer 0
		// while (getTxInProgressECAN1(0)) {
		// 	doHeartbeat(); // wait for transmission to end
		// }
		// //printf("After Transmission: %x, %x\n",msgBuf[0].data.u32[0], msgBuf[0].data.u32[1]);
		// _DMA0IF = 0;  // clear the flag; reg used for TX
		// DELAY_MS(10); // delay for reception
		// if(!_DMA1IF){ // DMA1 is used for RX - flag should be set if message was received
		// 	printf("Message ID 0x%X rejected by acceptance filter.\n", MSG_ID+u8_cnt);
		// } else {
		// 	_DMA1IF = 0; // RX message accepted
		// 	rx_buff_id = GET_FIFO_READBUFFER_ECAN1();
		// 	//printf("rx_buff_id = %d\n", rx_buff_id);
		// 	u32_in0 = msgBuf[rx_buff_id].data.u32[0];
		// 	u32_in1 = msgBuf[rx_buff_id].data.u32[1];
		// 	printf("Rx Buff: %d. Msg ID: 0x%X, Out: 0x%08lX%08lX, In: 0x%08lX%08lX\n",
		// 		rx_buff_id, msgBuf[rx_buff_id].w0.SID, u32_out0, u32_out1, u32_in0, u32_in1);
		// 	clrRxFullFlagECAN1(rx_buff_id);
		// }
		// //printf("u32_out0: %x, u32_out1 %x\n", u32_out0, u32_out0);
		// u32_out0 = rrot32(u32_out0);
		// u32_out1 = rrot32(u32_out1);
		// u8_cnt++;
		// if (u8_cnt == 8){
		// 	u8_cnt = 0;
		// }
		//printf("----------------------------------\n");
	} // end while
} // end main
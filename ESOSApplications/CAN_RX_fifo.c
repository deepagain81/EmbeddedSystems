/*
Author: Deepak Chapagain
Date  : April 1, 2019
T07   : It recreates the output shown in Fig 13.29 of text book.

*/

 #define NUM_TX_BUFS	(1)
 #define NUM_BUFS	(8)
ECANMSG msgBuf[NUM_BUFS]	__atterbute__((space(dma), aligned(NUM_BUFS*16)));

#define RX_BUFFER_ID	(15)	// 15 means FIFO for RX
void configECAN1()
{
	uint8_t u8_i;
	CHANGE_MODE_ECAN1(ECAN_MODE_CONFIGURE);
	configBaudECAN1();
	C1FCTRL = ECAN_FIFO_START_AREA_1 | ECAN_DMA_BUF_SIZE_8;	//sets buff start at 1(total 8)

	// here
	configRxFilterECAN1(0, MSG_ID, ECAN_MATCH_SID, RX_BUFFER_ID, 0);
	configRxMaskECAN(0, 0x7FC, 0, 1);
	clrRxFullOvfFlagsECAN1();	//clear all RX full, overflow flags.
	//First 8 buffs mist be configured as wither TX or RX
	for (u8_i=0; u8_i < 8; u8_i++)
	{
		if(u8_i < NUM_TX_BUFS){
			configTxRxBufferECAN1(u8_i, ECAN_TX_BUFF, 3);
		}else{
			configTxRxBufferECAN1(u8_i, ECAN_RX_BUFF, 3);
		}
	}
	// Do DMA configuration after ECAN has been initilized
	configDMA0();
	configDMA1();
	CHANGE_MODE_ECAN1(ECAN_MODE_NORMAL);
}//end void

int main(void)
{
	//Declare variables
	uint32_t u32_out0, u32_out1, u32_in0, u32_in1;
	uint8_t rx_buff_id, u8_cnt;
	//config hello msg
	configBasic(HELLO_MSG);
	configECAN1();		//Config ECAN and change mode to loopback
	CHANGE_MODE_ECAN1(ECAN_MODE_LOOPBACK);
	u32_out0 = 0xFEDCBA98;	u32_out1 = 0x76543210;	//random num
	u8_cnt = 0;
	while(1)
	{
		DELAY_MS(500);
		msgBuf[0].data.u32[0] = u32_out0;
		msgBuf[0].data.u32[1] = u32_out1;
		//format Buffer 0 for TX with SID=MSG_ID, data lenght=8 bytes
		formatStandardDataFrameECAN(&msgBuf[0], MSG_ID + u8_cnt, 8);
		startTxEcan1(0);	// start transmission of buff 0.
		while(getTxInProgressECAN1(0)){
			doHeartbeat();
		}

		_DMA0IF = 0; 	//DMA0 used for TX, clear the flag
		DELAY_MS(10);	//Delay for reception

		if(!_DMA1IF)
		{
			printf("Message ID 0x%x rejected by acceptance filter.\n",MSG_ID+u8_cnt );
		}else{
			_DMA1IF = 0;	//clear flag indicating Rx msg accepted
			rx_buff_id = GET_FIFO_READBUFFER_ECAN1();	//get the buff no that has current Rx data
			u32_in0 = msgBuf[rx_buff_id].data.u32[0];
			u32_in1 = msgBuf[rx_buff_id].data.u32[1];
			printf("Rx buff: %d. Msg ID: 0x%x, Out: ox%lx%lx, In: 0x%lx%lx \n",rx_buff_id, msgBuf[rx_buff_id].w0.SID, u32_out0, u32_out1, u32_in0, u32_in1);
			clrRxFullOvfFlagsECAN1(rx_buff_id);
		}
		//rotate to make new TX data
		u32_out0 = rrot32(u32_out0);	
		u32_out1 = rrot32(u32_out1);
		u8_cnt++;	//Change msg id

		if(u8_cnt == 8)	u8_cnt = 0;
	}//end while
}//end main

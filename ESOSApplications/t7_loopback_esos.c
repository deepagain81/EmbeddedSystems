// Created by Benjamin and Deepak
// 4/3/2019

#include "revF14.h"
#include "esos.h"
#include "esos_ecan.h"
#include "esos_pic24_ecan.h" // for __esos_ecan_hw_config_ecan()
#include "fall17lib.h"

// function declarations
uint32_t rrot32(uint32_t u32_x);

ESOS_USER_TASK ( heartbeat_LED ) {
    ESOS_TASK_BEGIN();
    while (1) {
        LED3 = !LED3;
        ESOS_TASK_WAIT_TICKS( 500 );
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK ( task_loopback ) {
	static uint32_t u32_out0 = 0xFEDCBA98;
	static uint32_t u32_out1 = 0x76543210;
	static uint32_t au32_send_buf[2];
	static MAILMESSAGE msg;
	static uint8_t msgCount;
	static uint16_t u16_can_id;
	static uint8_t u8_len;
	static uint32_t recv_buf[2] = {0};
	static char converted_buf[20];
	
	ESOS_TASK_BEGIN();
	esos_ecan_canfactory_subscribe( __pstSelf, 0x7a0, 0xfffc, MASKCONTROL_FIELD_NONZERO );
	ENABLE_DEBUG_MODE(); // prints what message buffer it was received in, and notes unaccepted messages
	while(1){
		// send u32_out0 and u32_out1 - total of 8 bytes
		au32_send_buf[0] = u32_out0;
		au32_send_buf[1] = u32_out1;
		ESOS_ECAN_SEND( 0x7a0 + msgCount, au32_send_buf, 8 );    //CAN_id, msg, msg_len in bytes
		ESOS_TASK_WAIT_ON_SEND_STRING("Sent");
		u32_out0 = rrot32(u32_out0);
		u32_out1 = rrot32(u32_out1);
		msgCount++;
		ESOS_TASK_YIELD();
		//ESOS_TASK_WAIT_TICKS(10);

		// receive
		if(ESOS_TASK_IVE_GOT_MAIL())
		{
	        ESOS_TASK_GET_NEXT_MESSAGE( &msg );
	        u16_can_id = msg.au16_Contents[0];
	        u8_len = ESOS_GET_PMSG_DATA_LENGTH( (&msg) ) - sizeof( uint16_t );
	        memcpy( recv_buf, &msg.au8_Contents[ sizeof( uint16_t ) ], u8_len );
	        utoar(recv_buf, converted_buf, 16, 20);
	        ESOS_TASK_WAIT_ON_SEND_STRING(", Msg ID: ");
	        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(u8_len);
	        ESOS_TASK_WAIT_ON_SEND_STRING(", Out: ");
	        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(u32_out0);
	        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(u32_out1);
	        ESOS_TASK_WAIT_ON_SEND_STRING(", In: ");
	        ESOS_TASK_WAIT_ON_SEND_STRING(converted_buf);
	        //ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(msg.data.u32[0]);
	        //ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(msg.data.u32[1]);
	        ESOS_TASK_WAIT_ON_SEND_STRING("\n");
    	}

		ESOS_TASK_WAIT_TICKS(50);
	}
	ESOS_TASK_END();
}

void user_init(){
	HARDWARE_CONFIG();
	__esos_ecan_hw_config_ecan(); // ECAN config
	CHANGE_MODE_ECAN1(ECAN_MODE_LOOPBACK);// set ECAN mode
	esos_RegisterTask(task_loopback);// register user task
	esos_RegisterTask(heartbeat_LED);
}

uint32_t rrot32(uint32_t u32_x) { // from example code
    if (u32_x & 0x1) {
      u32_x = u32_x >> 1;
      u32_x = u32_x | 0x80000000;
    } else u32_x = u32_x >> 1;
    return u32_x;
}
// Created by Benjamin and Jordan
// 4/2/2019

#include "revF14.h"
#include "fall17lib.h"
#include "esos_f14ui.h"
#include "esos_menu.h"
#include "esos_lcd44780.h"
#include "DAC_comms.h"
#include "esos_sensor.h"
#include "esos_ecan.h"
#include <stdlib.h>


ESOS_USER_TASK(listen_for_can_messages) {
	ESOS_TASK_BEGIN();
	uint8_t buf[8] = {0};
    uint8_t u8_len;
    uint16_t u16_can_id;
    int i; // see for loop below

    esos_uiF14_flashLED3(1000);
	esos_ecan_canfactory_subscribe( __pstSelf, 0x7a0, 0x0000, MASKCONTROL_FIELD_NONZERO );
	ESOS_TASK_WAIT_ON_SEND_STRING("Made it here!\n");
	//ENABLE_DEBUG_MODE();

	while(true){
		static MAILMESSAGE msg;
        
        //ESOS_TASK_WAIT_ON_SEND_STRING("Loop!\n");

        ESOS_TASK_WAIT_FOR_MAIL();
        ESOS_TASK_GET_NEXT_MESSAGE( &msg );
        u16_can_id = msg.au16_Contents[0];
        u8_len = ESOS_GET_PMSG_DATA_LENGTH( ( &msg - sizeof( uint16_t ) ) );
        memcpy( buf, &msg.au8_Contents[ sizeof( uint16_t ) ], u8_len );
        ESOS_TASK_WAIT_ON_SEND_STRING("! ");
        for(i = 0; i<8; i++){
        	ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(buf[i]);
    	}
        ESOS_TASK_WAIT_ON_SEND_STRING("\n");
        
        LED1 = buf[0];
        LED2 = buf[1];
        
        ESOS_TASK_YIELD();
	}
	ESOS_TASK_END();
}

void user_init(){
	config_esos_uiF14();
	esos_RegisterTask(CANFactory);
	esos_RegisterTask(listen_for_can_messages);
	//esos_RegisterTimer(heartbeat_LED, 500);
	//esos_RegisterTask(get_temperature);
}
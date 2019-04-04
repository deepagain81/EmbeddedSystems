/*
Author: Deepak Chapagain
Date  : April 03, 2019
		An application that only snoop on CAN message communication.
		1. Use mode "ECAN_MODE_LISEN_ONLY"
		2. listen-->decode-->print
		3. Use inString() to get UART input
		4. compare and filter to select what to print
*/


// Includes
#include    <string.h>
#include    "esos.h"
#include    "esos_pic24.h"
#include    "esos_ecan.h"
#include    "esos_f14ui.h"

// Defines
#define CONFIG_LED1()   CONFIG_RF4_AS_DIG_OUTPUT()
#define CONFIG_LED2()   CONFIG_RB14_AS_DIG_OUTPUT()
#define CONFIG_LED3()   CONFIG_RB15_AS_DIG_OUTPUT()
#define LED1            _LATF4
#define LED2            _LATB14
#define LED3            _LATB15

#define CONFIG_SW1()    {   CONFIG_RB13_AS_DIG_INPUT(); \
                            ENABLE_RB13_PULLUP(); \
                            DELAY_US( 1 ); \
                        }
#define CONFIG_SW2()    {   CONFIG_RB12_AS_DIG_INPUT(); \
                            ENABLE_RB12_PULLUP(); \
                            DELAY_US( 1 ); \
                        }
#define SW1             _RB13
#define SW2             _RB12

/* ************************************************************************ */
//Functions
ESOS_USER_TASK(listen_only)
{
	static uint8_t buf[8] = {0};
	static uint8_t u8_len;
	static uint16_t u16_canID;
	//
	static uint16_t u16_usrInput;

	ESOS_TASK_BEGIN();

	// Ask user whom to listen
	u16_usrInput = inString();

	//must subscribe to canfactory to hear can msg.
	esos_ecan_canfactory_subscribe(__pstSelf, 0x7a0, 0xfffc, MASKCONTROL_FIELD_NONZERO);	// param--> xxxx, rcv_id, xxxx, filter specification
	esos_uiF14_flashLED3(500);

	while(1)
	{
		// Make sure msg is received
		static MAILMESSAGE msg;
		ESOS_TASK_WAIT_FOR_MAIL();
		ESOS_TASK_GET_NEXT_MESSAGE(&msg);
		ESOS_TASK_WAIT_ON_SEND_STRING("GOT MAIL\n");
		// ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(msg[0]);
  //       ESOS_TASK_WAIT_ON_SEND_STRING(" ");
  //       ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(msg[1]);
  //       ESOS_TASK_WAIT_ON_SEND_STRING("\n");

        // get the id and msg itself
        u16_canID = msg.au16_Contents[0];	// this comes from frame structure

        if (u16_usrInput == u16_canID)
        {
        	u8_len = ESOS_GET_PMSG_DATA_LENGTH((&msg)) - sizeof(uint16_t);	// 
	        ESOS_TASK_WAIT_ON_SEND_STRING("u8_len: ");
	        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(u8_len);
	        ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	        //copy that msg into memory
	        memcpy(buf, &msg.au8_Contents[sizeof(uint16_t)], u8_len);	// param--> varName, msg itself(start point), len of msg to store
	        //print msg to check we got right msg
	        ESOS_TASK_WAIT_ON_SEND_STRING("Buf[0]:");
	        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(buf[0]);
	        ESOS_TASK_WAIT_ON_SEND_STRING("\nBuf[1]: ");
	        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(buf[1]);
	        ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	        //Update what needs to once we receive msg.

        }else{
        	ESOS_TASK_WAIT_ON_SEND_STRING("No user available with your input user_id.\n ");
        }
        


        ESOS_TASK_YIELD();
	}//end while
	ESOS_TASK_END();
}//end listen_only


void user_init(void)
{
	CONFIG_LED1();
    CONFIG_LED2();
    CONFIG_LED3();
    CONFIG_SW1();
    CONFIG_SW2();

    __esos_ecan_hw_config_ecan(); // ECAN config
    config_esos_uiF14();		// hw config
    CHANGE_MODE_ECAN1(ECAN_MODE_LISTEN_ONLY);

    //esos_RegisterTask( heartbeat_LED );
    esos_RegisterTask( CANFactory );
    esos_RegisterTask( listen_only );
}//end user_init
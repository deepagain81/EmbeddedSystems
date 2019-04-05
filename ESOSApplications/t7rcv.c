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
// *****************************
// app_ecan_receiver.c - receive a message using the CANFactory
// *****************************
// ESOS application program to receive a mail message using the CANFactory task.

// Includes
#include    <string.h>
#include    "esos.h"
#include    "esos_pic24.h"
#include    "esos_ecan.h"
#include    "esos_pic24_ecan.h"
//#include    "esos_f14ui.h"

// Defines
// Defines
#define TRUE            1
#define FALSE           0

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

// Prototypes

// Globals

/************************************************************************
 * User supplied functions
 ************************************************************************
 */


ESOS_USER_TASK ( ecan_receiver ) {
    static uint8_t buf[8] = {0};
    static uint8_t u8_len;
    static uint16_t u16_can_id;
    
    ESOS_TASK_BEGIN();
    
    esos_ecan_canfactory_subscribe( __pstSelf, 0x7a0, 0xffff, MASKCONTROL_FIELD_NONZERO );
    esos_uiF14_flashLED3(1000);
    ESOS_TASK_WAIT_TICKS( 100 ); // delay for proper TEXT startup
    
    while ( TRUE ) {
        static MAILMESSAGE msg;
        
        ESOS_TASK_WAIT_FOR_MAIL();
        ESOS_TASK_WAIT_ON_SEND_STRING("Mail Received\n");
        ESOS_TASK_GET_NEXT_MESSAGE( &msg );
        ESOS_TASK_WAIT_ON_SEND_STRING("Received: ");
        // ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(msg[0]);
        // ESOS_TASK_WAIT_ON_SEND_STRING(" ");
        // ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(msg[1]);
        // ESOS_TASK_WAIT_ON_SEND_STRING("\n");

        u16_can_id = msg.au16_Contents[0];
        u8_len = ESOS_GET_PMSG_DATA_LENGTH( (&msg) ) - sizeof( uint16_t );

        ESOS_TASK_WAIT_ON_SEND_STRING("u8_len: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(u8_len);
        ESOS_TASK_WAIT_ON_SEND_STRING("\n");

        memcpy( buf, &msg.au8_Contents[ sizeof( uint16_t ) ], u8_len );
        ESOS_TASK_WAIT_ON_SEND_STRING("Buf[0]:");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(buf[0]);
        ESOS_TASK_WAIT_ON_SEND_STRING("\nBuf[1]: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(buf[1]);
        ESOS_TASK_WAIT_ON_SEND_STRING("\n");

        ESOS_TASK_WAIT_ON_SEND_STRING("u8_len: ");
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(u8_len);
        ESOS_TASK_WAIT_ON_SEND_STRING("\n");
        
        LED1 = buf[0];
        LED2 = buf[1];
        // if(buf[0]){
        //     esos_uiF14_turnLED1On();
        // } else {
        //     esos_uiF14_turnLED1Off();
        // }
        // // LED2
        // if(buf[1]){
        //     esos_uiF14_turnLED2On();
        // } else {
        //     esos_uiF14_turnLED2Off();
        // }
        
        ESOS_TASK_YIELD();
    }
    
    ESOS_TASK_END();
}

/****************************************************
 *  user_init()
 ****************************************************
 */
void user_init ( void ) {
    __esos_unsafe_PutString( HELLO_MSG );
    //config_esos_uiF14();

    CONFIG_LED1();
    CONFIG_LED2();
    CONFIG_LED3();
    CONFIG_SW1();
    CONFIG_SW2();
    __esos_ecan_hw_config_ecan(); // ECAN config
    CHANGE_MODE_ECAN1(ECAN_MODE_NORMAL);

    //esos_RegisterTask( heartbeat_LED );
    esos_RegisterTask( CANFactory );
    esos_RegisterTask( ecan_receiver );
}

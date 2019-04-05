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
// app_test_message_type_sending.c - send a message using the CANFactory
// *****************************
// ESOS application program to send a mail message using the CANFactory task.

// Includes
#include    <string.h>
#include    "esos.h"
#include    "esos_pic24.h"
#include    "esos_ecan.h"
#include    "embedded_lab_CANLab.h"
#include    "esos_f14ui.h"

// Defines
#define USR_ECAN_BEACON_INTERVAL            ( 10*1000) /*should be 30  seconds*/
#define USR_ECAN_BEACON_TIMEOUT_INTERVAL    ( 30*1000) /*should be 120 seconds*/

// Prototypes

// Globals

/************************************************************************
 * User supplied functions
 ************************************************************************
 */

ESOS_USER_TASK ( test_message_type_sending ) {
    static uint8_t buf[8];
    static uint16_t u16_sending_can_message_id; // should encompass the destination board Team, member, and the message type
    static uint16_t u16_test_msg_types;
    
    ESOS_TASK_BEGIN();
    esos_uiF14_flashLED3(1000);
    ESOS_TASK_WAIT_TICKS( 100 ); // delay for proper TEXT startup
    u16_test_msg_types = 0;
    
    while ( 1 ) {
        buf[0] = !SW1;
        buf[1] = !SW2;
        buf[2] = buf[0];
        buf[3] = buf[1];
        buf[4] = buf[0];
        buf[5] = buf[1];
        buf[6] = buf[0];
        buf[7] = buf[1];
        
        LED1 = buf[0];
        LED2 = buf[1];

        u16_sending_can_message_id = calcMsgID(MY_ID) | u16_test_msg_types; // CANMSG_TYPE_LEDS
        u16_test_msg_types = (u16_test_msg_types + 1) % 10;
        
        ESOS_ECAN_SEND( u16_sending_can_message_id, buf, 8 );    //CAN_id, msg, msg_len
          ESOS_TASK_WAIT_ON_SEND_STRING("SENT\n");
        ESOS_TASK_WAIT_TICKS( 500 );
    }
    
    ESOS_TASK_END();
}

ESOS_USER_TASK ( transmit_beacon ) {
    static uint16_t u16_sending_can_message_id; // should encompass the destination board Team, member, and the message type
    uint8_t buf = 0;

    ESOS_TASK_BEGIN();
    while(1){
        u16_sending_can_message_id = calcMsgID(MY_ID) | CANMSG_TYPE_BEACON; // CANMSG_TYPE_BEACON
        buf = 0;
        ESOS_ECAN_SEND( u16_sending_can_message_id, buf, 0 );    //CAN_id, msg, msg_len
        ESOS_TASK_WAIT_ON_SEND_STRING("Beacon sent.\n");
        ESOS_TASK_WAIT_TICKS(5000); // should be 30 seconds
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK ( monitor_for_beacons ) {
    static uint8_t buf[8] = {0};
    static uint8_t u8_len;
    static uint16_t u16_canID;

    static uint8_t i; // for loop

    // received msg derived info
    static uint8_t  u8_received_team_id;
    static uint8_t  u8_received_member_id;
    static uint8_t  u8_received_message_type;
    static uint16_t u16_received_arr_index;
    
    ESOS_TASK_BEGIN();
    
    esos_ecan_canfactory_subscribe( __pstSelf, 0x000 | CANMSG_TYPE_BEACON, 0x001f, MASKCONTROL_FIELD_NONZERO );
    esos_uiF14_flashLED3(1000);
    ESOS_TASK_WAIT_TICKS( 100 ); // delay for proper TEXT startup
    
    while ( 1 ) {
        static MAILMESSAGE msg;
        
        //ESOS_TASK_WAIT_FOR_MAIL();
        if(ESOS_TASK_IVE_GOT_MAIL()){
            ESOS_TASK_GET_NEXT_MESSAGE( &msg );
        // get the id and msg itself
            u16_canID = msg.au16_Contents[0];   // this comes from frame structure
            // interpret msgid
            u8_received_team_id      = stripTeamID(u16_canID);
            u8_received_member_id    = stripMemberID(u16_canID);
            u8_received_message_type = stripTypeID(u16_canID);
            u16_received_arr_index   = getArrayIndexFromMsgID(u16_canID);
            // print msg id
            ESOS_TASK_WAIT_ON_SEND_STRING("t7synth TEST\n");
            ESOS_TASK_WAIT_ON_SEND_STRING("msg id: ");
            ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(u16_canID);
            ESOS_TASK_WAIT_ON_SEND_STRING(" team #");
            ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u8_received_team_id);
            ESOS_TASK_WAIT_ON_SEND_STRING(" member #");
            ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u8_received_member_id);
            // output member name, if in range
            ESOS_TASK_WAIT_ON_SEND_STRING("(");
            if(u16_received_arr_index < NUM_OF_IDS){
                ESOS_TASK_WAIT_ON_SEND_STRING(aCANID_IDs[u16_received_arr_index].psz_name);
                ESOS_TASK_WAIT_ON_SEND_STRING(" - ");
                ESOS_TASK_WAIT_ON_SEND_STRING(aCANID_IDs[u16_received_arr_index].psz_netID);
            } else {
                ESOS_TASK_WAIT_ON_SEND_STRING("???");
            }
            ESOS_TASK_WAIT_ON_SEND_STRING(")");
            // print msg type
            ESOS_TASK_WAIT_ON_SEND_STRING(" MSG TYPE: ");
            if(u8_received_message_type < 10){
                ESOS_TASK_WAIT_ON_SEND_STRING(CAN_MESSAGE_TYPE_DESCRIPTION[u8_received_message_type]);
            } else {
                ESOS_TASK_WAIT_ON_SEND_STRING("???");
            }

            ESOS_TASK_WAIT_ON_SEND_STRING("\n***-------------------------------***\n");
           
            u8_len = ESOS_GET_PMSG_DATA_LENGTH((&msg)) - sizeof(uint16_t);  // 
            ESOS_TASK_WAIT_ON_SEND_STRING("u8_len: ");
            ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u8_len);
            ESOS_TASK_WAIT_ON_SEND_STRING("\n");
        }//end if
        
        ESOS_TASK_YIELD();
    } // end while loop
    ESOS_TASK_END();
}

/****************************************************
 *  user_init()
 ****************************************************
 */
void user_init ( void ) {
    config_esos_uiF14();
    __esos_ecan_hw_config_ecan(); // ECAN config
    CHANGE_MODE_ECAN1(ECAN_MODE_NORMAL);
    __esos_unsafe_PutString( HELLO_MSG );
     
    esos_RegisterTask( CANFactory ); // from esos_ecan.c
    esos_RegisterTask( transmit_beacon );
    esos_RegisterTask( monitor_for_beacons );
    //esos_RegisterTask( test_message_type_sending );
}

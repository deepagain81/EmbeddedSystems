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
#include 	"revF14.h"
#include    "esos_f14ui.h"
#include 	"embedded_lab_CANLab.h"

// Defines

//Functions

uint32_t arrToUnsignedNumber(char *arr, uint16_t length, uint16_t base);
int charToHexValue(char c_input);
int power(int base, unsigned int exp);
bool isInCharRange(char input, char lowChar, char highChar);

ESOS_USER_TASK(listen_only)
{
	static uint8_t buf[8] = {0};
	static uint8_t u8_len;
	static uint16_t u16_canID;

	static uint8_t i; // for loop

	// received msg derived info
	static uint8_t  u8_received_team_id;
	static uint8_t  u8_received_member_id;
	static uint8_t  u8_received_message_type;
	static uint16_t u16_received_arr_index;
	//
	static uint8_t u8_usrInput;
	static char    char_input_buf[32]; // up to 16 for mask, up to 16 for filter
	static uint8_t u8_usr_input_index;
	static BOOL    b_hex_entry;

	static uint32_t new_filter = 0x7a0;
	static uint32_t new_mask   = 0x0000;

	ESOS_TASK_BEGIN();

	// Ask user whom to listen
	// u16_usrInput = inString(); // should use ESOS_GET...

	//must subscribe to canfactory to hear can msg.
	esos_ecan_canfactory_subscribe(__pstSelf, new_filter, new_mask, MASKCONTROL_FIELD_NONZERO);	// param--> current task(points to self), rcv_id, mask->it matches with bits, filter specification
	
		ESOS_TASK_WAIT_ON_SEND_STRING("new_filter: ");
		ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(new_filter);
		ESOS_TASK_WAIT_ON_SEND_STRING("   new_mask: ");
		ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(new_mask);
		ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	esos_uiF14_flashLED3(500);
	ESOS_TASK_WAIT_ON_SEND_STRING( HELLO_MSG );
	//ENABLE_DEBUG_MODE();
	b_hex_entry = 0;
	ESOS_TASK_WAIT_TICKS( 500 ); // delay for proper TEXT startup

	while(1)
	{
		// Make sure msg is received
		static MAILMESSAGE msg;
		//ESOS_TASK_WAIT_FOR_MAIL();
		if(ESOS_TASK_IVE_GOT_MAIL()){
			ESOS_TASK_WAIT_FOR_MAIL();
			ESOS_TASK_GET_NEXT_MESSAGE(&msg);

	        // get the id and msg itself
	        u16_canID = msg.au16_Contents[0];	// this comes from frame structure
	        // interpret msgid
			u8_received_team_id      = stripTeamID(u16_canID);
			u8_received_member_id    = stripMemberID(u16_canID);
			u8_received_message_type = stripTypeID(u16_canID);
			u16_received_arr_index   = getArrayIndexFromMsgID(u16_canID);
			// print msg id
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
	       
	    	u8_len = ESOS_GET_PMSG_DATA_LENGTH((&msg)) - sizeof(uint16_t);	// 
	        ESOS_TASK_WAIT_ON_SEND_STRING("u8_len: ");
	        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u8_len);
	        ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	        //copy that msg into memory
	        memset(buf, 0, sizeof(uint8_t[8]));
	        memcpy(buf, &msg.au8_Contents[sizeof(uint16_t)], u8_len);	// param--> varName, msg itself(start point), len of msg to store
	        //print data from the msg
	        for(i = 0; i<8; i++){
	        	ESOS_TASK_WAIT_ON_SEND_STRING("Buf[");
	        	ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(i);
	        	ESOS_TASK_WAIT_ON_SEND_STRING("] : ");
	        	ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING(buf[i]);
	        	ESOS_TASK_WAIT_ON_SEND_STRING("\n");
	        }
	        ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	        //code for "What to do once we receive msg goes here"
    	} // end if_task_has_mail

	    // check keyboard input
	    if(IS_ESOS_COMM_GOT_IN_DATA()){
	    	ESOS_TASK_WAIT_ON_GET_UINT8(u8_usrInput);
	    	ESOS_TASK_WAIT_ON_SEND_STRING("Enter the new mask and filter values ( 3 bit team id | 3 bit team member | 5 bit msg type):\n");
	    	b_hex_entry = (u8_usrInput == 'x' | u8_usrInput == 'X');
	    	if(!b_hex_entry){ // default to binary
	    		ESOS_TASK_WAIT_ON_SEND_STRING("Mask:   bbb bbb bbbbb   Filter:   bbb bbb bbbbb\n");
	    		ESOS_TASK_WAIT_ON_SEND_STRING("        ");
	    	} else { // entering hex
	    		ESOS_TASK_WAIT_ON_SEND_STRING("Mask: 0xXXX             Filter: 0xXXX\n");
	    		ESOS_TASK_WAIT_ON_SEND_STRING("        ");
	    	}
	    	u8_usr_input_index = 0;
	    	while(1){
	    		// get input char
	    		ESOS_TASK_WAIT_ON_GET_UINT8(u8_usrInput);
	    		if(!b_hex_entry && isInCharRange(u8_usrInput, '0', '1')){ // binary input (default)
	    			// echo char and process
	    			ESOS_TASK_WAIT_ON_SEND_UINT8(u8_usrInput);
	    			char_input_buf[u8_usr_input_index] = u8_usrInput;
	    			u8_usr_input_index++;
	    			// space out the entries
	    			if(u8_usr_input_index == 3 | u8_usr_input_index == 6 | u8_usr_input_index == 14 | u8_usr_input_index == 17){ // space out the entries
	    				ESOS_TASK_WAIT_ON_SEND_STRING(" ");
	    			} else if (u8_usr_input_index == 11) {// larger space between entries
	    				ESOS_TASK_WAIT_ON_SEND_STRING("             ");
	    			}
	    		} else if(b_hex_entry && (isInCharRange(u8_usrInput, '0', '9') | isInCharRange(u8_usrInput, 'a', 'f') | isInCharRange(u8_usrInput, 'A', 'F'))){
	    			// echo char and process
	    			ESOS_TASK_WAIT_ON_SEND_UINT8(u8_usrInput);
	    			char_input_buf[u8_usr_input_index] = u8_usrInput;
	    			u8_usr_input_index++;
	    			// space out the entries
	    			if(u8_usr_input_index == 3){
	    				ESOS_TASK_WAIT_ON_SEND_STRING("                       ");
	    			}
	    	 	}

	    		// should we stop the loop
	    		if(( !b_hex_entry && u8_usr_input_index >= 22)){
	    			// take our input and conver to mask and filter numbers
	    			//ESOS_TASK_WAIT_ON_SEND_STRING("\nEnded in binary entry. Input: ");
	    			char_input_buf[u8_usr_input_index] = '\0';
	    			//ESOS_TASK_WAIT_ON_SEND_STRING(char_input_buf);
	    			ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	    			esos_ecan_canfactory_unsubscribe(__pstSelf, new_filter, new_mask, MASKCONTROL_FIELD_NONZERO);
	    			new_mask   = arrToUnsignedNumber(char_input_buf,     11, 2);
	    			new_filter = arrToUnsignedNumber(char_input_buf+11, 11, 2);
	    			//esos_ecan_canfactory_subscribe(__pstSelf, new_filter, new_mask, MASKCONTROL_FIELD_NONZERO);

	    			ESOS_TASK_WAIT_ON_SEND_STRING("new_mask: ");
	    			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(new_mask);
	    			ESOS_TASK_WAIT_ON_SEND_STRING("    new_filter: ");
	    			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(new_filter);
	    			ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	    			ESOS_TASK_FLUSH_TASK_MAILBOX(__pstSelf);
	    			// do appropriate conversion here

	    			break;
	    		}
	    		else if(( b_hex_entry && u8_usr_input_index >= 6)){
	    			// take our input and conver to mask and filter numbers
	    			// ESOS_TASK_WAIT_ON_SEND_STRING("\nEnded in hex entry. Input: ");
	    			 char_input_buf[u8_usr_input_index] = '\0';
	    			// ESOS_TASK_WAIT_ON_SEND_STRING(char_input_buf);
	    			ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	    			esos_ecan_canfactory_unsubscribe(__pstSelf, new_filter, new_mask, MASKCONTROL_FIELD_NONZERO); // must unsubscribe first to use old variable values
	    			new_mask   = arrToUnsignedNumber(char_input_buf,     3, 16);
	    			new_filter = arrToUnsignedNumber((char_input_buf+3), 3, 16);
	    			//esos_ecan_canfactory_subscribe(__pstSelf, new_filter, new_mask, MASKCONTROL_FIELD_NONZERO);

	    			ESOS_TASK_WAIT_ON_SEND_STRING("new_mask: ");
	    			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(new_mask);
	    			ESOS_TASK_WAIT_ON_SEND_STRING("    new_filter: ");
	    			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(new_filter);
	    			ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	    			ESOS_TASK_FLUSH_TASK_MAILBOX(__pstSelf);
	    			// do appropriate conversion here
	    			
	    			break;
	    		}
	    		FLUSH_ESOS_COMM_IN_DATA();
	    		ESOS_TASK_YIELD();
	    	} // end while in menu mode
	    } // end if(key pressed) to start entry mode
        ESOS_TASK_YIELD();
	}//end while
	ESOS_TASK_END();
}//end listen_only


void user_init(void)
{
	//__esos_unsafe_PutString( HELLO_MSG );

    __esos_ecan_hw_config_ecan(); // ECAN config
    config_esos_uiF14();		// hw config
    CHANGE_MODE_ECAN1(ECAN_MODE_LISTEN_ONLY); // ECAN_LISTEN_ALL_MESSAGES  ECAN_MODE_LISTEN_ONLY

    //esos_RegisterTask( heartbeat_LED );
    esos_RegisterTask( CANFactory );
    esos_RegisterTask( listen_only );
}//end user_init

uint32_t arrToUnsignedNumber(char *arr, uint16_t length, uint16_t base){
	uint8_t u8_i;
	uint32_t u32_current_base = power( base, (length-1) );
	uint32_t u32_compounding_result = 0;
	//printf("original base: %zu, (base = %zu)\n",u32_current_base,base);
	
	for(u8_i = 0; u8_i < length; u8_i++){
		u32_compounding_result += charToHexValue(arr[u8_i]) * u32_current_base;
		//printf(" base: %zu - ",u32_current_base);printf("compounding_result: %zu\n", u32_compounding_result);
		u32_current_base /= base;
	}

	return u32_compounding_result;
}

int charToHexValue(char c_input){
	uint16_t compounding_result = 0;
	if(c_input >= '0' && c_input <= '9'){
		compounding_result = c_input - '0';
	} else if(c_input >= 'a' && c_input <= 'f') {
		compounding_result = c_input - 'a' + 10;
	} else if(c_input >= 'A' && c_input <= 'F'){
		compounding_result = c_input - 'A' + 10;
	} else {
		compounding_result = -1;
	}
	//printf("Converting %d - %d to %d.\n", c_input, '0', compounding_result);
	return compounding_result;
}

int power(int base, unsigned int exp) { // integer exponentiation
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}

bool isInCharRange(char input, char lowChar, char highChar) {
	return (input >= lowChar && input <= highChar);
}
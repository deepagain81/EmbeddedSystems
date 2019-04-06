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

// includes from fcn_synth.c
#include "revF14.h"
#include "fall17lib.h"
//#include "esos_f14ui.h"
#include "esos_menu.h"
#include "esos_lcd44780.h"
#include "DAC_comms.h"
#include "esos_sensor.h"
#include "esos_pic24_i2c.h"
#include <stdint.h> // for uint64_t
#include "stdio.h"

#define TEMPERATURE_SENSOR_I2C_CONVERSION_TIME (750) // ms

// Defines
#define USR_ECAN_BEACON_INTERVAL            ( 10*1000) /*should be 30  seconds*/
#define USR_ECAN_BEACON_TIMEOUT_INTERVAL    ( 30*1000) /*should be 120 seconds*/

// Prototypes - from fcn_synth.c
uint16_t usToTicks(uint16_t u16_us, uint16_t u16_pre);
void configTimer2(void);

// Globals - from fcn_synth.c
const uint8_t au8_sinetbl[] = {127,133,139,146,152,158,164,170,176,181,187,192,198,203,208,212,217,221,225,229,233,236,239,242,244,247,249,250,252,253,253,254,254,254,253,253,252,250,249,247,244,242,239,236,233,229,225,221,217,212,208,203,198,192,187,181,176,170,164,158,152,146,139,133,127,121,115,108,102,96,90,84,78,73,67,62,56,51,46,42,37,33,29,25,21,18,15,12,10,7,5,4,2,254,254,0,0,0,254,254,2,4,5,7,10,12,15,18,21,25,29,33,37,42,46,51,56,62,67,73,78,84,90,96,102,108,115,121};
const uint8_t au8_sqrtbl[]  = {254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t au8_tritbl[]  = {0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,254,252,248,244,240,236,232,228,224,220,216,212,208,204,200,196,192,188,184,180,176,172,168,164,160,156,152,148,144,140,136,132,128,124,120,116,112,108,104,100,96,92,88,84,80,76,72,68,64,60,56,52,48,44,40,36,32,28,24,20,16,12,8,4,0};
const uint8_t au8_usr1tbl[] = {64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint16_t u16_sensor_millivolts;
uint32_t u32_scaled_DAC_value;
uint16_t u16_I2C_temp;
uint8_t  u8_wav_val; // temp storage for the current step of the wave
uint16_t u16_volt_scale; // temp storage for current setting of wave amplitude
uint16_t u16_DAC_wave_out_index; // initilized in fcn_synth task
uint64_t sixty_four_test;
// Timer2 configuration

ESOS_USER_TASK( read_LM60_task );
ESOS_USER_TASK( read_1631_task );
ESOS_USER_TASK( set_LEDs_task );

// Menu variables - from fcn_synth.c
static esos_menu_longmenu_t my_menu = {
    .u8_numitems = 8,
    .u8_choice = 0, //Default
    .ast_items = {
        { "Set",
        "wvform"},
        { "Set",
        "freq"},
        { "Set",
        "ampltd"},
        { "Set",
        "duty"},
        { "Read",
        "LM60"},
        { "Read",
        "1631"},
        { "Set",
        "LEDs"},
        { "",
        "About..."},
    },
};

static esos_menu_longmenu_t my_menu_no_duty_cycle = {
    .u8_numitems = 7,
    .u8_choice = 0, //Default
    .ast_items = {
        { "Set",
        "wvform"},
        { "Set",
        "freq"},
        { "Set",
        "ampltd"},
        //{ "Set",
        //"duty"},
        { "Read",
        "LM60"},
        { "Read",
        "1631"},
        { "Set",
        "LEDs"},
        { "",
        "Back..."},
    },
};

// To display error msg
static esos_menu_staticmenu_t err = {
    .u8_numlines = 2,
    .u8_currentline = 0,
    .lines = {{"404"}, {"NotFnd"}},
};

// menu for selecting waveform type
static esos_menu_longmenu_t menu_setWvform = {
    .u8_numitems = 4,
    .u8_choice = 0, //Default
    .ast_items = {
        { "tri",
        "wvform"},
        { "sine",
        "wvform"},
        { "square",
        "wvform"},
        { "user1",
        "wvform"},
    },
};

// Selecting frequency
static esos_menu_entry_t freq = {
    .entries[0].label = "Freq= ",
    .entries[0].value = 64, //Default
    .entries[0].min = 64,
    .entries[0].max = 2047,
};

// Selecting amplitude
static esos_menu_entry_t ampltd = {
    .entries[0].label = "amp(dV)=",
    .entries[0].value = 16, //Default - unit is deciVolts
    .entries[0].min = 0,
    .entries[0].max = 33, // make this deciVolts
};

// Selecting duty cycle - should only appear when squarewave 
static esos_menu_entry_t duty = {
    .entries[0].label = "duty= ",
    .entries[0].value = 50, //Default
    .entries[0].min = 0,
    .entries[0].max = 100,
};

// typedef struct {
//  char label[8];
//  int32_t dynamic_data;
//  uint8_t format;
//  bool editable;
// } esos_menu_datadisplaymenu_t;

static esos_menu_datadisplaymenu_t read_LM60 = {
    .label = "LM60(C)",
    .dynamic_data = -1,
    .format = 0,
};

static esos_menu_datadisplaymenu_t read_1631 = {
    .label = "1631(C)",
    .dynamic_data = -1,
    .format = 0,
};

// Selecting LEDS
static esos_menu_entry_t LEDs = {
    .entries[0].label = "LEDs= ",
    .entries[0].value = 1, //Default
    .entries[0].min = 0,
    .entries[0].max = 7,
};

static esos_menu_staticmenu_t about_menu = {
    .u8_numlines = 11,
    .u8_currentline = 0,
    .lines = {"ESOS by",
             "Jones,",
             "Bruce, &",
             "Reese.",
             "Program",
             "by",
             "Benjamin",
             "Deepak",
             "Jordan",
             "Marcos",
             "Mehedi"}
};
// end menu variables

// fcn_synth tasks
// LCD 
ESOS_USER_TASK( fcn_synth ) {
    //static u8_temperture_data[2];
    ESOS_TASK_BEGIN();
    u16_DAC_wave_out_index = 0; // initilize to the first index of the wave
    //setDACA(0x3FF);
    shutdownDACA();
    shutdownDACB();
    //setDACA( 0x513 ); // proves that DACA works
    //setDACB( 0x0FF ); // proves that DACB works
    while(TRUE){
        //printf("Beginning of fcn_synth loop...\n");
        // Display main menu until sw3 pressed
        ESOS_TASK_WAIT_ESOS_MENU_LONGMENU(my_menu);
        //printf("LONG Menu exited...\n");

        if (my_menu.u8_choice < 0) // must have atleast one option in menu
            ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(err);
        else if(my_menu.u8_choice == 0){

            //Display waveform menu until sw3 pressed
            esos_lcd44780_clearScreen();
            ESOS_TASK_WAIT_ESOS_MENU_LONGMENU(menu_setWvform);
            //printf("setWvform.u8_choice: %d\n",menu_setWvform.u8_choice);

            // Inside the wvform menu, has another menu
            if (menu_setWvform.u8_choice < 0)// at least one option
                ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(err);
            else if (menu_setWvform.u8_choice > 0){
                // Display trig wave
                // test screen by writing 5 letters to both lines
                
                continue;
            }
            else if (menu_setWvform.u8_choice == 0){
                // Display sine wave
                continue;
            }
            else if (menu_setWvform.u8_choice == 1){
                // Display sine wave
                continue;
            }

            else if (menu_setWvform.u8_choice == 2){
                // Display sqaure wave
                // make duty cycle visible
                // reconfigure the timer appropriately
                // reconfigure the timer interrupt to not use array, and reconfigure timer between interrupts
                //configTimer2();
                continue;
            }

            else if (menu_setWvform.u8_choice == 3){
                // Display trig wave
                continue;
            }
            else if (menu_setWvform.u8_choice == 4){
                // Display user1 wave
                continue;
            }

            else if (menu_setWvform.u8_choice == 5){
                // Display user2 wave
                continue;
            }

            else{
                ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(err);
            }
        }//end if else choice==8
        else if (my_menu.u8_choice == 1){
            ESOS_TASK_WAIT_ESOS_MENU_ENTRY(freq);
            configTimer2();
            //printf("Back in ESOS!\n");
        } else if (my_menu.u8_choice == 2){
            ESOS_TASK_WAIT_ESOS_MENU_ENTRY(ampltd);
        } else if (my_menu.u8_choice == 3){
            ESOS_TASK_WAIT_ESOS_MENU_ENTRY(duty); // should only appear when square wave is selected
            //configTimer2();
        } else if (my_menu.u8_choice == 4){
            //readLM60();
            ESOS_TASK_WAIT_ESOS_MENU_DATADISPLAYMENU(read_LM60);
        } else if (my_menu.u8_choice == 5){
            //read1631();
            ESOS_TASK_WAIT_ESOS_MENU_DATADISPLAYMENU(read_1631);
        } else if (my_menu.u8_choice == 6){
            //setLEDS();
            ESOS_TASK_WAIT_ESOS_MENU_ENTRY(LEDs);
        } else if (my_menu.u8_choice == 7){
            // about menu
            about_menu.u8_currentline = 0;
            ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(about_menu);
        }
    }// end while
    ESOS_TASK_END();
}// end user task

ESOS_USER_TASK( read_LM60_task ) {
    ESOS_TASK_BEGIN();
    while(true){
        if(my_menu.u8_choice != 4){
            // do nothing
        } else {
            // read the sensor
            ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_3V3);
            ESOS_TASK_WAIT_SENSOR_READ(u16_sensor_millivolts, ESOS_SENSOR_AVG32, ESOS_SENSOR_FORMAT_VOLTAGE);
            ESOS_SENSOR_CLOSE();
            // convert to temperature
            u16_sensor_millivolts /= 3;
            read_LM60.dynamic_data = 100000 * ((int32_t)u16_sensor_millivolts - 424) / 625 / 1000; // removed decimal places
        }
        ESOS_TASK_WAIT_TICKS(10);
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK( read_1631_task ) {
    ESOS_TASK_BEGIN();
    static uint16_t u16_temperture_1631;
    static u8_temperture_data[2];
    static uint32_t timer_base_last_temperture_check;

    ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
    ESOS_TASK_WAIT_ON_WRITE1I2C1(0b10010000, 0x51); // initiate conversion
    ESOS_TASK_SIGNAL_AVAILABLE_I2C();
    u16_temperture_1631 = 0;
    timer_base_last_temperture_check = esos_GetSystemTick();

    while(true){
        // update menu display if neccessary
        if(my_menu.u8_choice != 5){
            // do nothing
        } else {
            // read the sensor
            read_1631.dynamic_data = (int32_t)((u16_temperture_1631 & 0b0111111111111111) >> 8) | ((u16_temperture_1631 & 0b1000000000000000)); // was -1
        }

        // check for a new value if neccessary
        if(esos_GetSystemTick() >= timer_base_last_temperture_check + TEMPERATURE_SENSOR_I2C_CONVERSION_TIME){
            // time for a new check
            //ESOS_TASK_WAIT_ON_SEND_STRING("making new I2C reading...\n");
            ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
            __PIC24_I2C1_START();
            __PIC24_I2C1_PUT(0b10010000); // device address + W(0)
            __PIC24_I2C1_PUT(0xAA); // address the temperature
            ESOS_TASK_WAIT_ON_READNI2C1( 0b10010001, u8_temperture_data, 2 );
            ESOS_TASK_SIGNAL_AVAILABLE_I2C();
            // update the timer
            timer_base_last_temperture_check = esos_GetSystemTick();
            // check data is valid, and output
                
            u16_temperture_1631 = ((uint16_t)u8_temperture_data[0] << 8) | u8_temperture_data[1];
            if(u16_temperture_1631 == 0xFF00){
                u16_temperture_1631 = 0x7F; // missing sensor
            } else if(u16_temperture_1631 == 0xC400){ // restart the conversion
                ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
                ESOS_TASK_WAIT_ON_WRITE1I2C1(0b10010000, 0x51); // initiate conversion
                ESOS_TASK_SIGNAL_AVAILABLE_I2C();
            }
            //printf("I2C read is 0x%X\n",u16_temperture_1631);
        }

        

        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK( set_LEDs_task ) {
    ESOS_TASK_BEGIN();
    while(true){
        // LED1 MSB
        if(LEDs.entries[0].value & 0b0000000000000100){
            esos_uiF14_turnLED1On();
        } else {
            esos_uiF14_turnLED1Off();
        }
        // LED2
        if(LEDs.entries[0].value & 0b0000000000000010){ // 16 bits
            esos_uiF14_turnLED2On();
        } else {
            esos_uiF14_turnLED2Off();
        }
        // ESOS_TASK_WAIT_ON_SEND_STRING(" LED's settings: ");
        // ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(LEDs.entries[0].value);
        // ESOS_TASK_WAIT_ON_SEND_STRING("\n");
        // LED3
        if(LEDs.entries[0].value & 0b0000000000000001){
            esos_uiF14_turnLED3On();
        } else {
            esos_uiF14_turnLED3Off();
        }
        ESOS_TASK_WAIT_TICKS(10);
    }
    ESOS_TASK_END();
}
/////////end fcn_synth.c user tasks

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

    // active board record keeping (u32_SW1_debounce_Timer_Base + __SWITCH_DEBOUNCING_INTERVAL) <= esos_GetSystemTick()
    static uint32_t u32_active_board_timout_time[NUM_OF_IDS] = {0}; // default to zero - no boards are discovered
    static uint16_t u16_timout_check_index;
    
    ESOS_TASK_BEGIN();
    
    esos_ecan_canfactory_subscribe( __pstSelf, 0x000 | CANMSG_TYPE_BEACON, 0x001f, MASKCONTROL_FIELD_NONZERO );
    esos_uiF14_flashLED3(1000);
    ESOS_TASK_WAIT_TICKS( 100 ); // delay for proper TEXT startup
    
    while ( 1 ) {
        static MAILMESSAGE msg;

        // current board is always active
        u32_active_board_timout_time[MY_ID] = esos_GetSystemTick() + USR_ECAN_BEACON_TIMEOUT_INTERVAL;
        
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

            // received message will be of type beacon - therefore, simply add to the list of active boards
            if(u16_received_arr_index >= 0 && u16_received_arr_index < NUM_OF_IDS){
                u32_active_board_timout_time[u16_received_arr_index] = esos_GetSystemTick() + USR_ECAN_BEACON_TIMEOUT_INTERVAL; // sets the system time to timeout this board
                ESOS_TASK_WAIT_ON_SEND_STRING("Beacon received from id ");
                ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u16_received_arr_index);
                ESOS_TASK_WAIT_ON_SEND_STRING(" - ");
                ESOS_TASK_WAIT_ON_SEND_STRING(aCANID_IDs[u16_received_arr_index].psz_netID);
                ESOS_TASK_WAIT_ON_SEND_STRING("\n");
            } else {
                ESOS_TASK_WAIT_ON_SEND_STRING("Received beacon does not have a valid board ID.\n");
            }
        }//end if

        // remove inactive beacons
        for(u16_timout_check_index = 0; u16_timout_check_index < NUM_OF_IDS; u16_timout_check_index++) {
            if(u32_active_board_timout_time[u16_timout_check_index] <= esos_GetSystemTick() && u32_active_board_timout_time[u16_timout_check_index] != 0){ // if system tick has passed timeout time
                u32_active_board_timout_time[u16_timout_check_index] = 0; // mark as invalid
                ESOS_TASK_WAIT_ON_SEND_STRING("Board has timeout out - Index: ");
                ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u16_timout_check_index);
                ESOS_TASK_WAIT_ON_SEND_STRING(" netid: ");
                ESOS_TASK_WAIT_ON_SEND_STRING(aCANID_IDs[u16_timout_check_index].psz_netID);
                ESOS_TASK_WAIT_ON_SEND_STRING("\n");
            } 
        } // end for loop - check for inactive beacons

        // for debugging, print the current list of active boards
        if(esos_uiF14_isSW2DoublePressed()){
            ESOS_TASK_WAIT_ON_SEND_STRING("Current active boards:\n");
            for(u16_timout_check_index = 0; u16_timout_check_index < NUM_OF_IDS; u16_timout_check_index++){
                if( u32_active_board_timout_time[u16_timout_check_index] != 0){
                    ESOS_TASK_WAIT_ON_SEND_STRING("netid: ");
                    ESOS_TASK_WAIT_ON_SEND_STRING(aCANID_IDs[u16_timout_check_index].psz_netID);
                    ESOS_TASK_WAIT_ON_SEND_STRING("\n");
                }
            }
            ESOS_TASK_WAIT_ON_SEND_STRING("----------------------------------\n");
        } // end if sw2 double pressed
        
        ESOS_TASK_YIELD();
    } // end while loop
    ESOS_TASK_END();
}

// ESOS tast to handle temperature messages - requests or incoming data?

//////////////////////////////////////////////////////////////////////////////////////////////////////
// functions from fcn_synth.c
// for timer use; avoid floats
uint16_t usToTicks(uint16_t u16_us, uint16_t u16_pre){
    uint64_t internal_freq;
    uint64_t timerTicks;
    internal_freq = 60000000; // 60 MHz
    timerTicks = ( FCY * (uint64_t)u16_us) / (uint64_t)u16_pre / (uint64_t)1E6;
    printf("u16_us: %d \nticks: %d \n",u16_us,(int)timerTicks);
    return (uint16_t)timerTicks;
}

// shouldn't implement hardware timers! Use ESOS!

void configTimer2(void){
    //set like thing for understanding could be set to register value 0x0020
    T2CON = T2_OFF | T2_IDLE_CON | T2_GATE_OFF
            | T2_32BIT_MODE_OFF
            | T2_SOURCE_INT
            | T2_PS_1_64;

    //PR2 = 5849;//msToU16Ticks(ISP_PERIOD, getTimerPrescale(T2CONbits))-1;     // results to 5849;
    PR2 = usToTicks(((uint32_t)1E6/freq.entries[0].value), 64) - 1; // -1 since timout is actually PR2 - 1; get desired microsec by 1 mega / freq
    printf("PR2: %d\n", PR2);
    TMR2 = 0;       // clear T2 value
    //_T2IF = 0;        // Clear interrupt flag
    //_T2IP = 3;        //priority
    //_T2IE = 1;        // Enable
    ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(ESOS_IRQ_PIC24_T2); // pg 625

    ESOS_REGISTER_PIC24_USER_INTERRUPT( ESOS_IRQ_PIC24_T2, ESOS_USER_IRQ_LEVEL1, _T2Interrupt);
    ESOS_ENABLE_PIC24_USER_INTERRUPT(ESOS_IRQ_PIC24_T2);
    T2CONbits.TON = 1;      // turn on the timer
    //printf("End of configTimer2(). Returning to ESOS...");
}//end timer config

//Interrupt service ( DAC comes here, i guess)
//void _ISR_T2Interrupt(void){
ESOS_USER_INTERRUPT( ESOS_IRQ_PIC24_T2 ) { // ESOS_IRQ_PIC24_T2
    // get variablesESOS_IRQ_PIC24_T2
    //esos_uiF14_turnLED1On();
    //printf("Hi\n");
    u16_volt_scale = ampltd.entries[0].value;
    if(menu_setWvform.u8_choice == 0){ //tri, sine, square, usr1
        u8_wav_val = au8_tritbl[u16_DAC_wave_out_index];
        //printf("tri - ");
    } else if(menu_setWvform.u8_choice == 1)  { // sine
        u8_wav_val = au8_sinetbl[u16_DAC_wave_out_index];
        //printf("sin - ");
    } else if(menu_setWvform.u8_choice == 2)  { // square
        u8_wav_val = au8_sqrtbl[u16_DAC_wave_out_index];
        //printf("sqr - ");
    }  else if(menu_setWvform.u8_choice == 3) { // usr1
        u8_wav_val = au8_usr1tbl[u16_DAC_wave_out_index];
        //printf("usr1- ");
    } else {
        u8_wav_val = 0x7F; // default to DC of half the max if there is a problem
    }
    //write to DAC
        // amplitude scaled value of the array - double length at least
        // u32_scaled_DAC_value = ((uint32_t)u8_wav_val * (uint32_t)u16_volt_scale * (uint32_t)16 / (uint32_t)33) + 
        //                        ((uint32_t)u8_wav_val * (uint32_t)u16_volt_scale * (uint32_t)15/(uint32_t)33 / (uint32_t)255); // max of 0xFFF, min of 0x000
        u32_scaled_DAC_value = (uint32_t)u8_wav_val * (uint32_t)u16_volt_scale;
        u32_scaled_DAC_value = u32_scaled_DAC_value << 6;
        u32_scaled_DAC_value = u32_scaled_DAC_value / ((uint32_t)255*33*2*2*2*2*2*2);
        // send value to DACA
        //printf("Preparing to setDACA inside interrupt...\n");
        setDACA((uint16_t)u32_scaled_DAC_value);
        //printf("wav_val: %d, scaled DAC: 0x%08lX index:%d, \n", u8_wav_val, u32_scaled_DAC_value, u16_DAC_wave_out_index);
    //increment index of array
    u16_DAC_wave_out_index = (u16_DAC_wave_out_index + 1)%128; // arrays are 128 elements
    //WAVEOUT = !WAVEOUT;
    _T2IF = 0;  // clear interrupt flag
}
// end functions from fcn_synth.c
//////////////////////////////////////////////////////////////////////////////////////////////////////

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

    // fcn_synth stuff
    //config_esos_uiF14();
    esos_menu_init();
    esos_RegisterTask(fcn_synth);
    esos_RegisterTask(read_LM60_task);
    esos_RegisterTask(read_1631_task);
    esos_RegisterTask(set_LEDs_task);
    esos_pic24_configI2C1(400); // 400kbits/sec is standard speed, 100kbits is slower
    configTimer2();
}

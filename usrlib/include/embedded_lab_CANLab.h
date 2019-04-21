#ifndef EMBEDDED_CANLAB_H
#define	EMBEDDED_CANLAB_H

#include <string.h>

typedef struct
{
    char *psz_name;
    char *psz_netID;
    uint8_t u8_teamID;
    uint8_t u8_memberID;
} CAN_ID;

#define NUM_OF_IDS 22 // 20 Students + 1 TA + 1 Professor

#define teamIDMask   0b0000011100000000   // eight teams
#define memberIDMask 0b0000000011100000   // eight members
#define typeIDMask   0b0000000000011111   // 32 possible MSGs
#define TEAM_ID_SHIFT_AMOUNT   8
#define MEMBER_ID_SHIFT_AMOUNT 5
#define TYPE_ID_SHIFT_AMOUNT   0

// Message Types
#define CANMSG_TYPE_POLL           0
#define CANMSG_TYPE_BEACON         1
#define CANMSG_TYPE_TEMPERATURE1   2
#define CANMSG_TYPE_TEMPERATURE2   3
#define CANMSG_TYPE_WAVEFORM       4
#define CANMSG_TYPE_POTENTIOMETER  5
#define CANMSG_TYPE_FREQUENCY      6
#define CANMSG_TYPE_AMPLITUDE      7
#define CANMSG_TYPE_DUTYCYCLE      8
#define CANMSG_TYPE_LEDS           9

const char* CAN_MESSAGE_TYPE_DESCRIPTION[10] = {
	"CANMSG_TYPE_POLL",           //0
	"CANMSG_TYPE_BEACON",         //1
	"CANMSG_TYPE_TEMPERATURE1",   //2
	"CANMSG_TYPE_TEMPERATURE2",   //3
	"CANMSG_TYPE_WAVEFORM",       //4
	"CANMSG_TYPE_POTENTIOMETER",  //5
	"CANMSG_TYPE_FREQUENCY",      //6
	"CANMSG_TYPE_AMPLITUDE",      //7
	"CANMSG_TYPE_DUTYCYCLE",      //8
	"CANMSG_TYPE_LEDS",           //9
};

// CAN ID Table
const CAN_ID aCANID_IDs[NUM_OF_IDS] = {
//"Name",         "netID",    teamID,    memberID,   // Array Index
 {"C Bush",      "cbb330",   1,         1 },        // 0
 {"W Carroll",   "woc17",    1,         2 },        // 1
 {"L Casey",     "lec426",   1,         3 },        // 2
 {"S Callicott", "sc2257",   1,         4 },        // 3
 {"J Fletcher",  "jdf469",   1,         5 },        // 4

 {"J Nguyen",    "jtn136",   2,         1 },        // 5
 {"N Siano",     "nrs171",   2,         2 },        // 6
 {"I Herbert",   "igh9",     2,         3 },        // 7
 {"A Ward",      "law448",   2,         4 },        // 8
 {"K Hunter",    "rkh134",   2,         5 },        // 9

 {"G Singh",     "gs656",    3,         1 },        // 10
 {"L Haugh",     "lrh282",   3,         2 },        // 11
 {"R Omalley",   "reo74",    3,         3 },        // 12
 {"B Fritzhugh", "bmf151",   3,         4 },        // 13
 {"R Jensen",    "rfj18",    3,         5 },        // 14

 {"D Chapagain", "dc2274",   4,         1 },        // 15
 {"M Merlin",    "mam1218",  4,         2 },        // 16
 {"M Farhad",    "mf1413",   4,         3 },        // 17
 {"B Wilkinson", "bcw253",   4,         4 },        // 18
 {"J Prather",   "jmp784",   4,         5 },        // 19
 
 {"BC Joiner",    "bcj162",  7,         1 },        // 20
 {"V Marojevic",  "Vuk",     7,         7 }         // 21
};

#define MY_ID             18 // Look above for your array index
#define MY_NAME           (aCANID_IDs[MY_ID].psz_name)
#define MY_NETID          (aCANID_IDs[MY_ID].psz_netID)
#define MY_TEAM_ID        (aCANID_IDs[MY_ID].u8_teamID)
#define MY_MEMBER_ID      (aCANID_IDs[MY_ID].u8_memberID)
#define MY_MSG_ID(type)   ((MY_TEAM_ID << TEAM_ID_SHIFT_AMOUNT) | (MY_MEMBER_ID << MEMBER_ID_SHIFT_AMOUNT) | (type & typeIDMask))

uint16_t calcMsgID(uint8_t u8_arrayIndex)
{
    return (aCANID_IDs[u8_arrayIndex].u8_teamID << TEAM_ID_SHIFT_AMOUNT) | (aCANID_IDs[u8_arrayIndex].u8_memberID << MEMBER_ID_SHIFT_AMOUNT);
}

uint8_t stripTeamID(uint16_t u16_MSG_ID)
{
    return (u16_MSG_ID & teamIDMask)    >> TEAM_ID_SHIFT_AMOUNT;
}

uint8_t stripMemberID(uint16_t u16_MSG_ID)
{
    return (u16_MSG_ID & memberIDMask)  >> MEMBER_ID_SHIFT_AMOUNT;
}

uint8_t stripTypeID(uint16_t u16_MSG_ID)
{
    return (u16_MSG_ID & typeIDMask)    >> TYPE_ID_SHIFT_AMOUNT;
}

int8_t getArrayIndexFromMsgID(uint16_t u16_MSG_ID)
{
    int8_t i8_arrayIndex = -1; // Default - Not Found (-1)
    uint8_t u8_arrayIndex;
	
    for (u8_arrayIndex = 0; u8_arrayIndex < NUM_OF_IDS; u8_arrayIndex++)
	{
        if (calcMsgID(u8_arrayIndex) == (u16_MSG_ID & (teamIDMask | memberIDMask)))
		{
            i8_arrayIndex = u8_arrayIndex;
            break;
        }
    }
	
    return i8_arrayIndex;
}

int8_t getNameFromMsgID(uint16_t u16_MSG_ID, char *psz_name, uint8_t u8_len)
{
    int8_t i8_arrayIndex;
	
    i8_arrayIndex = getArrayIndexFromMsgID(u16_MSG_ID);
    if (i8_arrayIndex != -1) // If the Message ID was found
	{
        strncpy(psz_name, aCANID_IDs[i8_arrayIndex].psz_name, u8_len);
        return 0;
    }
	
    return -1;
}

#endif	/* EMBEDDED_CANLAB_H */

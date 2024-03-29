/*******************************************************************
 * 
 * C header file for ESOS user-interface (UI) service
 * 
 *    requires the EMBEDDED SYSTEMS target rev. F14
 * 
 * ****************************************************************/

// modified 5pm 2/12
#ifndef   ESOS_UIF14_H
#define ESOS_UIF14_H
#include <stdlib.h> // for the function abs()
#include <stdbool.h>
#include <stdint.h>
#include "esos.h"
#include "esos_pic24.h"
#include "revF14.h"
//#include "esos_pic24_sensor.h"
#include "esos_sensor.h"
// STRUCTURES

#warning TODO: Add one-shot mode to SW1, SW2, and SW3

typedef struct {
    bool b_SW1Pressed;
    bool b_SW1DoublePressed;
    bool b_SW2Pressed;
    bool b_SW2DoublePressed;    
    bool b_SW3Pressed;
    bool b_SW3DoublePressed;    
    
    bool b_RPGAHigh;
    bool b_RPGBHigh;
    
    bool b_LED1On;
    uint16_t u16_LED1FlashPeriod;    
    bool b_LED2On;
    uint16_t u16_LED2FlashPeriod;        
    bool b_LED3On;
    uint16_t u16_LED3FlashPeriod;        
    
    uint16_t u16_RPGCounter;
    uint16_t u16_lastRPGCounter;
} _st_esos_uiF14Data_t;

// DEFINEs and CONSTANTs
#define __ESOS_TICKS_TO_MS(x)           (x/1)
#define __ESOS_MS_TO_TICKS(x)           (x*1)
#define __ESOS_UIF14_UI_PERIOD_MS       (10)
#define __SWITCH_DEBOUNCING_INTERVAL	(50)
#define __RPG_PINS_DEBOUNCING_INTERVAL  (5)
#define __RPG_HISTORY_BUFFER_LEN        (100)


// #define __RPG_VELOCITY_CALC_PERIOD      (100)
uint32_t get_RPG_SLOW();
void set_RPG_SLOW(uint32_t new_value);
uint32_t get_RPG_MEDIUM();
void set_RPG_MEDIUM(uint32_t new_value);
uint32_t get_RPG_FAST();
void set_RPG_FAST(uint32_t new_value);
uint32_t get_DOUBLE_PRESS();
void set_DOUBLE_PRESS(uint32_t new_value);

// PRIVATE DATA 
 
//_st_esos_uiF14Data_t _st_esos_uiF14Data; // should this be in the .c file?

// PRIVATE FUNCTION PROTOTYPES

uint16_t esos_uiF14_getRPGCounter (void);
void esos_ui_setRPGCounter (uint16_t);

uint16_t esos_uiF14_getLastRPGCounter (void);
void esos_ui_setLastRPGCounter (uint16_t);

ESOS_USER_TASK (__esos_uiF14_task); // changed to __esos_uiF14_task in c file
ESOS_USER_TASK (__esos_uiF14_RPG_tracking_task);

// PUBLIC API FUNCTION PROTOTYPES

inline bool esos_uiF14_isSW1Pressed (void);
inline bool esos_uiF14_isSW1Released (void);
inline bool esos_uiF14_isSW1DoublePressed (void);

inline bool esos_uiF14_isSW2Pressed (void);
inline bool esos_uiF14_isSW2Released (void);
inline bool esos_uiF14_isSW2DoublePressed (void);

inline bool esos_uiF14_isSW3Pressed (void);
inline bool esos_uiF14_isSW3Released (void);
inline bool esos_uiF14_isSW3DoublePressed (void);

inline void esos_uiF14_turnLED1On (void);
inline void esos_uiF14_turnLED1Off (void);
inline void esos_uiF14_toggleLED1 (void);
inline void esos_uiF14_flashLED1 (uint16_t);

inline void esos_uiF14_turnLED2On (void);
inline void esos_uiF14_turnLED2Off (void);
inline void esos_uiF14_toggleLED2 (void);
inline void esos_uiF14_flashLED2 (uint16_t);

inline void esos_uiF14_turnLED3On (void);
inline void esos_uiF14_turnLED3Off (void);
inline void esos_uiF14_toggleLED3 (void);
inline void esos_uiF14_flashLED3 (uint16_t);

inline void esos_uiF14_turnRedLEDOn (void);
inline void esos_uiF14_turnRedLEDOff (void);
inline void esos_uiF14_turnGreenLEDOn (void);
inline void esos_uiF14_turnGreenLEDOff (void);
inline void esos_uiF14_turnYellowLEDOn (void);
inline void esos_uiF14_turnYellowLEDOff (void);

inline uint16_t esos_uiF14_getRPGValue_u16 (void);
inline bool esos_uiF14_isRPGTurning (void);
inline bool esos_uiF14_isRPGTurningSlow (void);
inline bool esos_uiF14_isRPGTurningMedium (void);
inline bool esos_uiF14_isRPGTurningFast (void);
inline bool esos_uiF14_isRPGTurningCW (void);
inline bool esos_uiF14_isRPGTurningCCW (void);

void config_esos_uiF14();
int16_t esos_uiF14_getRPGVelocity_i16 (void);

inline bool RPGA_OUT(void);
inline bool RPGB_OUT(void);

// PUBLIC API ESOS TASK MACROS

#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED()              ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW1Pressed() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED()             ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW1Released() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED_AND_RELEASED() do {            \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();         \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();        \
                          } while (0) 
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_DOUBLE_PRESSED()       ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW1DoublePressed())

#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED()              ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW2Pressed() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED()             ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW2Released() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED_AND_RELEASED() do {            \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED();         \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED();        \
                          } while (0) 
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_DOUBLE_PRESSED()       ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW2DoublePressed())

#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED()              ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW3Pressed() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED()             ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW3Released() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED_AND_RELEASED() do {            \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED();         \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED();        \
                          } while (0) 
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_DOUBLE_PRESSED()       ESOS_TASK_WAIT_UNTIL(esos_uiF14_isSW3DoublePressed())

#warning ESOS RPG WAIT CLAUSES NOT YET IMPLEMENTED
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_UNTIL_TURNS()          // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_UNTIL_TURNS_CW()       // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_UNTIL_TURNS_CCW()      // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_MEDIUM()         // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_MEDIUM_CW()      // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_MEDIUM_CCW()     // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_FAST()           // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_FAST_CW()        // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_FAST_CCW()       // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_MAKES_REV(y)           // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_MAKES_CW_REV(y)        // not yet implemented
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_MAKES_CCW_REV(y)       // not yet implemented


#endif    // ESOS_UIF14_H

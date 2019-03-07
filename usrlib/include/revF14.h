// Created by bcw253
// 2/4/2019


// implement a macro to ensure libarary isn't implemented multiple times
#ifndef REV_F14
#define REV_F14
// Include appropriate pic24 and ESOS libraries here
#include "pic24_all.h"
	
// implement macros to initialized, use, and shutdown the following:
	
// LED1 (red)
#define LED1 (_LATF4)
#define CONFIG_LED1() CONFIG_RF4_AS_DIG_OUTPUT()
#define LED1_ON() (LED1=1)
#define LED1_OFF() (LED1=0)
#define LED1_TOGGLE() (LED1=!LED1)
// LED2 (yellow)
#define LED2 (_LATB14)
#define CONFIG_LED2() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED2_ON() (LED2=1)
#define LED2_OFF() (LED2=0)
#define LED2_TOGGLE() (LED2=!LED2)
// LED3 (green)
#define LED3 (_LATB15)
#define CONFIG_LED3() CONFIG_RB15_AS_DIG_OUTPUT()
// LED3 is configured differently; a high pin will turn LED3 off, and a low output will turn it on
#define LED3_ON() (LED3=0)
#define LED3_OFF() (LED3=1)
#define LED3_TOGGLE() (LED3=!LED3)
// SW1
#define SW1 (_RB13)
#define CONFIG_SW1() ({CONFIG_RB13_AS_DIG_INPUT(); ENABLE_RB13_PULLUP();})
#define SW1_PRESSED() (SW1 == 0)
#define SW1_RELEASED() (SW1 == 1)
#define READ_SW1() (SW1)
// SW2
#define SW2 (_RB12)
#define CONFIG_SW2() ({CONFIG_RB12_AS_DIG_INPUT(); ENABLE_RB12_PULLUP();}) 
#define SW2_PRESSED() (SW2 == 0)
#define SW2_RELEASED() (SW2 == 1)
#define READ_SW2() (SW2)
// SW3
#define SW3 (_RC15)
#define CONFIG_SW3() ({CONFIG_RC15_AS_DIG_INPUT(); ENABLE_RC15_PULLUP();})
#define SW3_PRESSED() (SW3 == 0)
#define SW3_RELEASED() (SW3 == 1) // SW3 has a pullup, so an unpressed switch is Logical high
#define READ_SW3() (SW3)
// RPG pin A
#define RPGA (_RB8)
#define CONFIG_RPGA()  ({CONFIG_RB8_AS_DIG_INPUT(); ENABLE_RB8_PULLUP();})
#define RPGA_IS_HIGH() (RPGA == 1)
#define RPGA_IS_LOW()  (RPGA == 0) // RPGA should have a pullup, since the switch grounds when connecting
// RPG pin B
#define RPGB (_RB9)
#define CONFIG_RPGB()  ({CONFIG_RB9_AS_DIG_INPUT(); ENABLE_RB9_PULLUP();})
#define RPGB_IS_HIGH() (RPGB == 1)
#define RPGB_IS_LOW()  (RPGB == 0)
// Potentiometer (Vpot)
#define VPOT (_RB2)
#define CONFIG_VPOT() CONFIG_RB2_AS_ANALOG()
    // use in esos_pic24_sensor
// Temperature sensor
#define VTEMP (_RB3)
#define CONFIG_VTEMP() CONFIG_RB3_AS_ANALOG()
// LCD pins
#define LCDD0 (_RE0)
#define LCDD1 (_RE1)
#define LCDD2 (_RE2)
#define LCDD3 (_RE3)
	/*LCDD0 to LCDD3 unused during 4 bit operation*/
#define LCDD4 (_RE4)
#define LCDD5 (_RE5)
#define LCDD6 (_RE6)
#define LCDD7 (_RE7)
#define LCDE  (_RD10)
#define LCDRW (_RD11)
#define LCDRS (_RC12)
#define LCD_BUSY (LCDD7) /*must be in read mode, with register select (RS) must be set to LOW to check busy flag*/

//*******************************************************************//	
	/*LCD PIN SET MACROS */
#define LCDD0_SET() (_LATE0=1)
#define LCDD0_RESET() (_LATE0=0)
#define LCDD1_SET() (_LATE1=1)
#define LCDD1_RESET() (_LATE1=0)
#define LCDD2_SET() (_LATE2=1)
#define LCDD2_RESET() (_LATE2=0)
#define LCDD3_SET() (_LATE3=1)
#define LCDD3_RESET() (_LATE3=0)
#define LCDD4_SET() (_LATE4=1)
#define LCDD4_RESET() (_LATE4=0)
#define LCDD5_SET() (_LATE5=1)
#define LCDD5_RESET() (_LATE5=0)
#define LCDD6_SET() (_LATE6=1)
#define LCDD6_RESET() (_LATE6=0)
#define LCDD7_SET() (_LATE7=1)
#define LCDD7_RESET() (_LATE7=0)

#define LCDE_SET() (_LATD10=1)
#define LCDE_RESET() (_LATD10=0)
#define LCDRW_SET() (_LATD11=1)
#define LCDRW_RESET() (_LATD11=0)
#define LCDRS_SET() (_LATC12=1)
#define LCDRS_RESET() (_LATC12=0)
#define LCD_BUSY_SET() (_LATE7=1)
#define LCD_BUSY_RESET() (_LATE7=0)
	/*LCD HARDWARE TEST PINS*/
#define IS_LCDD0_SET() (LCDD0==1)
#define IS_LCDD1_SET() (LCDD1==1)
#define IS_LCDD2_SET() (LCDD2==1)
#define IS_LCDD3_SET() (LCDD3==1)
#define IS_LCDD4_SET() (LCDD4==1)
#define IS_LCDD5_SET() (LCDD5==1)
#define IS_LCDD6_SET() (LCDD6==1)
#define IS_LCDD7_SET() (LCDD7==1)
#define IS_LCDE_SET()  (LCDE==1)
#define IS_LCDRW_SET() (LCDRW==1)
#define IS_LCDRS_SET() (LCDRS==1)
#define IS_LCD_BUSY_SET() (LCD_BUSY==1)
#define IS_LCD_BUSY() (IS_LCD_BUSY_SET()) /*FOR EASIER USE - preferred over IS_LCD_BUSY_SET()*/
	/*LCD CONFIGURATION MACROS*/
#define CONFIG_LCD_DATA_PINS_READ_MODE() {\
CONFIG_RE0_AS_DIG_INPUT();\
CONFIG_RE1_AS_DIG_INPUT();\
CONFIG_RE2_AS_DIG_INPUT();\
CONFIG_RE3_AS_DIG_INPUT();\
CONFIG_RE4_AS_DIG_INPUT();\
CONFIG_RE5_AS_DIG_INPUT();\
CONFIG_RE6_AS_DIG_INPUT();\
CONFIG_RE7_AS_DIG_INPUT();\
}
#define CONFIG_LCD_DATA_PINS_WRITE_MODE() {\
CONFIG_RE0_AS_DIG_OUTPUT();\
CONFIG_RE1_AS_DIG_OUTPUT();\
CONFIG_RE2_AS_DIG_OUTPUT();\
CONFIG_RE3_AS_DIG_OUTPUT();\
CONFIG_RE4_AS_DIG_OUTPUT();\
CONFIG_RE5_AS_DIG_OUTPUT();\
CONFIG_RE6_AS_DIG_OUTPUT();\
CONFIG_RE7_AS_DIG_OUTPUT();\
}
#define LCD_INITIAL_CONFIG() {\
CONFIG_RD10_AS_DIG_OUTPUT();/*LCD ENABLE PIN - should always be in write mode*/\
CONFIG_RD11_AS_DIG_OUTPUT();/*LCDRW PIN - should always be in write mode*/\
CONFIG_RC12_AS_DIG_OUTPUT();/*LCD REGISTER SELECT PIN - should always be in write mode*/\
CONFIG_LCD_DATA_PINS_WRITE_MODE();\
/*SET INITIAL LCD PIN VALUES*/\
LCDD0_RESET();\
LCDD1_RESET();\
LCDD2_RESET();\
LCDD3_RESET();\
LCDD4_RESET();\
LCDD5_RESET();\
LCDD6_RESET();\
LCDD7_RESET();\
LCDE_RESET();\
LCDRW_RESET();\
LCDRS_RESET();\
}
// CALL at the beginning of every user program during user_init
#define HARDWARE_CONFIG() ({\
configClock();\
/*setup hardware pinouts*/\
CONFIG_LED1();\
CONFIG_LED2();\
CONFIG_LED3();\
CONFIG_SW1();\
CONFIG_SW2();\
CONFIG_SW3();\
CONFIG_RPGA();\
CONFIG_RPGB();\
CONFIG_VPOT();\
CONFIG_VTEMP();\
LCD_INITIAL_CONFIG();\
/*LED Default State*/\
LED1_OFF();\
LED2_OFF();\
LED3_OFF();\
/* misc */ \
/* configDefaultUART(DEFAULT_BAUDRATE); */ \
/* printResetCause();*/ \
})
#endif

// Author: Benjamin Wilkinson
// 3/21/2019
// Functions to control the SPI controlled Digitial to Analog converter on the revF14 board

#include "revF14.h"


#define DAC_config_channelA       (0b0000000000000000)
#define DAC_config_channelB       (0b1000000000000000)
// skipped buffer config data
#define DAC_config_normalGain     (0b0010000000000000)
#define DAC_config_doubleGain     (0b0000000000000000)
#define DAC_config_activePin      (0b0001000000000000)
#define DAC_config_shutdownPin    (0b0000000000000000)

void setDACA(uint16_t u16_analog_target);
void setDACB(uint16_t u16_analog_target);
void shutdownDACA();
void shutdownDACB();
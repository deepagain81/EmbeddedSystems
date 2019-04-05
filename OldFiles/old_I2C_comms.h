// Author: Benjamin Wilkinson
// 3/22/2019

#include "revF14.h"
#include <stdbool.h>

void startI2C();
bool I2C_out(uint8_t data);
void write1_I2C(uint8_t u8_addr, uint8_t u8_d1);
void write2_I2C(uint8_t u8_addr, uint8_t u8_d1, uint8_t u8_d2);
void writeN_I2C(uint8_t u8_addr, uint8_t *pu8_d, uint16_t u16_cnt);
uint8_t readI2C(bool b_should_ack);
void rstartI2C();
void stopI2C();
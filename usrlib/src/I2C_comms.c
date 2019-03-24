// Author: Benjamin Wilkinson
// 3/22/2019

#include "I2C_comms.h"

void startI2C(){
	// while clock is high, send data high to low
	//I2C_SET_CLOCK_LOW();
	I2C_WRITE_HIGH();
	I2C_SET_CLOCK_HIGH();
	I2C_WRITE_LOW();
	I2C_SET_CLOCK_LOW();
}
bool I2C_out(uint8_t data){
	int i = 0;
	uint8_t data_to_send = data;
	bool return_val = false;
	CONFIG_I2C_WRITE();
	I2C_SET_CLOCK_LOW();
	// loop through data
	for(i = 0; i < 8; i++){
		if( data_to_send & 0b10000000 ){
			I2C_WRITE_HIGH();
		} else {
			I2C_WRITE_LOW();
		}
		data_to_send = data_to_send << 1;
		I2C_SET_CLOCK_HIGH();
		I2C_SET_CLOCK_LOW();
	}
	// check for acknowledgement - line will be pulled low
	CONFIG_I2C_READ();
	I2C_SET_CLOCK_HIGH();
	return_val = !IS_I2C_DATA_SET(); // slave will pull the line low to ACK
	I2C_SET_CLOCK_LOW();
	// reconfigure in write mode
	I2C_WRITE_HIGH(); // don't hold the bus low
	CONFIG_I2C_WRITE();
	return return_val;
	#warning check for ACK
}
void write1_I2C(uint8_t u8_addr, uint8_t u8_d1){
	// remember to reevaluate ACK conditions
	// start I2C
	startI2C();
	// send address
	if(!I2C_out(u8_addr)){
		//printf("Error:")
	}
	// send data
	if(!I2C_out(u8_d1)){
		// printf("Error")
	}
	// stop I2C
	stopI2C();
}
void write2_I2C(uint8_t u8_addr, uint8_t u8_d1, uint8_t u8_d2){
	startI2C();
	I2C_out(u8_addr);
	I2C_out(u8_d1);
	I2C_out(u8_d2);
	stopI2C();
}
void writeN_I2C(uint8_t u8_addr, uint8_t *pu8_d, uint16_t u16_cnt){
	int i;
	startI2C();
	for(i=0;i<u16_cnt;i++){
		I2C_out(pu8_d[i]);
	}
	stopI2C();
}
uint8_t readI2C(bool b_should_ack){
	uint8_t data_in = 0;
	int i = 0;
	I2C_SET_CLOCK_LOW();
	CONFIG_I2C_READ();
	for(i=0; i<8; i++){
		I2C_SET_CLOCK_HIGH();
		data_in = data_in | IS_I2C_DATA_SET();
		data_in = data_in << 1;
		I2C_SET_CLOCK_LOW();
	}
	CONFIG_I2C_WRITE();
	// ack
	if(b_should_ack){
		I2C_WRITE_LOW();
	}
	I2C_SET_CLOCK_HIGH();
	I2C_SET_CLOCK_LOW();
	I2C_WRITE_HIGH();
}
void rstartI2C(){
	startI2C();
}
void stopI2C(){
	// while clock is high, send data low to high
	I2C_SET_CLOCK_LOW();
	I2C_WRITE_LOW();
	I2C_SET_CLOCK_HIGH();
	I2C_WRITE_HIGH();
	//I2C_SET_CLOCK_LOW();
}

// I2C
// change data pin values while clock is pulled low normally
// change data pin values while clock is high to start(high to low), stop(low to high), or restart
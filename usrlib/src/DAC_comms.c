// Author: Benjamin Wilkinson
// 3/21/2019
// Functions to control the SPI controlled Digitial to Analog converter on the revF14 board

#include "DAC_comms.h"
#include "my_spi.h"
#include <stdio.h>
//#include "esos_pic24.h"
//#include "esos_f14ui.h"
//#include "esos_comm.h"

void setDACA(uint16_t u16_analog_target){
	uint16_t command_to_send = 0;
	int i; // see for loop
	
	command_to_send = u16_analog_target & 0b0000111111111111; // get 12-bit data intended for sending
	command_to_send = command_to_send | DAC_config_channelA | DAC_config_normalGain | DAC_config_activePin | 0b0100000000000000; // trying buffered
	//printf("Command to send: %d\n",command_to_send);
	// ESOS_TASK_WAIT_ON_SEND_STRING(" DACA command: ");
	// ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(command_to_send);
	// ESOS_TASK_WAIT_ON_SEND_STRING("\n");

	DAC_SET_SCK_LOW();
	// set CS to low to begin sending data
	DAC_SET_CS_LOW();
	for(i = 0; i<16; i++){ // 16 bits to send
		if(command_to_send & 0b1000000000000000){
			DAC_SET_SDO_HIGH();// send a 1
			//printf("High!");
		} else {
			DAC_SET_SDO_LOW(); // send a 0
			//printf("Low!");
		}
		DAC_SET_SCK_HIGH();
		DAC_SET_SCK_LOW();
		command_to_send = command_to_send << 1;
	}
	DAC_SET_CS_HIGH(); // stop sending data
	//printf("Command to send: %d\n",command_to_send);
}
void setDACB(uint16_t u16_analog_target){
	uint16_t command_to_send = 0;
	int i; // see for loop

	command_to_send = u16_analog_target & 0b0000111111111111; // get 12-bit data intended for sending
	command_to_send |= DAC_config_channelB | DAC_config_normalGain | DAC_config_activePin;

	DAC_SET_SCK_LOW();
	// set CS to low to begin sending data
	DAC_SET_CS_LOW();
	for(i = 0; i<16; i++){ // 16 bits to send
		if(command_to_send & 0b1000000000000000){
			DAC_SET_SDO_HIGH();// send a 1
			//printf("High!");
		} else {
			DAC_SET_SDO_LOW(); // send a 0
			//printf("Low!");
		}
		DAC_SET_SCK_HIGH();
		DAC_SET_SCK_LOW();
		command_to_send = command_to_send << 1;
	}
	DAC_SET_CS_HIGH(); // stop sending data
	//printf("Command to send: %d\n",command_to_send);
}
void shutdownDACA(){
	uint16_t command_to_send = 0;
	int i; // see for loop

	command_to_send |= DAC_config_channelA | DAC_config_normalGain | DAC_config_shutdownPin;

	DAC_SET_SCK_LOW();
	// set CS to low to begin sending data
	DAC_SET_CS_LOW();
	for(i = 0; i<16; i++){ // 16 bits to send
		if(command_to_send & 0b1000000000000000){
			DAC_SET_SDO_HIGH();// send a 1
			//printf("High!");
		} else {
			DAC_SET_SDO_LOW(); // send a 0
			//printf("Low!");
		}
		DAC_SET_SCK_HIGH();
		DAC_SET_SCK_LOW();
		command_to_send = command_to_send << 1;
	}
	DAC_SET_CS_HIGH(); // stop sending data
	//printf("Command to send: %d\n",command_to_send);
}
void shutdownDACB(){
	uint16_t command_to_send = 0;
	int i; // see for loop

	command_to_send |= DAC_config_channelB | DAC_config_normalGain | DAC_config_shutdownPin;

	DAC_SET_SCK_LOW();
	// set CS to low to begin sending data
	DAC_SET_CS_LOW();
	for(i = 0; i<16; i++){ // 16 bits to send
		if(command_to_send & 0b1000000000000000){
			DAC_SET_SDO_HIGH();// send a 1
			//printf("High!");
		} else {
			DAC_SET_SDO_LOW(); // send a 0
			//printf("Low!");
		}
		DAC_SET_SCK_HIGH();
		DAC_SET_SCK_LOW();
		command_to_send = command_to_send << 1;
	}
	DAC_SET_CS_HIGH(); // stop sending data
	//printf("Command to send: %d\n",command_to_send);
}
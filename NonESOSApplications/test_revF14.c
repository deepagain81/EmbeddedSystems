// Created by Benjamin and Marcas
// 2/4/2019

#include "pic24_all.h"
#include "revF14.h"

int main(void) {
	HARDWARE_CONFIG(); // config function from revF14.h

	// initialize LED state
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();

	while (1) {
		// Delay long enought to see LED blink.
		// DELAY_MS(250);
		// Toggle the LEDs.

		//LED1 = !LED1;
		//LED2 = !LED2;
		//LED3 = !LED3;

		// SW1
		if (SW1_PRESSED()) {
			LED1_ON();
		}
		else if (SW1_RELEASED()) {
			LED1_OFF();
		}
		// SW2
		if (SW2_PRESSED()) {
			LED2_ON();
		}
		else if (SW2_RELEASED()) {
			LED2_OFF();
		}
		// SW3
		if (SW3_PRESSED()) {
			LED3_ON();
		}
		else if (SW3_RELEASED()) {
			LED3_OFF();
		}
	}
}
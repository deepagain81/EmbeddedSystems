// File created by Benjamin Wilkinson
// 2/20/2019

/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

/**
 * \addtogroup ESOS_Task_Sensor_Service
 * @{
 */
 
/*** I N C L U D E S *************************************************/
#include "esos_pic24_sensor.h"


/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

/** \file
 *  Sensor support functions. \see hwxxx_sensor.h for details.
 */

/**
Configure and enable the sensor module for hwxxx hardware.
\param e_senCh   specifies sensor channel
\param e_senVRef specifies sensor voltage reference
\hideinitializer
 */
void esos_sensor_config_hw (esos_sensor_ch_t e_senCh, esos_sensor_vref_t e_senVRef)
{
#warning Selecting reference voltage is not yet implemented.
	AD1CON1bits.ADON = 0; // ADC must be off when configuring
	ADC1BUF0 = 0; // reset results register

	// configure ADC before turning on
	// AD1CON1.ADSIDL = 1; // Disable ADC in idle mode
	AD1CON1bits.AD12B = 1; // 0 - 10 bit output, multiple channels   1 - 12 bit output, one channel
	AD1CON1bits.FORM = 0b00; // unsigned integer output form
	AD1CON1bits.SSRC = 7; // stop sampling and start converting after ADC counter expires (length set by AD1CON1.SAMC)
	AD1CON1bits.SIMSAM = 0; // simultaneously sample, 10 bit mode only
	AD1CON1bits.ASAM = 0; // 0 - sample begins when SAMP bit is set  1 - sample again immediately

	AD1CON2bits.VCFG = 0; // use AAVSS and AVDD as voltage references

	AD1CON3bits.ADRC = 1; // use ADC's clock instead of the system clock
	AD1CON3bits.SAMC = 31; // max 31, min 0; how many clock cycles to sample for; current = 31 * 250ns = 7.75 us

	AD1CHS0bits.CH0NA = 0; // negative input is AVSS ??? or is that VREF???
	AD1CHS0bits.CH0SA = e_senCh; // read from appropriate channel

	AD1CON1bits.ADON = 1; // turn ADC module on ONLY AFTER configuration
}

/**
Determine truth of: the sensor is currently converting
\hideinitializer
 */
BOOL esos_sensor_is_converting_hw (void)
{
	return !AD1CON1bits.DONE; // Sampling bit doesn't convey that approximation occurs after sampling
}

/**
Initiate a conversion for a configured sensor
\hideinitializer
 */
void esos_sensor_initiate_conversion_hw (void)
{
	AD1CON1bits.SAMP = 1; // set sample bit to initiate conversion
}

/**
Receive the value from a conversion that has already been initiated
\hideinitializer
 */
uint16_t esos_sensor_getvalue_u16_hw (void)
{
	return ADC1BUF0;
}

/**
Release any pending conversions for the sensor
\hideinitializer
 */
void esos_sensor_release_hw (void)
{
	//AD1CON1.SAMP = 0; // stop sampling!
	AD1CON1bits.ADON = 0; // deactivate the module
}

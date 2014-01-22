/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 IrTxAdapter is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License 
 if not, see <http://www.gnu.org/licenses/>.
 
 */

#include <avr/eeprom.h>

typedef struct {
	uint16_t THROTTLE_MIN;
	uint16_t THROTTLE_MAX;
	uint16_t ROLL_MIN;
	uint16_t ROLL_MAX;	
	uint16_t PITCH_MIN;
	uint16_t PITCH_MAX;
	uint16_t YAW_MIN;
	uint16_t YAW_MAX;
	uint16_t AUX1_MIN;
	uint16_t AUX1_MAX;
} adcCalibration;

adcCalibration calib;

void adcInit() {
	pinMode(ADC_BUTTON1, INPUT_PULLUP);
	delay(50);
	if(digitalRead(ADC_BUTTON1)==LOW)
		adcCalibrate();
	else
		adcLoadCalibration();
}

void adcCalibrate() {
	while(digitalRead(ADC_BUTTON1)==LOW) 
		delay(50);
	uint16_t val;
	calib.THROTTLE_MIN = 1023;
	calib.THROTTLE_MAX = 0;
	calib.ROLL_MIN = 1023;
	calib.ROLL_MAX = 0;
	calib.PITCH_MIN = 1023;
	calib.PITCH_MAX = 0;
	calib.YAW_MIN = 1023;
	calib.YAW_MAX = 0;
	calib.AUX1_MIN = 1023;
	calib.AUX1_MAX = 0;
	unsigned long blink = 0;
	while(digitalRead(ADC_BUTTON1)==HIGH) {
		if( millis() >= blink) {
			digitalWrite( STATUS_LED, digitalRead(STATUS_LED)==LOW ? HIGH : LOW);
			blink = millis()+333;
		}
		val = analogRead(ADC_THROTTLE);
		if( val < calib.THROTTLE_MIN)
			calib.THROTTLE_MIN = val;
		if( val > calib.THROTTLE_MAX)
			calib.THROTTLE_MAX = val;
			
		val = analogRead(ADC_ROLL);
		if( val < calib.ROLL_MIN)
			calib.ROLL_MIN = val;
		if( val > calib.ROLL_MAX)
			calib.ROLL_MAX = val;
			
		val = analogRead(ADC_PITCH);
		if( val < calib.PITCH_MIN)
			calib.PITCH_MIN = val;
		if( val > calib.PITCH_MAX)
			calib.PITCH_MAX = val;
			
		val = analogRead(ADC_YAW);
		if( val < calib.YAW_MIN)
			calib.YAW_MIN = val;
		if( val > calib.YAW_MAX)
			calib.YAW_MAX = val;
			
		val = analogRead(ADC_AUX1);
		if( val < calib.AUX1_MIN)
			calib.AUX1_MIN = val;
		if( val > calib.AUX1_MAX)
			calib.AUX1_MAX = val;
	}
	adcSaveCalibration();
	digitalWrite( STATUS_LED, HIGH);
}

void adcGetRCData() {
	rcData[THROTTLE] = map( analogRead(ADC_THROTTLE), calib.THROTTLE_MIN, calib.THROTTLE_MAX, PPM_MIN, PPM_MAX);
	rcData[ROLL] = map( analogRead(ADC_ROLL), calib.ROLL_MIN, calib.ROLL_MAX, PPM_MIN, PPM_MAX);
	rcData[PITCH] = map( analogRead(ADC_PITCH), calib.PITCH_MIN, calib.PITCH_MAX, PPM_MIN, PPM_MAX);
	rcData[YAW] = map( analogRead(ADC_YAW), calib.YAW_MIN, calib.YAW_MAX, PPM_MIN, PPM_MAX);
	rcData[AUX1] = map( analogRead(ADC_AUX1), calib.AUX1_MIN, calib.AUX1_MAX, PPM_MIN, PPM_MAX);
}

void adcLoadCalibration() {
	eeprom_read_block((void*)&calib, (void*)0, sizeof(calib));
}

void adcSaveCalibration() {
	eeprom_write_block((void*)&calib, (void*)0, sizeof(calib));
}

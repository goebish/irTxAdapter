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
 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "config.h"

static boolean cppmNewValues = false;

int16_t rcData[RC_CHANS] = {0,};

void setup() {
	pinMode( PS0, INPUT_PULLUP);
	pinMode( PS1, INPUT_PULLUP);
	pinMode( STATUS_LED, OUTPUT);
	digitalWrite( STATUS_LED, HIGH);
	irInit();
#if USE_ADC
	adcInit();
#else
	cppmInit();
	while (!cppmNewValues) // get at least one good CPPM frame
		cppmGetInput();
#endif

#if DEBUG
	Serial.begin(19200);
#endif
}

void loop() {
	
#if DEBUG
	const char * protocols[] = {"San Huan","Syma","WLToys"};
	static unsigned long lastDebug = 0;
	if(millis()-lastDebug > 1000) {
		lastDebug = millis();
		Serial.println("CPPM input\n-----------");		
		Serial.print("Throttle : ");
		Serial.println(rcData[THROTTLE]);
		Serial.print("Pitch : ");
		Serial.println(rcData[PITCH]);
		Serial.print("Roll : ");
		Serial.println(rcData[ROLL]);
		Serial.print("Yaw : ");
		Serial.println(rcData[YAW]);
		Serial.print("AUX1 : ");
		Serial.println(rcData[AUX1]);
		Serial.print("Protocol :");
		Serial.println(protocols[currentProtocol()]);
		Serial.println();
	}
#endif

#if USE_ADC
	adcGetInput();
#else
	if( cppmNewValues)
		cppmGetInput();
#endif
	// delay between 2 packets must be 40ms minimum in CPPM mode
	switch(currentProtocol()) {
	case SH_602X:
		irSHsendPacket(shBuildPacket());
		delay(40);
		break;
	case SYMA_S107_32:
		irSYsendPacket(syBuildPacket());
		delay(100);
		break;
	case WLTOYS:
		irWLsendPacket(wlBuildPacket());
		delay(70);
	default: 
		;
	}
}

uint8_t currentProtocol()
{
	if(!digitalRead(PS0))
		return SYMA_S107_32;
	if(!digitalRead(PS1))
		return WLTOYS;
	return SH_602X;
}

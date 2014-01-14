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
uint8_t protocol = SH_602X;

void setup() {
	irInit();
	cppmInit();
#if DEBUG
	Serial.begin(19200);
#endif
}

void loop() {
#if DEBUG
	static unsigned long lastDebug = 0;
	if(millis()-lastDebug > 1000)
	{
		lastDebug = millis();
		Serial.println(rcData[THROTTLE]);
	}
#endif
	if (cppmNewValues)
		cppmGetInput();
	
	switch(protocol) {
	case SH_602X:
		irSHsendPacket(shBuildPacket());
		delay(40);
		break;
	default: 
		;
	}
}


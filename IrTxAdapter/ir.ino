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

#if (IR_PIN == 3)
	#define IR_LOW TCCR2A &= ~_BV(COM2B1)
	#define IR_HIGH TCCR2A |= _BV(COM2B1)
#elif (IR_PIN == 11)
	#define IR_HIGH TCCR2A &= ~_BV(COM2A1)
	#define IR_LOW TCCR2A |= _BV(COM2A1)
#else
	#error IR_PIN must be D3 or D11 !
#endif

void irInit()
{
	pinMode(IR_PIN, OUTPUT);
	digitalWrite(IR_PIN, LOW);
	// setup IR carrier
	TCCR2A = _BV(WGM20);
#if IR_PIN == 3
	byte s = 8000 / (IR_CLOCK_RATE / 1000);
#else // pin 11
	byte s = 8000 / ((IR_CLOCK_RATE<<1) / 1000);
	TCCR2A |= _BV(COM2A0);
#endif
	TCCR2B = _BV(WGM22) | _BV(CS20);
	OCR2A = s;
	OCR2B = s >> 1;
}

void irSHsendHeader() {
	IR_HIGH;
	delayMicroseconds(3750);
	IR_LOW;
	delayMicroseconds(1000);
	IR_HIGH;
}

void irSHsendZero() {
	IR_HIGH;
	delayMicroseconds(700); 
	IR_LOW;
	delayMicroseconds(300); 
	IR_HIGH;
}

void irSHsendOne() {
	IR_HIGH;
	delayMicroseconds(300);
	IR_LOW;
	delayMicroseconds(700);
	IR_HIGH;
}

void irSHsendPacket(uint32_t packet)
{
	noInterrupts();
	irSHsendHeader();
	for(uint8_t b=32; b>0; b--)
		if( packet & (uint32_t)1<<(b-1))
			irSHsendOne();
		else
			irSHsendZero();
	IR_LOW;
	interrupts();
}


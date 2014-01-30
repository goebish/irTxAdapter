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

/*
 San Huan 602x IR protocol:

 carrier frequency: 38kHz
 (all timings in µs)
 constant packet length
 header: 3750 pulse, 1000 space
 zero: 700 pulse, 300 space
 one: 300 pulse, 700 space
 end: 40000 space
 length of data: 32 bits

    byte0  |   byte1   |    byte 2   |   byte3
 A BBBBBBB | CCCC DDDD | E F G HHHHH | II JJJJJJ

 A = always 0
 B = throttle (0-120)
 C = yaw (0-15, 0 is neutral)
 D = pitch (0-15, 0 is neutral)
 E = yaw direction  1=left rudder, 0=right rudder
 F = pitch direction  1=nose up, 0=nose down
 G = yaw trim direction  1=trim left, 0=trim right
 H = yaw trim (0-31)
 I = band 11=A, 10=B, 01=C
 J = checksum ((byte0 + byte1 + byte2 + 0x0F) & 0x3F)

*/



enum {
	SHBAND_A = (uint8_t)0xc0,
	SHBAND_B = (uint8_t)0x80,
	SHBAND_C = (uint8_t)0x40
};

uint32_t shBuildPacket() {
	uint8_t packet[4] = {0,0,0,0};
	packet[0] = map(rcData[THROTTLE], PPM_MIN, PPM_MAX, 0, 120); // throttle
	packet[1] = map(rcData[YAW], MIDRC, rcData[YAW] > MIDRC ? PPM_MAX : PPM_MIN, 0, 15) << 4; // yaw
	packet[1] |= map(rcData[PITCH], MIDRC, rcData[PITCH] > MIDRC ? PPM_MAX : PPM_MIN, 0, 15); // pitch
	if( rcData[AUX1] > MIDRC)
		packet[2] |= (1<<5); // yaw trim direction
	packet[2] |= map(rcData[AUX1], MIDRC, rcData[AUX1] > MIDRC ? PPM_MAX : PPM_MIN, 0, 31); // yaw trim
#if DYNAMIC_YAW_TRIM	
	int yawtrim = rcData[AUX1]<MIDRC ? (int)-(packet[2] & 0x1F) : (int)(packet[2] & 0x1F); // [-31;31]
	packet[2] &= ~0x3F;
	if(rcData[YAW]<MIDRC) 
		yawtrim = map(rcData[YAW], PPM_MIN, MIDRC, -31, yawtrim); 
	else
		yawtrim = map(rcData[YAW], PPM_MAX,  MIDRC, 31, yawtrim);
	if(yawtrim <=0)
		packet[2] |= (1<<5);
	packet[2] |= abs(yawtrim) & 0x1F;
#endif
	if( rcData[YAW] <= MIDRC)
		packet[2] |= (1<<7); // yaw direction
	if( rcData[PITCH] <= MIDRC)
		packet[2] |= (1<<6); // pitch direction
	packet[3] = SHBAND_A | ((packet[0] + packet[1] + packet[2] + 0x0F) & 0x3F); // band + checksum
	return (uint32_t)(((uint32_t)packet[0]<<24) | ((uint32_t)packet[1]<<16) | ((uint16_t)packet[2]<<8) | packet[3]);
}

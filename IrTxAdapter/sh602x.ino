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

uint8_t shpacket[4];

enum {
	BAND_A = (uint8_t)0xc0,
	BAND_B = (uint8_t)0x80,
	BAND_C = (uint8_t)0x40
};

uint32_t shBuildPacket() {
	memset(&shpacket,0,4);
	shpacket[0] = map(rcData[THROTTLE], PPM_MIN, PPM_MAX, 0, 120); // throttle
	shpacket[1] = map(rcData[YAW], MIDRC, rcData[YAW] > MIDRC ? PPM_MAX : PPM_MIN, 0, 15) << 4; // yaw
	shpacket[1] |= map(rcData[PITCH], MIDRC, rcData[PITCH] > MIDRC ? PPM_MAX : PPM_MIN, 0, 15); // pitch
	if( rcData[YAW] <= MIDRC)
		shpacket[2] |= (1<<7); // yaw direction
	if( rcData[PITCH] <= MIDRC)
		shpacket[2] |= (1<<6); // pitch direction
	if( rcData[AUX1] > MIDRC)
		shpacket[2] |= (1<<5); // yaw trim direction
	shpacket[2] |= map(rcData[AUX1], MIDRC, rcData[AUX1] > MIDRC ? PPM_MAX : PPM_MIN, 0, 31); // yaw trim
	shpacket[3] = BAND_A | ((shpacket[0] + shpacket[1] + shpacket[2] + 0x0F) & 0x3F); // channel + checksum
	return (uint32_t)(((uint32_t)shpacket[0]<<24) | ((uint32_t)shpacket[1]<<16) | ((uint16_t)shpacket[2]<<8) | shpacket[3]);
}


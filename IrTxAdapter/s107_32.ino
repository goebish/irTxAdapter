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
 Syma "genuine" S107(g) IR protocol:

 carrier frequency: 38kHz or 57KHz
 (all timings in µs)
 constant packet length
 header: 2000 pulse, 2000 space, 1000 pulse
 zero: 380 pulse, 220 space
 one: 320 pulse, 600 space
 end: 140000 space
 length of data: 32 bits

    byte0  |   byte1   |   byte 2  |   byte3
 A BBBBBBB | C DDDDDDD | E FFFFFFF | I JJJJJJJ

 A = always 0
 B = yaw (127=full left - 0=full right) center depends of yaw trim
 C = always 0
 D = pitch (127=full backward, 0=full forward)
 E = band (1=A, 0=B ?)
 F = throttle (0-127)
 I = always 0
 J = yaw trim (127=full left, 0=full right), looks like the RX does not use this value, but the TX use it internally to adjust yaw center and window 

*/

enum {
	SYBAND_A = (uint8_t)0x80,
	SYBAND_B = (uint8_t)0x00	
};

uint32_t syBuildPacket() {
	int8_t packet[4]={0,0,0,0};
	packet[1] = map(rcData[PITCH], PPM_MIN, PPM_MAX, 127, 0);
	packet[2] = SYBAND_A | map(rcData[THROTTLE], PPM_MIN, PPM_MAX, 0, 127);
	packet[3] = map(rcData[AUX1], PPM_MIN, PPM_MAX, 127, 0);
	int16_t yaw_center = map( packet[3], 0, 127, 45, 85);
	bool windowed=true;
#ifdef DYNAMIC_YAW_TRIM
	windowed=false;
#endif
	if(rcData[YAW] <= MIDRC)
		packet[0] = map( rcData[YAW], PPM_MIN, MIDRC, windowed ? constrain(yaw_center+45, 0, 127) : 127, yaw_center);
	else
		packet[0] = map( rcData[YAW], MIDRC, PPM_MAX, yaw_center, windowed ? constrain(yaw_center-45, 0, 127) : 0);
	return (uint32_t)(((uint32_t)packet[0]<<24) | ((uint32_t)packet[1]<<16) | ((uint16_t)packet[2]<<8) | packet[3]);
}

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
 B = yaw (106=full left - 8=full right)
 C = always 0
 D = pitch (126=full backward, 1=full forward)
 E = band (1=A, 0=B ?)
 F = throttle (11=0% -  125=100%)
 I = always 0
 J = yaw trim (127=full left, 1=full right) 

*/

enum {
	SYBAND_A = (uint8_t)0x80,
	SYBAND_B = (uint8_t)0x00	
};

uint32_t syBuildPacket() {
	uint8_t packet[4]={0,0,0,0};
	packet[0] = map(rcData[YAW], PPM_MIN, PPM_MAX, 107, 8);
	packet[1] = map(rcData[PITCH], PPM_MIN, PPM_MAX, 126, 1);
	packet[2] = SYBAND_A | map(rcData[THROTTLE], PPM_MIN, PPM_MAX, 5, 125);
	packet[3] = map(rcData[AUX1], PPM_MIN, PPM_MAX, 127, 1);
#if DYNAMIC_YAW_TRIM
	if(rcData[YAW]<MIDRC) 
		packet[3] = map(rcData[YAW], PPM_MIN, MIDRC, 127, packet[3]) & 0x7F;
	else
		packet[3] = map(rcData[YAW], MIDRC, PPM_MAX, packet[3], 1) & 0x7F;
#endif
	return (uint32_t)(((uint32_t)packet[0]<<24) | ((uint32_t)packet[1]<<16) | ((uint16_t)packet[2]<<8) | packet[3]);
}

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
 Syma S107(g) genuine IR protocol:

 carrier frequency: 38kHz or 57KHz
 (all timings in µs)
 constant packet length
 header: 2000 pulse, 2000 space, 1000 pulse
 zero: 380 pulse, 220 space
 one: 220 pulse, 600 space
 end: 140000 space
 length of data: 32 bits

yaw, pitch, throttle, trim

    byte0  |   byte1   |   byte 2  |   byte3
 A BBBBBBB | C DDDDDDD | E FFFFFFF | II JJJJJJ

 A = always 0
 B = yaw (106=full left - 8=full right)
 C = always 0
 D = pitch (126=full backward 1=full forward)
 E = always 1
 F = throttle (11=0% -  125=100%)
 I = always 0 ?
 J = yaw trim ?
*/

uint32_t syBuildPacket() {
	uint8_t packet[4]={0,0,0,0};
	packet[0] = map(rcData[YAW], PPM_MIN, PPM_MAX, 106, 8);
	packet[1] = map(rcData[PITCH], PPM_MIN, PPM_MAX, 126, 1);
	packet[2] = map(rcData[THROTTLE], PPM_MIN, PPM_MAX, 11, 125);
	packet[3] = 52; // yaw trim middle position ?
	return (uint32_t)(((uint32_t)packet[0]<<24) | ((uint32_t)packet[1]<<16) | ((uint16_t)packet[2]<<8) | packet[3]);
}
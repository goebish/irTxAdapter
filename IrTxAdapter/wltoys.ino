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
 WLToys EV-05
 32 bits, transition time encoding
 
 header = 1400µs
 zero = 300 µs
 one = 800 µs
 70ms between packets
 
 ATTTTTTT YYYYPPPP ???????? ?D?BCCCC

 A = 0, 1 if 0% throttle
 T = throttle ( 0 - 127 )
 Y = yaw ( left = 1, center = 8, right = 15 )
 P = pitch ( backward = 1, center = 8, forward = 15)
 D = boost ( 1 = on )
 B = bubbles ( 1 = on )
 C = checksum = (N1+N2+N3+N4+N5+N6+N7+1)%16

*/

uint32_t wlBuildPacket() {
	uint8_t packet[4] = {0,0,0,0};
	// throttle
	packet[0] = map(rcData[THROTTLE], PPM_MIN, PPM_MAX, 0, 127);
	if(packet[0] == 0)
		packet[0] |= 0x80;
	// yaw
	packet[1] = map(rcData[YAW], PPM_MIN, PPM_MAX, 1, 15) << 4;
	// pitch
	packet[1] |= map(rcData[PITCH], PPM_MIN, PPM_MAX, 1, 15);
	// unknown (band + ???)
	packet[2] = 64;
	// options
	// TODO: add channel/buttons for boost, leds, bubbles ...
	
	uint8_t checksum = (((packet[0]>>4)&0x0F) + (packet[0]&0x0F) + ((packet[1]>>4)&0x0F) + (packet[1]&0x0F) + 
	                ((packet[2]>>4)&0x0F) + (packet[2]&0x0F) + ((packet[3]>>4)&0x0F) + 1 ) % 16;  
	packet[3] |= checksum;
	return (uint32_t)(((uint32_t)packet[0]<<24) | ((uint32_t)packet[1]<<16) | ((uint16_t)packet[2]<<8) | packet[3]);
}

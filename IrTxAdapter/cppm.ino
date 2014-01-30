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

#if !(USE_ADC)
#if (PPM_PIN != 2) && (PPM_PIN != 3)
  #error "PPM_PIN must be D2 or D3"
#endif

#if (IR_PIN == 3) && (PPM_PIN == 3) 
	#error "IR_PIN and PPM_PIN cannot use the same pin"
#endif
#endif

#define WidthOK(w) ((w>=900) && (w<=2200))

const uint8_t TxMap[] = {
  CPPM
};

static boolean cppmFrameOK = false;

volatile uint16_t cppmRaw[RC_CHANS] = { MIDRC, }; // interval [PPM_MIN;PPM_MAX]

void cppmInit(void) {
  pinMode(PPM_PIN, INPUT);
  attachInterrupt(PPM_PIN-2, rxInt, RISING);
} 

void rxInt(void) {
  uint32_t NowuS;
  int16_t Width;
  static uint32_t PrevuS = 0;
  static uint8_t chan = 0;

  NowuS = micros();
  interrupts();
  Width = NowuS - PrevuS;
  PrevuS = NowuS;
  if( Width > 3000) { 
    cppmNewValues = cppmFrameOK;
    cppmFrameOK = true;
    chan = 0;
  }
  else
    if (chan < RC_CHANS) { 
      if (WidthOK(Width))
        cppmRaw[chan] = Width;
      else 
        cppmFrameOK &= false;
      chan++;
    }
} 

void cppmGetInput(void) {
  uint8_t chan;
  int16_t v;

  for (chan = 0; chan < RC_CHANS; chan++) {
    noInterrupts();
    v = cppmRaw[chan];
    interrupts(); 
    rcData[TxMap[chan]] = constrain(v, PPM_MIN, PPM_MAX);
  }
  cppmNewValues = false;
} 


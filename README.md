
irTxAdapter
=============

This open source project can be used as a standalone adapter
translating PPM SUM signals (also known as CPPM) where available
from a transmitter, to toy helicopters IR signals.

It can also be used to modify an existing IR transmitter to
be compatible with multiple protocols (eg helicopter brands).

Code may be modified using the Arduino development environment and 
uploaded using the associated bootloader on the Atmega328
microcontroller.

Pinouts and other settings are defined in config.h

Currently supported helicopters:

- San Huan 6020, 6020-1, 6023-1, 6025-1
- Syma S107(g) 2 bands version (ground D12 to enable)
- WLToys EV-05 and similars (ground D10 to enable) 

Other protocols might be added later.

5th channel (AUX1) is used for yaw trim, assign it to a knob if possible.

If using pots instead of CPPM input (#define USE_ADC true) you must
calibrate sticks (only once): connect a button between ground and 
ADC_BUTTON1 (default = pin D4) and press it when powering on the transmitter.
When the status LED (default = pin D13) starts blinking, move every 
sticks and knob from side to side then press the button again.

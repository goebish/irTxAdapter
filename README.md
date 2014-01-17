
irTxAdapter
=============

This is an open source standalone adapter translating CPPM signals where 
available from a transmitter, to toy helicopters IR signals.

Code may be modified using the Arduino development environment and 
uploaded using the associated bootloader on the Atmega 328
processor. Pinouts are defined in config.h

Currently supported helicopters:

- San Huan 6020, 6020-1, 6023-1, 6025-1
- Syma S107(g) 2 bands version (experimental, connect D12 to ground to enable)

Other protocols might be added later

5th CPPM channel (AUX1) is used for yaw trim, assign it to a knob if possible.

The IrTxAdapter was developed for my own use and the helis I 
have but may be of interest to others.
# Type 2

![Shock collar]


## Protocol

The table below shows the 5 bytes of data sent for each message:

       7 6 5 4 3 2 1 0
      +-+-+-+-+-+-+-+-+
    0 | Chan  | Mode  | 
      +-+-+-+-+-+-+-+-+
    1 |  Transmitter  |
    2 |  ID           |
      +-+-+-+-+-+-+-+-+      
    3 | Power         |
      +-+-+-+-+-+-+-+-+
    4 |~Mode  |~Chan  | 
      +-+-+-+-+-+-+-+-+

Bytes:

* 0: Mode (lower nibble) can be 1, 2 or 4 for Shock, Vibrate or Beep respectively.
     Channel (upper nibble) - can be 8 or 15 for channel 1 or 2
    
* 1+2: Probably Transmitter ID. I _assume_ this is unique(-ish) and fixed per transmitter. I've only tested one remote, so can't be sure.

* 3:  Power level, 0-100

* 4:  Channel and Mode, reversed and inverted. E.g. if byte 0 is `1000 0010` (channel 1, vibrate) this would be `1011 1110`

Note that the message doen't have a checksum, and doesn't have to be repeated multiple times to work. So interfereance / a single bit error could easily result in an unexpected power level!
It seems odd there is some redundancy for mode/channel, but not power.

[Shock collar]: images/type2/collar.jpg "Type2 shock collar"


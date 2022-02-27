# Type 1

![Shock collar]

Since creating this library, I've realised that there are (at least) two different types of this collar which look *identical* on the outside, but very different on the inside and operate at different frequencies - 433HMz and 915MHz. This library was written for the 433MHz variant, and I've got no idea if it'll work for the 915MHz variant or if the protocol is also different (given how different the insides look, I suspect it _won't_ work).

![433vs915]

## Transmission format
The transmission format is probably much easier to explain with the benefit of a capture/diagram:
![bit format]

Each bit is ~1ms long and starts with a rising edge. If the signal is still high ~500uS after the rising edge, it's treated as a 1, otherwise a 0 (data is sent MSB first).

This makes it pretty easy to decode without the need for an accurate clock (or any clock recovery) on either the transmitter or receiver side.

## Protocol

The table below shows the 5 bytes of data sent for each message:

       0 1 2 3 4 5 6 7
      +-+-+-+-+-+-+-+-+
    0 |  Transmitter  |
    1 |  ID           |
      +-+-+-+-+-+-+-+-+
    2 | Mode  | Chan  | 
      +-+-+-+-+-+-+-+-+
    3 | Power         |
      +-+-+-+-+-+-+-+-+
    4 | Checksum      |
      +-+-+-+-+-+-+-+-+

For some reason, each message is repeated 3 times when a button is pressed, and the collars do seem to require this.

Bytes:

* 0+1:Transmitter ID. I _assume_ this is unique(-ish) and fixed per transmitter.

* 2:  Mode (lower nibble) can be 1, 2 or 3 for Shock, Vibrate or Beep respectively.
    Channel (upper nibble) - can be 0, 1 or 2 for channel 1, 2 or 3 

* 3:  Power level, 0-99. 0 doesn't do anything, but does seem to stop the collar going to sleep. >99 doesn't seem to do anything either.

* 4:  Checksum, which is the sum of bytes 0-3



[Shock collar]: images/type1/collar.jpg "Type1 shock collar"
[433vs915]: images/type1/433vs915.jpg
[bit format]: images/type1/cap.png

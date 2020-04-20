
Collar
=====
This is an Arduino library to control this specific type of shock collar which are easy to find at the moment on ebay:

![Collar](https://raw.githubusercontent.com/CrashOverride85/collar/master/images/collar.jpg)

Since creating this library, I've realised that there are (at least) two different types of this collar which look *identical* on the outside, but very different on the inside and operate at different frequencies - 433HMz and 915MHz. This library was written for the 433MHz variant, and I've got no idea if it'll work for the 915MHz variant or if the protocol is also different (given how different the insides look, I suspect it _won't_ work).

![433vs915](https://raw.githubusercontent.com/CrashOverride85/collar/master/images/433vs915.jpg)


There are also other libraries which can control other similar shock collars, e.g. [DogCollar2](https://github.com/flash89y/Arduino/tree/master/libraries/DogCollar2)


## Hardware
The only required hardware is an Arduino (tested on an Uno) and a 433MHz transmitter and/or receiver module which can be obtained cheaply from ebay.
The TX module can be used on any digital pin, but the RX module must be connected to either digital 2 or 3, as these have interrupts.

## Installation
Similar to pretty much any other Arduino library... put the contents of `collar` directory of the repo into the Arduino/sketchbook libraries folder.

## Examples
* ColRxExample - this will output to the serial console any messages received, including the ID of the transmitter - which can be used to clone the original transmitter, rather than needing to re-pair the collars.
I should probably point out that whilst the CollarTx library seems pretty reliable so far, the CollarRx library is not so great, and isn't particularity well written. But unless you want to use the original controller, it shouldn't really matter.

* ColTxSimple - about the simplest example possible that controls a collar - it sends a vibrate command with power=10 for channel 1. If the ID of the original collar is known (e.g. from the Rx example), it can be entered, otherwise an ID should be made up, and the collars paired following the usual process.

* ColTxSerial - allows all message types (shock/vibrate/beep ch1/2/3 with power level) to be transmitted using input from the serial console. It outputs instructions on the serial console on startup.

And for anyone interested in how it works...

## Transmission format
The transmission format is probably much easier to explain with the benefit of a capture/diagram:
![bit format](https://raw.githubusercontent.com/CrashOverride85/collar/master/images/cap.png)

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


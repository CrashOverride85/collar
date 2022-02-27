#include <collar.h>

/*
 * Simple shock collar transmit example 
 *
 * Before it can be used, either the original transmitters ID
 * needs to be found (e.g. using the Rx example) and entered into
 * "transmitter_id" below, or a value made up and the collar(s)
 * paired using the usual process:
 *   - press and hold the power button untill it beeps
 *   - reset the Arduino to send the command
 *   - if it works, the collar should give a long beep
 *   - reset the Arduino again to send the command - which should now work
 */

// Set this to whichever pin has the 433MHz transmitter connected to it
const uint8_t  tx_pin = 7;

// Either set this to the id of the original transmitter,
// or make a value up and re-pair
const uint16_t transmitter_id = 0x1234;

CollarTx *_tx;

void setup()
{
  Serial.begin(115200);
  Serial.print("Using ID: ");
  Serial.println(transmitter_id, HEX);
  _tx =  new CollarTx(tx_pin, transmitter_id);

  /* Valid options for: 
   *   channel - CH1, CH2, CH3
   *   mode    - BEEP, SHOCK, VIBE
   *   power   - 0-99
   */
  _tx->transmit(CH1, VIBE, 10);
}

void loop()
{

}


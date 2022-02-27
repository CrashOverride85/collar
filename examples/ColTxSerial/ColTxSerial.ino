#include <collar.h>

/*
 * Shock collar transmit example from serial input
 *
 * Before it can be used, either the original transmitters ID
 * needs to be found (e.g. using the Rx example) and entered into
 * "transmitter_id" below, or a value made up and the collar(s)
 * paired using the usual process:
 *   - press and hold the power button untill it beeps
 *   - send any command (e.g. 1V01)
 *   - if it works, the collar should give a long beep, and
 *     other commands will now work
 */


// Set this to whichever pin has the 433MHz transmitter connected to it
const uint8_t  tx_pin = 7;

// Either set this to the id of the original transmitter,
// or make a value up and re-pair
const uint16_t transmitter_id = 0x04D2;

CollarTx *_tx;

void setup()
{
  Serial.begin(115200);
  Serial.print("Using ID: ");
  Serial.println(transmitter_id, HEX);
  _tx =  new CollarTx(tx_pin, transmitter_id);
  print_help();
}

void print_help()
{
  Serial.println("\nExpected command format:");
  Serial.println("  <channel><command><power>");
  Serial.println("where:");
  Serial.println("   channel = 1-3");
  Serial.println("   command = B, S, V (Beep, Shock, Vibrate)");
  Serial.println("   power   = 00-99");
  Serial.println();
  Serial.println("e.g.:");
  Serial.println("  1V05 - Vibrate channel 1 at power 5");
  Serial.println("  2S10 - Shock channel 2 at power 10");
  Serial.println();  
}

void loop()
{
  static char serial_buffer[8];
  static uint8_t serial_pos=0;

  while (Serial.available() > 0)
  {
    char c = Serial.read();

    if ((c == '\n') || (c == '\r'))
    {
      serial_buffer[serial_pos] = '\0';

      if (serial_pos > 0)
      {
        process_message(serial_buffer);
        memset(serial_buffer, 0, sizeof(serial_buffer));
        serial_pos = 0;
      }
    }
    else
    {
      if (serial_pos < (sizeof(serial_buffer)-1))
      {
        serial_buffer[serial_pos++] = c;
      }
    }
  }
}

void process_message(const char *input_message)
{
  collar_channel channel;
  collar_mode mode;
  uint8_t power;

  if (strlen(input_message) != 4)
  {
    Serial.println("Message not expected 4 characters. Ignorning.");
    return;
  }

  // Channel
  switch (input_message[0])
  {
    case '1':
      channel = CH1;
      break;

    case '2':
      channel = CH2;
      break;

    case '3':
      channel = CH3;
      break;

    default:
      Serial.println("Unexpected channel");
      return;
  }

  // Mode
  switch (input_message[1])
  {
    case 'B':
    case 'b':
      mode = BEEP;
      break;

    case 'S':
    case 's':
      mode = SHOCK;
      break;

    case 'V':
    case 'v':
      mode = VIBE;
      break;

    default:
      Serial.println("Unexpected mode");
      return;
  }

  power = atoi(&input_message[2]);

  Serial.println("Transmitting...");
  _tx->transmit(channel, mode, power);
}


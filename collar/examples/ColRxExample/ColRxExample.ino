#include <collar.h>

/*
 * Simple shock collar receive example 
 */

// Set this to the pin which has the 433MHz receiver connected to it
// Important: *MUST* be either pin 2 or 3
const uint8_t  rx_pin = 3;

CollarRx *_rx;
volatile bool got_message = false;
struct collar_message *rx_message;

void setup() 
{
  Serial.begin(115200);
  _rx =  new CollarRx(rx_pin, message_callback, NULL);
}

// Called from within an interupt handler when a message is received,
// therefore it shouldn't do much.
void message_callback (const struct collar_message *msg, void *userdata)
{
  memcpy(rx_message, msg, sizeof(struct collar_message));
  got_message = true;
}


void loop() 
{
  if (got_message)
  {
    Serial.println("\nGot message:");
    CollarRx::print_message(rx_message);
    got_message = false;
    Serial.println("");
  }
}


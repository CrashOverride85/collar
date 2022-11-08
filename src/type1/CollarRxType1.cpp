#include "CollarRxType1.h"

#define TYPE1_START_PULSE_LEN_US 2200
#define TYPE1_START_PULSE_TOLLERANCE 100


CollarRxType1::CollarRxType1(uint8_t rx_pin, msg_cb_t cb, void *userdata, uint16_t id) : CollarRx { rx_pin, cb, userdata, id }
{

}

CollarRxType1::CollarRxType1(uint8_t rx_pin, msg_cb_t cb, void *userdata) : CollarRx { rx_pin, cb, userdata } 
{

}

void CollarRxType1::buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg)
{
  // wipe old message
  memset(msg, 0, sizeof(struct collar_message));

  // bytes 0&1 = ID
  memcpy(&msg->id, buffer, 2);

  // byte 2 = mode & channel
  msg->mode     = (collar_mode)(buffer[2] & 0x0F);
  msg->channel  = (collar_channel)((buffer[2] & 0xF0) >> 4);

  // byte 3 = power
  msg->power    =  buffer[3];

  // byte 4 = checksum
}

bool CollarRxType1::is_message_valid(const uint8_t buffer[5])
{
  // if we're filtering by ID, check it matches
  if (_use_id && (memcmp(buffer, &_id, 2)))
    return false;

  // calculate checksum
  uint8_t chk=0;
  for (uint8_t i=0; i < 4; i++)
    chk += buffer[i];

  // validate checksum is correct
  if (chk != buffer[4])
    return false;

  return true;
}

IRAM_ATTR void CollarRxType1::isr()
{
  static unsigned long rx_micros =0;
  static uint8_t pulse_count = 0;
  delayMicroseconds(500); // BAD: really shouldn't do this in an ISR
  uint8_t val = digitalRead(_rx_pin);

  static uint8_t buffer[5];        // expecting to receive 5 bytes
  static uint8_t byte_postion = 0; // keep track of current byte being received
  static uint8_t bit_position = 0; // keep track of expected next bit postion in byte

  // look for tranmission start state (rising edge->rising edge of ~2.2ms)
  int last_pulse_len = micros()-rx_micros ;
  if ((last_pulse_len > TYPE1_START_PULSE_LEN_US-TYPE1_START_PULSE_TOLLERANCE) &&
      (last_pulse_len < TYPE1_START_PULSE_LEN_US+TYPE1_START_PULSE_TOLLERANCE))
      {
        pulse_count = 0;

        byte_postion = 0;
        bit_position = 0;
        memset(buffer, 0, sizeof(buffer));
      }

  if (byte_postion < sizeof(buffer))
  {
    if (val)
      buffer[byte_postion] |= (1 << (7-bit_position));

    if (++bit_position >= 8)
    {
      bit_position = 0;
      byte_postion++;
    }

    if (byte_postion >=  sizeof(buffer))
    {
       if (is_message_valid(buffer))
       {
         buffer_to_collar_message(buffer, &_rx_msg);
         _cb(&_rx_msg, _userdata);
       }
    }
  }

  rx_micros = micros();

  if (pulse_count < 50)
  {
      pulse_count++;
  }
}

#include <CollarRxType2.h>

#define TYPE2_START_PULSE_LEN_US 2300
#define TYPE2_START_PULSE_TOLLERANCE 100


CollarRxType2::CollarRxType2(uint8_t rx_pin, msg_cb_t cb, void *userdata, uint16_t id) : CollarRx { rx_pin, cb, userdata, id }
{

}

CollarRxType2::CollarRxType2(uint8_t rx_pin, msg_cb_t cb, void *userdata) : CollarRx { rx_pin, cb, userdata } 
{

}

// If message is valid, returns true, and msg is populated with details
bool CollarRxType2::buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg)
{
  bool is_valid = true;

  // wipe old message
  memset(msg, 0, sizeof(struct collar_message));

  // byte 0 = channel + mode
  // mode
  switch (buffer[0] & 0x0F)
  {
    case 1:
      msg->mode = SHOCK;
      break;
      
    case 2:
      msg->mode = VIBE;
      break;
      
    case 4:
      msg->mode = BEEP;
      break;
      
    default:
      is_valid = false;
      break;    
  }

  // channel
  switch ((buffer[0] & 0xF0) >> 4)
  {
    case 8:
      msg->channel = CH1;
      break;
      
    case 15:
      msg->channel = CH2;
      break;
      
    default:
      is_valid = false;
      break;    
  }


  // byte 1+2 = id
  memcpy(&msg->id, buffer+1, 2);

  // byte 3 = power
  msg->power    =  buffer[3];
  if (msg->power > 100)
    is_valid = false;

  // byte 4 = reversed and inverse of chan+mode
  uint8_t channel_inverted_reversed = 0;
  if (msg->channel == CH1)
    channel_inverted_reversed = 14;
  else if (msg->channel == CH2)
    channel_inverted_reversed = 0;
  else
    is_valid = false;
  
  uint8_t mode_inverted_reversed = 0;
  if (msg->mode == SHOCK)
    mode_inverted_reversed = 7;
  else if (msg->mode == VIBE)
    mode_inverted_reversed = 11;
  else if (msg->mode == BEEP)
    mode_inverted_reversed = 13;
  else
    is_valid = false;
  
  if (((mode_inverted_reversed<< 4) | channel_inverted_reversed) != buffer[4])
    is_valid = false;

  // if we're filtering by ID, check it matches
  if (_use_id && (memcmp(buffer+1, &_id, 2)))
    return false;
  
  return is_valid;
}

void CollarRxType2::isr()
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
  if ((last_pulse_len > TYPE2_START_PULSE_LEN_US-TYPE2_START_PULSE_TOLLERANCE) &&
      (last_pulse_len < TYPE2_START_PULSE_LEN_US+TYPE2_START_PULSE_TOLLERANCE))
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
       if (buffer_to_collar_message(buffer, &_rx_msg))
       {
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

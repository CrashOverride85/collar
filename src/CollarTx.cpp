#include "collar.h"

void CollarTx::transmit (collar_channel channel, collar_mode mode, uint8_t power)
{
  collar_message msg;
  msg.id = _id;
  msg.channel = channel;
  msg.mode = mode;
  msg.power = power;
  transmit(msg);
}

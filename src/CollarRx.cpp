#include <collar.h>

CollarRx* CollarRx::_instance;


CollarRx::CollarRx(uint8_t rx_pin, msg_cb_t cb, void *userdata, uint16_t id)
{
  _rx_pin = rx_pin;
  _cb = cb;
  _userdata = userdata;
  _id = id;
  _use_id = true;

  rx_start();
}

CollarRx::CollarRx(uint8_t rx_pin, msg_cb_t cb, void *userdata)
{
  _rx_pin = rx_pin;
  _cb = cb;
  _userdata = userdata;
  _use_id = false;

  rx_start();
}

void CollarRx::print_message(struct collar_message *msg)
{
  Serial.print("ID:\t0x");
  Serial.println(msg->id, HEX);

  Serial.print("Chan.:\t");
  Serial.println(chan_to_str(msg->channel));

  Serial.print("Mode:\t");
  Serial.println(mode_to_str(msg->mode));

  Serial.print("Power:\t");
  Serial.println(msg->power);
}

const char *CollarRx::chan_to_str(collar_channel channel)
{
  switch (channel)
  {
    case CH1:
      return "CH1";

    case CH2:
      return "CH2";

    case CH3:
      return "CH3";

    default:
      return "CH?";
  }
}

const char *CollarRx::mode_to_str(collar_mode mode)
{
  switch (mode)
  {
    case SHOCK:
      return "Shock";

    case VIBE:
      return "Vibrate";

    case BEEP:
      return "Beep";

    default:
      return "MODE?";
  }
}

void CollarRx::rx_start()
{
  _instance = this;
  pinMode(_rx_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_rx_pin), CollarRx::s_isr, RISING);
}

IRAM_ATTR void CollarRx::s_isr()
{
  _instance->isr();
}



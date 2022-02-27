
#include "collar.h"

class CollarRxType2 : public CollarRx
{
  public:
    CollarRxType2(uint8_t rx_pin, msg_cb_t cb, void *userdata, uint16_t id);
    CollarRxType2(uint8_t rx_pin, msg_cb_t cb, void *userdata);

  private:
    bool buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg);
    void isr();

};

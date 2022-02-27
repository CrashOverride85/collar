
#include "collar.h"

class CollarRxType1 : public CollarRx
{
  public:
    CollarRxType1(uint8_t rx_pin, msg_cb_t cb, void *userdata, uint16_t id);
    CollarRxType1(uint8_t rx_pin, msg_cb_t cb, void *userdata);

  private:
    bool is_message_valid(const uint8_t buffer[5]);
    void buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg);
    void isr();

};

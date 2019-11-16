
#include <stdint.h>
#include "Arduino.h"

enum collar_mode { SHOCK=1, VIBE=2, BEEP=3 };
enum collar_channel { CH1=0, CH2=1, CH3=2 };

struct collar_message
{
  uint16_t id;
  collar_mode mode;
  collar_channel channel;
  uint8_t power;
};

class CollarTx
{
  public:
    CollarTx(uint8_t tx_pin, uint16_t id);
    void transmit (collar_channel channel, collar_mode mode, uint8_t power);
    void transmit (struct collar_message message);

  private:
    uint16_t _id;
    uint8_t _tx_pin;

    void tx_start();
    void tx_bit(bool one);
    void tx_byte(uint8_t val);
    void tx_buffer(uint8_t *buf, uint8_t buf_len);
};

#define START_PULSE_LEN_US 2200
#define START_PULSE_TOLLERANCE 100
typedef void (*msg_cb_t)(const struct collar_message *msg, void *userdata);

class CollarRx
{
  public:
    CollarRx(uint8_t rx_pin, msg_cb_t cb, void *userdata, uint16_t id);
    CollarRx(uint8_t rx_pin, msg_cb_t cb, void *userdata);
    static const char *chan_to_str(collar_channel channel);
    static const char *mode_to_str(collar_mode mode);
    static void print_message(struct collar_message *msg);
    static CollarRx *_instance;


  private:
    uint16_t _id;
    bool _use_id;
    uint8_t _rx_pin;
    void *_userdata;
    msg_cb_t _cb;
    struct collar_message _rx_msg;


    void rx_start();
    bool is_message_valid(const uint8_t buffer[5]);
    void buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg);
    void isr();
    static void s_isr();

};


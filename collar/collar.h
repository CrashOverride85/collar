
#ifndef _COLLAR_H
#define _COLLAR_H

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
    void transmit (collar_channel channel, collar_mode mode, uint8_t power);
    virtual void transmit (struct collar_message message) = 0;
    
  protected:
    uint16_t _id;
};


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
    static void s_isr();
    
  private:
    void rx_start();
    
  protected:
    uint16_t _id;
    bool _use_id;
    uint8_t _rx_pin;
    void *_userdata;
    msg_cb_t _cb;
    struct collar_message _rx_msg;
    virtual void isr() = 0;
};


#endif

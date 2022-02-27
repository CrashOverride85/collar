#ifndef _COLLARTXTYPE2_H
#define _COLLARTXTYPE2_H

#include <collar.h>


class CollarTxType2 : public CollarTx
{
  
  public:
    CollarTxType2(uint8_t tx_pin, uint16_t id);
    void transmit (struct collar_message message);

  private:
    uint8_t _tx_pin;
    
    void tx_start();
    void tx_bit(bool one);
    void tx_byte(uint8_t val);
    void tx_buffer(uint8_t *buf, uint8_t buf_len);  
};

#endif

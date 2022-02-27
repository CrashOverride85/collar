#include <CollarTxType2.h>

CollarTxType2::CollarTxType2(uint8_t tx_pin, uint16_t id)
{
  _tx_pin = tx_pin;
  _id = id;

  pinMode(_tx_pin, OUTPUT);
  digitalWrite(_tx_pin, LOW);
}

void CollarTxType2::transmit (struct collar_message message)
{
  uint8_t txbuf[5]; // bytes  0=chan&mode, 1+2=ID, 3=power, 4=mode&chan (reversed & inverted)

  uint8_t channel;
  if (message.channel == CH1)
    channel = 8;
  else if (message.channel == CH2)
    channel = 15;
  else
    return; // invalid channel for this type of collar
  
  txbuf[0] = ((channel << 4) | message.mode);
  memcpy(txbuf+1, &message.id, 2);

  // Power levels >100 are ignored by the collar
  if (message.power > 100)
    txbuf[3] = 100;
  else
    txbuf[3] = message.power;

  uint8_t channel_inverted_reversed;
  if (message.channel == CH1)
    channel_inverted_reversed = 14;
  else if (message.channel == CH2)
    channel_inverted_reversed = 0;
  else
    return; // invalid channel for this type of collar  
  
  uint8_t mode_inverted_reversed = 0;
  if (message.mode == SHOCK)
    mode_inverted_reversed = 7;
  else if (message.mode == VIBE)
    mode_inverted_reversed = 11;
  else if (message.mode == BEEP)
    mode_inverted_reversed = 13;
    
  txbuf[4] = ((mode_inverted_reversed<< 4) | channel_inverted_reversed);
  
  tx_buffer(txbuf, sizeof(txbuf));
  tx_buffer(txbuf, sizeof(txbuf));
}

void CollarTxType2::tx_start()
{
  digitalWrite(_tx_pin, HIGH);
  delayMicroseconds(1550);
  digitalWrite(_tx_pin, LOW);
  delayMicroseconds(770);
}

void CollarTxType2::tx_bit(bool one)
{
  if (one)
  {
    digitalWrite(_tx_pin, HIGH);
    delayMicroseconds(770);
    digitalWrite(_tx_pin, LOW);
    delayMicroseconds(230);
  }
  else
  {
    digitalWrite(_tx_pin, HIGH);
    delayMicroseconds(230);
    digitalWrite(_tx_pin, LOW);
    delayMicroseconds(770);
  }
}

void CollarTxType2::tx_byte(uint8_t val)
{
  for (int n=7; n >=0; n--)
    tx_bit(val & (1 << n));
}

// Transmit buffer
void CollarTxType2::tx_buffer(uint8_t *buf, uint8_t buf_len)
{
  tx_start();
  
  for (int n=0; n < buf_len; n++)
     tx_byte(buf[n]);

  tx_bit(0);
}

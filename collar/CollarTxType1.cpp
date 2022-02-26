#include <CollarTxType1.h>

CollarTxType1::CollarTxType1(uint8_t tx_pin, uint16_t id)
{
  _tx_pin = tx_pin;
  _id = id;

  pinMode(_tx_pin, OUTPUT);
  digitalWrite(_tx_pin, LOW);
}

void CollarTxType1::transmit (struct collar_message message)
{
  uint8_t txbuf[4]; // bytes 0+1=ID, 2=mode&channel, 3=power

  memcpy(txbuf, &message.id, 2);
  txbuf[2] = ((message.channel << 4) | message.mode);

  // Power levels >99 are ignored by the collar
  if (message.power > 99)
    txbuf[3] = 99;
  else
    txbuf[3] = message.power;

  // The collar seems to expect to receive the same message 3 times
  tx_buffer(txbuf, sizeof(txbuf));
  tx_buffer(txbuf, sizeof(txbuf));
  tx_buffer(txbuf, sizeof(txbuf));
}

void CollarTxType1::tx_start()
{
  digitalWrite(_tx_pin, HIGH);
  delayMicroseconds(1550);
  digitalWrite(_tx_pin, LOW);
  delayMicroseconds(660);
}

void CollarTxType1::tx_bit(bool one)
{
  if (one)
  {
    digitalWrite(_tx_pin, HIGH);
    delayMicroseconds(830);
    digitalWrite(_tx_pin, LOW);
    delayMicroseconds(200);
  }
  else
  {
    digitalWrite(_tx_pin, HIGH);
    delayMicroseconds(360);
    digitalWrite(_tx_pin, LOW);
    delayMicroseconds(670);
  }
}

void CollarTxType1::tx_byte(uint8_t val)
{
  for (int n=7; n >=0; n--)
    tx_bit(val & (1 << n));
}


// Transmit buffer & calculate/include checksum
void CollarTxType1::tx_buffer(uint8_t *buf, uint8_t buf_len)
{
  uint8_t check=0;
  tx_start();

  for (int n=0; n < buf_len; n++)
  {
     check += buf[n];
     tx_byte(buf[n]);
  }
  tx_byte(check);

  // Not sure why this is needed. The original remote sends it, and it seems temperamental without it, so whatever...
  tx_bit(0);
}

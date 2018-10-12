#ifndef __CRC8_H__
#define __CRC8_H__

unsigned char crc8(const unsigned char *buf, int len);

#ifdef CRYPTO_TEST
int crc8Test();
#endif

#endif //__CRC8_H__

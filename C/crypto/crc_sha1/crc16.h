#ifndef __CRC16_H__
#define __CRC16_H__

unsigned short crc16(const char *buf, int len);

#ifdef CRYPTO_TEST
int crc16Test();
#endif

#endif //__CRC16_H__

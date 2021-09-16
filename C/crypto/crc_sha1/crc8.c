#include "crc8.h"

#define CRC8_POLY 0x31 //Polynomial 0x31(X8 + X5 + X4 + 1)
#define CRC8_INIT 0x00 //Initialization 0xFF
unsigned char crc8(const unsigned char *buf, int len)
{
	int counter;
	unsigned char crc = CRC8_INIT;

	while(len--) {
		crc ^= *buf++;
		for(counter = 0; counter < 8; counter++) {
			if(crc & 0x01) {
				//crc = (crc >> 1) ^ CRC8_POLY;
				crc ^= 0x18;
				crc >>= 1;
				crc |= 0x80;
			} else
				crc >>= 1;
		}
	}

	return crc;
}


/* Test main */
#ifdef CRYPTO_TEST
#include <stdio.h>

int crc8Test() {
	printf("a1 == %02x\n",
			(unsigned char) crc8((char*)"123456789", 9));
	return 0;
}
#endif


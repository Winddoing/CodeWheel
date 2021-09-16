/*#############################################################
 *     File Name	: test.c
 *     Author		: wqshao
 *     Created Time	: 2018年10月12日 星期五 17时02分18秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "crc8.h"
#include "crc16.h"
#include "crc64.h"
#include "sha1.h"


/* 在线验证： https://www.lammertbies.nl/comm/info/crc-calculation.html */
/* 
 * "123456789" CRC Calculate
 *
 * 1 byte checksum		221
 * CRC-16				0xBB3D
 * CRC-16 (Modbus)		0x4B37
 * CRC-16 (Sick)		0x56A6
 * CRC-CCITT (XModem)	0x31C3
 * CRC-CCITT (0xFFFF)	0x29B1
 * CRC-CCITT (0x1D0F)	0xE5CC
 * CRC-CCITT (Kermit)	0x8921
 * CRC-DNP				0x82EA
 * CRC-32				0xCBF43926
 * */

#ifdef CRYPTO_TEST
void crypto_base_test()
{
	crc8Test();
	crc16Test();
	crc64Test();
	sha1Test();
}
#endif

int main(int argc, const char *argv[])
{
#ifdef CRYPTO_TEST
	printf("======================\n");
	crypto_base_test();
	printf("======================\n");
#endif

	//char s_crc8[32] = "61:63:3a:38:33:3a";
	char s_crc8[32] = "123456789";
	unsigned char v_crc8 = crc8(s_crc8, strlen(s_crc8));
	printf("crc8: %#x [%d]\n", v_crc8, v_crc8);

	char s_crc16[32] = "61:63:3a:38:33:3a";
	unsigned short v_crc16 = crc16(s_crc16, strlen(s_crc16));
	printf("crc16: %#x [%d]\n", v_crc16, v_crc16);

	return 0;
}

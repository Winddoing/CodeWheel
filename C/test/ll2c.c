/*#############################################################
 *     File Name	: aa.c
 *     Author		: wqshao
 *     Created Time	: 2018年05月30日 星期三 12时43分02秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

/**
 * @brief  main 
 *
 * gcc编译器的版本不同可能最后结果不一样
 * 测试正常保存
 */
int main(int argc, const char *argv[])
{

	unsigned char dst[30];
	unsigned long long rtpTime = 0x1234567898765;
	unsigned long long rtp_time_s = 0;
	int i = 0, j = 56;

	memset(dst, 0, sizeof(char) * 30);

	for (i = 0;  i < sizeof(rtpTime);  i++) {
		dst[19 - i] = (unsigned char)((rtpTime >> j) & 0xFF);
		printf("===> func: %s, line: %d, rtpTime: %016llx, %d, dst[%d]=%02x\n",
				__func__, __LINE__, (rtpTime >> j) & 0xFF, j, 19 - i, dst[19 - i]);
		j -= 8;
	}

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	j = 56;

	for (i = 0;  i < sizeof(rtp_time_s);  i++) {
		rtp_time_s |= (unsigned long long)dst[19 - i] << j;
		j -= 8;
	}
	printf("===> func: %s, line: %d,  old: %016llx\n", __func__, __LINE__, rtpTime);
	printf("===> func: %s, line: %d,  new: %016llx\n", __func__, __LINE__, rtp_time_s);


	return 0;
}

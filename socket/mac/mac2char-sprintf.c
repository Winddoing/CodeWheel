/*#############################################################
 *     File Name	: mac2char-sprintf.c
 *     Author		: wqshao
 *     Created Time	: 2018年06月12日 星期二 11时16分16秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]        
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"                           

int main(int argc, const char *argv[])
{
	char *mac_str = "04:e6:76:c3:63:dc";
	unsigned char mac[6] = {0};
	int v[6];
	char mac2str[32];
	int i = 0;

	for (i = 0; i < strlen(mac_str); i++) {
		printf("%c ", mac_str[i]);
	}
	printf("\nlen=%ld\n", strlen(mac_str));
	printf("mac_str: %s\n", mac_str);

	sscanf(mac_str, MACSTR, &v[0],&v[1],&v[2],&v[3],&v[4],&v[5]);
	for (i=0;i<6;i++) 
		mac[i]=(unsigned char)v[i];


	for (i = 0; i < strlen(mac); i++) {
		printf("%c ", mac[i]);
	}
	printf("\nlen=%ld\n", strlen(mac));

	sprintf(mac2str, MACSTR, MAC2STR(mac));  

	printf("mac2str=%s\n", mac2str);

	return 0;
}

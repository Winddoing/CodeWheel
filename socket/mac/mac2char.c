/*#############################################################
 *     File Name	: mac2char.c
 *     Author		: wqshao
 *     Created Time	: 2018年06月12日 星期二 10时57分10秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

int hex2num(char c)
{
	if (c>='0' && c<='9') return c - '0';
	if (c>='a' && c<='z') return c - 'a' + 10;//这里+10的原因是:比如16进制的a值为10
	if (c>='A' && c<='Z') return c - 'A' + 10;

	printf("unexpected char: %c", c);
	return -1;
}

int str2mac(const char * szMac, char * pMac)  
{  
	const char * pTemp = szMac;  
	int i;

	for (i = 0;i < 6;++i)  
	{  
		pMac[i] = hex2num(*pTemp++) * 16;  
		pMac[i] += hex2num(*pTemp++);  
	}     
	return 0;
} 

int main()
{
	char szValue[32] = "11AABB22CC33 ";
	char szBSSID[6] = "";

	str2mac(szValue, szBSSID);
	printf("Value = %s\n", szValue);
	printf("bssid : %02x-%02x-%02x-%02x-%02x-%02x \n", 
			st_keys.bssid_name[0], st_keys.bssid_name[1],
			st_keys.bssid_name[2], st_keys.bssid_name[3],
			st_keys.bssid_name[4], st_keys.bssid_name[5]);
	return 0;
}

#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[3];

	str[0]=201; /*201为C9的十进制值*/
	str[1]=219; /*219为DB的十进制值*/
	str[2]='\0';
	printf("C9DB的汉字的值为:%s",str);
	return 0;
}

/*#############################################################
 *     File Name	: dele_str_last_char.c
 *     Author		: wqshao
 *     Created Time	: 2018年07月05日 星期四 16时48分12秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

static void clean_string(char * const str) {
	char *start = str;
	char *end = str;
	char *p = str;
	while(*p) {
		switch(*p) {
		case ' ':
		case '\r':
		case '\n':
			if(str != start) {
				*start = *p;
				start++;
			}
			break;
		default:
			*start = *p;
			start++;
			end = start;
		}
		p++;
	}
	*end = '\0';
}

int main(int argc, const char *argv[])
{
	char s1[20] = "abcdef123\r\n";
	char s2[20] = "abcd�";

	printf("s1: %s\n", s1);
	printf("s2: %s\n", s2);

	clean_string(s1);
	clean_string(s2);

	printf("s1: %s\n", s1);
	printf("s2: %s\n", s2);

	return 0;
}


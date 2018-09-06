/*#############################################################
 *     File Name	: string.c
 *     Author		: wqshao
 *     Created Time	: 2018年09月06日 星期四 14时00分14秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

int main()
{
	char *s = "aa bb cc";
	printf("s: [%s]\n", s);

	printf("s: strlen=%ld\n", strlen(s));

	/* char */
	unsigned char uc = 128;
	char c = 128;

	printf("uc-d=%d\n",uc);      //128
	printf("c-d=%d\n",c);       //-128
	printf("uc-u=%u\n",uc);      //128
	printf("c-u=%u\n",c);       //4294967168
	printf("uc-08x=%08x\n",uc);    //0x00000080
	printf("c-x=%x\n",c);       //0xffffff80
	
	/* sizeof */

	struct __attribute__ ((__packed__)) sdshdr5 {                               
	    unsigned char flags; /* 3 lsb of type, and 5 msb of string length */    
	    char buf[];                                                             
	};                                                                          
	printf("packed sizeof=%ld\n", sizeof(struct sdshdr5));

	struct sdshdr55 {                                    
	    unsigned char flags; /* 3 lsb of type, and 5 msb of string length */         
/*	    char buf[];                                                                  */
	    char buf;                                                                  
	};                                                                               
	printf("sizeof=%ld\n", sizeof(struct sdshdr55));

	struct ssss {
		int aaa;
		int *a;
	};

	printf("int struct sizeof=%ld\n", sizeof(struct ssss));

	printf("char sizeof=%ld\n", sizeof(char));
	printf("int sizeof=%ld\n", sizeof(int));


	return 0;
}

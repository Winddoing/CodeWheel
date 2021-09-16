/*#############################################################
 *     File Name	: sha512_test.c
 *     Author		: winddoing
 *     Created Time	: 2021年09月16日 星期四 14时26分12秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <openssl/sha.h>
#include <string.h>

static void dump_hex(char* md)
{
	char chars[16] = {'0', '1', '2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    int i = 0, j = 0;
    for (i = 0; i < strlen(md); i += 1) {
        char val = *(unsigned char *)(md + i);
        for (j = 1; j >= 0; j--) {
            printf("%c", chars[(val >> (4 * j)) & 0xf]);
        }
    }
    printf("\n");
}

static char *calc_sha512(char *data) {
    SHA512_CTX ctx;
    char *md = malloc(sizeof(char)*(SHA512_DIGEST_LENGTH+1));

    SHA512_Init(&ctx);
    SHA512_Update(&ctx, data, strlen(data));
    SHA512_Final(md, &ctx);

    //fwrite(&md, strlen(md), 1, stdout);
    dump_hex(md);

    return md;
}

int main(int argc, const char *argv[])
{
    calc_sha512("foo");

    char* cmd = "echo -n  \"foo\" | sha512sum";
    printf("Run %s\n", cmd);
    system(cmd);

    return 1;
}

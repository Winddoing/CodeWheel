/*#############################################################
 *     File Name	: base_conv.c
 *     Author		: winddoing
 *     Created Time	: 2021年06月08日 星期二 16时32分16秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void NumberSystem(char *in,int s_in,char *out,int s_out)
{
    int v = 0, n;
    for(char *p = in; *p; p++)
        v=v*s_in+(*p-((*p>='a')?('a'-10):'0'));

    for(n=31; v&&n; v=v/s_out)
        out[n--] = (((v%s_out)<10)?'0':('a'-10))+(v%s_out);

    memmove(out, out+n+1, 32-n-1);
    out[32-n-1] = 0;
}

int main(int argc, const char *argv[])
{
    char out[33];
    for(int s = 2; s < 36; s++) {
        NumberSystem("a1b2c3", 16, out, s);
        printf("16:a1b2c3 => %2d:%s\n", s, out);
    }
    for(int s = 2; s < 36; s++) {
        NumberSystem("10000", 10, out, s);
        printf("10:10000 => %2d:%s\n", s, out);
    }
}

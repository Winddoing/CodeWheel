/*#############################################################
 *     File Name	: itoa.c
 *     Author		: winddoing
 *     Created Time	: 2019年04月17日 星期三 11时42分57秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

static char* itoa(int value, char* result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

int main(int argc, const char *argv[])
{
    char a[10];

    itoa(0, a, 2);
    printf("===> func: %s, line: %d, b=%s\n", __func__, __LINE__, a);
    
    return 0;
}

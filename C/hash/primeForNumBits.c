/*#############################################################
 *     File Name	: primeForNumBits.c
 *     Author		: winddoing
 *     Created Time	: 2020年06月06日 星期六 23时30分02秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
    The prime_deltas array contains the difference between a power
    of two and the next prime number:
    prime_deltas[i] = nextprime(2^i) - 2^i
    Basically, it's sequence A092131 from OEIS, assuming:
    - nextprime(1) = 1
    - nextprime(2) = 2
    and
    - left-extending it for the offset 0 (A092131 starts at i=1)
    - stopping the sequence at i = 28 (the table is big enough...)
*/
static const unsigned char prime_deltas[] = {
    0,  0,  1,  3,  1,  5,  3,  3,  1,  9,  7,  5,  3, 17, 27,  3,
    1, 29,  3, 21,  7, 17, 15,  9, 43, 35, 15,  0,  0,  0,  0,  0
};
/*
    The primeForNumBits() function returns the prime associated to a
    power of two. For example, primeForNumBits(8) returns 257.
*/
static inline int primeForNumBits(int numBits)
{
    return (1 << numBits) + prime_deltas[numBits];
}
/*
    Returns the smallest integer n such that
    primeForNumBits(n) >= hint.
*/
static int countBits(int hint)
{
    int numBits = 0;
    int bits = hint;
    while (bits > 1) {
        bits >>= 1;
        numBits++;
    }
    if (numBits >= (int)sizeof(prime_deltas)) {
        numBits = sizeof(prime_deltas) - 1;
    } else if (primeForNumBits(numBits) < hint) {
        ++numBits;
    }
    return numBits;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("%s No parameters, E.g:[%s 12]\n", argv[0], argv[0]);
        return -1;
    }
    int a = atoi(argv[1]);

    printf("a=%d, primeForNumBits=%d, countBits=%d\n", 
            a, primeForNumBits(a), countBits(a));
    return 0;
}
